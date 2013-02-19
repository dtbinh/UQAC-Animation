#pragma once

#include <string>
#ifndef __MAC_OS_X__
#include <GL/glew.h>
#else
#include <OpenGL/gl.h>
#endif

class CPoint3D;

extern GLuint prog_phong, prog_water;

void shader_initialize(const std::string& shaders_path);
void shader_dispose();

void shader_setuniform(const GLuint program, const char* name, const bool val);
void shader_setuniform(const GLuint program, const char* name, const int val);
void shader_setuniform(const GLuint program, const char* name, const float val);
void shader_setuniform(const GLuint program, const char* name, const CPoint3D&);
void shader_setuniform3fv(const GLuint program, const char* name, const float*);
void shader_setuniform3fv(const GLuint program, const char* name, const float*);
void shader_setuniform2f(const GLuint program, const char* name, const float& a, const float& b);
