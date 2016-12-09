#include <stdio.h>
#include <linux/stddef.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/Xaw3d/Label.h>
#include <X11/Xaw3d/Form.h>

#include "graph.h"

void getInput (Widget W, char * query,
					char * defAnswer, int * result, graph * ooo,
					void (* proc) (void*, char*));
void procInt (void * x, char * text)
{	*((int *)x) = atoi (text);}
void procFloat (void * x, char * text)
{	*((float *)x) = atof (text);}
void procString (void * x, char * text)
{	strcpy((char *)x, text);}
void procClrMapString (void * x, char * text)
{		SetColormap (&one, text);}

void PrintImage (graph * ooo)
{
	XImage * im = CreateXImage (ooo ->width, ooo ->height);
	int i, j;
	FILE * out = fopen ("/tmp/dump", "w");
	fprintf (out, "P5\n%d %d\n255\n", ooo ->width, ooo ->height);
	for (i = 0; i <  ooo ->height; i++)
		for (j = 0; j <  ooo ->width; j++){
			unsigned char c = XGetPixel (im, j, i);
			if (c == 0)
				c = 255;
			else{
				if (c != 1)
					c = 255 - c;
			}
			fwrite (&c, 1, 1, out);
		}
	fclose (out);	
	XDestroyImage (im);
	{
		char buf [1000];
		sprintf (buf, "cat /tmp/dump | rsh stupid /usr/contributed/bin/pnmtops > %s.ps",
					ooo ->data .DisNames [ooo ->View]);
		system (buf);
	}
}
	
void MenuSelect (Widget w, XtPointer paneNum,  XtPointer pIII)
{
	char defAnsw [100];
	char answ [100];
	printf ("Read Page Menu item %d was selected (widget=%d, data=%d)\n",
			  (int)paneNum, w, pIII);
	switch ((int)paneNum){
	case 0:
		sprintf (defAnsw, "%d", one .interpolate);
		getInput (w, "Enter Number of Frames to Interpolate",
					 defAnsw, &(one .interpolate), &one, procInt);
		break;
	case 1:
		sprintf (defAnsw, "%d", one .showMiss);
		getInput (w, "Set Missing Value",
					 defAnsw, &(one .showMiss), &one, procInt);
		break;
	case 2:
		sprintf (defAnsw, "%f", one .subsample);
		getInput (w, "Set subsampling rate >0 and <= 1",
					 defAnsw, (int*)&(one .subsample), &one, procFloat);
		break;
	case 3:
		sprintf (defAnsw, "%s", "clr");
		getInput (w, "Set colormap to:",
					 defAnsw, (int*)answ, &one, procClrMapString);
		break;
	case 4:		
		PrintImage (&one);
		break;
	}
}

int WorkProc (graph * ooo)
{
	if (one .data .NMonths - 1 ==  one .data .Month)
		ooo ->direction = -1;
	if (0 ==  one .data .Month)
		ooo ->direction = 1;
	ScrollProcHTime (NULL, ooo ->horizontalTime, - ooo ->direction);	
	return 0;
}


void playCback (Widget w, graph * ooo)
{
	fprintf (stderr, "in playCback %d\n", ooo ->direction);
	
	if (!ooo ->direction){
		ooo ->direction = 1;
		ooo ->work =
			XtAppAddWorkProc (ooo ->appCon, (XtWorkProc)WorkProc, ooo);
		XtVaSetValues (w,
			XtNlabel, "Stop",
			NULL);
	}else{
		XtRemoveWorkProc (ooo ->work);
		ooo ->direction = 0;
		XtVaSetValues (w,
			XtNlabel, "Play",
			NULL);
	}
}

typedef struct {
	Widget plot, plotDat, oldWin;
	Window oldPix;	
	Pixmap pix;
	int oldWid, oldHei;
	int month, dis;
	int wid, hei;
} recordData;


