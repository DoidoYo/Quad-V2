// PID.h

#ifndef _PID_h
#define _PID_h

class PID
{
 private:
	 double _kp, _ki, _kd, _max;
	 double _error, _error_sum, _last_error, _set, _in, _out;

 public:
	void init(double kp, double ki, double kd, double max);
	double compute(double input, double set);
	void reset();
	void setTuning(double kp, double ki, double kd, double max);
};

#endif

