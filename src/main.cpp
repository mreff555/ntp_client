#include "NtpClient.hpp"
#include "NtpMessageStruct.hpp"
#include <string>

int main()
{
  const char *server = "pool.ntp.org";
  NtpMessageStruct packet;
  NtpClient client;

  // Send an NTP request to the server
  if (client.ntp_request(server, &packet) < 0)
  {
    return -1;
  }

  uint8_t stratum = ntohl(packet.stratum);
  uint8_t precision = ntohl(packet.precision);
  //    char ref_id[4];
  //    uint32_t temp = ntohl(packet.ref_id);
  //    memcpy(&ref_id, &temp, sizeof(ref_id));

  // printf("NTP server: %s\n", server);
  // printf("Stratum: %u\n", 0x000F & ntohl(packet.stratum));
  // printf("Poll: %u\n", 0x000F & ntohl(packet.poll));
  // printf("Precision: %u\n", 0x000F & ntohl(packet.precision));

  printf("Reference ID: 0x%X\n", ntohl(packet.ref_id));

  printf("Reference time in seconds: %s\n",
         client.getReferenceTimeInSeconds(packet).c_str());
  printf("Original time in seconds: %s\n",
         client.getOriginalTimeInSeconds(packet).c_str());
  printf("TX time in seconds: %s\n", client.getTxTimeInSeconds(packet).c_str());
  printf("RX time in seconds: %s\n", client.getRxTimeInSeconds(packet).c_str());
  return 0;
}
