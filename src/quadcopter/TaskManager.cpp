/*
 * TaskManager.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: Gabriel Fernandes
 */


#include "TaskManager.h"

int TaskManager::counter = 0;
uint64_t TaskManager::_hz[TASK_SIZE] = {};
uint64_t TaskManager::_timer[TASK_SIZE] = {};
Function TaskManager::_function[TASK_SIZE] = {};

void TaskManager::addTask(Function function, int hz) {
	_function[counter] = function;
	_hz[counter] = (1000000/hz);
	counter++;
}

void TaskManager::clearTasks() {
	counter = 0;
}

void TaskManager::run() {

	for(int i=0;i<counter;i++) {

		uint64_t time = micros();
		if((time - _timer[i]) >= (_hz[i])) {
			(*_function[i])();
			_timer[i] = time;
		}

	}

}
