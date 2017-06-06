
# include <iostream>
# include <gsim/gs.h>
# include "app_window.h"

# define M_PI           3.14159265358979323846f  /* pi */

AppWindow::AppWindow ( const char* label, int x, int y, int w, int h )
          :GlutWindow ( label, x, y, w, h )
 {
   initPrograms ();
   addMenuEntry ( "Option 0", evOption0 );
   addMenuEntry ( "Option 1", evOption1 );
   _viewaxis = false;
   _flatn = true;
   _normals = true;
   _fovy = GS_TORAD(60.0f);
   _rotx = _roty = 0;
   _w = w;
   _h = h;

 

   Q_pressed = false; //front tire
   A_pressed = false; //back tire
   W_pressed = false; //handle bars
   S_pressed = false; //pedals
   E_pressed = false;
   D_pressed = false;
   UP_pressed = false;
   DOWN_pressed = false;
   LEFT_pressed = false;
   RIGHT_pressed = false;
   SPACE_pressed = false;
   _xfor = 0.0f;
   _xleft = 0.0f;
   _rot = 0.0f;
   _anglew = 0.0f;
   _anglef = 0.0f;
   _anglep = 0.0f;
   _anglel = 0.0f;
   bikex = 0.15f;
   bikez = 0.0f;
   bikey = 0.55f;

   shad = false;
   inc = 0.0005f;
   incl = 0.01;

   std::cout << "Jered Gallegos Project 1" << std::endl;
   std::cout << "Default:Flat Shading " << std::endl;
   std::cout << "Press c to toggle flat/smooth shading "<<std::endl;
   std::cout << "Use arrow keys to move bike" << std::endl;
   std::cout << "	UP to move forward" << std::endl;
   std::cout << "	DOWN to stop moving" << std::endl;
   std::cout << "	LEFT to move left" << std::endl;
   std::cout << "	RIGHT to move right" << std::endl;

   std::cout << "Press Q/A to test WHEEL rotation" << std::endl;
   std::cout << "Press W/S to test PEDAL rotation" << std::endl;
   std::cout << "Press E/D to test FORK rotation" << std::endl;
  

 }

GsMat myShadowMatrix(float ground[4], float light[4])
{
	float  dot;
	float  shadowMat[16];

	dot = ground[0] * light[0] +
		ground[1] * light[1] +
		ground[2] * light[2] +
		ground[3] * light[3];

	shadowMat[0] = dot - light[0] * ground[0];
	shadowMat[1] = 0.0f - light[0] * ground[1];
	shadowMat[2] = 0.0f - light[0] * ground[2];
	shadowMat[3] = 0.0f - light[0] * ground[3];

	shadowMat[4] = 0.0f - light[1] * ground[0];
	shadowMat[5] = dot - light[1] * ground[1];
	shadowMat[6] = 0.0f - light[1] * ground[2];
	shadowMat[7] = 0.0f - light[1] * ground[3];

	shadowMat[8] = 0.0f - light[2] * ground[0];
	shadowMat[9] = 0.0f - light[2] * ground[1];
	shadowMat[10] = dot - light[2] * ground[2];
	shadowMat[11] = 0.0f - light[2] * ground[3];

	shadowMat[12] = 0.0f - light[3] * ground[0];
	shadowMat[13] = 0.0f - light[3] * ground[1];
	shadowMat[14] = 0.0f - light[3] * ground[2];
	shadowMat[15] = dot - light[3] * ground[3];

	return GsMat(shadowMat);
}

