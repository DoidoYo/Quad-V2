/*
 * Tasks.cpp
 *
 *  Created on: Jan 24, 2016
 *      Author: Gabriel Fernandes
 */

#include "quad.h"

uint64_t armedTimer;
void TASK_controller()
{

	//smoothing gyro rate for rate mode
	gyroRateSmooth.x = (gyroRateSmooth.x * .8) + (gyroRate.x * .2);
	gyroRateSmooth.y = (gyroRateSmooth.y * .8) + (gyroRate.y * .2);
	gyroRateSmooth.z = (gyroRateSmooth.z * .8) + (gyroRate.z * .2);

	/*accelSmooth.x = (accelSmooth.x * .8) + (accelAccel.x * .2);
	accelSmooth.y = (accelSmooth.y * .8) + (accelAccel.y * .2);
	accelSmooth.z = (accelSmooth.z * .8) + (accelAccel.z * .2);*/

	//getting angle from gyro rate
	/*gyroAngle.x += gyroRateSmooth.x * .004;
	gyroAngle.y += gyroRateSmooth.y * .004;
	gyroAngle.z += gyroRateSmooth.z * .004;*/


	//angle from accelerometer
	accelAngle.x =  atan2(accelAccel.y, sqrt(pow(accelAccel.x,2) + pow(accelAccel.z,2))) * (180/M_PI);
	accelAngle.y = -atan2(accelAccel.x, sqrt(pow(accelAccel.y,2) + pow(accelAccel.z,2))) * (180/M_PI);

	//combining the two
	angle.x = .8 * (angle.x + gyroRate.x * .004) + (.2 * accelAngle.x);
	angle.y = .8 * (angle.y + gyroRate.y * .004) + (.2 * accelAngle.y);

	//printf2("angle: %i \t %i \t|%i \t %i \n", (int)angle.x,(int)angle.y,(int)gyroAngle.x,(int)gyroAngle.y);


	if (inputConnected)
	{
		//flight loop
		if (Armed() && input[RC_THROTTLE] > 1100)
		{
			armedTimer = millis();

			//scale input accordingly

			Mode = (input[RC_AUX1] > 1450)?MODE_STAB: MODE_GYRO;

			pos3D quadSet;
			vector quadPos;

			scaledInput[RC_THROTTLE] = (input[RC_THROTTLE] > 1800) ? 1800 : input[RC_THROTTLE];
			quadSet.z = map(input[RC_YAW], STICK_MIN, STICK_MAX, -RATE_YAW_SCALE, RATE_YAW_SCALE, STICK_DEAD_MIN, STICK_DEAD_MAX);
			if (Mode == MODE_GYRO)
			{
				quadSet.x = map(input[RC_ROLL], STICK_MIN, STICK_MAX, -RATE_ROLL_SCALE, RATE_ROLL_SCALE, STICK_DEAD_MIN, STICK_DEAD_MAX);
				quadSet.y = map(input[RC_PITCH], STICK_MIN, STICK_MAX, -RATE_PITCH_SCALE, RATE_PITCH_SCALE, STICK_DEAD_MIN, STICK_DEAD_MAX);

				quadPos = gyroRateSmooth;
			}
			else if (Mode == MODE_STAB || Mode == MODE_BARO)
			{
				quadSet.x = map(input[RC_ROLL], STICK_MIN, STICK_MAX, -STAB_ROLL_SCALE, STAB_ROLL_SCALE, STICK_DEAD_MIN, STICK_DEAD_MAX);
				quadSet.y = map(input[RC_PITCH], STICK_MIN, STICK_MAX, -STAB_PITCH_SCALE, STAB_PITCH_SCALE, STICK_DEAD_MIN, STICK_DEAD_MAX);

				quadPos = accelAngle;

				if (Mode == MODE_BARO)
				{
					//change throttle
				}
			}

			//printf2("%i \t %i \t %i \n", (int)quadPos.x, (int)quadPos.y, (int)quadPos.z);

			quadPos.z = gyroRateSmooth.z;

			processQuad(scaledInput[RC_THROTTLE], quadPos, quadSet);
		}
		else if (Armed())
		{
			if ((millis() - armedTimer) > ARMED_TIMEOUT)
			{
				Disarm();
			}
			else
			{
				//idle state?
				motorsSetAll(1100);

				pid[ROLL].reset();
				pid[PITCH].reset();
				pid[YAW].reset();
			}
		}
	}
	else
	{
		//Disarm();
		//descend or something
	}

}

void TASK_gyro()
{
	gyro.read(gyroRate);
	gyroRate.z *= -1;
}

void TASK_accel()
{
	accel.read(accelAccel);
	//f2("accel: %i \t %i \t %i \n", (int) accelAccel.x, (int) accelAccel.y, (int) accelAccel.z);
}

void TASK_checkReceiver()
{
	inputCheck();
}

uint64_t stickTimer;
void TASK_stickReader()
{
	int delayOver = ((millis() - stickTimer) > STICK_TIMEOUT) ? 1 : 0;

	//arm
	if (STICKRIGHT(input[RC_YAW]) && STICKDOWN(input[RC_THROTTLE]))
	{
		if (delayOver)
		{
			Arm();
			armedTimer = millis();
		}
	}
	else if (STICKLEFT(input[RC_YAW]) && STICKDOWN(input[RC_THROTTLE]))
	{
		if (delayOver)
		{
			Disarm();
		}
	}
	else if (STICKDOWN(input[RC_PITCH]) && STICKRIGHT(input[RC_ROLL]))
	{
		if (delayOver && !Armed())
		{
			LED_R.high();
			gyro.calibrate(LED_G);
			LED_R.low();
		}
	}
	else if (STICKDOWN(input[RC_PITCH]) && STICKLEFT(input[RC_ROLL]))
	{
		if (delayOver && !Armed())
		{
			LED_G.high();
			accel.calibrate(LED_R);
			LED_G.low();
		}
	}
	else
	{
		stickTimer = millis();
	}
}
