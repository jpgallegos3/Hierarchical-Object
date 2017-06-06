# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"

class Light
{
public:
	GsVec pos;
	GsColor amb, dif, spe; //ambient,diffuse, specular
	Light() {};
	Light(const GsVec& p, const GsColor& a, const GsColor& d, const GsColor& s)
		: pos(p), amb(a), dif(d), spe(s) {}
};

class Material
{
public:
	GsColor amb, dif, spe; //ambient,diffuse, specular
	float sh; // shininess
	Material() {};
	Material(const GsColor& a, const GsColor& d, const GsColor& s, float sn)
		: amb(a), dif(d), spe(s), sh(sn) {}
};