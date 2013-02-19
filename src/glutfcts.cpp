


#ifndef __MAC_OS_X__
#include <GL/glew.h>
#include <GL/glut.h>
#else
#include "GLUT/glut.h"
#endif

#include <string>
#include <iostream>
#include <cmath>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;

#include "glutfcts.h"
#include "point3d.h"
#include "diagnostic.h"
#include "textures.h"

#include "CMaillage.h"






char	WND_NAME[] = "proto";

int main_wnd_id;
int sub_wnd_id;
VBO TheVBO;

static int sub_win_heigth(32);

using namespace std;


static bool opengl2_supported = false;

// Menus
#define M_EXIT			1

static float transx = 0.0, transy = 0.0, transz = -50.0, rotx = 0.0, roty = 0.0;
static int ox = -1, oy = -1;
static int mot = 0;
#define PAN	1
#define ROT	2
#define MVZ	3



static GLfloat red1[]    = { 0.7, 0.2, 0.2, 1.0 };
static GLfloat green1[]    = { 0.2, 0.7, 0.0, 1.0 };
static GLfloat blue1[]    = { 0.2, 0.2, 0.7, 1.0 };

bool texLinearFiltering = true;
bool Phong = true;
GLfloat shininess=0.25;
GLuint curTexId = 0;

// --------------------------------------------------------------------------------
//  Fonction: draw_string
//
//  Par:   Yves Chiricota
//  Date:  02/03/07
//  MAJ:
// --------------------------------------------------------------------------------
static void  draw_string(char *s)
{
	unsigned int i;
	for (i = 0; i < strlen(s); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, s[i]);
};



// --------------------------------------------------------------------------------
//  Fonction: menu_main
//
//  Par:   Yves Chiricota
//  Date:  02/03/07
//  MAJ:
// --------------------------------------------------------------------------------
void menu_main(int item)
{
	switch (item)
    {
		case M_EXIT:
			exit(0);
			break;
	}
}

// --------------------------------------------------------------------------------
//  Fonction: create_menus
//
//  Par:   Yves Chiricota
//  Date:  02/03/07
//  MAJ:
// --------------------------------------------------------------------------------
void create_menus()
{
	int menu;
    
	menu = glutCreateMenu(menu_main);
	glutAddMenuEntry("Exit",M_EXIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}




// --------------------------------------------------------------------------------
//  Fonction: keyboard
//
//  Par:   Yves Chiricota
//  Date:  02/03/07
//  MAJ: 05/02/13 par Pierre Rouveyrol
// --------------------------------------------------------------------------------
void keyboard(unsigned char key, int x, int y)
{
    
    
	switch(key)
	{   case 'w': //shininess x 2. Till 32.0
            if(shininess<32)
                shininess = shininess*2;
            glMaterialf(GL_FRONT,GL_SHININESS,shininess);
            break;
            
        case 's': //shininess x 0.5. Till 0.25
            if(shininess>0.25)
                shininess = shininess*0.5;
            glMaterialf(GL_FRONT,GL_SHININESS,shininess);
            break;
            
        case 'p': //Enables/Disables Phong shading
            if(Phong)
            {
                glShadeModel(GL_FLAT);
                Phong = false;
            }
            else
            {
                glShadeModel(GL_SMOOTH);
                Phong = true;
            }
            break;
        case 'i':
            texLinearFiltering = !texLinearFiltering;
            break;
        case 't':
            curTexId = (curTexId+1)%nb_texture;
            break;
        case 'q':
        case  27:   // ESC
            exit(0);
            
            
	}
    
    glutPostRedisplay();
}


// --------------------------------------------------------------------------------
//  Fonction: pan
//
//  Par:   Yves Chiricota
//  Date:  02/03/07
//  MAJ:
// --------------------------------------------------------------------------------
void pan(int x, int y)
{
	transx += (x - ox)/300.;
	transy -= (y - oy)/300.;
	ox = x;
	oy = y;
	glutPostRedisplay();
}

// --------------------------------------------------------------------------------
//  Fonction: rotate
//
//  Par:   Yves Chiricota
//  Date:  02/03/07
//  MAJ:
// --------------------------------------------------------------------------------
void rotate(int x, int y)
{
	rotx += x - ox;
	roty += y - oy;
	ox = x;
	oy = y;
	glutPostRedisplay();
}

// --------------------------------------------------------------------------------
//  Fonction: movez
//
//  Par:   Yves Chiricota
//  Date:  02/03/07
//  MAJ:
// --------------------------------------------------------------------------------
void movez(int y)
{
	transz += (y - oy) / 30.;
	oy = y;
	glutPostRedisplay();
}

// --------------------------------------------------------------------------------
//  Fonction: motion
//
//  Par:   Yves Chiricota
//  Date:  02/03/07
//  MAJ:
// --------------------------------------------------------------------------------
void motion(int x, int y)
{
	if ( mot == PAN )
		pan(x, y);
	else if ( mot == ROT )
		rotate(x, y);
	else if ( mot == MVZ )
		movez(y);
}

// --------------------------------------------------------------------------------
//  Fonction: mouse
//
//  Par:   Yves Chiricota
//  Date:  02/03/07
//  MAJ:
// --------------------------------------------------------------------------------
void mouse(int button, int state, int x, int y)
{
	if ( state == GLUT_DOWN )
	{
		switch ( button )
		{
			case GLUT_LEFT_BUTTON:
			{
				int modifier = glutGetModifiers();
                //cout << modifier << endl;
                ox = x;
                oy = y;
				switch ( modifier )
				{
					case GLUT_ACTIVE_SHIFT:
						mot = MVZ;
						motion(x, y);
						break;
					case GLUT_ACTIVE_CTRL:
						break;
					case GLUT_ACTIVE_ALT:
						mot = PAN;
						motion(x, y);
						break;
					default:
						mot = ROT;
						motion(x, y);
						break;
				}
			}
			case GLUT_MIDDLE_BUTTON:
				break;
			case GLUT_RIGHT_BUTTON:
				break;
		}
	}
	else if ( state == GLUT_UP )
	{
		mot = 0;
	}
}



// --------------------------------------------------------------------------------
//  Fonction: reshape
//
//  Par:   Yves Chiricota
//  Date:  02/03/07
//  MAJ:
// --------------------------------------------------------------------------------
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)w/(GLfloat)h, 0.1, 500.0);
	glMatrixMode(GL_MODELVIEW);
    
    
	glutSetWindow(sub_wnd_id);
	glutReshapeWindow(w - 1, sub_win_heigth);
	glutPositionWindow(1, h - sub_win_heigth);
	glutSetWindow(main_wnd_id);
};


