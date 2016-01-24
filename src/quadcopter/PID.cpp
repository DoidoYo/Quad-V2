// 
// 
// 

#include "PID.h"

void PID::init(float kp, float ki, float kd, float max)
{
	_kp = kp;
	_ki = ki;
	_kd = kd;
	_max = max;

	_error_sum = 0;
	_last_error = 0;
}

float PID::compute(float input, float set) {
	_error = input - set;

	_error_sum += _ki * _error; //compute integral error
	if (_error_sum > _max) { _error_sum = _max; }  //account for restrictions
	else if (_error_sum < -_max) { _error_sum = -_max; }; //yeet

	_out = (_kp * _error) +             //P
		(_error_sum) +                  //I
		(_kd * (_error - _last_error)); //D

	if (_out > _max) { _out = _max; }
	else if (_out < -_max) { _out = -_max; };

	_last_error = _error;

	return _out;
}

void PID::reset() {
	_error_sum = 0;
	_last_error = 0;
}

void PID::setTuning(float kp, float ki, float kd, float max) {
	_kp = kp;
	_ki = ki;
	_kd = kd;
	_max = max;
}
