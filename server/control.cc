#include "control.h"

Control::Control (int special_index) :
	_special_index (special_index)
{

}

void Control::process_g (double delta_t)
{

}

void Control::process_c (double delta_t)
{
	for (size_t i = 0; i < _control_outs.size(); ++i)
	{
		_control_outs[i] = *(_control_ins[0] + _special_index + i);
	}
}



