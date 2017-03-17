/*
 * uart.c
 *
 *  Created on: Feb 7, 2017
 *      Author: dktrotti
 */

#include "uart.h"

/* System and Synchronization Variables*/
alt_up_rs232_dev *uart;
OS_EVENT *cmdQ;

void uart_init(OS_EVENT *cmdQueue) {
	uart = alt_up_rs232_open_dev(RS232_0_NAME);
	cmdQ = cmdQueue;

	alt_up_rs232_enable_read_interrupt(uart);
}

void uarttask(void *pdata) {
	IOWR_ALTERA_AVALON_PIO_DATA(GREEN_LEDS_BASE, 0x00);

	long time = 0;
	char *buf;
	alt_u8 curr;
	alt_u8 parity;
	int index = 0;
	int err;
	bool inframe = false;

	buf = (char *) malloc(sizeof(char) * (MSG_MAX_LEN + 1));
	memset(buf, 0, MSG_MAX_LEN + 1);

	while (1) {
		if (alt_up_rs232_get_used_space_in_read_FIFO(uart) > 0) {

			if (OSTimeGet() - time > MSG_TIMEOUT) {
				// Clear the message
		    	index = 0;
	        	memset(buf, 0, MSG_MAX_LEN + 1);
		    	inframe = false;
		    	printf("Message timeout\n");
			}

			time = OSTimeGet();

	        err = alt_up_rs232_read_data(uart, &curr, &parity);

			printf("%c", curr);

	        if (err != 0) {
	        	// TODO: There was an error, deal with it
	        	printf("read error\n");
	        	IOWR_ALTERA_AVALON_PIO_DATA(GREEN_LEDS_BASE, 0x01);
	        }

	        if (parity != 0) {
	        	// TODO: Error in byte, discard buffer
	        	printf("parity error\n");
	        	IOWR_ALTERA_AVALON_PIO_DATA(GREEN_LEDS_BASE, 0x02);
	        }

	        if (curr == MSG_OPEN_CHAR) {
	        	index = 0;
	        	memset(buf, 0, MSG_MAX_LEN + 1);
	        	inframe = true;
	        } else if (curr == MSG_CLOSE_CHAR && inframe) {
	        	buf[index] = '\0';

	        	printf("\nMsg: %s\n", buf);
	        	err = OSQPost(cmdQ, (void *)buf);

	        	if (err != OS_NO_ERR) {
	        		printf("Queue post error\n");
	        	}

	        	index = 0;
	        	buf = malloc(sizeof(char) * (MSG_MAX_LEN + 1));
	        	memset(buf, 0, MSG_MAX_LEN + 1);

	        	inframe = false;

	        } else if (inframe){
	        	buf[index] = curr;
	        	index++;
	        } else {
	        	// Ignore char
	        	index = 0;
	        	memset(buf, 0, MSG_MAX_LEN + 1);
	        }

        	if (index > MSG_MAX_LEN) {
	        	index = 0;
	        	memset(buf, 0, MSG_MAX_LEN + 1);
	        	inframe = false;
        	}
		}

		//OSTimeDlyHMSM(0, 0, 0, 0);
	}
}

int uart_write(char *buf, int len) {
	// TODO: Not thread safe, need lock
	int err;

	err = alt_up_rs232_write_data(uart, MSG_OPEN_CHAR);
	if (err != 0) {
		return err;
	}

	int i = 0;
	for (i = 0; i < len; i++) {
		if (buf[i] == '\0') {
			break;
		} else {
			err = alt_up_rs232_write_data(uart, buf[i]);
			if (err != 0) {
				return err;
			}
		}
	}

	err = alt_up_rs232_write_data(uart, MSG_CLOSE_CHAR);
	if (err != 0) {
		return err;
	}

	// TODO: Replace with defined value
	return 0;
}