// --------------------------------------------------------------------------------
//  Fonction: idle
//
//  Par:   Yves Chiricota
//  Date:  02/03/07
//  MAJ:
// --------------------------------------------------------------------------------
void idle()
{
    glutSetWindow(main_wnd_id);
    glutPostRedisplay();
    glutSetWindow(sub_wnd_id);
    glutPostRedisplay();
    glutSetWindow(main_wnd_id);
    
}



// --------------------------------------------------------------------------------
//  Fonction: display
//
//  Par:   Yves Chiricota
//  Date:  02/03/07
//  MAJ:
// --------------------------------------------------------------------------------
void display()
{
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //printOpenGLError;
    
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
    glLoadIdentity();
	glTranslatef(transx, transy, transz);
	glRotatef(roty, 1., 0., 0.);
	glRotatef(rotx, 0., 1., 0.);
    
    
#if 1 // Repere
    glColor3fv(red1);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(10.0, 0.0, 0.0);
    glColor3fv(green1);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 10.0, 0.0);
    glColor3fv(blue1);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 10.0);
    glEnd();
    
#endif
    glBindTexture(GL_TEXTURE_2D, id_texture[curTexId]);
    if(texLinearFiltering)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    TheVBO.Draw();
    
    glPopMatrix();
	glutSwapBuffers();
    //printOpenGLError;
    
}



// Sous fenêtre.
//

static char buf[1024];

// --------------------------------------------------------------------------------
//  Fonction: sub_reshape
//
//  Par:   Yves Chiricota
//  Date:  02/03/07
//  MAJ:
// --------------------------------------------------------------------------------
void  sub_reshape(int w, int h)
{
	glViewport (0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0.0F, 1.0F, 0.0F, 1.0F);
};



