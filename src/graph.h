#ifndef GRAPH_H
#define GRAPH_H


#define AppName "Graph"
#include "data.h"
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

#define MISSING  -1

typedef struct { 
/* XWindows stuff*/
	char * DisplayName;
	Display * dis;
  	int screen, depth;
	Visual * vis;
	Colormap cmap, defCmap;
	XColor * color;
	int nOfEntries;
	int foreground, background,
		missingColor;
	Window rwin;
	Pixmap pixmap, stiple;
	Cursor Wait, Cross;
	XGCValues val;
	XSetWindowAttributes attr;
	GC textgc, xorgc, xorOutlinegc, currentgc, highlightgc, 
		stip1gc, bitgc;
	XFontStruct *fontInfo;

/* Xt stuff*/
	XtAppContext appCon;
	XtWorkProcId work;
	Widget   toplevel, app, view, vpane, mainW, bb,
				out, help, scroll,
				viewWidget, transWidget,  
				horizontalTime, horizontalLambda,
				form,
				playButton, recordButton,
				fillButton, viewLabel,
				transformationLabel,  lambdaLabel, timeLabel,
				mapArea;
	Widget menu, menuButton;

/* Map drawing stuff */
	int width, height, MapWidth, MapHeight, MapShiftX, MapShiftY;
	float mapAspect;
	int PlotColorBar, horizontalBar, PlotText, NLabels, ColorBarWidth;
	char * legendTextAtt;
	
	int fill, interpolate, showMiss, direction;
	float Lambda;
	float subsample;
	
	Data data;
	int NViews, View;

	int NTransformations, Transformation;
	char ** TransformationNames;

	int DoRecording;
} graph;

extern graph one;

#ifdef __cplusplus
extern "C" {
#endif
void initOpenGl ();	
void PlotTimeSeries (graph * ooo, Window win,
							int disease, int state,
							int wid, int hei);
void Redraw (graph * ooo);
void Rescale (graph * ooo);
void ChangeLambda (double lambda);
void svHandler (int sig);
void initGraph (char * cmapName);
void putColorScale (int n, float * x);
void closeGraph (graph * ooo);
void clearScreen (int col);
int  LinearToThermo (int i);
void putImage (int * image, int atX, int atY, int byX, int byY);
void putImageExpand (int * image, int atX, int atY, int byX, int byY,
							float subsample);
void getImage (int * image, int atX, int atY, int byX, int byY);
void SetColormap (graph * ooo, char *  cmapName);


int  random_sample (int lineSize, int retSize,
						 int w, int h, short * line,
						 short * ret);
void fillLineGl (int col, int len, short * line, int pattern);
void fillLine
	(
	 int col,     /* Color */
	 int length,  /* Number of points in the line
						  (half the length of the array line */
	 short * line, /* the array of points x1,y1,x2,y2,... */
	 int pattern
	 );
void drawLine
	(
	 int col,     /* Color */
	 int length,  /* Number of points in the line
						  (half the length of the array line */
	 short * line /* the array of points x1,y1,x2,y2,... */
	 );
void drawPoints
	(
	 int col,     /* Color */
	 int length,  /* Number of points in the line
						  (half the length of the array line */
	 short * line /* the array of points x1,y1,x2,y2,... */
	 );
void text
	(
	 int x, /* horizontal offset */
	 int y, /* vertical offset   */
	 char * str /* text itself */
	 );
void verText (int x, int y, float rotation, int allignment, char * str);
void magText (float mag);
void setTextAtt
	(
	 int color, /* color of the text */
	 char * font /* Name of the X font (as shown by xlsfonts) */
	 );

void flushGraph (void);
XImage * CreateXImage (int width, int height);
void waiting (graph *);
void doneWaiting (graph *);


#ifdef  __cplusplus	
};
#endif	

#endif	








