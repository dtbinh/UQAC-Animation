/*
 *  textures.h
 *  water
 *
 *  Created by ychirico on 09-02-28.
 *  Copyright 2009 UQAC. All rights reserved.
 *
 */



#include <string>

#ifndef __MAC_OS_X__
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include "OpenGL/gl.h"
#endif


const int nb_texture = 2;
extern GLuint id_texture[nb_texture];
bool  load_texture(const std::string& path_textures, GLuint& texture_name, int& w, int& h);
bool  load_texture(const std::string& path_textures, GLuint& texture_name);


