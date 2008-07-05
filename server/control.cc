#include "control.h"

Control::Control (float value) :
	_value (value)
{

}

void Control::process_g (double delta_t)
{

}

void Control::process_c (double delta_t)
{
	for (size_t i = 0; i < (_control_ins.size() < _control_outs.size() ? _control_ins.size() : _control_outs.size()); ++i)
	{
		_control_outs[i] = *_control_ins[i];
	}
}



