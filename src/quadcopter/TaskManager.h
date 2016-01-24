/*
 * TaskManager.h
 *
 *  Created on: Jan 21, 2016
 *      Author: Gabriel Fernandes
 */

#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

#include "stm32f10x.h"
#include "libs/Timer.h"

#define TASK_SIZE 100

typedef void (*Function)();

class TaskManager {

public:

	static void addTask(Function, int);
	static void clearTasks();
	static void run();

private:
	static int counter;
	static Function _function[TASK_SIZE];
	static uint64_t _hz[TASK_SIZE];
	static uint64_t _timer[TASK_SIZE];

};

#endif /* TASKMANAGER_H_ */
