#include "binary_op.h"

#include <iostream>

extern "C"
{
	GUnit *create (size_t index, int special_index)
	{
		return (GUnit*) new BinaryOp (special_index); 
	}

	size_t get_num_of_units ()
	{
		return 1;
	}

	const char *get_unit_name (size_t index)
	{
		if (index == 0)
			return "BinaryOpUGen";
		else
			return 0;
	}
}


BinaryOp::BinaryOp (int special_index) :
	_special_index (special_index)
{
}

BinaryOp::~BinaryOp ()
{
	// std::cout << "[BinaryOp]: Destructor" << std::endl;
}

void BinaryOp::process_g (double delta_t)
{
	switch (_special_index)
	{
		case 0:
			{
				/* throw away olde output */
			  _graphics_outs[0]._graphics.clear ();

				/* and copy the inputs to da outputs */
				for (size_t i = 0; i < _graphics_ins[0]->_graphics.size (); ++i)
				{
					_graphics_outs[0]._graphics.push_back (_graphics_ins[0]->_graphics[i]);
				}
				/* [of both inputs :)] */
				for (size_t i = 0; i < _graphics_ins[1]->_graphics.size (); ++i)
				{
					_graphics_outs[0]._graphics.push_back (_graphics_ins[1]->_graphics[i]);
				}
			}
		break;

		default:
			//std::cout << "[BinaryOp]: BinaryOp ignored. Only + is supported for graphics rate right now. Special index: " << _special_index << std::endl;
		break;
	}
}

void BinaryOp::process_c (double delta_t)
{
	switch (_special_index)
	{
		case 0:
			_control_outs[0] = *_control_ins[0] + *_control_ins[1];
		break;

		case 1:
			_control_outs[0] = *_control_ins[0] - *_control_ins[1];
		break;

		case 2:
			_control_outs[0] = *_control_ins[0] * *_control_ins[1];
		break;

		case 3:
			_control_outs[0] = *_control_ins[0] / *_control_ins[1];
		break;

		default:
		break;
	}
}
