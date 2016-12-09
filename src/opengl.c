

#include <X11/Xlib.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include "graph.h"

static GLXPixmap gpixmap;

void initOpenGl (graph * ooo)
{
	XVisualInfo *visInf, temp;
	GLXContext cont;
	int len;
	temp .visualid = ooo ->vis ->visualid;
	temp .screen = ooo ->screen;
	visInf = XGetVisualInfo (ooo ->dis, VisualScreenMask 
								  | VisualIDMask,  &temp, &len);
	cont = glXCreateContext (ooo ->dis, visInf, NULL, False);
	glXWaitGL ();
	gpixmap = glXCreateGLXPixmap (ooo ->dis, visInf, ooo ->pixmap);
	glXMakeCurrent (ooo ->dis, gpixmap, cont);

	glClearIndex (one .missingColor);
	glLoadIdentity ();
	{
    	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    	GLfloat mat_shininess[] = { 500.0 };
    	GLfloat light_position[] = { 0.0, 200.0, 0.0, 0.0 };

	 	int atX = ooo ->MapShiftX;
		int atY = ooo ->MapShiftY;
		int mW = ooo ->MapWidth;
		int mH = ooo ->MapHeight;
		glViewport(atX, atY, mW, mH);
		glOrtho (0, mW, mH, 0, -256, 0);

/*    	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    	glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular);
    	glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess);
    	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    	glEnable(GL_LIGHTING);
    	glEnable(GL_LIGHT0);
    	glDepthFunc(GL_LESS);
    	glEnable(GL_DEPTH_TEST);
*/
	}
}


void fillLineGl (int col, int length, short * line, int pattern)
{
	int i;
	float r = one .color [col] . red / (float)0xffff;
	float g = one .color [col] . green / (float)0xffff;
	float b = one .color [col] . blue / (float)0xffff;
/*	printf ("%f %f %f\n", r, g, b);*/
	glIndexi (col);
/*	glIndexf ((r + g + b)/3);*/
	glBegin (GL_POLYGON);
		for (i = 0; i < length; i++){
			float x = line [2*i];
			float y = line [2*i+1];
			float z = col;
			glVertex3f (x, y, z);
		}
	glEnd ();
	glFlush ();
}