void AppWindow::glutIdle() {
	if (UP_pressed) {
		
		_xfor += inc*cos(_rot); //move forawrd
		_xleft -= inc*sin(_rot);
		_anglew -= 0.01f;
		_anglep -= 0.01f;

		bikex += inc*cos(_rot);
		bikez -= inc*sin(_rot);


		if (_anglef < 0) {
			_anglef += 0.0025f;
		}

		if (_anglef > 0) {
			_anglef -= 0.0025f;
		}
	}

	if(DOWN_pressed) {
		_xfor = _xfor; //stop
		_anglew = _anglew;
		_anglep = _anglep;

		bikex = bikex;
		bikez = bikez;
	}

	if (LEFT_pressed) {
		_rot += 0.0005f;

		_xfor += inc*cos(_rot);
		_xleft -= inc*sin(_rot); //move left
		bikex += inc*cos(_rot);
		bikez -= inc*sin(_rot);

		
		_anglew -= 0.001f;
		if (_anglef < (M_PI / 4)) {
			_anglef += 0.005f;
		}

		_anglep -= 0.01f;
		
	
	}

	if(RIGHT_pressed) {

		_rot -= 0.0005f;

		_xfor += inc*cos(_rot);
		_xleft -= inc*sin(_rot); //move right
		bikex += inc*cos(_rot);
		bikez -= inc*sin(_rot);
		
		_anglew -= 0.001f;
		if (_anglef > -(M_PI / 4)) {
			_anglef -= 0.005f;
		}

		_anglep -= 0.01f;
		
	}

	if (Q_pressed) {
		//test tire rotation forward
		_anglew -= 0.01f;
	}

	if (A_pressed) {
		//test tire rotation forward
		_anglew += 0.01f;
	}

	if (W_pressed) {
		//test pedal rotation forward
		_anglep -= 0.01f;
	}

	if (S_pressed) {
		//test pedal rotation backward
		_anglep += 0.01f;
	}

	if (E_pressed) {
		//test fork rotation left
		if (_anglef < (M_PI / 4)) {
			_anglef += 0.005f;
		}
	}

	if (D_pressed) {
		//test fork rotation right
		if (_anglef > -(M_PI / 4)) {
			_anglef -= 0.005f;
		}
	}
	redraw();

}


void AppWindow::initPrograms ()
 {
   // Load your shaders and link your programs here:
   _vertexsh.load_and_compile ( GL_VERTEX_SHADER, "../3dcolored.vert" );
   _fragsh.load_and_compile ( GL_FRAGMENT_SHADER, "../flat.frag" );
   _prog.init_and_link ( _vertexsh, _fragsh );

   _vertg.load_and_compile(GL_VERTEX_SHADER, "../smtlgouraud.vert");
   _fragg.load_and_compile(GL_FRAGMENT_SHADER, "../gouraud.frag");
   _progg.init_and_link(_vertg, _fragg);

   _vertgtex.load_and_compile(GL_VERTEX_SHADER, "../texgouraud.vert");
   _fraggtex.load_and_compile(GL_FRAGMENT_SHADER, "../texgouraud.frag");
   _proggtex.init_and_link(_vertgtex, _fraggtex);

   int tex = 0;
   tex_cap1.init(_proggtex, tex);
   tex_shad.init(_progg);
   //std::cout << "Tex" << std::endl;

   // Init my scene objects:
   _axis.init ( _prog );

   f_wheel.init(_progg);
   b_wheel.init(_progg);
   line_w.init(_progg);

   frame1.init(_progg);
   frame2.init(_progg);
   frame3.init(_progg);
   frame4.init(_progg);
   frame5.init(_progg);
   seat.init(_progg);

   fork1.init(_progg);
   fork2.init(_progg);
   fork3.init(_progg);
   fork4.init(_progg);

   pedal1.init(_progg);
   pedal2.init(_progg);
   pedal3.init(_progg);

   floor.init(_progg);
   rebuild();


   

 }

void AppWindow::rebuild() {

	tex_cap1.build(0.75f, 0.25f, 0.25f, 20, _flatn, GsColor::white);
	tex_shad.build(0.75f, 0.25f, 0.25f, 20, _flatn, GsColor::red);

	floor.build(0.01f, 20.0f, 20.0f, 4, _flatn, GsColor::white);

	f_wheel.build(0.025f, 0.125f, 0.125, 20, _flatn, GsColor::gray);
	b_wheel.build(0.025f, 0.125f, 0.125, 20, _flatn, GsColor::gray);
	line_w.build(0.1875f, 0.0135f, 0.0135f, 4, _flatn, GsColor::white);

	frame1.build(0.2f, 0.0125f, 0.0125f, 20, _flatn, GsColor::lightblue);
	frame2.build(0.35f, 0.0125f, 0.0125f, 20, _flatn, GsColor::lightblue);
	frame3.build(0.4f, 0.0125f, 0.0125f, 20, _flatn, GsColor::lightblue);
	frame4.build(0.3f, 0.0125f, 0.0125f, 20, _flatn, GsColor::lightblue);
	frame5.build(0.461f, 0.0125f, 0.0125f, 20, _flatn, GsColor::lightblue);
	seat.build(0.05f, 0.1f, 0.05f, 3, _flatn, GsColor::lightgray);

	fork1.build(0.36f, 0.0125f, 0.0125f, 20, _flatn, GsColor::lightblue);
	fork2.build(0.1f, 0.0125f, 0.0125f, 20, _flatn, GsColor::lightblue);
	fork3.build(0.3f, 0.0125f, 0.0125f, 20, _flatn, GsColor::lightblue);
	fork4.build(0.3f, 0.00625f, 0.00625f, 20, _flatn, GsColor::lightblue); 

	pedal1.build(0.05f, 0.00625f, 0.00625f, 20, _flatn, GsColor::gray);
	pedal2.build(0.1f, 0.00625f, 0.00625f, 20, _flatn, GsColor::red);
	pedal3.build(0.05f, 0.0125f, 0.0125f, 4, _flatn, GsColor::orange); //pedal



}

