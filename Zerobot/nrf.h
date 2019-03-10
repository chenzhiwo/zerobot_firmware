#ifndef ZERORADIO_NRF_H_
#define ZERORADIO_NRF_H_

#include "hardware.h"
#include "gpio.h"
#include "spi.h"
#include "exti.h"

typedef struct NRFDev
{
	SPIDev * spi;
	EXTIDev * irq;
	GPIODev * ce;
	void (*irq_callback)(const struct NRFDev * dev);
	void * args;
} NRFDev;

extern NRFDev * NRF_DEVS[];

extern const size_t N_NRF_DEVS;

// SPI commands.
#define NRF_CMD_R_REGISTER          0b00000000
#define NRF_CMD_W_REGISTER          0b00100000
#define NRF_CMD_R_RX_PAYLOAD        0b01100001
#define NRF_CMD_W_TX_PAYLOAD        0b10100000
#define NRF_CMD_FLUSH_TX            0b11100001
#define NRF_CMD_FLUSH_RX            0b11100010
#define NRF_CMD_REUSE_TX_PL         0b11100011
#define NRF_CMD_R_RX_PL_WID         0b01100000
#define NRF_CMD_W_ACK_PAYLOAD       0b10101000
#define NRF_CMD_W_TX_PAYLOAD_NO_ACK 0b10110000
#define NRF_CMD_NOP                 0b11111111

// Register address.
#define NRF_REG_CONFIG 		0x00
#define NRF_REG_EN_AA 		0x01
#define NRF_REG_EN_RXADDR 	0x02
#define NRF_REG_SETUP_AW 	0x03
#define NRF_REG_SETUP_RETR 	0x04
#define NRF_REG_RF_CH	 	0x05
#define NRF_REG_RF_SETUP    0x06
#define NRF_REG_STATUS      0x07
#define NRF_REG_OBSERVE_TX	0x08
#define NRF_REG_RPD			0x09
#define NRF_REG_RX_ADDR_P0	0x0a
#define NRF_REG_RX_ADDR_P1	0x0b
#define NRF_REG_RX_ADDR_P2	0x0c
#define NRF_REG_RX_ADDR_P3	0x0d
#define NRF_REG_RX_ADDR_P4	0x0e
#define NRF_REG_RX_ADDR_P5	0x0f
#define NRF_REG_TX_ADDR		0x10
#define NRF_REG_RX_PW_P0	0x11
#define NRF_REG_RX_PW_P1	0x12
#define NRF_REG_RX_PW_P2	0x13
#define NRF_REG_RX_PW_P3	0x14
#define NRF_REG_RX_PW_P4	0x15
#define NRF_REG_RX_PW_P5	0x16
#define NRF_REG_FIFO_STATUS	0x17
#define NRF_REG_DYNPD		0x1c
#define NRF_REG_FEATURE		0x1d

// Frequently used bits.
#define NRF_PRIM_RX_BIT		0b00000001
#define NRF_RX_DR_BIT		0b01000000
#define NRF_TX_DS_BIT		0b00100000
#define NRF_MAX_RT_BIT		0b00010000
#define NRF_TX_EMPTY_BIT	0b00010000
#define NRF_RX_EMPTY_BIT	0b00000001

// Hardware limits.
#define NRF_FIFO_LEN_MAX 		32
#define NRF_ADDRESS_WIDTH_MAX	5

// Custom
#define NRF_HAL_SPI_TIMEOUT		0xffff
#define NRF_REG_CONFIG_DEFAULT	0b00001011
#define NRF_ADDRESS_WIDTH		3
#define NRF_RF_CHANNEL			100

void nrfInit(void);

static inline void nrfEnable(const NRFDev * dev)
{
	gpioWrite(dev->ce, true);
}

static inline void nrfStandby(const NRFDev * dev)
{
	gpioWrite(dev->ce, false);
}

static inline uint8_t nrfCmd(const NRFDev * dev, uint8_t cmd)
{
	spiRecvSend(dev->spi, &cmd, 1);
	return cmd;
}

static inline void nrfCmdIO(const NRFDev * dev, uint8_t cmd, uint8_t * buffer,
		size_t len)
{
	spiRecvSendCmd(dev->spi, cmd, buffer, len);
}

