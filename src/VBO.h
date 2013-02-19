//
//  VBO.h
//  proto
//
//  Created by Jean Meyblum et Pierre Rouveyrol on 2013-01-30.
//  Copyright (c) 2013 YC. All rights reserved.
//

#ifndef __proto__VBO__
#define __proto__VBO__

#ifndef __MAC_OS_X__
#include <GL/gl.h>
#else
#include "OpenGL/gl.h"
#endif

#include <iostream>

#endif /* defined(__proto__VBO__) */

class VBO
{
private :
    unsigned int mp_nb_indices;
    GLuint mp_buffer_id[2];

    
public:
    VBO(){};
    void SetBuffers(GLfloat* vb, unsigned int nbvtx, GLuint* vi, unsigned int nbi);
    void Draw();
};