recordData * StartRecWindow (int wid, int hei)
{
	Widget  plotDat, box, form, close;
	recordData * p = (recordData *) malloc (sizeof (recordData));
	p ->wid = wid;
	p ->hei = hei;
	p ->month = 0;
	p ->dis = 0;
	p ->plot  = XtVaCreatePopupShell
 		("Record", 											
		transientShellWidgetClass,
      one .toplevel,
      XtNdepth, one .depth, 				  
		XtNcolormap, one .cmap,
		XtNvisual, one .vis,
		NULL);
	box = XtVaCreateManagedWidget
		("Box",
		 formWidgetClass, p ->plot,
		 NULL);
	p ->plotDat = XtVaCreateManagedWidget ("plotData",
      labelWidgetClass,  box,
      XtNdepth, one .depth, 				  
		XtNcolormap, one .cmap,
		XtNvisual, one .vis,
      XtNorientation, (XtOrientation) XtorientVertical,
		XtNwidth, p ->wid,
		XtNheight, p ->hei,
      NULL);
	
	XtPopup (p ->plot, XtGrabNone);

	p ->pix = XCreatePixmap (one .dis, XtWindow (p ->plotDat),
									  p ->wid, p ->hei, one .depth);
	XtVaSetValues (p ->plotDat, XtNbitmap, p ->pix,
					NULL);

	p ->oldPix  = one .pixmap;
	p ->oldWin  = one .mainW;
	p ->oldWid  = one .MapWidth;
	p ->oldHei  = one .MapHeight;	
	one .legendTextAtt = "my7";
	one .pixmap = p ->pix;
	one .mainW = p ->plotDat;
	one .width = wid;
	one .height = hei;
	one .MapShiftX = 80;
	one .ColorBarWidth = 60;
	one .MapShiftY = 190;
	one .MapWidth = wid - 2*one .MapShiftX;
	one .MapHeight = hei - one .MapShiftY * 1.2;
	one .horizontalBar = 1;
	clearScreen (one .missingColor);
	{
		char shape [16*16];
		Pixmap ss;
		int i;
		XColor a, b;
		a .pixel = a .red = a .green = a .blue = a .flags = 0;
		b .pixel = b .red = b .green = b .blue = b .flags = 0;
		for (i = 0; i < 16*16; i++)
			shape [i] = 0;
		ss = XCreatePixmapFromBitmapData (one .dis, XtWindow (p ->plotDat),
				      shape, 16, 16, 1, 0, 1);
		
		XDefineCursor (one .dis, XtWindow (p ->plotDat),
							XCreatePixmapCursor (one .dis, ss, ss, &a, &b, 0, 0));
	}
	return p;
}

int WorkRecProc (graph * ooo)
{	
	if (one .data .NMonths - 1 ==  one .data .Month){
		ScrollProcHTime (NULL, ooo ->horizontalTime,
							  - ooo ->direction);
		ooo ->DoRecording = 1;
		return 1;
	}else
		ScrollProcHTime (NULL, ooo ->horizontalTime,
							  - ooo ->direction);
	return 0;
}
extern void ColorLegend ();
void Record (void)
{
	int i;
	ColorLegend ();
	flushGraph ();
	if (one .DoRecording){
		fprintf (stderr, "BRECORD--------------\n");
/*		scanf ("%d", &i);*/
#ifdef DO_REC
		recordLVR ();
#endif
/*		scanf ("%d", &i);*/
		fprintf (stderr, "ARECORD--------------\n");
	}
}

void recordCback (Widget w, graph * ooo)
{
        char str [100];
	static XtWorkProcId work = 0;
	static recordData * p;
	fprintf (stderr, "in recordCback %d\n", ooo ->direction);
	if (!ooo ->DoRecording){
		ooo ->direction = 1;
		ooo ->DoRecording = 1;
		p = StartRecWindow (1080, 720);
		/* init Scan Converter */
                sprintf (str, "echo 'EXTZM 0' | %s/otto", getenv ("MIASMA"));
                system (str);
		sprintf (str, "echo 'WINDW 020 010 3c0' | %s/otto", getenv ("MIASMA")); 
                system (str);
#ifdef DO_REC
		/* init LVR */
		initLVR (ooo ->data .DisNames [ooo ->View]);
#endif
		work =
			XtAppAddWorkProc (ooo ->appCon, (XtWorkProc)WorkRecProc,
									ooo);
		XtVaSetValues (w,
			XtNlabel, "Stop Recording",
			NULL);
	}else{
		XtRemoveWorkProc (work);
		work = 0;
		ooo ->DoRecording = 0;
		one .mainW = p ->oldWin;
		one .pixmap = p ->oldPix;
		XtVaSetValues (w, XtNlabel, "Record", NULL);
		XtDestroyWidget (p ->plot);
		XFreePixmap (ooo ->dis, p ->pix);
		free (p);
	}
}

void fillCback (Widget w, graph * ooo)
{
	fprintf (stderr, "in fillCback %p %p %d\n", w, ooo, ooo ->fill);
	
	if (ooo -> fill){
		XtVaSetValues (w,
			XtNlabel, "Fill",
			NULL);
	}else{
		XtVaSetValues (w,
			XtNlabel, "Smooth",
			NULL);
	}
	ooo ->fill = ! ooo ->fill;
	clearScreen (one .missingColor);
	Redraw (ooo);
}