static inline uint8_t nrfReadReg(const NRFDev * dev, uint8_t address)
{
	address |= NRF_CMD_R_REGISTER;
	spiRecvCmd(dev->spi, address, &address, 1);
	return address;
}

static inline void nrfWriteReg(const NRFDev * dev, uint8_t address, uint8_t reg)
{
	address |= NRF_CMD_W_REGISTER;
	spiSendCmd(dev->spi, address, &reg, 1);
}

static inline void nrfReadRegBytes(const NRFDev * dev, uint8_t address,
		uint8_t * buffer, size_t len)
{
	address |= NRF_CMD_R_REGISTER;
	spiRecvCmd(dev->spi, address, buffer, len);
}

static inline void nrfWriteRegBytes(const NRFDev * dev, uint8_t address,
		uint8_t * buffer, size_t len)
{
	address |= NRF_CMD_W_REGISTER;
	spiSendCmd(dev->spi, address, buffer, len);
}

static inline uint8_t nrfStatus(const NRFDev * dev)
{
	return nrfCmd(dev, NRF_CMD_NOP);
}

static inline void nrfClearIRQ(const NRFDev * dev)
{
	nrfWriteReg(dev, NRF_REG_STATUS, nrfStatus(dev));
}

static inline uint8_t nrfFIFOStatus(const NRFDev * dev)
{
	return nrfReadReg(dev, NRF_REG_FIFO_STATUS);
}

static inline void nrfFlushRxFIFO(const NRFDev * dev)
{
	nrfCmd(dev, NRF_CMD_FLUSH_RX);
}

static inline void nrfFlushTxFIFO(const NRFDev * dev)
{
	nrfCmd(dev, NRF_CMD_FLUSH_TX);
}

static inline void nrfDumpRegs(const NRFDev * dev, uint8_t * buffer)
{
	uint8_t i = 0;
	for (i = NRF_REG_CONFIG; i <= NRF_REG_FEATURE; i++)
	{
		buffer[i] = nrfReadReg(dev, i);
	}
}

static inline void nrfESBPRX(const NRFDev * dev, uint8_t address)
{
	static uint8_t addr[NRF_ADDRESS_WIDTH] =
	{ 0 };

	nrfStandby(dev);

	// pipe0 to broadcast.
	addr[0] = UINT8_MAX;
	nrfWriteRegBytes(dev, NRF_REG_RX_ADDR_P0, addr, sizeof(addr));

	addr[0] = address;
	nrfWriteRegBytes(dev, NRF_REG_RX_ADDR_P1, addr, sizeof(addr));
	// PRX mode.
	nrfWriteReg(dev, NRF_REG_CONFIG, NRF_REG_CONFIG_DEFAULT | NRF_PRIM_RX_BIT);
}

static inline void nrfESBPTX(const NRFDev * dev, uint8_t address)
{
	static uint8_t addr[NRF_ADDRESS_WIDTH] =
	{ 0 };

	nrfStandby(dev);

	addr[0] = address;
	nrfWriteRegBytes(dev, NRF_REG_RX_ADDR_P0, addr, sizeof(addr));
	nrfWriteRegBytes(dev, NRF_REG_TX_ADDR, addr, sizeof(addr));
	// PTX mode.
	nrfWriteReg(dev, NRF_REG_CONFIG, NRF_REG_CONFIG_DEFAULT & ~NRF_PRIM_RX_BIT);
}

static inline size_t nrfReadRxFIFOLength(const NRFDev * dev)
{
	uint8_t len = 0;
	nrfCmdIO(dev, NRF_CMD_R_RX_PL_WID, &len, 1);
	return len;
}

static inline void nrfReadRxFIFO(const NRFDev * dev, uint8_t * buffer,
		size_t len)
{
	nrfCmdIO(dev, NRF_CMD_R_RX_PAYLOAD, buffer, len);
}

static inline void nrfWriteTxFIFO(const NRFDev * dev, uint8_t * buffer,
		size_t len,
		bool use_ack)
{
	if (use_ack)
	{
		nrfCmdIO(dev, NRF_CMD_W_TX_PAYLOAD, buffer, len);
	}
	else
	{
		nrfCmdIO(dev, NRF_CMD_W_TX_PAYLOAD_NO_ACK, buffer, len);
	}
}

void nrfESBInit(const NRFDev * dev);

void nrfIRQHandler(const EXTIDev * dev);

#endif /* ZERORADIO_NRF_H_ */
