/*
 * servo.h
 *
 *  Created on: Mar 17, 2017
 *      Author: djr1
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "includes.h"
#include "altera_avalon_pio_regs.h"

#define WAIT_FOREVER 		0

/* Max rotation of our hardware is 60 degrees in either direction */
#define MAX_ROTATION 80
#define MIN_ROTATION -80

/**
 * Initializes semaphores for servo operation
 * @param servo0Sem pointer to the semaphor for servo 0
 * @param servo1Sem pointer to the semaphor for servo 1
 */
void servo_init(OS_EVENT *servo0Sem, OS_EVENT *servo1Sem);

/**
 * Task for moving the servos, and checking the angle is within bounds
 * @param pdata
 */
void servotask(void *pdata);

/**
 * Reads when a button is pressed and increments the appropriate servo angle.
 */
void button_read();

/**
 * Ensures that the angle being sent to the servo does not exceed max rotations limits
 * @param angle Angle to be checked
 * @return Returns the original angle if within bounds, otherwise returns the bound exceeded.
 */
short checkRotationLimits(short angle);

#endif /* SERVO_H_ */
