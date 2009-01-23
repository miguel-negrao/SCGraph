#include "control.h"

Control::Control (int special_index) :
	_special_index (special_index)
{
	// std::cout << "Control: special_index = " << _special_index << std::endl;
}

void Control::process_g (double delta_t)
{

}

void Control::process_c (double delta_t)
{
	std::cout << "si: " << _special_index;
	for (size_t i = 0; i < _control_outs.size(); ++i)
	{
		std::cout << " " << *(_control_ins[0] + _special_index + i);
		_control_outs[i] = *(_control_ins[0] + _special_index + i);
	}
	std::cout << std::endl;
}



