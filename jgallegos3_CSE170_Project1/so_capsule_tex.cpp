# include "so_capsule_tex.h"
# include "math.h"
# define M_PI           3.14159265358979323846  /* pi */
void SoCapsuleTex::init(const GlProgram& prog, int tex)
{
	// Define buffers needed:

	set_program(prog);
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(3);       // will use 2 buffers: one for coordinates and one for normals

	uniform_locations(12); // will send 11 variables
	uniform_location(0, "vTransf");
	uniform_location(1, "vProj");
	uniform_location(2, "lPos");
	uniform_location(3, "la");
	uniform_location(4, "ld");
	uniform_location(5, "ls");
	uniform_location(6, "ka");
	uniform_location(7, "kd");
	uniform_location(8, "ks");
	uniform_location(9, "sh");
	uniform_location(10, "Tex1");
	uniform_location(11, "influence");

	GsImage I;

	
		if (!I.load("../image1.png")) { std::cout << "COULD NOT LOAD IMAGE!\n"; }

		glGenTextures(1, &_texid1); // generated ids start at 1
		glBindTexture(GL_TEXTURE_2D, _texid1);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, I.w(), I.h(), 0, GL_RGBA, GL_UNSIGNED_BYTE, I.data());
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		I.init(0, 0); // free image from CPU
	
}

void SoCapsuleTex::build(float len, float rt, float rb, float nfaces, bool _flatn, GsColor color)
{

	P.clear(); N.clear(), NL.clear(); T.clear(); // set size to zero, just in case
	P.reserve(18); N.reserve(18); T.reserve(18);// reserve space to avoid re-allocations from the calls below

	const float l = len / 2.0f;
	const float ns = 0.05f;
	float angle = 0.0f;
	float inc = 2 * M_PI / nfaces;
	float deltainc = inc / 4.0f;
	GsVec n1, n2, n3;
	GsPnt top_center, bot_center, top, top_inc, bot, bot_inc, f1c;

	for (float i = 0; i < nfaces; i++) {

		top_center = GsPnt(0.0f, l, 0.0f);
		bot_center = GsPnt(0.0f, -l, 0.0f);
		top = GsPnt(rt*cos(angle), l, rt*sin(angle));
		top_inc = GsPnt(rt*cos(angle + inc), l, rt*sin(angle + inc));
		bot = GsPnt(rb*cos(angle), -l, rb*sin(angle));
		bot_inc = GsPnt(rb*cos(angle + inc), -l, rb*sin(angle + inc));

		//Top Circle
		P.push_back(top_center);
		P.push_back(top_inc);
		P.push_back(top);

		T.push_back(GsVec2(i / nfaces, 1)); //top_center
		T.push_back(GsVec2((i+1) / nfaces, 0)); //top_inc
		T.push_back(GsVec2( i/ nfaces, 0)); //top

		//Smooth Shading
			n1 = GsVec(0, 1, 0);
			n2 = GsVec(0, 1, 0);
			n3 = GsVec(0, 1, 0);

			N.push_back(n1); N.push_back(n2); N.push_back(n3);

			n1 *= ns; n2 *= ns; n3 *= ns;
			NL.push_back(top_center); NL.push_back(top_center + n1);
			NL.push_back(top); NL.push_back(top + n3);
	

		//Bottom Circle
		P.push_back(bot_center);
		P.push_back(bot);
		P.push_back(bot_inc);

		T.push_back(GsVec2(i / nfaces, 1)); //bot_center
		T.push_back(GsVec2( i / nfaces, 0)); //bot
		T.push_back(GsVec2((i+1) / nfaces, 0)); //bot_inc

		//Smooth Shading
			n1 = GsVec(0, -1, 0);
			n2 = GsVec(0, -1, 0);
			n3 = GsVec(0, -1, 0);

			
			N.push_back(n1); N.push_back(n2); N.push_back(n3);

			n1 *= ns; n2 *= ns; n3 *= ns;
			NL.push_back(bot_center); NL.push_back(bot_center + n1);
			NL.push_back(bot_inc); NL.push_back(bot_inc + n3);
		

		//Diagonal Triangle 1 Side
		P.push_back(bot);
		P.push_back(top);
		P.push_back(bot_inc);

		T.push_back(GsVec2(i / nfaces, 0));// bot
		T.push_back(GsVec2(i / nfaces, 1)); //top
		T.push_back(GsVec2((i + 1) / nfaces, 0)); //bot_inc

		//Smooth Shading
			n1 = GsVec(cos(angle), 0, sin(angle)); //bot
			n2 = GsVec(cos(angle), 0, sin(angle)); // top
			n3 = GsVec(cos(angle + inc), 0, sin(angle + inc)); //bot_inc

			N.push_back(n1); N.push_back(n2); N.push_back(n3);

			n1 *= ns; n2 *= ns; n3 *= ns;
			NL.push_back(bot); NL.push_back(bot + n1);
			NL.push_back(bot_inc); NL.push_back(bot_inc + n3);
		
		//Diagonal Triangle 2 Side
		P.push_back(top);
		P.push_back(top_inc);
		P.push_back(bot_inc);

		T.push_back(GsVec2(i/nfaces, 1)); //top
		T.push_back(GsVec2((i + 1) / nfaces, 1)); //top_inc
		T.push_back(GsVec2((i + 1) / nfaces, 0)); //bot_inc

		
		 //Smooth Shading
			n1 = GsVec(cos(angle), 0, sin(angle)); // top
			n2 = GsVec(cos(angle + inc), 0, sin(angle + inc)); // top_inc
			n3 = GsVec(cos(angle + inc), 0, sin(angle + inc)); //bot_inc

			N.push_back(n1); N.push_back(n2); N.push_back(n3);

			n1 *= ns; n2 *= ns; n3 *= ns;
			NL.push_back(top); NL.push_back(top + n1);
			NL.push_back(bot_inc); NL.push_back(bot_inc + n3);
		
		angle += inc;
		
	}

	fcolor = color;

	// send data to OpenGL buffers:
	glBindVertexArray(va[0]);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
	glBufferData(GL_ARRAY_BUFFER, T.size() * 2 * sizeof(float), &T[0], GL_STATIC_DRAW);
	

	glBindVertexArray(0); // break the existing vertex array object binding

						  // save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = P.size();


	// free non-needed memory:
	P.resize(0); N.resize(0); T.resize(0);

}

