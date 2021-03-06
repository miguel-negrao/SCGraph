#include "sum4.h"

#include <iostream>

extern "C"
{
	GUnit *create (size_t index, int special_index)
	{
		return (GUnit*) new Sum4 (); 
	}

	size_t get_num_of_units ()
	{
		return 1;
	}

	const char *get_unit_name (size_t index)
	{
		if (index == 0)
			return "Sum4";
		else
			return 0;
	}
}


Sum4::Sum4 ()
{
}

Sum4::~Sum4 ()
{
	// std::cout << "[Sum4]: Destructor" << std::endl;
}

void Sum4::process_g (double delta_t)
{
	/* throw away olde output */
	_graphics_outs[0]._graphics.clear ();
	
	/* and copy the inputs to da outputs */
	for (size_t i = 0; i < _graphics_ins[0]->_graphics.size (); ++i)
		{
			_graphics_outs[0]._graphics.push_back (_graphics_ins[0]->_graphics[i]);
		}
	/* [of all inputs :)] */
	for (size_t i = 0; i < _graphics_ins[1]->_graphics.size (); ++i)
		{
			_graphics_outs[0]._graphics.push_back (_graphics_ins[1]->_graphics[i]);
		}
	/* [of all inputs :)] */
	for (size_t i = 0; i < _graphics_ins[2]->_graphics.size (); ++i)
		{
			_graphics_outs[0]._graphics.push_back (_graphics_ins[2]->_graphics[i]);
		}
	/* [of all inputs :)] */
	for (size_t i = 0; i < _graphics_ins[3]->_graphics.size (); ++i)
		{
			_graphics_outs[0]._graphics.push_back (_graphics_ins[3]->_graphics[i]);
		}
}

void Sum4::process_c (double delta_t)
{
	_control_outs[0] = *_control_ins[0] + *_control_ins[1] + *_control_ins[2] +  *_control_ins[3];
}
