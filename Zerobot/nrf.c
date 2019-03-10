#include "nrf.h"

void nrfInit(void)
{
	int i = 0;
	for (i = 0; i < N_NRF_DEVS; i++)
	{
		NRF_DEVS[i]->irq->callback = nrfIRQHandler;
	}
}

void nrfESBInit(const NRFDev * dev)
{
	uint8_t i = 0;

	// Enter standby mode.
	nrfStandby(dev);

	// Power up and enable CRC16.
	nrfWriteReg(dev, NRF_REG_CONFIG, NRF_REG_CONFIG_DEFAULT);
	// Enable auto ACK on pipe0 and pipe1.
	nrfWriteReg(dev, NRF_REG_EN_AA, 0b00000011);
	// Enable Rx pipe0 and pipe1.
	nrfWriteReg(dev, NRF_REG_EN_RXADDR, 0b00000011);
	// 3 bytes address width.
	nrfWriteReg(dev, NRF_REG_SETUP_AW, 0b00000001);
	// Wait up to 500us and re-transmit up to 15 times.
	nrfWriteReg(dev, NRF_REG_SETUP_RETR, 0b00011111);
	// Setup RF channel.
	nrfWriteReg(dev, NRF_REG_RF_CH, NRF_RF_CHANNEL);
	// Setup RF 2Mbps
	nrfWriteReg(dev, NRF_REG_RF_SETUP, 0b00001110);
	// Enable dynamic payload length from pipe0 and pipe1.
	nrfWriteReg(dev, NRF_REG_DYNPD, 0b00000011);
	// Enable dynamic payload length for all channel and W_TX_PAYLOAD_NOACK command.
	nrfWriteReg(dev, NRF_REG_FEATURE, 0b00000101);

	// Flush all FIFO.
	for (i = 0; i < 3; i++)
	{
		nrfFlushRxFIFO(dev);
		nrfFlushTxFIFO(dev);
	}

	// Clear all IRQ bits.
	nrfClearIRQ(dev);
}

void nrfIRQHandler(const EXTIDev * dev)
{
	int i = 0;
	for (i = 0; i < N_NRF_DEVS; i++)
	{
		if (dev == NRF_DEVS[i]->irq)
		{
			if (NRF_DEVS[i]->irq_callback != NULL)
			{
				NRF_DEVS[i]->irq_callback(NRF_DEVS[i]);
			}
		}
	}
}
