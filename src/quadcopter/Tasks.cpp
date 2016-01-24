/*
 * Tasks.cpp
 *
 *  Created on: Jan 24, 2016
 *      Author: Gabriel Fernandes
 */

#include "quad.h"

uint64_t armedTimer;
void TASK_controller() {
	if (inputConnected) {
		//flight loop
		if (Armed == ARMED && scaledInput[RC_THROTTLE] > 1050) {
			armedTimer = millis();

			int roll_scale, pitch_scale;
			float roll_set, pitch_set;

			if (Mode == MODE_GYRO) {

				roll_scale = RATE_ROLL_SCALE;
				pitch_scale = RATE_PITCH_SCALE;

				roll_set = angle.x;
				pitch_set = angle.y;

			} else if (Mode == MODE_STAB || Mode == MODE_BARO) {

				roll_scale = STAB_ROLL_SCALE;
				pitch_scale = STAB_PITCH_SCALE;

				//compute attitude

				//make roll_set and pitch_set equal to current attitude
			}

			scaledInput[RC_THROTTLE] =
					(input[RC_THROTTLE] > 1800) ? 1800 : input[RC_THROTTLE];
			scaledInput[RC_YAW] = map(input[RC_YAW], STICK_MIN, STICK_MAX,
					-RATE_YAW_SCALE, RATE_YAW_SCALE, STICK_DEAD_MIN,
					STICK_DEAD_MAX);
			scaledInput[RC_ROLL] = map(input[RC_ROLL], STICK_MIN, STICK_MAX,
					-roll_scale, roll_scale, STICK_DEAD_MIN,
					STICK_DEAD_MAX);
			scaledInput[RC_PITCH] = map(input[RC_PITCH], STICK_MIN,
			STICK_MAX, -pitch_scale, pitch_scale,
			STICK_DEAD_MIN,
			STICK_DEAD_MAX);

			//if flying

		} else if (Armed == ARMED) {
			if ((millis() - armedTimer) > ARMED_TIMEOUT) {
				Armed = UNARMED;
			}

			//idle state?
			//make motors = 1100
		} else {
			//UNARMED
			//make motors = 1000
		}
	} else {
		Armed = UNARMED;
		//descend or something
	}
}

void TASK_gyro() {
	vector v;
	gyro.read(v);

	angle.x = (angle.x * .8) + (v.x * .2);
	angle.x = (angle.y * .8) + (v.y * .2);
	angle.x = (angle.z * .8) + (v.z * .2);
}

void TASK_checkReceiver() {
	checkReceiverConnection();
}

uint64_t stickTimer;
void TASK_stickReader() {
	int delayOver = ((millis() - stickTimer) > 1500) ? 1 : 0;

	//arm
	if (STICKRIGHT(input[RC_YAW]) && STICKDOWN(input[RC_THROTTLE])) {
		if (delayOver) {
			Armed = ARMED;
			armedTimer = millis();
		}
	} else if (STICKLEFT(input[RC_YAW]) && STICKDOWN(input[RC_THROTTLE])) {
		if (delayOver) {
			Armed = UNARMED;
		}
	} else if(STICKDOWN(input[RC_PITCH]) && STICKRIGHT(input[RC_ROLL])) {
		if (delayOver) {
			gyro.calibrate();
		}
	} else {
		stickTimer = millis();
	}
}
