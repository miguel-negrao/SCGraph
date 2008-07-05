#ifndef SCGRAOH_FREQ_AMP_HH
#define SCGRAOH_FREQ_AMP_HH

#include "../unit.h"

class FreqAmp : public GUnit
{
	float _value;

	public:
		FreqAmp ();
		~FreqAmp ();

		virtual void process_c (double delta_t);
};

#endif
