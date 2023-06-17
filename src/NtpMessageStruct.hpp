#ifndef NTP_MESSAGE_STRUCT_HPP
#define NTP_MESSAGE_STRUCT_HPP

#include <cstdint>

#define NTP_PORT 123

// Difference between a UNIX timestamp (Starting Jan, 1st 1970) and a NTP
// timestamp (Starting Jan, 1st 1900
#define NTP_TIMESTAMP_DELTA 2208988800ull

// NTP packet structure (Big endian)
// 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3
// 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
struct NtpMessageStruct
{
  // Word 1
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  // |LI | VN  |Mode |    Stratum    |     Poll      |   Precision   |
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  uint8_t li_vn_mode; // LI (0-1), VN (2-4), Mode (5-7)
  uint8_t stratum;    // Stratum
  uint8_t poll;       // Poll
  uint8_t precision;  // Precision

  // Word 2
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  // |                          Root Delay                           |
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  uint32_t root_delay; // Root Delay

  // Word 3
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  // |                       Root Dispersion                         |
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  uint32_t root_dispersion; // Root Dispersion

  // Word 4
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  // |                    Reference Identifier                       |
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  uint32_t ref_id; // Reference Identifier
                   // Word 5 / 6
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  // |                                                               |
  // |                   Reference Timestamp (64)                    |
  // |                                                               |
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  uint32_t ref_ts_sec;  // Reference Timestamp
  uint32_t ref_ts_frac; // Reference Timestamp Fraction

  // Word 7 / 8
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  // |                                                               |
  // |                   Originate Timestamp (64)                    |
  // |                                                               |
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  uint32_t orig_ts_sec;  // Origin Timestamp
  uint32_t orig_ts_frac; // Origin Timestamp Fraction

  // Word 9 / 10
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  // |                                                               |
  // |                    Receive Timestamp (64)                     |
  // |                                                               |
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  uint32_t rx_ts_sec;
  uint32_t rx_ts_frac;

  // Word 11 / 12
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  // |                                                               |
  // |                    Transmit Timestamp (64)                    |
  // |                                                               |
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  uint32_t tx_ts_sec;
  uint32_t tx_ts_frac;

  // Word 13 - 15
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  // |                                                               |
  // |                                                               |
  // |                  Authenticator (optional) (96)                |    Word
  // 13 - 15 |                                                               |
  // |                                                               |
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  // char authenticator[96];
};

#endif /* NTP_MESSAGE_STRUCT_HPP */