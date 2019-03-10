#ifndef ZEROBOT_SPI_H_
#define ZEROBOT_SPI_H_

#include "hardware.h"
#include "gpio.h"

#define SPI_TIMEOUT 0xffff

static inline void spiInit(void)
{
}

static inline void spiRecv(const SPIDev * dev, uint8_t * buffer, size_t len)
{
	gpioWrite(dev->cs, false);
	HAL_SPI_Receive(dev->spi, buffer, len, SPI_TIMEOUT);
	gpioWrite(dev->cs, true);
}

static inline void spiSend(const SPIDev * dev, uint8_t * buffer, size_t len)
{
	gpioWrite(dev->cs, false);
	HAL_SPI_Transmit(dev->spi, buffer, len, SPI_TIMEOUT);
	gpioWrite(dev->cs, true);
}

static inline void spiRecvSend(const SPIDev * dev, uint8_t * buffer, size_t len)
{
	gpioWrite(dev->cs, false);
	HAL_SPI_TransmitReceive(dev->spi, buffer, buffer, len, SPI_TIMEOUT);
	gpioWrite(dev->cs, true);
}

static inline void spiRecvCmd(const SPIDev * dev, uint8_t cmd, uint8_t * buffer,
		size_t len)
{
	gpioWrite(dev->cs, false);
	HAL_SPI_Transmit(dev->spi, &cmd, 1, SPI_TIMEOUT);
	HAL_SPI_Receive(dev->spi, buffer, len, SPI_TIMEOUT);
	gpioWrite(dev->cs, true);
}

static inline void spiSendCmd(const SPIDev * dev, uint8_t cmd, uint8_t * buffer,
		size_t len)
{
	gpioWrite(dev->cs, false);
	HAL_SPI_Transmit(dev->spi, &cmd, 1, SPI_TIMEOUT);
	HAL_SPI_Transmit(dev->spi, buffer, len, SPI_TIMEOUT);
	gpioWrite(dev->cs, true);
}

static inline void spiRecvSendCmd(const SPIDev * dev, uint8_t cmd,
		uint8_t * buffer, size_t len)
{
	gpioWrite(dev->cs, false);
	HAL_SPI_Transmit(dev->spi, &cmd, 1, SPI_TIMEOUT);
	HAL_SPI_TransmitReceive(dev->spi, buffer, buffer, len, SPI_TIMEOUT);
	gpioWrite(dev->cs, true);
}

#endif /* ZEROBOT_SPI_H_ */