// mouse events are in window coordinates, but your 2D scene is in [0,1]x[0,1],
// so make here the conversion when needed
GsVec2 AppWindow::windowToScene ( const GsVec2& v )
 {
   // GsVec2 is a lighteweight class suitable to return by value:
   return GsVec2 ( (2.0f*(v.x/float(_w))) - 1.0f,
                    1.0f - (2.0f*(v.y/float(_h))) );
 }

// Called every time there is a window event
void AppWindow::glutKeyboard ( unsigned char key, int x, int y )
 {
   switch ( key )
    {  //Joint motion animation
   case 'q':  Q_pressed = true; A_pressed = false; W_pressed = false; S_pressed = false; E_pressed = false; D_pressed = false; LEFT_pressed = false; UP_pressed = false; RIGHT_pressed = false; DOWN_pressed = false; break;
   case 'a':  A_pressed = true; Q_pressed = false; W_pressed = false; S_pressed = false; E_pressed = false; D_pressed = false; LEFT_pressed = false; UP_pressed = false; RIGHT_pressed = false; DOWN_pressed = false; break;
   case 'w':  W_pressed = true; A_pressed = false; Q_pressed = false; S_pressed = false; E_pressed = false; D_pressed = false; LEFT_pressed = false; UP_pressed = false; RIGHT_pressed = false; DOWN_pressed = false; break;
   case 's':  S_pressed = true; A_pressed = false; W_pressed = false; Q_pressed = false; E_pressed = false; D_pressed = false; LEFT_pressed = false; UP_pressed = false; RIGHT_pressed = false; DOWN_pressed = false; break;
   case 'e':  E_pressed = true; A_pressed = false; W_pressed = false; S_pressed = false; Q_pressed = false; D_pressed = false; LEFT_pressed = false; UP_pressed = false; RIGHT_pressed = false; DOWN_pressed = false; break;
   case 'd':  D_pressed = true; A_pressed = false; W_pressed = false; S_pressed = false; E_pressed = false; Q_pressed = false; LEFT_pressed = false; UP_pressed = false; RIGHT_pressed = false; DOWN_pressed = false; break;
   
   case 'c': _flatn = !_flatn; rebuild();  redraw(); break;
   //case 'x': _flatn = true; rebuild(); redraw(); break;
   //case 'z': _wire = !_wire; redraw(); break;
   case ' ': SPACE_pressed = !SPACE_pressed; redraw(); break;
   
   
   case 27 : exit(1); // Esc was pressed
	}
 }

