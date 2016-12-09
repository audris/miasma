#include <stdio.h>
#include <linux/stddef.h>
#include <stdlib.h>
#include <math.h>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw3d/Paned.h>
#include <X11/Xaw3d/Scrollbar.h>
#include <X11/Xaw3d/Box.h>
#include <X11/Xaw3d/Form.h>
#include <X11/Xaw3d/Toggle.h>
#include <X11/Xaw3d/Label.h>
#include <X11/Xaw3d/List.h>

#include "graph.h"
#include "action.h"

XtCallbackProc selectView (Widget w, XtPointer position, XawListReturnStruct * data)
{
	int old = one .View;
	waiting (&one);
	fprintf (stderr, "In action select, widget %p, %d %s\n",
				w, data ->list_index, data ->string);
	one .View = data ->list_index;

	if (old != one .View){
		clearScreen (one .missingColor);
		Redraw (&one);
	}
	doneWaiting (&one);	
	return 0;
}

XtCallbackProc selectTrans (Widget w, XtPointer position,
									  XawListReturnStruct * data)
{
	int old = one .Transformation;
	waiting (&one);
	one .Transformation = data ->list_index;;
	if (old != one .Transformation){
		Rescale (&one);
		clearScreen (one .missingColor);
		Redraw (&one);
	}
	doneWaiting (&one);	
	return 0;
}

XtCallbackProc ScrollProcHTime (Widget w, XtPointer data, XtPointer position)
{
	int pos = (int) position;
	int old = one .data .Month;
	if (pos < 0){
		if (one .data .Month < one .data .NMonths - 1)
			one .data .Month++;
	}else
		if (one .data .Month)
			one .data .Month--;
	if (data)
		XawScrollbarSetThumb (data, (float)(1.0*one .data .Month/
													one .data .NMonths),
								1.0/one .data .NMonths);
	if (old != one .data .Month){
		static char buf [100];
		Redraw (&one);
		sprintf (buf, "%.4d/%.2d", year (&(one .data)),
					month (&(one .data)));
		XtVaSetValues (one .timeLabel,
			XtNlabel, buf,
			NULL);
	}
	fprintf (stderr, "In action ScrollProc, widget %p, data %p, position %d\n",
				w, data, pos);
	return 0;
}


XtCallbackProc JumpProcHTime (Widget w, XtPointer data, XtPointer percent)
{
	int old = one .data .Month;
	float perc = *((float *) percent);
	waiting (&one);
	fprintf (stderr, "In action jumpProc, widget %p, percent %f\n",
				w, perc);
	one .data .Month = one .data .NMonths * perc;
	if (one .data .Month == one .data .NMonths)
		one .data .Month --;
	if (old != one .data .Month){
		static char buf [100];
		Redraw (&one);
		sprintf (buf, "%.4d/%.2d", year (&(one .data)),
					month (&(one .data)));
		XtVaSetValues (one .timeLabel,
			XtNlabel, buf,
			NULL);
	}
	doneWaiting (&one);	
	return 0;
}



XtCallbackProc JumpProcHLambda (Widget w, XtPointer data, XtPointer percent)
{
	static char buf [100];
	float perc = *((float *) percent);
	waiting (&one);
	fprintf (stderr, "In action jumpProc, widget %p, percent %f\n",
				w, perc);	
	one .Lambda = pow (perc, 4);
	ChangeLambda (one .Lambda);
	Redraw (&one);
	sprintf (buf, "%.3e", one .Lambda);
	XtVaSetValues (one .lambdaLabel,
		XtNlabel, buf,
		NULL);
	doneWaiting (&one);
	return 0;
}


typedef struct {
	Widget plot, plotDat, changeState,
		changeDisease, stateW, diseaseW;
	Pixmap pix;
	int state, dis;
	int wid, hei;
} plotData;

