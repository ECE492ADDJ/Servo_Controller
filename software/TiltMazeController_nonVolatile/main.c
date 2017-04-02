/*
 * main.c
 *
 * The following code is based on hello_ucosii.c from the Altera
 * Hello World UCOSII template.
 *
 *  Created on: Feb 7, 2017
 *      Author: dktrotti
 */


#include <stdio.h>
#include <stdlib.h>
#include "includes.h"
#include "uart.h"
#include "command.h"
#include "servo.h"

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    task_servo_stk[TASK_STACKSIZE];
OS_STK    task_main_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */
#define TASK_SERVO_PRIORITY     2
#define TASK_MAIN_PRIORITY      1

/* Definition of global variables */
OS_EVENT *cmdQ;
OS_EVENT *servo0Sem;
OS_EVENT *servo1Sem;
void *cmdarr[16];

void maintask(void* pdata) {
	INT8U err;
	cmd_t command_type;
	INT16U command_arg;
	char *buf;
	char resp_buf[CMD_LEN + CMD_ARGLEN]; // Buffer for query responses

	// TODO: Remove these variables
	INT16U tempx;
	INT16U tempy;
	extern short servo0TargetAngle;
	extern short servo1TargetAngle;

	while (1) {
		buf = (char *) OSQPend(cmdQ, 0, &err);

		if (err != OS_NO_ERR) {
			printf("Queue pend error\n");
		}

		command_type = process_cmd_buf(buf, &command_arg);

		switch (command_type) {
		case PING:
			uart_write(ACK_STR, ACK_LEN);
			printf("Received PING\n");
			break;
		case SET_X_ACC:
			servo0TargetAngle = (short) command_arg;
			uart_write(ACK_STR, ACK_LEN);
			printf("Received SET_X_ACC with arg: %d\n", command_arg);
			break;
		case SET_Y_ACC:
			servo1TargetAngle = (short) command_arg;
			uart_write(ACK_STR, ACK_LEN);
			printf("Received SET_Y_ACC with arg: %d\n", command_arg);
			break;
		case GET_X_ACC:
			//sprintf(resp_buf, "%s%c%c", CMD_GETX, (char) ((tempx & 0xff00) >> 8), (char) (tempx & 0x00ff));
			sprintf(resp_buf, "%s%.4x", CMD_GETX, (INT16U) servo0TargetAngle);
			uart_write(resp_buf, CMD_LEN + CMD_ARGLEN);
			printf("Received GET_X_ACC\n");
			break;
		case GET_Y_ACC:
			//sprintf(resp_buf, "%s%c%c", CMD_GETY, (char) ((tempy & 0xff00) >> 8), (char) (tempy & 0x00ff));
			sprintf(resp_buf, "%s%.4x", CMD_GETY, (INT16U) servo1TargetAngle);
			uart_write(resp_buf, CMD_LEN + CMD_ARGLEN);
			printf("Received GET_Y_ACC\n");
			break;
		case INVALID:
			uart_write(NACK_STR, NACK_LEN);
			printf("Received INVALID\n");
			break;
		}

		free(buf);
	}
}

int main(void)
{
	printf("Hello world\n");

	cmdQ = OSQCreate(&cmdarr[0], 16);
	servo0Sem = OSSemCreate(1);
	servo1Sem = OSSemCreate(1);

	uart_init(cmdQ);
	servo_init(servo0Sem, servo1Sem);

	OSTaskCreateExt(servotask,
				NULL,
				(void *)&task_servo_stk[TASK_STACKSIZE-1],
				TASK_SERVO_PRIORITY,
				TASK_SERVO_PRIORITY,
				task_servo_stk,
				TASK_STACKSIZE,
				NULL,
				0);

	OSTaskCreateExt(maintask,
			NULL,
			(void *)&task_main_stk[TASK_STACKSIZE-1],
			TASK_MAIN_PRIORITY,
			TASK_MAIN_PRIORITY,
			task_main_stk,
			TASK_STACKSIZE,
			NULL,
			0);

	OSStart();
	return 0;
}
