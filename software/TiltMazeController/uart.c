/*
 * uart.c
 *
 *  Created on: Feb 7, 2017
 *      Author: dktrotti
 */

#include "uart.h"

/* System and Synchronization Variables*/
FILE* uart_fp;
OS_EVENT *cmdQ;

alt_up_character_lcd_dev *char_lcd;

void uart_init(OS_EVENT *cmdQueue) {
	INT8U err;
	uart_fp = fopen (UART_0_NAME, "r+");
	cmdQ = cmdQueue;

	char_lcd = alt_up_character_lcd_open_dev(CHARACTER_LCD_0_NAME);
	if (char_lcd == NULL) {
		printf("No char lcd.\n");
	}
	alt_up_character_lcd_init(char_lcd);

	// TODO: Remove magic numbers
	IOWR(UART_0_BASE, 3, 0x80);

	err = alt_ic_isr_register(UART_0_IRQ_INTERRUPT_CONTROLLER_ID,
			UART_0_IRQ,
			interrupt_uart,
			NULL,
			NULL);

	if (err != 0) {
		printf("Failed to attach interrupt.\n");
	} else {
		printf("Attached interrupt.\n");
	}
}

//void uarttask(void *pdata) {
//	IOWR_ALTERA_AVALON_PIO_DATA(GREEN_LEDS_BASE, 0x00);
//
//	long time = 0;
//	char *buf;
//	alt_u8 curr;
//	alt_u8 parity;
//	int index = 0;
//	int err;
//	bool inframe = false;
//
//	buf = (char *) malloc(sizeof(char) * (MSG_MAX_LEN + 1));
//	memset(buf, 0, MSG_MAX_LEN + 1);
//
//	while (1) {
//		if (alt_up_rs232_get_used_space_in_read_FIFO(uart) > 0) {
//
//			if (OSTimeGet() - time > MSG_TIMEOUT) {
//				// Clear the message
//		    	index = 0;
//	        	memset(buf, 0, MSG_MAX_LEN + 1);
//		    	inframe = false;
//		    	printf("Message timeout\n");
//			}
//
//			time = OSTimeGet();
//
//	        err = alt_up_rs232_read_data(uart, &curr, &parity);
//
//			printf("%c", curr);
//
//	        if (err != 0) {
//	        	// TODO: There was an error, deal with it
//	        	printf("read error\n");
//	        	IOWR_ALTERA_AVALON_PIO_DATA(GREEN_LEDS_BASE, 0x01);
//	        }
//
//	        if (parity != 0) {
//	        	// TODO: Error in byte, discard buffer
//	        	printf("parity error\n");
//	        	IOWR_ALTERA_AVALON_PIO_DATA(GREEN_LEDS_BASE, 0x02);
//	        }
//
//	        if (curr == MSG_OPEN_CHAR) {
//	        	index = 0;
//	        	memset(buf, 0, MSG_MAX_LEN + 1);
//	        	inframe = true;
//	        } else if (curr == MSG_CLOSE_CHAR && inframe) {
//	        	buf[index] = '\0';
//
//	        	printf("\nMsg: %s\n", buf);
//	        	err = OSQPost(cmdQ, (void *)buf);
//
//	        	if (err != OS_NO_ERR) {
//	        		printf("Queue post error\n");
//	        	}
//
//	        	index = 0;
//	        	buf = malloc(sizeof(char) * (MSG_MAX_LEN + 1));
//	        	memset(buf, 0, MSG_MAX_LEN + 1);
//
//	        	inframe = false;
//
//	        } else if (inframe){
//	        	buf[index] = curr;
//	        	index++;
//	        } else {
//	        	// Ignore char
//	        	index = 0;
//	        	memset(buf, 0, MSG_MAX_LEN + 1);
//	        }
//
//        	if (index > MSG_MAX_LEN) {
//	        	index = 0;
//	        	memset(buf, 0, MSG_MAX_LEN + 1);
//	        	inframe = false;
//        	}
//		}
//
//		//OSTimeDlyHMSM(0, 0, 0, 1);
//	}
//}

static void interrupt_uart(void * context, alt_u32 id) {
	static long time = 0;
	static bool inframe = false;
	static int index = 0;
	static char buf[MSG_MAX_LEN + 1];
	int err;

	char c = getc(uart_fp);

	if (OSTimeGet() - time > MSG_TIMEOUT) {
		// Clear the message
    	index = 0;
    	memset(&buf[0], 0, sizeof(char) * (MSG_MAX_LEN + 1));
    	inframe = false;
	}

	time = OSTimeGet();

    if (c == MSG_OPEN_CHAR) {
    	index = 0;
    	memset(&buf[0], 0, sizeof(char) * (MSG_MAX_LEN + 1));
    	inframe = true;
    } else if (c == MSG_CLOSE_CHAR && inframe) {
    	buf[index] = '\0';

    	char* tempbuf = malloc(sizeof(char) * (MSG_MAX_LEN + 1));
    	memcpy(tempbuf, &buf[0], sizeof(char) * (MSG_MAX_LEN + 1));
    	err = OSQPost(cmdQ, (void *)tempbuf);

    	if (err != OS_NO_ERR) {
    		//TODO: Handle post error
    	}

    	index = 0;
    	memset(&buf[0], 0, sizeof(char) * (MSG_MAX_LEN + 1));
    	inframe = false;

    } else if (inframe){
    	buf[index] = c;
    	index++;
    } else {
    	// Ignore char
    	index = 0;
    	memset(&buf[0], 0, sizeof(char) * (MSG_MAX_LEN + 1));
    }

	if (index >= MSG_MAX_LEN) {
    	index = 0;
    	memset(&buf[0], 0, sizeof(char) * (MSG_MAX_LEN + 1));
    	inframe = false;
	}
}

int uart_write(char *buf, int len) {
	// TODO: Not thread safe, need lock
	char send_buf[MSG_MAX_LEN];
	memset(&send_buf[0], 0, sizeof(char) * (MSG_MAX_LEN));

	sprintf(send_buf, "<%s>", buf);
	int num_bytes = fwrite(&send_buf[0], sizeof(char), len + 2, uart_fp);
	if (fflush(uart_fp) != 0) {
		printf("Error occurred during flush.\n");
	}
	return num_bytes;

//	err = alt_up_rs232_write_data(uart, MSG_OPEN_CHAR);
//	if (err != 0) {
//		return err;
//	}
//
//	int i = 0;
//	for (i = 0; i < len; i++) {
//		if (buf[i] == '\0') {
//			break;
//		} else {
//			err = alt_up_rs232_write_data(uart, buf[i]);
//			if (err != 0) {
//				return err;
//			}
//		}
//	}
//
//	err = alt_up_rs232_write_data(uart, MSG_CLOSE_CHAR);
//	if (err != 0) {
//		return err;
//	}
//
//	// TODO: Replace with defined value
//	return 0;
}
