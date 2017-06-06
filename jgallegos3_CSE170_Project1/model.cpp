
# include <stdio.h>
# include <string.h>
# include <stdlib.h>

# include "model.h"

#ifdef WIN32
#pragma warning(disable : 4996) // to remove fopen warning as being unsafe
#endif

void Model::init(const GlProgram& prog)
{
	// Define buffers needed:

	set_program(prog);
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(2);       // will use 3 buffers: one for vertices , normals, and colors

	uniform_locations(10); // will send 9 variables
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

}

void Model::load(const char* file)
{
	FILE* f = fopen(file, "rt");
	if (!f) // one more try: try to open again in the ".." folder
	{
		char s[120];
		sprintf(s, "../%s", file);
		f = fopen(s, "rt");
		//printf("Loaded file: [%s]\n", file);

	}
	if (!f) // not found
	{
		printf("Could not load [%s]!\n", file); exit(1);
	}

	int i;
	char token[256];

	while (true)
	{
		fscanf(f, "%s", token);
		if (feof(f)) break;

		if (strcmp(token, "vertices") == 0)
		{
			fscanf(f, "%d", &vsize);
			delete[] V;
			V = new GsVec[vsize];
			for (i = 0; i < vsize; i++)
			{
				fscanf(f, "%f %f %f", &V[i].x, &V[i].y, &V[i].z);
			}
		}
		else if (strcmp(token, "vertices_per_face") == 0)
		{
			fscanf(f, "%d", &fsize);
			delete[] F;
			F = new Fac[fsize];
			for (i = 0; i < fsize; i++)
			{
				fscanf(f, "%d %d %d", &F[i].va, &F[i].vb, &F[i].vc);
			}
		}
		else if (strcmp(token, "normals") == 0)
		{
			fscanf(f, "%d", &nsize);
			delete[] N;
			N = new GsVec[nsize];
			for (i = 0; i < nsize; i++)
			{
				fscanf(f, "%f %f %f", &N[i].x, &N[i].y, &N[i].z);
			}
		}
		else if (strcmp(token, "normals_per_face") == 0)
		{
			if (!F) { printf("Error in file!\n"); exit(1); }
			for (i = 0; i < fsize; i++)
			{
				fscanf(f, "%d %d %d", &F[i].na, &F[i].nb, &F[i].nc);
			}
		}
		else if (strcmp(token, "colors") == 0)
		{
			if (!F) { printf("Error in file!\n"); exit(1); }
			int a, b, c;
			for (i = 0; i < fsize; i++)
			{
				fscanf(f, "%d %d %d", &a, &b, &c);
				F[i].r = (unsigned char)a;
				F[i].g = (unsigned char)b;
				F[i].b = (unsigned char)c;
			}
		}
	}
}

void Model::build(bool _flatn)
{
	const float ns = 0.5f;
	P.clear(); NN.clear(); C.clear(); NL.clear();
	
	P.reserve(18); NN.reserve(18); C.reserve(18), NL.reserve(18);
	GsColor fcolor;
	GsVec n1, n2, n3, v1, v2, v3, f1c;
	fcolor = GsColor(F[fsize/2].r, F[fsize/2].g, F[fsize/2].b, 255);

	for (int i = 0; i < fsize; i++) {
		//Push Back Vertices
		v1 = V[F[i].va];
		v2 = V[F[i].vb];
		v3 = V[F[i].vc];

		P.push_back(v1);
		P.push_back(v2);
		P.push_back(v3);

		if (_flatn) {
			//flat shading
				//Push Back Normals

			n1 = normal(v1, v2, v3);

			NN.push_back(n1);
			NN.push_back(n1);
			NN.push_back(n1);

			n1 *= ns;
			f1c = (v1+v2+v3) / 3.0f;
			NL.push_back(f1c); NL.push_back(f1c + n1);
			

		}

		else
		{ //Smooth Shading

			//Push Back Normals
			n1 = N[F[i].na];
			n2 = N[F[i].nb];
			n3 = N[F[i].nc];

			NN.push_back(n1);
			NN.push_back(n2);
			NN.push_back(n3);

			NL.push_back(v1); NL.push_back(v1 + n1);
			NL.push_back(v2); NL.push_back(v2 + n2);
			NL.push_back(v3); NL.push_back(v3 + n3);




		}
	}
		glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
		glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
		//glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
		//glBufferData(GL_ARRAY_BUFFER, C.size() * 4 * sizeof(gsbyte), &C[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
		glBufferData(GL_ARRAY_BUFFER, NN.size() * 3 * sizeof(float), &NN[0], GL_STATIC_DRAW);

		// save size so that we can free our buffers and later just draw the OpenGL arrays:
		_numpoints = P.size();

		// free non-needed memory:
		P.resize(0); C.resize(0); NN.resize(0),// NL.resize(0);

		m.amb = fcolor;
		m.dif = fcolor;
		m.spe = GsColor::white;
		m.sh = 8.0f;
}

void Model::draw(const GsMat& tr, const GsMat& pr, const Light& l, bool _wire)
{

	// Draw Lines:
	glUseProgram(prog);
	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/*glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);*/

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


	if (_wire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, _numpoints);
	}

	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, _numpoints);
	}

}

	