XtCallbackProc JumpProcState (Widget w,
										XtPointer data,
										XtPointer percent)
{
	plotData * p = (plotData *) data;
	float perc = *((float *) percent);
	int newState = one .data .NStates * perc;
	waiting (&one);
	if (newState > one .data .NStates - 1)
		newState = one .data .NStates - 1;
	if (newState != p ->state){
		XtVaSetValues (p ->stateW,
			XtNlabel, one .data .StateNames [newState],
			NULL);
		p ->state= newState;
		PlotTimeSeries (&one, p ->pix, p ->dis,
							 p ->state,
							p ->wid, p ->hei);
		PlotTimeSeries (&one, XtWindow(p ->plotDat), p ->dis,
							 p ->state,
							p ->wid, p ->hei);
	}
	doneWaiting (&one);
	return 0;
}

XtCallbackProc JumpProcDis (Widget w,
										XtPointer data,
										XtPointer percent)
{
	plotData * p = (plotData *) data;
	float perc = *((float *) percent);
	int newState = one .data .NDis * perc;
	waiting (&one);
	if (newState > one .data .NDis - 1)
		newState = one .data .NDis - 1;
	if (newState != p ->dis){
		XtVaSetValues (p ->diseaseW,
			XtNlabel, one .data .DisNames [newState],
			NULL);
		p ->dis = newState;
		PlotTimeSeries (&one, p ->pix, p ->dis,
							 p ->state,
							p ->wid, p ->hei);
		PlotTimeSeries (&one, XtWindow(p ->plotDat), p ->dis,
							 p ->state,
							p ->wid+3, p ->hei+3);
	}
	doneWaiting (&one);	
	return 0;
}

static XtCallbackProc ClosePlotCback (Widget w, plotData *p)
{
	XtDestroyWidget(XtParent(XtParent(XtParent(w))));
	XFreePixmap (one .dis, p ->pix);
	free (p);
}

XtCallbackProc CtrlLeftClickAction (Widget w, XButtonEvent * event)
{
	Widget  plotDat, box, mbox, form, close;
	plotData * p = (plotData *) malloc (sizeof (plotData));
	waiting (&one);
	p ->wid = 800;
	p ->hei = 200;
	p ->state = 0;
	p ->dis = 0;
	fprintf (stderr, "In action CtrlLeftClickAction at %d %d\n",
				event ->x, event ->y);
	p ->plot  = XtVaCreatePopupShell
		("Time Series Plot", 											
		topLevelShellWidgetClass,
      one .toplevel,
      XtNdepth, one .depth, 				  
		XtNcolormap, one .cmap,
		XtNvisual, one .vis,
		XtNminHeight, p ->hei+80,
		XtNminWidth, p ->wid+45,
		XtNmaxHeight, p ->hei+80,
		XtNmaxWidth, p ->wid+45,
/*		XtNtitle, (String) one .data .DisNames [one .data .Disease],
      XtNiconName, (String) one .data .DisNames [one .data .Disease],*/
		NULL);
	box = XtVaCreateManagedWidget
		("Box",
		 formWidgetClass, p ->plot,
		 NULL);
	mbox = XtVaCreateManagedWidget
		("Box",
		 formWidgetClass, box,
		 NULL);
	p ->diseaseW = XtVaCreateManagedWidget
		(one .data .DisNames [p ->dis],
		 labelWidgetClass, mbox,
		 NULL);
	p ->stateW = XtVaCreateManagedWidget
		(one .data .StateNames [p ->state],
		 labelWidgetClass, mbox,
		 XtNfromHoriz, p ->diseaseW,
		 NULL);
	close = XtVaCreateManagedWidget
		("Close",
		 commandWidgetClass, mbox,
		 XtNfromHoriz, p ->stateW,
		 NULL);
	p ->changeDisease = XtVaCreateManagedWidget
		("Change Disease",
		 scrollbarWidgetClass, box,
		 XtNfromVert, mbox/*p ->diseaseW*/,
		 XtNthickness, 20,
		 XtNlength, p ->hei,
		 XtNorientation, XtorientVertical,
		 NULL);
	p ->plotDat = XtVaCreateManagedWidget ("plotData",
      labelWidgetClass,  box,
      XtNdepth, one .depth, 				  
		XtNcolormap, one .cmap,
		XtNvisual, one .vis,
      XtNorientation, (XtOrientation) XtorientVertical,
		XtNwidth, p ->wid,
		XtNheight, p ->hei,
      XtNfromVert, mbox/*p ->diseaseW*/,
		XtNfromHoriz, p ->changeDisease,
      NULL);
	p ->changeState = XtVaCreateManagedWidget
		("Change State",
		 scrollbarWidgetClass, box,
		 XtNfromVert, p ->plotDat,
		 XtNthickness, 20,
		 XtNlength, p ->wid,
		 XtNorientation, XtorientHorizontal,
		 NULL);	
	XtPopup (p ->plot, XtGrabNone);

	XtAddCallback(p ->changeDisease, XtNjumpProc,
					  (XtCallbackProc)JumpProcDis,
					  (XtPointer) p);
	XtAddCallback(p ->changeState, XtNjumpProc,
					  (XtCallbackProc) JumpProcState,
					  (XtPointer) p);
	
	p ->pix = XCreatePixmap (one .dis, XtWindow (p ->plotDat),
									  p ->wid, p ->hei, one .depth);
	XtAddCallback(close, XtNcallback,
    (XtCallbackProc) ClosePlotCback, (XtPointer)p);
	XtVaSetValues (p ->plotDat, XtNbitmap, p ->pix,
					NULL);
	
	PlotTimeSeries (&one, p ->pix, one .View, 0,
							p ->wid, p ->hei);
	doneWaiting (&one);
	return 0;
}


