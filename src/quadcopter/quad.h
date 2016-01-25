/*
 * quad.h
 *
 *  Created on: Jan 23, 2016
 *      Author: Gabriel Fernandes
 */

#ifndef QUAD_H_
#define QUAD_H_

#include "stm32f10x.h"

#include "math.h"

#include "libs/USART1.h"
#include "libs/Timer.h"
#include "libs/Util.h"

#include "quadcopter/Input.h"
#include "quadcopter/PID.h"
#include "quadcopter/Motors.h"
#include "quadcopter/TaskManager.h"

#include "libs/I2C.h"
#include "libs/GPIO.h"

#include "sensors/BMP180.h"
#include "sensors/L3G4200D.h"
#include "sensors/ADXL345.h"

///////////////////////////////////////
/////      STICK SCALING     //////////
//////////////////////////////////////

#define RATE_ROLL_SCALE  120
#define RATE_PITCH_SCALE 120
#define RATE_YAW_SCALE   120

#define STAB_ROLL_SCALE 45
#define STAB_PITCH_SCALE 45

//////////////////////////////////

#define ROLL_KP 1.3
#define ROLL_KI 0.05
#define ROLL_KD 15
#define ROLL_MAX 400

#define PITCH_KP 1.3
#define PITCH_KI 0.05
#define PITCH_KD 15
#define PITCH_MAX 400

#define YAW_KP 4
#define YAW_KI 0.02
#define YAW_KD 0
#define YAW_MAX 400

#define ROLL 0
#define PITCH 1
#define YAW 2
///////////////////////////////

#define MOTOR_MIN 1000
#define MOTOR_MAX 2000

#define ARMED 1
#define UNARMED 0

#define ARMED_TIMEOUT 5000
#define STICK_TIMEOUT 1500

#define STICK_DEAD 50
#define STICK_HIGH 1800
#define STICK_LOW 1100
#define STICK_MAX 1000
#define STICK_MIN 1900
#define STICK_DEAD_MAX ((STICK_MAX+STICK_MIN)/2)+50
#define STICK_DEAD_MIN ((STICK_MAX+STICK_MIN)/2)-50

///////////////////////////////

#define MODE_GYRO 0
#define MODE_STAB 1
#define MODE_BARO 2
#define MODE_LOST 3
#define MODE_GPS  4

//////////////////////////////////////
#define BUTTON_PIN GPIO_Pin_0
#define BUTTON_PORT GPIOA

#define LEDR_PIN GPIO_Pin_5
#define LEDR_PORT GPIOA

#define LEDG_PIN GPIO_Pin_4
#define LEDG_PORT GPIOA

#define BUZZER_PIN GPIO_Pin_5
#define BUZZER_PORT GPIOB
////////////////////////////////

extern GPIO LED_G;
extern GPIO LED_R;
extern GPIO LED_SYS;
extern GPIO BUZZER;
extern GPIO BUTTON;

extern I2C i2c;

extern BMP180 baro;
extern L3G4200D gyro;
extern ADXL345 accel;

extern int _armed, Mode;

extern int scaledInput[6];

extern vector gyroRate, gyroRateSmooth, gyroAngle;
extern vector accelAccel,accelSmooth, accelAngle;
extern vector angle;

extern float pidOut[3];
extern PID pid[3];

void computeFlight(int throttle, int roll, int pitch, int yaw, int mode);

void TASK_gyro();
void TASK_accel();
void TASK_checkReceiver();
void TASK_stickReader();
void TASK_controller();

void Arm();
void Disarm();
int Armed();

void processQuad(int throttle, pos3D desired, vector current);

///////////////////////////

void constrain(int &i, int min, int max);
int map(int x, int min, int max, int nmin, int nmax, int deadmin,
		int deadmax);
int STICKRIGHT(int s);
int STICKLEFT(int s);
int STICKUP(int s);
int STICKDOWN(int s);
////////////////////////////
#endif /* QUAD_H_ */
