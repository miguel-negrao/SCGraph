#ifndef SCGRAOH_INOUT_HH
#define SCGRAOH_INOUT_HH

#include "unit.h"

class In : public GUnit
{
	public:
		In ()
		{
			/* we have a single graphics rate output */
			// cow_ptr<GraphicsBus> b (new GraphicsBus);
			// _graphics_outs.push_back (b);
		}

		void process_g (double delta_t);
		void process_c (double delta_t);
};

// TODO: Multi in and multi out

class Out : public GUnit
{
	public:
		Out ()
		{
			/* we have no outputs, thus, nothing to do */
		}

		void process_g (double delta_t);
		void process_c (double delta_t);
};

class ReplaceOut : public GUnit
{
	public:
		ReplaceOut ()
		{
			/* we have no outputs, thus, nothing to do */
		}

		void process_g (double delta_t);
		void process_c (double delta_t);
};

#endif

