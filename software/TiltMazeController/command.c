/*
 * command.c
 *
 *  Created on: Feb 7, 2017
 *      Author: dktrotti
 */

#include "command.h"

cmd_t process_cmd_buf(char *buf, INT16U *arg) {
	if (strncmp(buf, CMD_PING, CMD_LEN) == 0) {
		*arg = 0;
		return PING;
	} else if (strncmp(buf, CMD_SETX, CMD_LEN) == 0) {
		// TODO: Make sure argument is there
		//*arg = ((INT16U) buf[CMD_ARGPOS] << 8) + ((INT16U) buf[CMD_ARGPOS + 1]);
		*arg = get_arg(buf);
		return SET_X_ACC;
	} else if (strncmp(buf, CMD_SETY, CMD_LEN) == 0) {
		// TODO: Make sure argument is there
		//*arg = ((INT16U) buf[CMD_ARGPOS] << 8) + ((INT16U) buf[CMD_ARGPOS + 1]);
		*arg = get_arg(buf);
		return SET_Y_ACC;
	} else if (strncmp(buf, CMD_GETX, CMD_LEN) == 0) {
		*arg = 0;
		return GET_X_ACC;
	} else if (strncmp(buf, CMD_GETY, CMD_LEN) == 0) {
		*arg = 0;
		return GET_Y_ACC;
	} else {
		*arg = 0;
		return INVALID;
	}
}

INT16U get_arg(char *buf) {
	INT16U rv;

	sscanf(&buf[CMD_ARGPOS], "%x", &rv);

	return rv;
}
