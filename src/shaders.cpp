
#include "shaders.h"

#ifndef __MAC_OS_X__
#include <GL/glew.h>
#include <GL/glut.h>
#else
#include "GLUT/glut.h"
#endif


#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "diagnostic.h"
#include "textfile.h"
#include "point3d.h"

using namespace std;


typedef  unsigned int uint;


GLuint prog_phong = 0, prog_water = 0;
GLuint vphong, fphong, vwater, fwater;


// --------------------------------------------------------------------------------
//  Fonction: set_shaders
//  
//  Par:   Yves Chiricota
//  Date:  02/03/07
//  MAJ:   
// --------------------------------------------------------------------------------
static bool load_shader(const string& path, const string& vname, const string& fname, GLuint& p, GLuint& v, GLuint& f)
{
	char *vs = NULL;
	char *fs = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	char fshader_fname[1024];
	strcpy(fshader_fname, (path +fname).c_str());

	char vshader_fname[1024];
	strcpy(vshader_fname, (path + vname).c_str());

	vs = textFileRead(vshader_fname);
	fs = textFileRead(fshader_fname);

	if ( !vs || !fs )
	{
		printf("Unable to read shaders\n");
		return false;
	}

	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);

	free(vs);
	free(fs);

	glCompileShader(v);
	glCompileShader(f);

	printShaderInfoLog(v);
	printShaderInfoLog(f);

	p = glCreateProgram();

	glAttachShader(p,v);
	glAttachShader(p,f);

	glLinkProgram(p);
	printProgramInfoLog(p);

	return true;
}


void set_shaders(const string& shader_path)
{ 
    //load_shader(shader_path, "fond.vert", "fond.frag", prog_fond, vfond, ffond);
    //load_shader(shader_path, "water.vert", "water.frag", prog_water, vwater, fwater);
    //load_shader(shader_path, "repere.vert", "repere.frag", prog_repere, vrepere, frepere);
    load_shader(shader_path, "phong.vert", "phong.frag", prog_phong, vphong, fphong);
    load_shader(shader_path, "water.vert", "water.frag", prog_water, vwater, fwater);
}





// --------------------------------------------------------------------------------
//  Fonction: GPU_initialize
//  
//  Par:   Yves Chiricota
//  Date:  02/03/07
//  MAJ:   
// --------------------------------------------------------------------------------
void shader_initialize(const string& shaders_path)
{
	set_shaders(shaders_path);
}
	

static
void shader_dispose(GLuint& p, GLuint& v, GLuint& f)
{ 
    glDetachShader(p, v);
    glDetachShader(p, f);
	glDeleteShader(v);
	glDeleteShader(f);
	glDeleteProgram(p);
}

// --------------------------------------------------------------------------------
//  Fonction: GPU_quit
//  
//  Par:   Yves Chiricota
//  Date:  02/03/07
//  MAJ:   
// --------------------------------------------------------------------------------
void shader_dispose()
{
    //shader_dispose(prog_fond, vfond, vfond);
    //shader_dispose(prog_water, vwater, fwater);
}

#pragma mark -

void shader_setuniform(const GLuint program, const char* name, const bool val)
{
    GLuint loc = glGetUniformLocation(program, name);
	glUniform1i(loc, val); 
}

void shader_setuniform(const GLuint program, const char* name, const int val)
{
    GLuint loc = glGetUniformLocation(program, name);
	glUniform1i(loc, val); 
}

void shader_setuniform(const GLuint program, const char* name, const float val)
{
    GLuint loc = glGetUniformLocation(program, name);
	glUniform1f(loc, val); 
}

void shader_setuniform(const GLuint program, const char* name, const CPoint3D& p)
{
    GLuint loc = glGetUniformLocation(program, name);
	glUniform3f(loc, p[0], p[1], p[2]); 
}

void shader_setuniform3fv(const GLuint program, const char* name, const float* p)
{
    GLuint loc = glGetUniformLocation(program, name);
	glUniform3f(loc, p[0], p[1], p[2]); 
}


void shader_setuniform2fv(const GLuint program, const char* name, const float* p)
{
    GLuint loc = glGetUniformLocation(program, name);
	glUniform2f(loc, p[0], p[1]); 
}


void shader_setuniform2f(const GLuint program, const char* name, const float& a, const float& b)
{
    GLuint loc = glGetUniformLocation(program, name);
	glUniform2f(loc, a, b); 
}





















