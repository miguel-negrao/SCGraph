#ifndef SCGRAPH_TRIANGLE_HH
#define SCGRAPH_TRIANGLE_HH

#include "../unit.h"

class Point : public GUnit
{
	cow_ptr<Geometry> _g;

	int _segments;

	public:
		Point ();
		~Point ();

		virtual void process_g (double delta_t);
};

#endif
