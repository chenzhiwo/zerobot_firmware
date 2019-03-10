#include "msg_io.h"
#include "board.h"
#include "osal.h"
#include "ring_buffer.h"
#include "led.h"
#include "crc.h"
#include "uart.h"

#define BYTE_OFFSET(pointer, offset) (((const uint8_t*)pointer) + offset)

#define N_CALLBACKS 32
#define RX_BUFFER_SIZE 512

#define START_SIGN '\n'

typedef MSG_PACKED struct
{
	uint8_t start;
	uint8_t channel;
	uint8_t length;
	uint8_t checksum;
	uint8_t payload[];
} MsgHeader;

typedef uint16_t Checksum;

#define HEADER_SIZE (sizeof(MsgHeader))
#define CHECKSUM_SIZE (sizeof(Checksum))

void uartIDLECallback(const UARTDev * dev);
void uartTCCallback(const UARTDev * dev);

void uartRecvThread(void const * arg);

osSemaphoreDef(uart_rx_semap);
osSemaphoreId uart_rx_semap;
osSemaphoreDef(uart_tx_semap);
osSemaphoreId uart_tx_semap;

osThreadDef(uart_recv_thread, uartRecvThread, osPriorityIdle, 0, 128);
osThreadId uart_recv_thread;

extern UARTDev uart1;
UARTDev (* const uart) = &uart1;

MsgCallback CALLBACKS[N_CALLBACKS] =
{ NULL };

void msgIOInit(void)
{
	uart_rx_semap = osSemaphoreCreate(osSemaphore(uart_rx_semap), 1);
	osSemaphoreRelease(uart_rx_semap);

	uart_tx_semap = osSemaphoreCreate(osSemaphore(uart_tx_semap), 1);
	osSemaphoreRelease(uart_tx_semap);

	uart->tc_callback = uartTCCallback;

	uart_recv_thread = osThreadCreate(osThread(uart_recv_thread), NULL);
}

void msgSetCallback(Channel topic_id, MsgCallback callback)
{
	if (topic_id < N_CALLBACKS)
	{
		CALLBACKS[topic_id] = callback;
	}
}

void msgSend(Channel topic_id, void * payload, size_t len)
{
	Checksum checksum;
	static uint8_t buffer[HEADER_SIZE + sizeof(MsgBuffer) + CHECKSUM_SIZE];
	MsgHeader *ptr = (MsgHeader *) buffer;

	// This will be release in uartTCCallback().
	osSemaphoreWait(uart_tx_semap, osWaitForever);

	ptr->start = START_SIGN;
	ptr->channel = topic_id;
	ptr->length = len;
	ptr->checksum = ptr->start ^ ptr->channel ^ ptr->length;

	memcpy(ptr->payload, payload, len);

	checksum = crcCCITT(ptr->payload, len);
	*((uint16_t *) BYTE_OFFSET(ptr->payload, len)) = checksum;
	uartSendAsync(uart, buffer, HEADER_SIZE + ptr->length + CHECKSUM_SIZE);
}

void uartRecvThread(void const * arg)
{
	static uint8_t buffer_data[RX_BUFFER_SIZE + sizeof(MsgHeader)
			+ sizeof(MsgBuffer) + sizeof(Checksum)];
	static RingBuffer buffer;
	MsgHeader * ptr;
	uint16_t checksum, msg_checksum;
	size_t msg_size;

	uart->idle_callback = uartIDLECallback;

	osThreadSetPriority(osThreadGetId(), osPriorityRealtime);

	ringBufferInit(&buffer, buffer_data, sizeof(buffer_data),
			sizeof(MsgHeader) + sizeof(MsgBuffer) + sizeof(Checksum));
	uartRecvStart(uart, ringBufferData(&buffer), ringBufferCapacity(&buffer));

	while (true)
	{
		// header.
		while (true)
		{
			ringBufferSetTailIndex(&buffer, uartRecvIndex(uart));
			if (ringBufferSize(&buffer) >= sizeof(MsgHeader))
			{
				break;
			}
			else
			{
				osSemaphoreWait(uart_rx_semap, 1);
			}
		}

		ptr = (MsgHeader *) ringBufferRead(&buffer, sizeof(MsgHeader));
		if ((ptr->start != START_SIGN)
				|| ((ptr->start ^ ptr->channel ^ ptr->length) != ptr->checksum))
		{
			ringBufferPop(&buffer, 1);
			continue;
		}

		msg_size = ptr->length;

		while (true)
		{
			ringBufferSetTailIndex(&buffer, uartRecvIndex(uart));
			if (ringBufferSize(&buffer) >= HEADER_SIZE + msg_size + CHECKSUM_SIZE)
			{
				break;
			}
			else
			{
				osSemaphoreWait(uart_rx_semap, 1);
			}
		}

		ptr = (MsgHeader *) ringBufferRead(&buffer, msg_size);
		checksum = crcCCITT((uint8_t *) ptr->payload, ptr->length);
		msg_checksum = *((uint16_t *) BYTE_OFFSET(ptr->payload,
				ptr->length));

		// msg_checksum == 0 means discard checksum.
		if ((msg_checksum != checksum) && (msg_checksum != 0))
		{
			ringBufferPop(&buffer, 1);
			continue;
		}

		if (CALLBACKS[ptr->channel] != NULL)
		{
			CALLBACKS[ptr->channel](ptr->payload);
		}

		ringBufferPop(&buffer, msg_size);
		ledToggle(LED_DEVS[1]);
	}
}

void uartIDLECallback(const UARTDev * dev)
{
	osSemaphoreRelease(uart_rx_semap);
}

void uartTCCallback(const UARTDev * dev)
{
	osSemaphoreRelease(uart_tx_semap);
}
