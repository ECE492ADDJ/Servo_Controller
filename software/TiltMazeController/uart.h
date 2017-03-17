/*
 * uart.h
 *
 *  Created on: Feb 7, 2017
 *      Author: dktrotti
 */

#ifndef UART_H_
#define UART_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "includes.h"
#include "altera_avalon_pio_regs.h"
#include "altera_up_avalon_rs232.h"
#include "altera_up_avalon_character_lcd.h"

#define MSG_OPEN_CHAR '<'
#define MSG_CLOSE_CHAR '>'
#define MSG_MAX_LEN 8
#define MSG_TIMEOUT 500	// Message timeout in OS ticks

/**
 * Initializes the uart.
 * @param cmdQueue Queue that the uart should post complete messages to
 */
void uart_init(OS_EVENT *cmdQueue);

/**
 * Simple task for polling the uart and posting messages to the queue.
 * @param pdata
 */
void uarttask(void *pdata);

/**
 * Writes the characters from the given buffer to the uart tx line.
 * @param buf Buffer containing the characters
 * @param len Number of characters to be written
 * @return OS_NO_ERR if successful, or a system error code on failure
 */
int uart_write(char *buf, int len);

#endif /* UART_H_ */
