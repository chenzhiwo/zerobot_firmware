#ifndef ZEROBOT_CRC_H_
#define ZEROBOT_CRC_H_

#include "stddef.h"
#include "stdint.h"

extern const uint_fast16_t CRC_CCITT_TABLE[256];

static inline uint16_t crcCCITT(const uint8_t* data, size_t len)
{
	uint_fast16_t crc = 0x1d0f;
	int index;

	while (len--)
	{
        index = ((crc >> 8) ^ *data) % 256;
		crc = CRC_CCITT_TABLE[index] ^ (crc << 8);
		data++;
	}
	return crc;
}

#endif /* ZEROBOT_CRC_H_ */