// --------------------------------------------------------------------------------
//  Fonction: sub_display
//
//  Par:   Yves Chiricota
//  Date:  02/03/07
//  MAJ:
// --------------------------------------------------------------------------------
void  sub_display()
{
	glutSetWindow(sub_wnd_id);
	glClearColor(0.50, 0.50, 0.50, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glColor3f (0.9F, 0.9F, 0.9F);
	glBegin (GL_LINE_LOOP);
    glVertex2f (0.0F, 0.0F);
    glVertex2f (0.0F, 0.99F);
    glVertex2f (0.999F, 0.99F);
    glVertex2f (0.999F, 0.0F);
	glEnd ();
    
	glColor3f (1.0F, 1.0F, 1.0F);
     
    
    
	if ( !opengl2_supported )
		sprintf(buf, "OpenGL 2 not supported, cannot use GPU");
	else
	{
		sprintf(buf, "OpenGL 2 supported");
	}
    
	glRasterPos2f (0.72F, 0.65F);
	draw_string(buf);
    
	glutSwapBuffers();
	glutSetWindow(main_wnd_id); // Important pour éviter un bug GPU.
    
};



// --------------------------------------------------------------------------------
//  Fonction: discretise
//  Cette fonction calcule le maillage de taille M × N et le place dans le VBO passe en reference.
//  Par:   Jean Meyblum et Pierre Rouveyrol
//  Date:  01/02/13
//  MAJ:
// --------------------------------------------------------------------------------

bool discretise(int M, int N, float R, float r, VBO& v)
{

    CMaillage theMaillage(M+1,N+1);
    for(int i=0; i<M+1; i++)
    {
        
        for(int j=0; j<N+1; j++)
        {
            //calculer les coordonnes des points
            float u = 2*M_PI*(float)i/M;
            float v = 2*M_PI*(float)j/N;
            float x = (R+r*cos(v))*cos(u);
            float y = (R+r*cos(v))*sin(u);
            float z = r*sin(v);
            
            //derivees partielles normale
            float xu = sin(u)*(-(r*cos(v)+R));
            float yu = cos(u)*(r*cos(v)+R);
            float zu = 0;
            
            float xv = -r*cos(u)*sin(v);
            float yv = -r*sin(u)*sin(v);
            float zv = r*cos(v);
            
            float xn = yu * zv - yv * zu;
            float yn =  zu * xv - zv * xu;
            float zn = xu * yv - xv * yu;
            
            float nm = sqrt(xn*xn+yn*yn+zn*zn);
            vector<float> cur_sommet;
        
            // position x y z
            cur_sommet.push_back(x);
            cur_sommet.push_back(y);
            cur_sommet.push_back(z);
            // normal x y z
            cur_sommet.push_back((xn/nm));
            cur_sommet.push_back((yn/nm));
            cur_sommet.push_back((zn/nm));
            // texture coord s t
            cur_sommet.push_back((float)j/N);
            cur_sommet.push_back((float)i/M);
            
            theMaillage.AddSommet(cur_sommet);
        }

    }
    theMaillage.FillVBO(v);
    return 0;
}



// --------------------------------------------------------------------------------
//  Fonction: initialize_glut
//
//  Par:   Yves Chiricota
//  Date:  02/03/07
//  MAJ:
// --------------------------------------------------------------------------------
void initialize_glut(int main_w, int main_h, int sub_h)
{
	sub_win_heigth = sub_h;
    
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    
	
	glutInitWindowSize(main_w, main_h);
	glutInitWindowPosition(10, 10);
	main_wnd_id = glutCreateWindow(WND_NAME);
    
    int minor, major;
    getGlVersion(major, minor);
	if ( major >= 2 )
		opengl2_supported = true;
	else
		opengl2_supported = false;

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	create_menus();
    
	sub_wnd_id = glutCreateSubWindow(main_wnd_id, 1 , main_h - sub_h, main_w - 1 , sub_h);
	glutDisplayFunc(sub_display);
	glutReshapeFunc(sub_reshape);
    
	glutSetWindow(main_wnd_id);
    
    discretise(4, 4, 12, 4, TheVBO);

}


void    exit()
{
    glDeleteTextures(1, &id_texture[0]);
}



#pragma mark -
#pragma mark geometry


void init_geometry()
{
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    
    GLfloat LightAmbient[]=		{ 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightSpecular[]=	{ 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightPosition1[]=	{ 0.0f, 0.0f, 30.0f, 1.0f };
    
    GLfloat mat_ambient[]=		{ 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat mat_diffuse[]=		{ 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat mat_specular[]=     { 0.8f, 0.8f, 0.8f, 1.0f };
    
    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialf(GL_FRONT,GL_SHININESS,shininess);
    
    glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);	// Setup The Specular Light
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition1);	// Position The Light
	glEnable(GL_LIGHT0);
}


