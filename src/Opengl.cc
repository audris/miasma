#include <math.h>
#include <X11/Xlib.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include "graph.h"
#include "util.h"

static float SX = 1, SY = 1, SZ = 1,
	TX = 0, TY = 0, TZ = 0,	
	ROTA = 0, ROTB=0;
	
void fillSmoothGl (char * id, int hor, int ver, 
	iarray& regular,
	int atX, int atY,  
	int w, int h,
	iarray& dat)
{
	int i, j;
	atX + atY;
/*	XSetForeground (one .dis, one .currentgc, one .missingColor);
	XFillRectangle (one .dis, one .pixmap, one .currentgc, 0, 0,
	one .width, one .height);
	*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity ();
	gluLookAt (sin (ROTA*2.0/one .width * M_PI/2),
				  -cos (ROTB*2.0/one .width*M_PI/2 + M_PI/2), -1,
				  0, 0, 0,
				  0, 1, 1);
//	glTranslatef (TX, -TY, TZ);	
//	glScalef (SX, SY, SZ);
	glOrtho (0, one .width,
				one .height, 0, -256, 256);
	for (i = 0; i < hor-1; i++){
		glBegin (GL_TRIANGLE_STRIP);
		for (j = 0; j < ver-1; j++){
			int bl =  (dat) (i*ver + j);
			int br =  (dat) ((i+1)*ver + j);
			int tl =  (dat) (i*ver + j + 1);
			int tr =  (dat) ((i+1)*ver + j + 1);
  			int lX = (regular) (0, i*ver+j);  
			int rX = (regular) (0, (i+1)*ver+j+1);  
			int bY = (regular) (1, i*ver+j); 
			int tY = (regular) (1, (i+1)*ver+j+1);
//			printf ("%d %d %d %d\n", bl, br, tl, tr);
			if (id [int(lX + (h-bY)*w)]){
				glIndexi (LinearToThermo (bl));
 				glVertex3i (lX, bY, bl);
				glIndexi (LinearToThermo (br));
 				glVertex3i (rX, bY, br);
				glIndexi (LinearToThermo (tl));
	 			glVertex3i (lX, tY, tl);
				glIndexi (LinearToThermo (tr));
 				glVertex3i (rX, tY, tr);
			}
		}
		glEnd ();
	}
	glFlush ();
	glPopMatrix();
}



extern "C" void RedrawSmoothLeft (float tX, float tY, float tZ)
{
	TX = tX;
	TY = tY;
	TZ = tZ;
	Redraw (&one);
}

extern "C" void RedrawSmoothMiddle ( float sX, float sY, float sZ)
{
	SX = sX;
	SY = sY;
	SZ = sZ;
	Redraw (&one);
}

extern "C" void RedrawSmoothRight (float rotA, float rotB)
{
	ROTA = rotA;
	ROTB = rotB;
	Redraw (&one);
}






