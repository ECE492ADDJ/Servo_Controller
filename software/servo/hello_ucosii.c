/*************************************************************************
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
* All rights reserved. All use of this software and documentation is     *
* subject to the License Agreement located at the end of this file below.*
**************************************************************************
* Description:                                                           *
* The following is a simple hello world program running MicroC/OS-II.The * 
* purpose of the design is to be a very simple application that just     *
* demonstrates MicroC/OS-II running on NIOS II.The design doesn't account*
* for issues such as checking system call return codes. etc.             *
*                                                                        *
* Requirements:                                                          *
*   -Supported Example Hardware Platforms                                *
*     Standard                                                           *
*     Full Featured                                                      *
*     Low Cost                                                           *
*   -Supported Development Boards                                        *
*     Nios II Development Board, Stratix II Edition                      *
*     Nios Development Board, Stratix Professional Edition               *
*     Nios Development Board, Stratix Edition                            *
*     Nios Development Board, Cyclone Edition                            *
*   -System Library Settings                                             *
*     RTOS Type - MicroC/OS-II                                           *
*     Periodic System Timer                                              *
*   -Know Issues                                                         *
*     If this design is run on the ISS, terminal output will take several*
*     minutes per iteration.                                             *
**************************************************************************/


#include <stdio.h>
#include "includes.h"

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];
OS_STK    task3_stk[TASK_STACKSIZE];

#define WAIT_FOREVER 		0

OS_EVENT *servo1Sem;
OS_EVENT *servo2Sem;
INT8U err;

/* Definition of Task Priorities */

#define TASK1_PRIORITY      1
#define TASK2_PRIORITY      2
#define TASK3_PRIORITY      3

/* Max rotation of our hardware is 60 degrees in either direction */
#define MAX_ROTATION 80
#define MIN_ROTATION -80

int servo1Angle;
int servo2Angle;

/* Reads from keys 2 and 3, then changes the angle of servo 0 if pressed */
void task1(void* pdata)
{
	 int button3Value, button2Value, button1Value, button0Value;

	 int *button3 = (int *) KEY_3_BASE;
	 int *button2 = (int *) KEY_2_BASE;
	 int *button1 = (int *) KEY_1_BASE;
	 int *button0 = (int *) KEY_0_BASE;

  while (1)
  { 
	  button3Value = *button3;
	  button2Value = *button2;
	  button1Value = *button1;
	  button0Value = *button0;

	  if(button0Value == 0 || button1Value == 0){
		  OSSemPend(servo2Sem, 0, &err);
		  	  if(button0Value == 0){
		  		servo2Angle--;
		  	  }
		  	  else{
		  		servo2Angle++;
		  	  }
		  OSSemPost(servo2Sem);
	  }

	  if(button3Value == 0 || button2Value == 0){
		  OSSemPend(servo1Sem, 0, &err);
		  	  if(button3Value == 0){
		  		servo1Angle--;
		  	  }
		  	  else{
		  		servo1Angle++;
		  	  }
		  OSSemPost(servo1Sem);
	  }
	  OSTimeDlyHMSM(0, 0, 0, 20);
  }
}

/* Sends the current angle to servo 0 */
void task2(void* pdata)
{
	int *servoAddr0 = (int *) SERVO_0_BASE;
	int *servoAddr1 = (int *) SERVO_1_BASE;
  while (1){
	if(servo1Angle > MAX_ROTATION){
		servo1Angle = MAX_ROTATION;
	}else if(servo1Angle < MIN_ROTATION){
		servo1Angle = MIN_ROTATION;
	}

	if(servo2Angle > MAX_ROTATION){
		servo2Angle = MAX_ROTATION;
	}else if(servo2Angle < MIN_ROTATION){
		servo2Angle = MIN_ROTATION;
	}
	*servoAddr0 = servo1Angle;
	*servoAddr1 = servo2Angle;
	OSTimeDlyHMSM(0, 0, 0, 20);
  }
}

/* Serial commmunication task */
void task3(void* pdata)
{
  while (1){
	  OSSemPend(servo1Sem, 0, &err);
	  	  servo1Angle= (servo1Angle +10) % MAX_ROTATION;
	  OSSemPost(servo1Sem);
	OSTimeDlyHMSM(0, 0, 1, 0);
  }
}

/* The main function creates four task and starts multi-tasking */
/* NOTE: SW(0) MUST be in the upper position to run. Otherwise the reset will cause problems. */
int main(void)
{
  printf("Reset!\n");

  servo1Sem = OSSemCreate(1);
  servo2Sem = OSSemCreate(1);

  servo1Angle = 0;
  servo2Angle = 0;

  OSTaskCreateExt(task1,
                  NULL,
                  (void *)&task1_stk[TASK_STACKSIZE-1],
                  TASK1_PRIORITY,
                  TASK1_PRIORITY,
                  task1_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
              
               
  OSTaskCreateExt(task2,
                  NULL,
                  (void *)&task2_stk[TASK_STACKSIZE-1],
                  TASK2_PRIORITY,
                  TASK2_PRIORITY,
                  task2_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSTaskCreateExt(task3,
                  NULL,
                  (void *)&task3_stk[TASK_STACKSIZE-1],
                  TASK3_PRIORITY,
                  TASK3_PRIORITY,
                  task3_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSStart();
  return 0;
}

/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *
******************************************************************************/
