#include "bordered_rectangle.h"

#include <iostream>

extern "C"
{
	GUnit *create (size_t index, int special_index)
	{
		return (GUnit*) new BorderedRectangle (); 
	}

	size_t get_num_of_units ()
	{
		return 1;
	}

	const char *get_unit_name (size_t index)
	{
		if (index == 0)
			return "GBorderedRectangle";
		else
			return 0;
	}
}


BorderedRectangle::BorderedRectangle () :
	_g(new Geometry)
{
	/* create face data - only one face now */
	cow_ptr<Face> face (new Face);
	face.touch()->_geometry_type = Face::QUADS;

	// face->_material._emissive_color._c[0] = 1.0;

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

	// this is the outline
	cow_ptr<Face> outline (new Face);
	outline.touch()->_geometry_type = Face::LINE_LOOP;

	outline.touch()->_vertices.push_back (Vector3D ());
	outline.touch()->_normals.push_back (Vector3D (0,0,1));

	outline.touch()->_vertices.push_back (Vector3D ());
	outline.touch()->_normals.push_back (Vector3D (0,0,1));

	outline.touch()->_vertices.push_back (Vector3D ());
	outline.touch()->_normals.push_back (Vector3D (0,0,1));

	outline.touch()->_vertices.push_back (Vector3D ());
	outline.touch()->_normals.push_back (Vector3D (0,0,1));
	//_graphics_outs[0].touch()->_graphics.push_back(_g);

	_g.touch()->_faces.push_back (outline);

}

BorderedRectangle::~BorderedRectangle ()
{
	// std::cout << "[BorderedRectangle]: Destructor" << std::endl;
}

void BorderedRectangle::process_g (double delta_t)
{
	_graphics_outs[0]._graphics.clear();

	Face *f = _g.touch()->_faces[0].touch();

	f->_vertices[0]._c[0] = - (*_control_ins[0])/2.0;
	f->_vertices[0]._c[1] = - (*_control_ins[1])/2.0;
	f->_vertices[0]._c[2] = 0;

	f->_vertices[1]._c[0] = (*_control_ins[0])/2.0;
	f->_vertices[1]._c[1] = - (*_control_ins[1])/2.0;
	f->_vertices[1]._c[2] = 0;

	f->_vertices[2]._c[0] = (*_control_ins[0])/2.0;
	f->_vertices[2]._c[1] = (*_control_ins[1])/2.0;
	f->_vertices[2]._c[2] = 0;

	f->_vertices[3]._c[0] = - (*_control_ins[0])/2.0;
	f->_vertices[3]._c[1] = (*_control_ins[1])/2.0;
	f->_vertices[3]._c[2] = 0;

	f->_face_color = ColorRGBA(
		(*_control_ins[3]),
		(*_control_ins[4]),
		(*_control_ins[5]),
		(*_control_ins[6])
	);

	// outline
	Face *l = _g.touch()->_faces[1].touch();

	l->_vertices[0]._c[0] = - (*_control_ins[0])/2.0;
	l->_vertices[0]._c[1] = - (*_control_ins[1])/2.0;
	l->_vertices[0]._c[2] = 0;

	l->_vertices[1]._c[0] = (*_control_ins[0])/2.0;
	l->_vertices[1]._c[1] = - (*_control_ins[1])/2.0;
	l->_vertices[1]._c[2] = 0;

	l->_vertices[2]._c[0] = (*_control_ins[0])/2.0;
	l->_vertices[2]._c[1] = (*_control_ins[1])/2.0;
	l->_vertices[2]._c[2] = 0;

	l->_vertices[3]._c[0] = -(*_control_ins[0])/2.0;
	l->_vertices[3]._c[1] = (*_control_ins[1])/2.0;
	l->_vertices[2]._c[2] = 0;

	l->_face_color = ColorRGBA(
		(*_control_ins[7]),
		(*_control_ins[8]),
		(*_control_ins[9]),
		(*_control_ins[10])
	);

	l->_thickness = (*_control_ins[2]);

	_graphics_outs[0]._graphics.push_back(_g);
}
