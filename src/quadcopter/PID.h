// PID.h

#ifndef _PID_h
#define _PID_h

class PID
{
 private:
	 float _kp, _ki, _kd, _max;
	 float _error, _error_sum, _last_error, _set, _in, _out;

 public:
	void init(float kp, float ki, float kd, float max);
	float compute(float input, float set);
	void reset();
	void setTuning(float kp, float ki, float kd, float max);
};

#endif

