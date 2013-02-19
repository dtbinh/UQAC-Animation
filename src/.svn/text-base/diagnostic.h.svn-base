#pragma once


#ifndef __MAC_OS_X__
#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#else
#include "OpenGL/gl.h"
#endif

#include <string>

#ifdef PRINTOGLERRORS
#define printOpenGLError printOglError(__FILE__, __LINE__);
#else
#define printOpenGLError
#endif
int printOglError(std::string file, int line);
int printOglError(char *file, int line);
void printProgramInfoLog(GLuint obj);
void printShaderInfoLog(GLuint obj);
bool checkFramebufferStatus();
void getGlVersion(int& major, int& minor);
