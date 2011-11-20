#include "material.h"

/** a material */
Material::Material () :
	_shinyness (0.0)
{
	for (int i = 0; i < 4; ++i)
	{	
		_ambient_reflection._c[i] = 1.0;
		_diffuse_reflection._c[i] = 1.0;
		_specular_reflection._c[i] = 1.0;
		_emissive_color._c[i] = 0.0;
	}
}

