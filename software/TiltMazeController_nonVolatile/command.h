/*
 * command.h
 *
 *  Created on: Feb 7, 2017
 *      Author: dktrotti
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <string.h>
#include "includes.h"

#define ACK_LEN		3
#define ACK_STR		"ACK"

#define NACK_LEN	4
#define NACK_STR	"NACK"

#define CMD_LEN		2
#define CMD_ARGPOS	2
#define CMD_ARGLEN 	4

#define CMD_PING	"PG"
#define CMD_SETX	"SX"
#define CMD_SETY	"SY"
#define CMD_GETX	"GX"
#define CMD_GETY	"GY"

typedef enum {
	INVALID,
	PING,
	SET_X_ACC,
	SET_Y_ACC,
	GET_X_ACC,
	GET_Y_ACC
} cmd_t;

/**
 * Processes a command string to find the command type and optional argument
 * @param buf Character buffer containing the command to be parsed
 * @param arg Pointer to the location for the optional argument to be stored
 * @return Returns the type of the command
 */
cmd_t process_cmd_buf(char *buf, INT16U *arg);

/**
 * Contains the logic to be used in the command processing task
 * @param pdata
 */
void cmd_task(void* pdata);

/**
 * Extracts the optional argument from a command
 * @note The argument should be expressed as a 4 digit lowercase hexadecimal number (0x0001-0xffff)
 * @param buf Character buffer containing the command
 * @return Returns the value of the argument
 */
INT16U get_arg(char *buf);

#endif /* COMMAND_H_ */
