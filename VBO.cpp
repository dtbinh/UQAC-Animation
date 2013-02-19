//
//  VBO.cpp
//  proto
//
//  Created by Jean Meyblum et Pierre Rouveyrol on 2013-01-30.
//  Copyright (c) 2013 YC. All rights reserved.
//
#include <iostream>
#include <fstream>
#include "VBO.h"

void VBO::SetBuffers(GLfloat* vb, unsigned int nbvtx, GLuint* vi, unsigned int nbi){
    mp_nb_indices = nbi;
    glGenBuffers(2, mp_buffer_id );
    
    // Sommets.
    glBindBuffer(GL_ARRAY_BUFFER, mp_buffer_id[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*nbvtx, vb , GL_STATIC_DRAW);
    
    // Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mp_buffer_id[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*nbi,  &vi[0], GL_STATIC_DRAW);
}


void VBO::Draw()
{

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Sélection du tampon de sommets.
    glBindBuffer(GL_ARRAY_BUFFER, mp_buffer_id[0]);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glVertexPointer(3, GL_FLOAT, 8*sizeof(float), 0);
    glTexCoordPointer(2, GL_FLOAT,8*sizeof(float), (void*)(6*sizeof(float)));
    glNormalPointer(GL_FLOAT, 8*sizeof(float), (void*)(3*sizeof(float)));
    
    // Sélection du tampon d'indices.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mp_buffer_id[1]);
    
    // On dessine.
    glDrawElements(GL_TRIANGLES, mp_nb_indices, GL_UNSIGNED_INT, (void*)0 );

    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