void AppWindow::glutSpecial ( int key, int x, int y )
 {
   bool rd=true;
   const float incr=GS_TORAD(2.5f);
   const float incf=0.05f;
   switch ( key )
   {
   case GLUT_KEY_LEFT:      /*_roty-=incr*/ LEFT_pressed = true; UP_pressed = false; RIGHT_pressed = false; DOWN_pressed = false; Q_pressed = false; A_pressed = false; W_pressed = false; S_pressed = false; E_pressed = false; D_pressed = false; break;
	case GLUT_KEY_RIGHT:     /*_roty+=incr*/  RIGHT_pressed = true; UP_pressed = false; LEFT_pressed = false; DOWN_pressed = false; Q_pressed = false; A_pressed = false; W_pressed = false; S_pressed = false; E_pressed = false; D_pressed = false; break;
	  case GLUT_KEY_UP:        /*_rotx-=incr*/ UP_pressed = true; LEFT_pressed = false; RIGHT_pressed = false; DOWN_pressed = false; Q_pressed = false; A_pressed = false; W_pressed = false; S_pressed = false; E_pressed = false; D_pressed = false; break;
      case GLUT_KEY_DOWN:      /*_rotx+=incr*/ UP_pressed = false;  LEFT_pressed = false; RIGHT_pressed = false; DOWN_pressed = false; Q_pressed = false; A_pressed = false; W_pressed = false; S_pressed = false; E_pressed = false; D_pressed = false; break;
      case GLUT_KEY_PAGE_UP:   _fovy-=incf; break;
      case GLUT_KEY_PAGE_DOWN: _fovy+=incf; break;
      case GLUT_KEY_HOME:      _fovy=GS_TORAD(60.0f); _rotx=_roty=0; break;
      default: return; // return without rendering
	}
   if (rd) redraw(); // ask the window to be rendered when possible
 }

void AppWindow::glutMouse ( int b, int s, int x, int y )
 {
   // The mouse is not used in this example.
   // Recall that a mouse click in the screen corresponds
   // to a whole line traversing the 3D scene.
 }

void AppWindow::glutMotion ( int x, int y )
 {
 }

void AppWindow::glutMenu ( int m )
 {
   std::cout<<"Menu Event: "<<m<<std::endl;
 }

void AppWindow::glutReshape ( int w, int h )
 {
   // Define that OpenGL should use the whole window for rendering
   glViewport( 0, 0, w, h );
   _w=w; _h=h;
 }

