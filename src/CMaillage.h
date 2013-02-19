//
//  CMaillage.h
//  proto
//
//  Created by Jean Meyblum et Pierre Rouveyrol on 2013-01-30.
//  Copyright (c) 2013 YC. All rights reserved.
//

#ifndef __proto__CMaillage__
#define __proto__CMaillage__

#include <iostream>
#include "VBO.h"

using namespace std;

#endif /* defined(__proto__CMaillage__) */

#pragma once

class CMaillage;
typedef CMaillage CMaillage;

class CMaillage {
    
private :

    GLfloat * m_sommets;
	GLuint * m_indices;
    int m_xsize;
    int m_ysize;
    int m_cur_sommets;
    
public:
    
    CMaillage(int xsize, int ysize);
    
    void AddSommet(float* sommet);
    
    void FillVBO(VBO& vb);
    
};