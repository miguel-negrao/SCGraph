#ifndef SCGRAPH_CONTROL_HH
#define SCGRAPH_CONTROL_HH

#include "unit.h"

class Control : public GUnit
{
	public:
		Control (float value = 0);

		void process_g (double delta_t);
		void process_c (double delta_t);

		float _value;
};

#endif

