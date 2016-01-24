

#include "stm32f10x.h"

#include "libs/Timer.h"
#include "libs/USART1.h"

#include "quadcopter/quad.h"

int main(void) {

	initTimer();
	initUSART1(115200);

	inputInit();
	motorsInit();

	LED_G.init(LEDG_PORT, LEDG_PIN, GPIO_Mode_Out_PP);
	LED_R.init(LEDR_PORT, LEDR_PIN, GPIO_Mode_Out_PP);
	LED_SYS.init(GPIOB, GPIO_Pin_1, GPIO_Mode_Out_PP);
	BUZZER.init(BUZZER_PORT, BUZZER_PIN, GPIO_Mode_Out_PP);
	BUTTON.init(BUTTON_PORT, BUTTON_PIN, GPIO_Mode_IPU);

	LED_SYS.high();

	if(BUTTON.read() == 0) {
		LED_G.high();
		LED_R.high();
		motorsSetAll(MOTOR_MAX);
		delayMillis(4000);
		motorsSetAll(MOTOR_MIN);
		LED_G.low();
		LED_R.low();
	}

	i2c.init(I2C1, 100000);

	gyro.init(&i2c);

	LED_R.high();
	gyro.calibrate();
	LED_R.low();

	pid[ROLL].init(ROLL_KP, ROLL_KI, ROLL_KD, ROLL_MAX);
	pid[PITCH].init(PITCH_KP, PITCH_KI, PITCH_KD, PITCH_MAX);
	pid[YAW].init(YAW_KP, YAW_KI, YAW_KD, YAW_MAX);

	TaskManager::addTask(TASK_gyro, 250);
	TaskManager::addTask(TASK_controller, 250);
	TaskManager::addTask(TASK_checkReceiver, 55);
	TaskManager::addTask(TASK_stickReader, 20);

	while (1) {
		TaskManager::run();
	}

}
