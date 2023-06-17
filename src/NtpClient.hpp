#ifndef NTP_CLIENT_HPP
#define NTP_CLIENT_HPP

#include "NtpMessageStruct.hpp"
#include <memory>

class NtpClient
{
public:
  std::string getReferenceTimeInSeconds(NtpMessageStruct &packet) const;

  std::string getOriginalTimeInSeconds(NtpMessageStruct &packet) const;

  std::string getRxTimeInSeconds(NtpMessageStruct &packet) const;

  std::string getTxTimeInSeconds(NtpMessageStruct &packet) const;

  int ntp_request(const char *server, NtpMessageStruct *packet);

private:
};

#endif /* NTP_CLIENT_HPP */
