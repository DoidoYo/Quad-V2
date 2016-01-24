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
		if (Armed() && input[RC_THROTTLE] > 1050) {
			armedTimer = millis();

			int roll_scale, pitch_scale;
			float pid_roll_set, pid_pitch_set;

			if (Mode == MODE_GYRO) {

				roll_scale = RATE_ROLL_SCALE;
				pitch_scale = RATE_PITCH_SCALE;

				pid_roll_set = angleRate.x;
				pid_pitch_set = angleRate.y;

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
			pidOut[ROLL] = pid[ROLL].compute(pid_roll_set,
					-scaledInput[RC_ROLL]);
			pidOut[PITCH] = pid[PITCH].compute(pid_pitch_set,
					-scaledInput[RC_PITCH]);
			pidOut[YAW] = pid[YAW].compute(angleRate.z, -scaledInput[RC_YAW]);


			motors[M_FR] = scaledInput[RC_THROTTLE] + pidOut[ROLL] + pidOut[PITCH] - pidOut[YAW];
			motors[M_FL] = scaledInput[RC_THROTTLE] - pidOut[ROLL] + pidOut[PITCH] + pidOut[YAW];
			motors[M_BL] = scaledInput[RC_THROTTLE] - pidOut[ROLL] - pidOut[PITCH] - pidOut[YAW];
			motors[M_BR] = scaledInput[RC_THROTTLE] + pidOut[ROLL] - pidOut[PITCH] + pidOut[YAW];

			motorsUpdate();

		} else if (Armed()) {
			if ((millis() - armedTimer) > ARMED_TIMEOUT) {
				Disarm();
			} else {
				//idle state?
				motorsSetAll(1100);

				pid[ROLL].reset();
				pid[PITCH].reset();
				pid[YAW].reset();
			}
		}
	} else {
		Disarm();
		//descend or something
	}
}

void TASK_gyro() {
	vector v;
	gyro.read(v);
	v.z *= -1;

	angleRate.x = (angleRate.x * .8) + (v.x * .2);
	angleRate.y = (angleRate.y * .8) + (v.y * .2);
	angleRate.z = (angleRate.z * .8) + (v.z * .2);

	//printf2("rate: %i \t %i \t %i \n", (int)angleRate.x,(int)angleRate.y,(int)angleRate.z);

}

void TASK_checkReceiver() {
	inputCheck();
}

uint64_t stickTimer;
void TASK_stickReader() {
	int delayOver = ((millis() - stickTimer) > STICK_TIMEOUT) ? 1 : 0;

	//arm
	if (STICKRIGHT(input[RC_YAW]) && STICKDOWN(input[RC_THROTTLE])) {
		if (delayOver) {
			Arm();
			armedTimer = millis();
		}
	} else if (STICKLEFT(input[RC_YAW]) && STICKDOWN(input[RC_THROTTLE])) {
		if (delayOver) {
			Disarm();
		}
	} else if (STICKDOWN(input[RC_PITCH]) && STICKRIGHT(input[RC_ROLL])) {
		if (delayOver) {
			LED_R.high();
			gyro.calibrate();
			LED_R.low();
		}
	} else {
		stickTimer = millis();
	}
}
