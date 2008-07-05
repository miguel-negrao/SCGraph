#ifndef SCGRAOH_BINARY_OP_HH
#define SCGRAOH_BINARY_OP_HH

#include "../unit.h"

class BinaryOp : public GUnit
{
	int _special_index;

	public:
		BinaryOp (int special_index);
		~BinaryOp ();

		virtual void process_g (double delta_t);
		virtual void process_c (double delta_t);
};

#endif
