// Ensure the header file is included only once in multi-file projects
#ifndef SO_CAPSULE_H
#define SO_CAPSULE_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "so_light_mat.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:


class SoCapsule : public GlObjects
{
private:
	
	std::vector<GsPnt>   P; // coordinates
	std::vector<GsVec>   N; // normals
	int _numpoints;         // saves the number of points
	Material m;
	GsColor fcolor;

public:
	std::vector<GsVec>   NL;// normal lines

public:

	void init(const GlProgram& prog);
	void build(float len, float rt, float rb, int nfaces, bool _flatn, GsColor color);
	void draw(const GsMat& tr, const GsMat& pr, const Light& l, bool shad);
};




#endif // SO_CAPSULE_H#pragma once
