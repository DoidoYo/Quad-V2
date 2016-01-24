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

int _armed = UNARMED, Mode = MODE_GYRO;

int scaledInput[6];
vector angleRate;

float pidOut[3];
PID pid[3];

void Arm() {
	LED_G.high();
	_armed = ARMED;
}

void Disarm() {
	LED_G.low();
	motorsSetAll(1000);
	_armed = UNARMED;
}

int Armed() {
	return _armed;
}

int map(int x, int min, int max, int nmin, int nmax, int deadmin, int deadmax) {

	int nx = 0;

	if (x < deadmax && x > deadmin) {
		return (nmin + nmax) / 2;
	}
	nx = (x - min) * (nmax - nmin) / (max - min) + nmin;
	if (nx > nmax) {
		return nmax;
	}
	if (nx < nmin) {
		return nmin;
	}
	return nx;
}

int STICKRIGHT(int s) {
	return ((s > STICK_HIGH) ? 1 : 0);
}
int STICKLEFT(int s) {
	return ((s < STICK_LOW) ? 1 : 0);
}
int STICKUP(int s) {
	return ((s > STICK_HIGH) ? 1 : 0);
}
int STICKDOWN(int s) {
	return ((s < STICK_LOW) ? 1 : 0);
}
