#include "quad.h"

#include <iostream>

extern "C"
{
	GUnit *create (size_t index, int special_index)
	{
		if (index == 0)
			return (GUnit*) new Quad (); 

		if (index == 1)
			return (GUnit*) new TexQuad (); 

		return 0;
	}

	size_t get_num_of_units ()
	{
		return 2;
	}

	const char *get_unit_name (size_t index)
	{
		if (index == 0)
			return "GQuad";

		if (index == 1)
			return "GTexQuad";

		return 0;
	}
}


Quad::Quad () :
	_g(new Geometry)
{
	/* create face data - only one face now */
	cow_ptr<Face> face (new Face);
	face.touch()->_geometry_type = Face::QUADS;

	// face.touch()->_material._emissive_color._c[0] = 1.0;

	face.touch()->_vertices.push_back (Vector3D ());
	face.touch()->_normals.push_back (Vector3D (0,0,1));

	face.touch()->_vertices.push_back (Vector3D ());
	face.touch()->_normals.push_back (Vector3D (0,0,1));

	face.touch()->_vertices.push_back (Vector3D ());
	face.touch()->_normals.push_back (Vector3D (0,0,1));

	face.touch()->_vertices.push_back (Vector3D ());
	face.touch()->_normals.push_back (Vector3D (0,0,1));

	face.touch()->_face_color = ColorRGBA (1,1,1,1);

	_g.touch()->_faces.push_back (face);
}

Quad::~Quad ()
{
	// std::cout << "[Quad]: Destructor" << std::endl;
}

void Quad::process_g (double delta_t)
{
	_graphics_outs[0]._graphics.clear();

	for (size_t i = 0; i < 3; ++i)
	{
		_g.touch()->_faces[0].touch()->_vertices[0]._c[i] = *_control_ins[i];
		_g.touch()->_faces[0].touch()->_vertices[1]._c[i] = *_control_ins[i+3];
		_g.touch()->_faces[0].touch()->_vertices[2]._c[i] = *_control_ins[i+6];
		_g.touch()->_faces[0].touch()->_vertices[3]._c[i] = *_control_ins[i+9];

		_g.touch()->_faces[0].touch()->_normals[0]._c[i] = *_control_ins[i+12];
		_g.touch()->_faces[0].touch()->_normals[1]._c[i] = *_control_ins[i+12];
		_g.touch()->_faces[0].touch()->_normals[2]._c[i] = *_control_ins[i+12];
		_g.touch()->_faces[0].touch()->_normals[3]._c[i] = *_control_ins[i+12];
	}

	_graphics_outs[0]._graphics.push_back (_g);
}


TexQuad::TexQuad () :
	_g(new Geometry)
{
	/* create face data - only one face now */
	cow_ptr<Face> face (new Face);
	face.touch()->_geometry_type = Face::QUADS;

	// face.touch()->_material._emissive_color._c[0] = 1.0;

	for (int i = 0; i < 4; ++i)
	{
		face.touch()->_vertices.push_back (Vector3D ());
		face.touch()->_normals.push_back (Vector3D ());
		face.touch()->_texture_coordinates.push_back (Vector2D());
	}

	face.touch()->_face_color = ColorRGBA (1,1,1,1);

	_g.touch()->_faces.push_back (face);
}

TexQuad::~TexQuad ()
{
	// std::cout << "[TexQuad]: Destructor" << std::endl;
}

void TexQuad::process_g (double delta_t)
{
	// std::cout << "texquad" << std::endl;
	_graphics_outs[0]._graphics.clear();

	for (size_t i = 0; i < 3; ++i)
	{
		_g.touch()->_faces[0].touch()->_vertices[0]._c[i] = *_control_ins[i];
		_g.touch()->_faces[0].touch()->_vertices[1]._c[i] = *_control_ins[i+3];
		_g.touch()->_faces[0].touch()->_vertices[2]._c[i] = *_control_ins[i+6];
		_g.touch()->_faces[0].touch()->_vertices[3]._c[i] = *_control_ins[i+9];

		_g.touch()->_faces[0].touch()->_normals[0]._c[i] = *_control_ins[i+21];
		_g.touch()->_faces[0].touch()->_normals[1]._c[i] = *_control_ins[i+21];
		_g.touch()->_faces[0].touch()->_normals[2]._c[i] = *_control_ins[i+21];
		_g.touch()->_faces[0].touch()->_normals[3]._c[i] = *_control_ins[i+21];
	}

	for (size_t i = 0; i < 2; ++i)
	{
		_g.touch()->_faces[0].touch()->_texture_coordinates[0]._c[i] = *_control_ins[i+12];
		_g.touch()->_faces[0].touch()->_texture_coordinates[1]._c[i] = *_control_ins[i+14];
		_g.touch()->_faces[0].touch()->_texture_coordinates[2]._c[i] = *_control_ins[i+16];
		_g.touch()->_faces[0].touch()->_texture_coordinates[3]._c[i] = *_control_ins[i+18];
	}

	// std::cout << (unsigned int)*_control_ins[20] << std::endl;
	_g.touch()->_faces[0].touch()->_texture_index = (unsigned int)*_control_ins[20];

	// std::cout << (unsigned int)*_control_ins[20] << std::endl;

	_graphics_outs[0]._graphics.push_back (_g);
}
