/*
 * quad.cpp
 *
 *  Created on: Jan 24, 2016
 *      Author: Gabriel Fernandes
 */

#include "quad.h"

GPIO LED_G;
GPIO LED_R;
GPIO LED_SYS;
GPIO BUZZER;
GPIO BUTTON;

I2C i2c;

BMP180 baro;
L3G4200D gyro;
ADXL345 accel;

int _armed = UNARMED, Mode = MODE_GYRO;

int scaledInput[6];

vector gyroRate, gyroRateSmooth, gyroAngle;
vector accelAccel, accelSmooth, accelAngle;
vector angle;

float pidOut[3];
PID pid[3];

void processQuad(int throttle, pos3D desired, vector current)
{
	//if flying
	pidOut[ROLL] = pid[ROLL].compute(current.x, desired.x);
	pidOut[PITCH] = pid[PITCH].compute(current.y, desired.y);
	pidOut[YAW] = pid[YAW].compute(current.z, desired.z);

	motors[M_FR] = throttle + pidOut[ROLL] + pidOut[PITCH] - pidOut[YAW];
	motors[M_FL] = throttle - pidOut[ROLL] + pidOut[PITCH] + pidOut[YAW];
	motors[M_BL] = throttle - pidOut[ROLL] - pidOut[PITCH] - pidOut[YAW];
	motors[M_BR] = throttle + pidOut[ROLL] - pidOut[PITCH] + pidOut[YAW];

	constrain(motors[M_FR], 1100, MOTOR_MAX);
	constrain(motors[M_FL], 1100, MOTOR_MAX);
	constrain(motors[M_BL], 1100, MOTOR_MAX);
	constrain(motors[M_BR], 1100, MOTOR_MAX);

	motorsUpdate();

}

void Arm()
{
	LED_G.high();
	_armed = ARMED;
}

void Disarm()
{
	LED_G.low();
	motorsSetAll(1000);
	_armed = UNARMED;
}

int Armed()
{
	return _armed;
}

void constrain(int &i, int min, int max)
{
	if (i > max)
	{
		i = max;
	}
	if (i < min)
	{
		i = min;
	}
}

int map(int x, int min, int max, int nmin, int nmax, int deadmin, int deadmax)
{

	int nx = 0;

	if (x < deadmax && x > deadmin)
	{
		return (nmin + nmax) / 2;
	}
	nx = (x - min) * (nmax - nmin) / (max - min) + nmin;
	if (nx > nmax)
	{
		return nmax;
	}
	if (nx < nmin)
	{
		return nmin;
	}
	return nx;
}

int STICKRIGHT(int s)
{
	return ((s > STICK_HIGH) ? 1 : 0);
}
int STICKLEFT(int s)
{
	return ((s < STICK_LOW) ? 1 : 0);
}
int STICKUP(int s)
{
	return ((s > STICK_HIGH) ? 1 : 0);
}
int STICKDOWN(int s)
{
	return ((s < STICK_LOW) ? 1 : 0);
}
