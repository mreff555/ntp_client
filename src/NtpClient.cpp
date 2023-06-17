#include "NtpClient.hpp"
#include <arpa/inet.h>
#include <iomanip>
#include <netdb.h> // For getaddrinfo
#include <netinet/in.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <vector>

std::string NtpClient::getReferenceTimeInSeconds(NtpMessageStruct &packet) const
{
  float seconds = ntohl(packet.ref_ts_sec) - NTP_TIMESTAMP_DELTA;
  float fraction = (ntohl(packet.ref_ts_frac) - NTP_TIMESTAMP_DELTA) / 16;
  std::stringstream ss;
  ss << std::fixed << std::setprecision(0) << seconds << "." << fraction;
  return ss.str();
}

std::string NtpClient::getOriginalTimeInSeconds(NtpMessageStruct &packet) const
{
  float seconds = ntohl(packet.orig_ts_sec) - NTP_TIMESTAMP_DELTA;
  float fraction = (ntohl(packet.orig_ts_frac) - NTP_TIMESTAMP_DELTA) / 16;
  std::stringstream ss;
  ss << std::fixed << std::setprecision(0) << seconds << "." << fraction;
  return ss.str();
}

std::string NtpClient::getRxTimeInSeconds(NtpMessageStruct &packet) const
{
  float seconds = ntohl(packet.tx_ts_sec) - NTP_TIMESTAMP_DELTA;
  float fraction = (ntohl(packet.tx_ts_frac) - NTP_TIMESTAMP_DELTA) / 16;
  std::stringstream ss;
  ss << std::fixed << std::setprecision(0) << seconds << "." << fraction;
  return ss.str();
}

std::string NtpClient::getTxTimeInSeconds(NtpMessageStruct &packet) const
{
  float seconds = ntohl(packet.rx_ts_sec) - NTP_TIMESTAMP_DELTA;
  float fraction = (ntohl(packet.rx_ts_frac) - NTP_TIMESTAMP_DELTA) / 16;
  std::stringstream ss;
  ss << std::fixed << std::setprecision(0) << seconds << "." << fraction;
  return ss.str();
}

// Create and send an NTP request to the specified server
int NtpClient::ntp_request(const char *server, NtpMessageStruct *packet)
{
  struct addrinfo hints, *res, *p;
  int status;
  char ipstr[INET6_ADDRSTRLEN];

  typedef std::vector<std::string> IpVector;
  IpVector ipVector;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if ((status = getaddrinfo(server, NULL, &hints, &res)) != 0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 2;
  }

  printf("Requesting NTP response from %s:\n\n", server);
  for (p = res; p != NULL; p = p->ai_next)
  {
    void *addr;
    size_t sz = 5;
    char ipver[sz];
    if (p->ai_family == AF_INET)
    {
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
      addr = &(ipv4->sin_addr);
      memcpy(&ipver, "IPv4", sz);
    }
    else
    {
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
      addr = &(ipv6->sin6_addr);
      memcpy(&ipver, "IPv6", sz);
    }

    inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
    // printf(" %s: %s\n", ipver, ipstr);
    ipVector.push_back(ipstr);
  }

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sockfd < 0)
  {
    perror("Failed to create socket");
    return -1;
  }

  // Set the NTP server address and port
  struct sockaddr_in servaddr;
  servaddr.sin_family = res->ai_family;
  servaddr.sin_port = htons(NTP_PORT);
  if (inet_aton(ipstr, &servaddr.sin_addr) == 0)
  {
    perror("Invalid server address");
    return -1;
  }

  // Send the NTP request
  memset(packet, 0, sizeof(NtpMessageStruct));
  IpVector::iterator it;

  for (it = ipVector.begin(); it < ipVector.end(); ++it)
  {
    printf("Trying server [%ld of %lu] - %s: ", it - ipVector.begin() + 1,
           ipVector.size(), it->c_str());
    inet_aton(it->c_str(), &servaddr.sin_addr);
    // NTP version 3, client mode, unauthenticated (00 011 011)
    // packet->li_vn_mode = 0x1b; // 0001 1011
    packet->li_vn_mode = (0 | 4 << 3 | 3);

    if (sendto(sockfd, packet, sizeof(NtpMessageStruct), 0,
               (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
      perror("Failed to send NTP request");
      printf("FAIL!\n");
      continue;
    }
    else
    {
      printf("SUCCESS!\n");
      // break;
    }
  }

  // Receive the NTP response
  memset(packet, 0, sizeof(NtpMessageStruct));
  socklen_t addrlen = sizeof(servaddr);
  if (recvfrom(sockfd, packet, sizeof(NtpMessageStruct), 0,
               (struct sockaddr *)&servaddr, &addrlen) < 0)
  {
    perror("Failed to receive NTP response");
    return -1;
  }

  freeaddrinfo(res);
  close(sockfd);
  return 0;
}