void SoCapsuleTex::draw(const GsMat& tr, const GsMat& pr, const Light& l, float influence, bool shad)
{

	if (shad == true) {
		m.amb = GsColor::black;
		m.dif = GsColor::black;
		m.spe = GsColor::white;
		m.sh = 8.0f;
	}

	else {

		m.amb = fcolor;
		m.dif = fcolor;
		m.spe = GsColor::white;
		m.sh = 8.0f;
		glBindTexture(GL_TEXTURE_2D, _texid1);

	}
	// Draw Lines:
	glUseProgram(prog);
	glBindVertexArray(va[0]);

	

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[2]); // textures
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0); // false means no normalization

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); // false means no normalization


	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);

	float f[4]; //we convert below our color values to be in [0,1]
	glUniform3fv(uniloc[2], 1, l.pos.e);
	glUniform4fv(uniloc[3], 1, l.amb.get(f));
	glUniform4fv(uniloc[4], 1, l.dif.get(f));
	glUniform4fv(uniloc[5], 1, l.spe.get(f));

	glUniform4fv(uniloc[6], 1, m.amb.get(f));
	glUniform4fv(uniloc[7], 1, m.dif.get(f));
	glUniform4fv(uniloc[8], 1, m.spe.get(f));
	glUniform1fv(uniloc[9], 1, &m.sh);
	glUniform1f(uniloc[11], influence);

	glDrawArrays(GL_TRIANGLES, 0, _numpoints);
	glBindVertexArray(0);

}