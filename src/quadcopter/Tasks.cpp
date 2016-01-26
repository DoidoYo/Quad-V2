/*
 * Tasks.cpp
 *
 *  Created on: Jan 24, 2016
 *      Author: Gabriel Fernandes
 */

#include "quad.h"

uint16_t timeLast;

uint64_t armedTimer;
void TASK_controller()
{
	uint64_t timeNow = millis();
	uint64_t dt = timeNow - timeLast;
	timeLast = timeNow;

	//smoothing gyro rate for rate mode
	gyroRateSmooth.x = (gyroRateSmooth.x * .8) + (gyroRate.x * .2);
	gyroRateSmooth.y = (gyroRateSmooth.y * .8) + (gyroRate.y * .2);
	gyroRateSmooth.z = (gyroRateSmooth.z * .8) + (gyroRate.z * .2);

	accelSmooth.x = (accelSmooth.x * .8) + (accelAccel.x * .2);
	accelSmooth.y = (accelSmooth.y * .8) + (accelAccel.y * .2);
	accelSmooth.z = (accelSmooth.z * .8) + (accelAccel.z * .2);

	//getting angle from gyro rate
	gyroAngle.x += gyroRateSmooth.x * dt / 1000;
	gyroAngle.y += gyroRateSmooth.y * dt / 1000;
	gyroAngle.z += gyroRateSmooth.z * dt / 1000;


	//angle from accelerometer
	accelAngle.x =  atan2(accelSmooth.y, sqrt(pow(accelSmooth.x,2) + pow(accelSmooth.z,2))) * (180/M_PI);
	accelAngle.y = -atan2(accelSmooth.x, sqrt(pow(accelSmooth.y,2) + pow(accelSmooth.z,2))) * (180/M_PI);

	//combining the two
	angle.x = .85 * (angle.x + gyroAngle.x * (dt/1000)) + (.15 * accelAngle.x);
	angle.y = .85 * (angle.y + gyroAngle.y * (dt/1000)) + (.15 * accelAngle.y);
	printf2("angle: %i \t %i \t|%i \t %i \t|%i \t%i |%i \t%i \t%i \n", (int)angle.x,(int)angle.y,(int)gyroAngle.x,(int)gyroAngle.y,(int) accelAngle.x, (int) accelAngle.y, (int)gyroRateSmooth.x,(int)gyroRateSmooth.y,(int)gyroRateSmooth.z);

	if (inputConnected)
	{
		//flight loop
		if (Armed() && input[RC_THROTTLE] > 1100)
		{
			armedTimer = millis();

			//scale input accordingly

			pos3D quadSet;
			vector quadPos;

			int throttle;

			throttle = (input[RC_THROTTLE] > 1800) ? 1800 : input[RC_THROTTLE];
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

				quadPos = angle;

				if (Mode == MODE_BARO)
				{
					//change throttle
				}
			}

			//printf2("%i \t %i \t %i \n", (int)quadPos.x, (int)quadPos.y, (int)quadPos.z);

			quadPos.z = gyroRateSmooth.z;

			processQuad(throttle, quadPos, quadSet);
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
