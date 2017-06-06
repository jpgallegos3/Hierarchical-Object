
// Ensure the header file is included only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H

# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"
# include "so_axis.h"
# include "so_capsule.h"
# include "so_capsule_tex.h"
# include "so_lines.h"
#include  "math.h"



// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
 { private :
    // OpenGL shaders and programs:
    GlShader _vertexsh, _fragsh, _vertg, _fragg, _vertgtex, _fraggtex;
    GlProgram _prog, _progg, _proggtex;
    // My scene objects:
    SoAxis _axis;
	Light _light;

	SoCapsule f_wheel;
	SoCapsule b_wheel;
	SoCapsule line_w;

	SoCapsule frame1;
	SoCapsule frame2;
	SoCapsule frame3;
	
	SoCapsule frame4;
	SoCapsule frame5;
	SoCapsule seat;

	SoCapsule fork1;
	SoCapsule fork2;
	SoCapsule fork3;
	SoCapsule fork4;
	SoCapsule fork5;

	SoCapsule pedal1;
	SoCapsule pedal2;
	SoCapsule pedal3;

	SoCapsule floor;
	
	SoCapsuleTex tex_cap1;
	SoCapsule tex_shad;
    // App data:
    enum MenuEv { evOption0, evOption1 };
	float _rotx, _roty, _fovy, incr, bikex, bikey, bikez, _anglel, _xfor, _xleft, _rot, _anglew, _anglef, _anglep, inc, incl;
	bool  _viewaxis, _flatn, _normals, LEFT_pressed, RIGHT_pressed, UP_pressed, DOWN_pressed, Q_pressed, A_pressed, W_pressed, S_pressed, E_pressed, D_pressed, SPACE_pressed,shad;
    int _w, _h;

   public :
    AppWindow ( const char* label, int x, int y, int w, int h );
    void initPrograms ();
    GsVec2 windowToScene ( const GsVec2& v );

   private : // functions derived from the base class
	virtual void glutIdle();
    virtual void glutMenu ( int m );
    virtual void glutKeyboard ( unsigned char key, int x, int y );
    virtual void glutSpecial ( int key, int x, int y );
    virtual void glutMouse ( int b, int s, int x, int y );
    virtual void glutMotion ( int x, int y );
    virtual void glutDisplay ();
    virtual void glutReshape ( int w, int h );
	virtual void rebuild();
 };

#endif // APP_WINDOW_H
