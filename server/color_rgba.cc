#include "color_rgba.h"

/** RED GREEN BLUE ALPHA */
ColorRGBA::ColorRGBA (float r, float g, float b, float a)
{
	_c[0] = r;
	_c[1] = g;
	_c[2] = b;
	_c[3] = a;
}
