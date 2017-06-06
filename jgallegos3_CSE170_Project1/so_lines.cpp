
# include "so_lines.h"

SoLines::SoLines()
 {
   _numelements = 0;
 }

// init is called only once:
void SoLines::init (const GlProgram& prog)
 {
   // Build program:
   // Note: the program below could be shared with SoAxis (no need to comppile both 
   //  because they are the same), but ok here to avoid adding code for sharing
	set_program(prog);

   // Define buffers needed:
   gen_vertex_arrays ( 1 ); // will use 1 vertex array
   gen_buffers ( 2 );       // will use 2 buffers: one for coordinates and one for colors
   uniform_locations ( 2 ); // will send 2 variables: the 2 matrices below
   uniform_location ( 0, "vTransf" );
   uniform_location ( 1, "vProj" );
 }

// build may be called everytime the object changes (not the case for this axis object):
void SoLines::build ( const std::vector<GsVec>& lines)
 {
   P.clear(); C.clear();
   P.reserve(18); C.reserve(18);
   P = lines; // copy operator will copy all data
  
   
   for (int i = 0; i < P.size() ; i++)
   {
	   C.push_back(GsColor::red);

   }

   
   // send data to OpenGL buffers:

   glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
   glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
   glBufferData(GL_ARRAY_BUFFER, P.size() * 4 * sizeof(gsbyte), &C[0], GL_STATIC_DRAW);


   
   // save size so that we can free our buffers and later draw the OpenGL arrays:
   _numelements = P.size();

   // free non-needed memory:
   P.resize(0); C.resize(0);
 }

// draw will be called everytime we need to display this object:
void SoLines::draw ( GsMat& tr, GsMat& pr )
 {
   // Prepare program:
   glUseProgram ( prog );
   glBindVertexArray(va[0]);

   glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

   glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // colors
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

   glUniformMatrix4fv ( uniloc[0], 1, GL_FALSE, tr.e );
   glUniformMatrix4fv ( uniloc[1], 1, GL_FALSE, pr.e );

   // Draw:
   glDrawArrays ( GL_LINES, 0, _numelements );
  
 }

