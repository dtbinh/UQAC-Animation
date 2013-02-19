
#include <iostream>

#ifndef __MAC_OS_X__
#include <GL/glew.h>
#include <GL/glut.h>
#else
#include "GLUT/glut.h"
#endif


#include <stdio.h>
#include "textures.h"
#include "glutfcts.h" 

#define WIN_WIDTH 1200
#define WIN_HEIGHT 900
#define SUB_WINDOW_HEIGHT 32


using namespace std;

#ifndef __MAC_OS_X__
string texture_path("./");
#else
string texture_path("./");
#endif

string texturename [] = {"roche0.jpg", "grass1.jpg"};
// --------------------------------------------------------------------------------
//  Fonction: main
//
//  Par:   Yves Chiricota
//  Date:  02/03/07
//  MAJ:   
// --------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    
    char* pwd[1024];
    
    *pwd = getenv("PWD");
    printf("wd : %s\n", *pwd);
    
    
    
    
    glutInit(&argc, argv);
    initialize_glut(WIN_WIDTH, WIN_HEIGHT, SUB_WINDOW_HEIGHT);	
    
     
    
    
#ifndef __MAC_OS_X__
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
	    fprintf(stderr, "Error %s\n", glewGetErrorString(err));
	    exit(1);
	}

	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	if (GLEW_ARB_vertex_program)
    	fprintf(stdout, "Status: ARB vertex programs available.\n");

	if (glewGetExtension("GL_ARB_fragment_program"))
    	fprintf(stdout, "Status: ARB fragment programs available.\n");

	if (glewIsSupported("GL_VERSION_1_4  GL_ARB_point_sprite"))
    	fprintf(stdout, "Status: ARB point sprites available.\n");
#endif

    for(int i =0; i< nb_texture;i++)
    {
        if ( !load_texture(texture_path+texturename[i], id_texture[i]) )
            cerr << "Texture manquante : " << texture_path+texturename[i] << endl;
    }
    
    init_geometry();
    
    glutMainLoop();
    
    exit();
    
 
	return 1;
}



