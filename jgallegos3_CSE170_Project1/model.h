
// Ensure the header file is included only once in multi-file projects
// Pragma once is a non-standard but widely supported preprocessor directive
# pragma once

# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "so_capsule.h"


// a class to store a model with triangular faces
class Model : public GlObjects
 { public :
    struct Fac { int va, vb, vc; // vertices indices of the (triangular) face
                 int na, nb, nc; // normal indices
                 int r, g, b; // r g b color

               };

    GsVec* V; // vertex table
    GsVec* N; // normals table
    Fac* F; // faces table
    int vsize, nsize, fsize; // sizes of the tables
	Material m;

	std::vector<GsVec> P;
	std::vector<GsVec> NN;
	std::vector<GsColor> C;
	std::vector<GsVec> NL;
	int _numpoints;

   public :

    /*! constructor */
    Model () { vsize=nsize=fsize=0; V=N=0; F=0; }

    /*! destructor */
   ~Model () { delete[] V; delete[] N; delete[] F; }

    /*! load model */
    void init(const GlProgram& prog);
    void load ( const char* file );
	void build(bool _flatn);
	void Model::draw(const GsMat& tr, const GsMat& pr, const Light& l, bool _wire);

 };