// here we will redraw the scene according to the current state of the application.
void AppWindow::glutDisplay ()
 {
   // Clear the rendering window
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // Build a cross with some lines (if not built yet):
   if ( _axis.changed ) // needs update
    { _axis.build(1.0f); // axis has radius 1.0
    }
   
   //_capsule.build(1.0f, _rt, _rb, num_f, _flatn);
   

   // Define our scene transformation:
   GsMat rx, ry, stransf, rotate_l, rotate_90x, rotate_90z, rotate_180x, rotate_45z, rotate_45y, rotate_f5, rotate_fo1, trans_tex1, trans_seat, trans_f, trans_w, trans_f1, trans_f2, trans_f3, trans_f4, trans_f5, trans_fo1, trans_fo2, trans_fo3, trans_fo4, trans_p1, trans_p2, trans_p3, trans_px, rotate_w, rotate_f, rotate_p, rotate_p2, forward, rotate;
   rx.rotx ( _rotx );
   ry.roty ( _roty );
   stransf = rx*ry; // set the scene transformation matrix
   rotate_90x.rotx(M_PI / 2);
   rotate_90z.rotz(M_PI / 2);
   rotate_45z.rotz(-M_PI / 6);
   rotate_180x.rotx(M_PI);
   rotate_45y.roty(M_PI / 4);
   rotate_f5.rotz(-0.698132f);
   rotate_fo1.rotz(0.2783f); 
   rotate_l.rotz(_anglel);
   
   trans_f.translation(GsVec(0.0f, -0.15f, 0.0f));
   trans_w.translation(GsVec(0.6f, 0.0f, 0.0f));
   trans_f1.translation(GsVec(0.1f, 0.0f, 0.0f));
   trans_f2.translation(GsVec(0.2f, 0.175f, 0.0f));
   trans_f3.translation(GsVec(0.0f, 0.2f, 0.0f));
   trans_f4.translation(GsVec(0.35f, 0.35f, 0.0f));
   trans_f5.translation(GsVec(0.35f, 0.17f, 0.0f));
   trans_seat.translation(GsVec(0.2f, 0.375f, 0.0f));
   trans_tex1.translation(GsVec(-1.0f, 0.25f, -1.0f));

   trans_fo1.translation(GsVec(0.55f, 0.17f, 0.0f));
   trans_fo2.translation(GsVec(0.5f, 0.4f, 0.0f));
   trans_fo3.translation(GsVec(0.5f, 0.45f, 0.0f));
   trans_fo4.translation(GsVec(0.55f, 0.17f, 0.013f));

   trans_p1.translation(GsVec(0.0f, 0.0f, 0.025f));
   trans_p2.translation(GsVec(0.0f, 0.05f, 0.05f));
   trans_p3.translation(GsVec(0.0f, 0.1f, 0.06875f));
   trans_px.translation(GsVec(0.2f, 0.0f, 0.0f));

   forward.translation(GsVec(_xfor, 0.0f, _xleft));
   rotate.roty(_rot);
   

   rotate_w.rotz(_anglew);
   rotate_f.roty(_anglef);//back_wheel, and all fork parts
   rotate_p.rotz(_anglep);//pedal rotation
   rotate_p2.rotz(-_anglep);
   // Define our projection transformation:
   // (see demo program in gltutors-projection.7z, we are replicating the same behavior here)
   GsMat camview, persp, sproj;

   //fixed camera angle
   GsVec eye(1,1,2), center(0.3f,0,0), up(0,1,0);

   if (SPACE_pressed) {
	   eye = GsVec( bikex, bikey, bikez);
	   center = GsVec(1000000.0f*cos(-_rot) , bikey, 1000000.0f*sin(-_rot));

   }
   camview.lookat ( eye, center, up ); // set our 4x4 "camera" matrix

   float aspect=1.0f, znear=0.1f, zfar=50.0f;
   persp.perspective ( _fovy, aspect, znear, zfar ); // set our 4x4 perspective matrix

   // Our matrices are in "line-major" format, so vertices should be multiplied on the 
   // right side of a matrix multiplication, therefore in the expression below camview will
   // affect the vertex before persp, because v' = (persp*camview)*v = (persp)*(camview*v).
   sproj = persp * camview; // set final scene projection

   //  Note however that when the shader receives a matrix it will store it in column-major 
   //  format, what will cause the values to be transposed, and we will then have in our 
   //  shaders vectors on the left side of a multiplication to a matrix.

   // Draw:
   if (_viewaxis) { _axis.draw(stransf, sproj);
   }

   if (_normals) {
	   //_linesrh.draw(stransf*translate_f*rotate_rh3*translate_la*rotate_rh2*translate_rh*rotate_rh1*scaling, sproj);
	   //_linesla.draw(stransf*translate_f*rotate_rh3*translate_la*rotate_rh2*scaling, sproj);
	   //_linesua.draw(stransf*translate_f*rotate_rh3*scaling, sproj);
   }

   _light = Light(GsVec(-1, 1, 1), GsColor(90, 90, 90, 255), GsColor::white, GsColor::white);
   float ground[4] = { 0,1.0f,0,0.125f }; //y = -0.125
   float light[4] = { -1, 1, 1 ,1.0f };

   
   GsMat ShadowProjMatrix = myShadowMatrix(ground, light);
   //rhand.draw(stransf*translate_f*rotate_rh3*translate_la*rotate_rh2*translate_rh*rotate_rh1*scaling, sproj, _light, _wire);
   //rlowerarm.draw(stransf*translate_f*rotate_rh3*translate_la*rotate_rh2*scaling, sproj, _light, _wire);
   //rupperarm.draw(stransf*translate_f*rotate_rh3*scaling, sproj, _light, _wire);

   tex_cap1.draw(stransf*trans_tex1, sproj, _light, 0.25f, shad);
   tex_shad.draw(stransf*ShadowProjMatrix*trans_tex1, sproj, _light, !shad);

   f_wheel.draw(stransf*forward*rotate*rotate_w*rotate_90x, sproj, _light, shad);
   b_wheel.draw(stransf*forward*rotate*trans_w*rotate_f*rotate_w*rotate_90x, sproj, _light, shad);

   for (int i = 0; i < 20; i++) {
	   
	   rotate_l.rotz(_anglel + (2 * M_PI / 20)*i);
	   line_w.draw(stransf*forward*rotate*rotate_w*rotate_l, sproj, _light, shad);
	   line_w.draw(stransf*forward*rotate*trans_w*rotate_f*rotate_w*rotate_l, sproj, _light, shad);
   }

   frame1.draw(stransf*forward*rotate*trans_f1*rotate_90z, sproj, _light, shad);
   frame2.draw(stransf*forward*rotate*trans_f2, sproj, _light, shad);
   frame3.draw(stransf*forward*rotate*rotate_45z*trans_f3, sproj, _light, shad);
   frame4.draw(stransf*forward*rotate*trans_f4*rotate_90z, sproj, _light, shad);
   frame5.draw(stransf*forward*rotate*trans_f5*rotate_f5, sproj, _light, shad);
   seat.draw(stransf*forward*rotate*trans_seat, sproj, _light, shad);

   fork1.draw(stransf*forward*rotate*trans_fo1*rotate_fo1*rotate_f, sproj, _light, shad);
   fork2.draw(stransf*forward*rotate*trans_fo2*rotate_f, sproj, _light, shad);
   fork3.draw(stransf*forward*rotate*trans_fo3*rotate_f*rotate_90x, sproj, _light, shad);
   fork4.draw(stransf*forward*rotate*trans_fo4*rotate_fo1, sproj, _light, shad);

   pedal1.draw(stransf*forward*rotate*trans_px*rotate_p*trans_p1*rotate_90x, sproj, _light, shad);
   pedal2.draw(stransf*forward*rotate*trans_px*rotate_p*trans_p2, sproj, _light, shad);
   pedal3.draw(stransf*forward*rotate*trans_px*rotate_p*trans_p3*rotate_90x, sproj, _light, shad);

   pedal1.draw(stransf*forward*rotate*trans_px*rotate_180x*rotate_p2*trans_p1*rotate_90x, sproj, _light, shad);
   pedal2.draw(stransf*forward*rotate*trans_px*rotate_180x*rotate_p2*trans_p2, sproj, _light, shad);
   pedal3.draw(stransf*forward*rotate*trans_px*rotate_180x*rotate_p2*trans_p3*rotate_90x, sproj, _light, shad);

   floor.draw(stransf*trans_f*rotate_45y, sproj, _light, shad);

   //Shadow
   
   f_wheel.draw(stransf*ShadowProjMatrix*forward*rotate*rotate_w*rotate_90x, sproj, _light, !shad);
   b_wheel.draw(stransf*ShadowProjMatrix*forward*rotate*trans_w*rotate_f*rotate_w*rotate_90x, sproj, _light, !shad);

   frame1.draw(stransf*ShadowProjMatrix*forward*rotate*trans_f1*rotate_90z, sproj, _light, !shad);
   frame2.draw(stransf*ShadowProjMatrix*forward*rotate*trans_f2, sproj, _light, !shad);
   frame3.draw(stransf*ShadowProjMatrix*forward*rotate*rotate_45z*trans_f3, sproj, _light, !shad);
   frame4.draw(stransf*ShadowProjMatrix*forward*rotate*trans_f4*rotate_90z, sproj, _light, !shad);
   frame5.draw(stransf*ShadowProjMatrix*forward*rotate*trans_f5*rotate_f5, sproj, _light, !shad);
   

   fork1.draw(stransf*ShadowProjMatrix*forward*rotate*trans_fo1*rotate_fo1*rotate_f, sproj, _light, !shad);
   fork2.draw(stransf*ShadowProjMatrix*forward*rotate*trans_fo2*rotate_f, sproj, _light, !shad);
   fork3.draw(stransf*ShadowProjMatrix*forward*rotate*trans_fo3*rotate_f*rotate_90x, sproj, _light, !shad);
   

   pedal1.draw(stransf*ShadowProjMatrix*forward*rotate*trans_px*rotate_p*trans_p1*rotate_90x, sproj, _light, !shad);
   pedal2.draw(stransf*ShadowProjMatrix*forward*rotate*trans_px*rotate_p*trans_p2, sproj, _light, !shad);
   pedal3.draw(stransf*ShadowProjMatrix*forward*rotate*trans_px*rotate_p*trans_p3*rotate_90x, sproj, _light, !shad);

   pedal1.draw(stransf*ShadowProjMatrix*forward*rotate*trans_px*rotate_180x*rotate_p2*trans_p1*rotate_90x, sproj, _light, !shad);
   pedal2.draw(stransf*ShadowProjMatrix*forward*rotate*trans_px*rotate_180x*rotate_p2*trans_p2, sproj, _light, !shad);
   pedal3.draw(stransf*ShadowProjMatrix*forward*rotate*trans_px*rotate_180x*rotate_p2*trans_p3*rotate_90x, sproj, _light, !shad);

   
   // Swap buffers and draw:
   glFlush();         // flush the pipeline (usually not necessary)
   glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front

}

