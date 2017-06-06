// Ensure the header file is included only once in multi-file projects
#ifndef SO_CAPSULETEX_H
#define SO_CAPSULETEX_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include <gsim/gs_shared.h>
# include <gsim/gs_image.h>
# include <gsim/stb_image.h>
# include <gsim/stb_image_write.h>
# include "ogl_tools.h"
# include "so_capsule.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:


class SoCapsuleTex : public GlObjects
{
private:
	
	std::vector<GsPnt>   P; // coordinates
	std::vector<GsVec>   N; // normals
	std::vector<GsVec2>  T; //texture coords
	int _numpoints;         // saves the number of points
	gsuint _texid1;
	GsColor fcolor;
	Material m;

public:
	std::vector<GsVec>   NL;// normal lines

public:

	void init(const GlProgram& prog, int tex);
	void build(float len, float rt, float rb, float nfaces, bool _flatn, GsColor fcolor);
	void draw(const GsMat& tr, const GsMat& pr, const Light& l, float influence, bool shad);
};




#endif // SO_CAPSULETEX_H#pragma once