typedef struct {
	Widget plot, plotDat, changeMonth,
		changeDisease, monthW, diseaseW;
	Pixmap pix;
	int month, dis;
	int wid, hei;
} mapData;



static XtCallbackProc CloseMapCback (Widget w, mapData *p)
{
	XtDestroyWidget(XtParent(XtParent(w)));
	XFreePixmap (one .dis, p ->pix);
	free (p);
}

XtCallbackProc ShiftLeftClickAction (Widget w, XButtonEvent * event)
{
	Widget  plotDat, box, form, close;
	mapData * p = (mapData *) malloc (sizeof (mapData));
	waiting (&one);
	p ->wid = one .width;
	p ->hei = one .height;
	p ->month = 0;
	p ->dis = 0;
	fprintf (stderr, "In action ShiftLeftClickAction at %d %d\n",
				event ->x, event ->y);

	p ->plot  = XtVaCreatePopupShell
		("Map", 											
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
	p ->diseaseW = XtVaCreateManagedWidget
		(one .data .DisNames [p ->dis],
		 labelWidgetClass, box,
		 NULL);
	p ->monthW = XtVaCreateManagedWidget
		("Month",
		 labelWidgetClass, box,
		 XtNfromHoriz, p ->diseaseW,
		 NULL);
	close = XtVaCreateManagedWidget
		("Close",
		 commandWidgetClass, box,
		 XtNfromHoriz, p ->monthW,
		 NULL);
	p ->changeDisease = XtVaCreateManagedWidget
		("Change Disease",
		 scrollbarWidgetClass, box,
		 XtNfromVert, p ->diseaseW,
		 XtNthickness, 20,
		 XtNlength, p ->hei,
		 XtNorientation, XtorientVertical,
		 NULL);
	p ->plotDat = XtVaCreateManagedWidget ("plotData",
      labelWidgetClass,  box,
      XtNdepth, one .depth, 				  
		XtNcolormap, one .cmap,
		XtNvisual, one .vis,
      XtNorientation, (XtOrientation) XtorientVertical,
		XtNwidth, p ->wid,
		XtNheight, p ->hei,
      XtNfromVert, p ->diseaseW,
		XtNfromHoriz, p ->changeDisease,
      NULL);
	p ->changeMonth = XtVaCreateManagedWidget
		("Change Month",
		 scrollbarWidgetClass, box,
		 XtNfromVert, p ->plotDat,
		 XtNthickness, 20,
		 XtNlength, p ->wid,
		 XtNorientation, XtorientHorizontal,
		 NULL);
	
	XtPopup (p ->plot, XtGrabNone);

/*	XtAddCallback(p ->changeDisease, XtNjumpProc,
					  (XtCallbackProc)JumpProcDis,
					  (XtPointer) p);
	XtAddCallback(p ->changeState, XtNjumpProc,
					  (XtCallbackProc) JumpProcState,
					  (XtPointer) p);
*/
	
	p ->pix = XCreatePixmap (one .dis, XtWindow (p ->plotDat),
									  p ->wid, p ->hei, one .depth);
	XtAddCallback(close, XtNcallback,
    (XtCallbackProc) CloseMapCback, (XtPointer)p);

	XtVaSetValues (p ->plotDat, XtNbitmap, p ->pix,
					NULL);

	{
		Window mainPix  = one .pixmap;
		Widget	mainWin = one .mainW;
		int pcol = one .PlotColorBar;
		int ptex = one .PlotText;
		one .pixmap = p ->pix;
		one .mainW = p ->plotDat;
		one .PlotText = one .PlotColorBar = 0;
		clearScreen (one .missingColor);
		Redraw (&one);
		one .pixmap =  mainPix;
		one .mainW = mainWin;
		one .PlotText = ptex;
		one .PlotColorBar = pcol;
	}
	doneWaiting (&one);
	return 0;
}
void RedrawSmoothLeft (float tX, float tY, float tZ);
void RedrawSmoothMiddle ( float sX, float sY, float sZ);
void RedrawSmoothRight ( float rotA, float rotB);

XtCallbackProc LeftClickAction (Widget w, XButtonEvent * event)
{
	fprintf (stderr, "In action LeftClickAction at %d %d\n",
				event ->x, event ->y);
	RedrawSmoothLeft (event ->x - one.width/2, +one.height/2-event ->y, 0);
	return 0;
}


XtCallbackProc MiddleClickAction (Widget w, XButtonEvent * event)
{
	RedrawSmoothMiddle (one .width/2.0/event ->x, one.height/2.0/event ->y, 0);
	return 0;
}

XtCallbackProc RightClickAction (Widget w, XButtonEvent * event)
{
	printf ("in action RightClickAction\n");
	RedrawSmoothRight (one .width/2.0 - event ->x, one .height/2.0 - event ->y);
	return 0;
}

XtCallbackProc Middle2ClickAction (Widget w, XButtonEvent * event)
{
	printf ("in action Middle2ClickAction\n");
	return 0;
}



XtCallbackProc ShiftMiddleClickAction (Widget w, XButtonEvent * event)
{
	printf ("in action ShiftMiddleClickAction\n");
	return 0;
}

XtCallbackProc ShiftRightClickAction (Widget w, XButtonEvent * event)
{
	printf ("in action ShiftRightClickAction\n");
	return 0;
}

XtCallbackProc CtrlRightClickAction (Widget w, XButtonEvent * event)
{
	printf ("in action CtrlRightClickAction\n");
	return 0;
}

XtCallbackProc Right2ClickAction (Widget w, XButtonEvent * event)
{
	printf ("in action Right2ClickAction\n");
	return 0;
}

XtCallbackProc ExposeAction (Widget w, XButtonEvent * event)
{
	printf ("in action exp %d %d\n", event ->x, event ->y);
	return 0;
}

XtCallbackProc Quit (Widget w, XtPointer ooo)
{
	closeGraph ((graph *)ooo);
	exit (0);
}

XtCallbackProc Help (Widget w, XtPointer onePt)
{
	char buf [1000];
	graph * ooo =  ((graph *)onePt);
	
	sprintf (buf, "%s/Description", getenv ("MIASMA"));
	help (ooo, buf);
	return 0;
}







