#include <stdio.h>
#include <linux/stddef.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <X11/Intrinsic.h> 
#include <X11/Shell.h> 
#include <X11/StringDefs.h>
#include <X11/Xaw3d/Paned.h>
#include <X11/Xaw3d/Label.h>
#include <X11/Xaw3d/Box.h>
#include <X11/Xaw3d/Paned.h>
#include <X11/Xaw3d/Scrollbar.h>
#include <X11/Xaw3d/List.h>
#include <X11/Xaw3d/Command.h>
#include <X11/Xaw3d/MenuButton.h>
#include <X11/Xaw3d/SimpleMenu.h>
#include <X11/Xaw3d/SmeBSB.h>
#include <X11/Xaw3d/Viewport.h>


#include "graph.h"	
#include "menu.h"	
#include "action.h"	

float saturation = .5;

typedef void (*gr) (int, int, short*);
gr act [] =  { (gr)drawPoints, (gr)drawLine, (gr)fillLine };

graph one;
static XtTranslations tr;
static XtActionsRec actionsTable [] = {
		{(String)"LeftClickAction",   (XtActionProc)LeftClickAction},
		{(String)"ShiftLeftClickAction",   (XtActionProc)ShiftLeftClickAction},
		{(String)"CtrlLeftClickAction",   (XtActionProc)CtrlLeftClickAction},
		{(String)"MiddleClickAction",   (XtActionProc)MiddleClickAction},
		{(String)"Middle2ClickAction",   (XtActionProc)Middle2ClickAction},
		{(String)"ShiftMiddleClickAction", (XtActionProc)ShiftMiddleClickAction},
		{(String)"ShiftRightClickAction", (XtActionProc)ShiftRightClickAction},
		{(String)"CtrlRightClickAction", (XtActionProc)CtrlRightClickAction},
		{(String)"RightClickAction", (XtActionProc)RightClickAction},
		{(String)"Right2ClickAction", (XtActionProc)Right2ClickAction},
		{(String)"ExposeAction",   (XtActionProc)ExposeAction},
};

void makeColorMap (XColor * color, char * name);

void SetupMainWindow (graph * ooo)
{
	if (ooo ->mainW != 0){
		XtDestroyWidget (one .mainW);
		XFreePixmap (one .dis, one .pixmap);
	}
	one .mainW = XtVaCreateManagedWidget(AppName, labelWidgetClass, one .form,
		XtNheight, one .height, XtNwidth, one .width,
		XtNx, 0, XtNy, 0,
		NULL);
	XtVaSetValues (one .mainW,  XtNcursor, one .Cross, 0);
	XtVaSetValues (one .form, XtNtranslations, tr,	NULL);
	one .pixmap = XCreatePixmap (one .dis, XtWindow (one .mainW),
									  one .width, one .height,
									  one .depth);
	XtVaSetValues (one .mainW, XtNbitmap, one .pixmap,
					NULL);

}

void SetColormap (graph * ooo, char *  cmapName)
{
	static first = 1;

	fprintf (stderr, "In SetColormap %d %s\n", first,  cmapName);

	switch (ooo ->depth){
	case 1:
		if (first)			
			ooo ->cmap = XCreateColormap (ooo ->dis, ooo ->rwin, ooo ->vis, AllocNone);
		ooo ->foreground = XWhitePixel (ooo ->dis, ooo ->screen);
		ooo ->background = XBlackPixel (ooo ->dis, ooo ->screen);			
		ooo ->missingColor = ooo ->background;			
		break;
	case 24:
		ooo ->foreground = 255 + (255 << 8) + (255 << 16);
		ooo ->background = 100 + (100 << 8) + (100 << 16);			
		ooo ->missingColor = 100 + (100 << 8) + (100 << 16);
		if (first)
			ooo ->color = (XColor *) malloc (ooo ->nOfEntries * sizeof (XColor));
      makeColorMap (ooo ->color, cmapName);	
		if (first)			
			ooo ->cmap = XCreateColormap (ooo ->dis, ooo ->rwin, ooo ->vis, AllocAll);
       //ooo ->cmap = ooo ->defCmap;
      
		break;
	case 8:
		ooo ->foreground = 1;
		ooo ->background = 255;		
		ooo ->missingColor = 0;
		if (first)
			ooo ->color = (XColor *) malloc (ooo ->nOfEntries * sizeof (XColor));
		makeColorMap (ooo ->color, cmapName);
		if (first)			
			ooo ->cmap = XCreateColormap (ooo ->dis, ooo ->rwin, ooo ->vis,
											  AllocAll);
		if (0 == XStoreColors (ooo ->dis, ooo ->cmap,
									  ooo ->color, ooo ->nOfEntries)){
			fprintf (stderr, "Can not store colors\n");
		}else{
			fprintf (stderr, "Color Map Allocated\n");
		}		
		break;
	}
	first = 0;
}

#define myWhite 0
#define myBlack 1
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

static myXerror = 0;
int errH (Display * dis, XErrorEvent *err);
static char bits[] = {
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x3f, 0xff,
   0x3f, 0xfe, 0x7f, 0xc0, 0xff, 0x3f, 0x1f, 0x00, 0xff, 0x7f, 0xc0, 0xff,
   0x3f, 0x0f, 0x80, 0xff, 0x7f, 0xc0, 0xff, 0x3f, 0x07, 0xc0, 0xff, 0xff,
   0xf1, 0xff, 0x3f, 0x1f, 0xfc, 0xff, 0xff, 0xf1, 0xff, 0x3f, 0x3f, 0xfc,
   0xff, 0xff, 0xf1, 0xff, 0x3f, 0x7f, 0xfc, 0xff, 0xff, 0xf1, 0xff, 0x3f,
   0x7f, 0xfc, 0xcf, 0xff, 0xf1, 0xff, 0x3f, 0x7f, 0xfc, 0xc3, 0xff, 0xf1,
   0xff, 0x3f, 0x7f, 0xfc, 0xe0, 0xff, 0xf1, 0xff, 0x3f, 0x7f, 0x7c, 0xf0,
   0xff, 0xf1, 0xff, 0x3f, 0x7f, 0x3c, 0xf0, 0xff, 0xf1, 0xff, 0x3f, 0x7f,
   0x3c, 0xf0, 0xff, 0xf1, 0xff, 0x3f, 0x7f, 0xfc, 0xf0, 0xff, 0xf1, 0xff,
   0x3f, 0x7f, 0x7c, 0xf8, 0xff, 0xf1, 0xff, 0x3f, 0x7f, 0x7c, 0xf0, 0xff,
   0xf1, 0xff, 0x3f, 0x3f, 0x7c, 0xf0, 0xff, 0xf1, 0xff, 0x3f, 0x7f, 0x7c,
   0xf0, 0xff, 0xf1, 0xff, 0x3f, 0x7f, 0x7c, 0xf0, 0xff, 0xf1, 0xff, 0x3f,
   0x3f, 0xfc, 0xf0, 0xff, 0xf1, 0xff, 0x3f, 0x3f, 0xfc, 0xf0, 0xff, 0xf1,
   0xff, 0x3f, 0x1d, 0xfe, 0xf0, 0xff, 0xf1, 0xff, 0x3f, 0x09, 0xff, 0xf0,
   0xff, 0xf1, 0xff, 0x3f, 0x01, 0xff, 0xf0, 0xff, 0xf1, 0xff, 0x3f, 0x03,
   0xff, 0xf8, 0xff, 0xf1, 0xff, 0x3f, 0x07, 0xff, 0xf8, 0xff, 0xf1, 0xff,
   0x3f, 0x07, 0xfe, 0xf8, 0xff, 0xf1, 0xff, 0x3f, 0x1f, 0x7c, 0xf8, 0xff,
   0xf1, 0xff, 0x3f, 0x1f, 0x78, 0xf8, 0xff, 0xf1, 0xff, 0x3f, 0x7f, 0x38,
   0xf8, 0xff, 0xf1, 0xff, 0x3f, 0xff, 0x3f, 0xf8, 0xfe, 0xf1, 0xff, 0x3f,
   0xff, 0x3f, 0x50, 0xff, 0xf1, 0xff, 0x3f, 0xff, 0x1f, 0x00, 0xff, 0xf1,
   0xff, 0x3f, 0xff, 0x0f, 0x80, 0xff, 0xf1, 0xff, 0x3f, 0xff, 0x07, 0xe0,
   0xff, 0xf1, 0xff, 0x3f, 0xff, 0x83, 0xff, 0xff, 0xf1, 0xff, 0x3f, 0xff,
   0xfb, 0xff, 0xff, 0xf1, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff,
   0x3f, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff,
   0xf1, 0xff, 0x3f, 0xff, 0xff, 0xff, 0x7f, 0xc0, 0xff, 0x3f, 0xff, 0xff,
   0xff, 0x7f, 0xc0, 0xff, 0x3f, 0xff, 0xff, 0xff, 0x7f, 0xc0, 0xff, 0x3f,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0x3f};

Boolean StrToColor (Display * dis, XrmValuePtr args, Cardinal * nargs,
						  XrmValuePtr from, XrmValuePtr to)
{
	static Pixel p;
	if (!strcasecmp (from ->addr, "white") ||
		 !strcasecmp (from ->addr, "XtDefaultForeground")){
		p = one .foreground;
	}else{
		p = one .background;
	}
	fprintf (stderr, "in converter %s %d\n", from ->addr, p);
	to ->size = sizeof (Pixel);
 	to ->addr = (XtPointer)(&p);
	return TRUE;
}

void initGraph (char * cmapName)
{
	static first = 1;
	char buf [255];
	int i;
	XGCValues val;
	XVisualInfo *list, templ;
	char *fontname="9x15b*";

	if (!first){
/*		XFillRectangle (one .dis, one .pixmap, one .myblackgc, 0, 0, w, h);*/
		return;
	}else{
		one .fontInfo = NULL;
		one .DisplayName = NULL;
		one .dis = NULL;
	}
	first = 0;
	
	{
		int i = 0;
		XtToolkitInitialize ();
		one .appCon = XtCreateApplicationContext ();
		XtAppSetTypeConverter (one .appCon,
									  XtRString, XtRPixel,
								     (XtTypeConverter)StrToColor,
									  (XtConvertArgList)NULL,
									  0,
									  XtCacheAll, NULL);  
		if ((one .dis = XtOpenDisplay (one .appCon, one .DisplayName, AppName,
							"Animation", NULL, 0, &i, NULL)) == NULL){
			printf ("Can not open Display\n");
			exit (-1);
		}
	}

	one .screen = XDefaultScreen (one .dis);
	one .depth = XDefaultDepth (one .dis, one .screen);
	one .defCmap = XDefaultColormap (one .dis, one .screen); 
	one .rwin = RootWindow(one .dis, one .screen);
	one .vis = XDefaultVisual (one .dis, one .screen);
   one .nOfEntries = XDisplayCells (one .dis, one .screen);


  	XSetErrorHandler (errH);
	XSynchronize (one .dis, True);
	one .vis = XDefaultVisual (one .dis, one .screen);
	if ((one .fontInfo=XLoadQueryFont(one .dis, fontname))==NULL){
		fprintf(stderr,"Cannot open %s font\n", fontname);
		exit(-1);
	}
	{
		int nitems;
		templ .screen = one .screen;
		templ .depth = one .depth;
		templ .colormap_size = 0x100;
		if (templ .depth == 24)
			templ .class = DirectColor;
		else
			templ .class = PseudoColor;
			
	
		list = XGetVisualInfo (one .dis, VisualScreenMask
								  | VisualClassMask 
								  | VisualDepthMask
								  | VisualColormapSizeMask,
								  &templ, &nitems);
		if (nitems){
			one .vis = list [0] .visual;
			one .depth = list [0] .depth;
			one .nOfEntries = list [0] .colormap_size;
			fprintf (stderr,
				"Visual %x of depth %d, class %d ncolors %d number %d\n",
							list [0] .visualid, one .depth,
						list [0] .class, list [0] .colormap_size, nitems);
		}else{
			
			one .depth = XDefaultDepth (one .dis, one .screen);
			templ .depth = one .depth;
			templ .class = PseudoColor;
			templ .colormap_size = 0x100;
			list = XGetVisualInfo (one .dis, VisualScreenMask | VisualClassMask
								  | VisualDepthMask | VisualColormapSizeMask,
								  &templ, &nitems);
			if (nitems){
				one .vis = list [0] .visual;
				one .depth = templ .depth;;
				one .nOfEntries = templ .colormap_size;
				fprintf (stderr,
				"No visuals at depth 24, using default visual at depth %d\n",
							one .depth);
			}else{
				fprintf (stderr,
				"No visuals at depth 24, or depth %d with 256 colors\n",
							one .depth);
				exit (-1);
			}
		}
	}
      
   SetColormap (&one, cmapName);

	
/********* WIDGETS *************************/	
	one .toplevel = XtVaAppCreateShell (AppName, AppName,
		applicationShellWidgetClass,
		one .dis,
		XtNdepth, one .depth, 				  
		XtNcolormap, one .cmap,
		XtNvisual, one .vis,
		XtNiconPixmap,  XCreateBitmapFromData (one .dis, one .rwin,
															bits, 54, 54),
		XtNmaxHeight, XDisplayHeight (one .dis, one .screen) - 50,
		XtNmaxWidth, XDisplayWidth (one .dis, one .screen) - 50,
		NULL);
	
	one .vpane = XtVaCreateManagedWidget("vpane", panedWidgetClass,
		one .toplevel,
		NULL);
	one .bb = XtVaCreateManagedWidget("buttonbox", boxWidgetClass,
		one .vpane,
		NULL);

/******** MENU ********************/	
	one .out = XtVaCreateManagedWidget("Exit", commandWidgetClass,
		one .bb,
		NULL);
	XtAddCallback (one .out, XtNcallback, (XtCallbackProc)Quit,
						(XtPointer *)&one);
	one .help = XtVaCreateManagedWidget("Help", commandWidgetClass,
		one .bb,
		NULL);
	XtAddCallback (one .help, XtNcallback, (XtCallbackProc)Help,
						(XtPointer *)&one);
	one .playButton = XtVaCreateManagedWidget("Play",
		commandWidgetClass,
		one .bb,
		NULL);
	XtAddCallback (one .playButton, XtNcallback,
						(XtCallbackProc)playCback,
						(XtPointer *)&one);
	one .recordButton = XtVaCreateManagedWidget("Record",
		commandWidgetClass,
		one .bb,
		NULL);
	XtAddCallback (one .recordButton, XtNcallback,
						(XtCallbackProc)recordCback,
						(XtPointer *)&one);
	one .fillButton = XtVaCreateManagedWidget("Smooth",
		commandWidgetClass,
		one .bb,
		NULL);
	XtAddCallback (one .fillButton, XtNcallback,
						(XtCallbackProc)fillCback,
						(XtPointer *)&one);
	one .menuButton =
		XtVaCreateManagedWidget("Menu", menuButtonWidgetClass,
		one .bb,
		XtNmenuName, "Menu",
		NULL);
	one .menu = XtVaCreatePopupShell("Menu", simpleMenuWidgetClass,
		one .menuButton,
		XtNdepth, one .depth, 				  
		XtNcolormap, one .cmap,
		XtNvisual, one .vis,
		NULL);
	for (i = 0; i < XtNumber (MenuItems); i++){
		Widget entry = XtVaCreateManagedWidget(MenuItems [i],
			smeBSBObjectClass,
			one .menu,
			NULL);
		XtAddCallback(entry, XtNcallback, MenuSelect, (XtPointer) i);
	}


	one .view = XtVaCreateManagedWidget("view", boxWidgetClass,
		one .vpane,
/*		XtNallowHoriz, True, XtNallowVert, True,*/
													
		NULL);
	one .form = XtVaCreateManagedWidget("form", formWidgetClass, one .view,
		NULL);

	one .viewWidget = XtVaCreateManagedWidget("viewWidget",
	  listWidgetClass,
     one .form,
	  XtNdefaultColumns, 3,													  
	  XtNforceColumns, True, 
	  XtNnumberStrings,  one .NViews,
	  XtNlist, one .data .DisNames,
	  NULL);
	XtAddCallback(one .viewWidget, 
	  XtNcallback, (XtCallbackProc)selectView, (XtPointer)
					  one .viewWidget);
	XawListHighlight (one .viewWidget, one .View);

	one .transWidget = XtVaCreateManagedWidget("transWidget",
	  listWidgetClass,
     one .form,																
/*	  XtNdefaultColumns, 1,													  
	  XtNforceColumns, True, */
	  XtNnumberStrings,  one .NTransformations,
	  XtNlist, one .TransformationNames,
	  XtNfromHoriz, one .viewWidget,
	  NULL);
	XtAddCallback(one .transWidget, 
	  XtNcallback, (XtCallbackProc)selectTrans, (XtPointer)
					  one .transWidget);
	XawListHighlight (one .transWidget, one .Transformation);


	one .form = XtVaCreateManagedWidget("form1", formWidgetClass, one .view,
		NULL);
	sprintf (buf, "%.4d/%.2d", year (&(one .data)),
				month (&(one .data)));
	one .timeLabel = XtVaCreateManagedWidget(buf, labelWidgetClass,
	  one .form,
	  NULL);
	sprintf (buf, "%.3e", one .Lambda);
	one .lambdaLabel = XtVaCreateManagedWidget(buf, labelWidgetClass,
	  one .form,
	  XtNfromHoriz, one .timeLabel,
	  NULL);
/************* Map Window************************************************/
  	one .mapArea = XtVaCreatePopupShell
		("Map Window",
		topLevelShellWidgetClass,
      one .toplevel,
      XtNdepth, one .depth, 				  
		XtNcolormap, one .cmap,
		XtNvisual, one .vis,
		XtNtitle, (String) "Map Window",
      XtNiconName, (String) "Map Window",
		NULL);
	one .form = XtVaCreateManagedWidget
		("form2", formWidgetClass, one .mapArea,
		NULL);	
	one .mainW = XtVaCreateManagedWidget
		("mainW", labelWidgetClass, one .form,
		XtNheight, one .height, XtNwidth, one .width,
		NULL);
	one .horizontalLambda = XtVaCreateManagedWidget
		("horizontalLambda",
		 scrollbarWidgetClass,
		 one .form,
		 XtNorientation, XtorientVertical,
		 XtNthickness, 20,
		 XtNlength, one .height,
		 XtNminimumThumb, 2,
		 XtNtopOfThumb, &one .Lambda,
		 XtNfromHoriz, one .mainW,
		 NULL);

	one .horizontalTime = XtVaCreateManagedWidget
		("horizontalTime",
		 scrollbarWidgetClass, one .form,
		 XtNorientation, XtorientHorizontal,
		 XtNthickness, 20,
		 XtNlength, one .width,
		 XtNminimumThumb, MAX (1, (one .width) / one .data .NMonths),
		 XtNfromVert, one .mainW,
		 NULL);	
	XtAddCallback(one .horizontalTime, XtNjumpProc,
					  (XtCallbackProc)JumpProcHTime,
					  (XtPointer) one .horizontalTime);
	XtAddCallback(one .horizontalTime, XtNscrollProc,
					  (XtCallbackProc)ScrollProcHTime,
					  (XtPointer) one .horizontalTime);
	XtAddCallback(one .horizontalLambda, XtNjumpProc,
					  (XtCallbackProc)JumpProcHLambda,
					  (XtPointer) one .horizontalLambda);
	

	XtAppAddActions(one .appCon, actionsTable, XtNumber (actionsTable));
	tr = XtParseTranslationTable (
		  "Shift <Btn2Down>     : ShiftMiddleClickAction()\n\
         <Btn2Down>(2)        : Middle2ClickAction()    \n\
         <Btn2Down>           : MiddleClickAction()     \n\
         Shift <Btn3Down>     : ShiftRightClickAction() \n\
         Ctrl <Btn3Down>      : CtrlRightClickAction()  \n\
         <Btn3Down>(2)        : Right2ClickAction()     \n\
         <Btn3Down>           : RightClickAction()      \n\
         Shift <Btn1Down>     : ShiftLeftClickAction()  \n\
         Ctrl <Btn1Down>      : CtrlLeftClickAction()   \n\
         <Btn1Down>           : LeftClickAction()       \n\
			<Expose>             :   ExposeAction()");
	one .Wait = XCreateFontCursor (one .dis, XC_watch);
	one .Cross = XCreateFontCursor (one .dis, XC_cross);

	
	XtRealizeWidget(one .toplevel);
/*	XtRealizeWidget(one .mapArea);*/
	XtPopup (one .mapArea, XtGrabNone);
/*	XRaiseWindow(one .dis, XtWindow (one .mapArea));
*/	
/* GC, STIPPLES, etc. */
	one .stiple = XCreatePixmap (one .dis, XtWindow (one .mainW), 9, 9, 1);	
	one .currentgc = XCreateGC (one .dis, XtWindow (one .mainW),0,&val);
	one .bitgc    =  XCreateGC (one .dis, one .stiple, 0,&val);
	one .stip1gc = XCreateGC (one .dis,XtWindow (one .mainW), 0, &val);
	if (one .depth == 24)
		val .plane_mask = (0xff << 16) + (0xff << 8);
	else
		val .plane_mask = 0xf << 2;			
	val .function = GXcopyInverted;
	val .graphics_exposures = True;
	one .highlightgc = XCreateGC (one .dis, XtWindow (one .mainW),
		/*GCGraphicsExposures | */GCFunction | GCPlaneMask, &val);

	one .xorgc = XCreateGC (one .dis, XtWindow (one .mainW), 0, &val);	
	XSetLineAttributes (one .dis, one .xorgc, 1, LineSolid,
							  CapNotLast, JoinBevel);
	XSetFunction (one .dis, one .xorgc, GXxor);
	XSetForeground (one .dis, one .xorgc, 255);
	one .xorOutlinegc = XCreateGC (one .dis, XtWindow (one .mainW), 0, &val);	
	XSetLineAttributes (one .dis, one .xorOutlinegc, 4, LineSolid,
							  CapNotLast, JoinBevel);
	XSetFunction (one .dis, one .xorOutlinegc, GXxor);
	XSetForeground (one .dis, one .xorOutlinegc, 255);


	
	XSetLineAttributes (one .dis, one .currentgc, 0, LineSolid,
							  CapButt, JoinMiter);
	XSetLineAttributes (one .dis, one .stip1gc, 1, LineSolid,
							  CapButt, JoinMiter);
	XSetForeground (one .dis, one .bitgc, 1);
	one .textgc = XCreateGC (one .dis, XtWindow (one .mainW), 0, &val);
	XSetForeground (one .dis, one .textgc, one .foreground);
	XSetBackground (one .dis, one .textgc, one .missingColor);
	XSetFont(one .dis, one .textgc, one .fontInfo->fid);

	{
		int i, j;
		XSetForeground (one .dis, one .bitgc, 1);
		XFillRectangle (one .dis, one .stiple, one .bitgc, 0, 0, 9, 9);
		XSetForeground (one .dis, one .bitgc, 0);
		XDrawPoint (one .dis, one .stiple, one .bitgc, 0, 0);
		XDrawPoint (one .dis, one .stiple, one .bitgc, 0, 1);
		
/*		for (i = 0; i < 4; i++)
			for (j = 0; j <1; j++)
				XDrawPoint (one .dis, one .stiple, bitgc, i, j);
*/		XSetStipple (one .dis, one .stip1gc, one .stiple);
		XSetFillStyle (one .dis, one .stip1gc, FillStippled);
	}
	one .pixmap = XCreatePixmap (one .dis, XtWindow (one .mainW),
									  one .width, one .height,
									  one .depth);	
	XtVaSetValues (one .mainW,  XtNcursor, one .Cross, 0);
	XtVaSetValues (one .form, XtNtranslations, tr,	NULL);
	XtVaSetValues (one .mainW, XtNbitmap, one .pixmap,
					NULL);

	clearScreen (one .background);

	initOpenGl (&one);
}



void closeGraph (graph * ooo)
{
	if (ooo ->pixmap)
		XFreePixmap (ooo ->dis, ooo ->pixmap);
	if (ooo ->cmap)
		XFreeColormap (ooo ->dis, ooo ->cmap);
	if (ooo ->color)
		free (ooo ->color);
	XtDestroyWidget (ooo ->toplevel);
	XtCloseDisplay (ooo ->dis);
}


void flushGraph (void)
{
	XCopyArea (one .dis, one .pixmap,
				  XtWindow (one .mainW),
				  one .currentgc,
				  0, 0,
				  one .width, one .height,
				  4, 0);
}
				  
void drawLine (int col, int length, short * line)
{
	XSetForeground (one .dis, one .currentgc, col);
	XDrawLines (one .dis, one .pixmap, one .currentgc, (XPoint *)line,
					length, CoordModeOrigin);
}

void fillLine (int col, int length, short * line, int pattern)
{
	if (pattern){
/*		fillLineGl (col, length, line, pattern);*/
		XSetForeground (one .dis, one .stip1gc, col);
		XFillPolygon (one .dis, one .pixmap, one .stip1gc,
						  (XPoint *) line, length, Nonconvex,
					  CoordModeOrigin);
	}else{
		XSetForeground (one .dis, one .currentgc, col);
		XFillPolygon(one .dis, one .pixmap, one .currentgc, (XPoint *)line, length, Nonconvex,
						 CoordModeOrigin);
	}	
}

void drawPoints (int col, int length, short * line)
{
	XSetForeground (one .dis, one .currentgc, col);
	XDrawPoints (one .dis, one .pixmap, one .currentgc, (XPoint *)line, length, CoordModeOrigin);
}
						 





	
int errH (Display * dis, XErrorEvent *err)
{
	char buf [200];
	fprintf(stderr, 
"code=%d, text:%s\nMajor code:%d,\nMinor Code:%d,\nType:%d,\nserial:%ld,\nresourceId:%lu\n",
			 (unsigned)err ->error_code,
			 XGetErrorText (dis, err ->error_code, buf, 200),
			 (unsigned)err ->request_code,
			 (unsigned)err ->minor_code,
			 err ->type,
			 err ->serial,
			 err ->resourceid);
	myXerror = 1;
	exit (1);
}

XImage * CreateXImage (int width, int height)
{
	XImage * im;
	if (one .depth > 1)
		 im =
			XGetImage (one .dis, one .pixmap, 0, 0, width, height, -1, ZPixmap);
	else
		im =
			XGetImage (one .dis, one .pixmap, 0, 0, width, height, -1, XYPixmap);
	return im;
}

void makeColorMap (XColor * color, char * name)
{
	int npoints = 5, i;
	int *points = 0;
	int rgb [] = {
		240, 240, 240,
		225, 225, 50,
		225, 50,  50,
		127, 50,  50,
		0,   0,   0};
	int gray [] = {
		255, 255, 255,
		192, 192, 192,
		128, 128, 128,
		64, 64, 64,
		1,   1,   1};
	int gbr [] = {
		255, 255, 255,
		50, 255, 255,
		50,  255, 50,
		50,  127, 50,
		0,   0,   0};
	int grb [] = {
		255, 255, 255,
		255, 255, 50,
		50,  255, 50,
	   50,  127, 50,
		0,   0,   0};
	int rbg [] = {
		255, 255, 255,
		255, 50, 255,
	   255, 50, 50,
	   127, 50, 50,
		0,   0,   0};
	int brg [] = {
		240, 240, 240,
		210, 120, 210,
		112,  61, 187,
	   10,  60, 110,
		0,   0,   0};
	int bgr [] = {
		255, 255, 255,
		50, 255, 255,
		50,  50, 255,
	   50,  50, 127,
		0,   0,   0};
	if (!strncmp("gray", name, 6))
		points = gray;
	if (!strncmp("rgb", name, 6))
		points = rgb;
	if (!strncmp("grb", name, 3))
		points = grb;
	if (!strncmp("gbr", name, 3))
		points = gbr;
	if (!strncmp("rbg", name, 3))
		points = rbg;
	if (!strncmp("brg", name, 3))
		points = brg;
	if (!strncmp("bgr", name, 3))
		points = bgr;

	color [0] .pixel = 0;
	color [0] .red   = 100 * 0xff;       
	color [0] .green = 100 * 0xff;       
	color [0] .blue  = 100 * 0xff;       
	color [0] .flags = DoRed | DoGreen | DoBlue;
	if (!points){
		int ncol, col=0;
		float r, g, b;
		FILE * clr;
		char buf [255];
		sprintf (buf, "%s/%s", getenv ("MIASMA"), name);
		clr = fopen (buf, "r");
		if (clr == NULL){
			fprintf (stderr, "Can  not open color file %s\n", name);
			exit (1);
		}
		fscanf (clr, "%d", &ncol);
		for (i = 1; i < 256; i++){
			if ((i-1)*ncol/255 >= col){
				fscanf (clr, "%f %f %f", &r, &g, &b);
				col ++;
			}
			color [i] .pixel = i;
			color [i] .flags = DoRed | DoGreen | DoBlue;
			color [i] .red   = (int)(r * 0xff);
			color [i] .green = (int)(g * 0xff);
			color [i] .blue  = (int)(b * 0xff);
		}
		fclose (clr);
		return;
	}
	for (i = 1; i < 256; i++){
		int div = (256 - 1)/(npoints-1);
		int inc = (i-1)%div;
		int left = (i-1)/div;
		int step = (left+1)*div - left * div;
		int index1 = left*3, index2 = MIN((npoints-1),(left+1))*3; 
		color [i] .pixel = i;
      color [i] .flags = DoRed | DoGreen | DoBlue;
      color [i] .red =
			(points [index1]*(step-inc) +
								 points [index2]*inc)/(step) * 0xff;
		color [i] .green =
			(points [index1+1]*(step-inc) +
								 points [index2+1]*inc)/step * 0xff;
		color [i] .blue =
			(points [index1+2]*(step-inc) +
								 points [index2+2]*inc)/step * 0xff;
	}
}


/* Text manipulations */
void text (int x, int y, char * str)
{
	XDrawImageString(one .dis, one .pixmap, one .textgc, x, y, str, strlen(str));
}

void setTextAtt (int color, char * font_name)
{
  	if (one .fontInfo != NULL)
		XFreeFont (one .dis, one .fontInfo);
	if ((one .fontInfo = XLoadQueryFont(one .dis, font_name))==NULL){
		fprintf (stderr,"Cannot open %s font\n",font_name);
		exit(-1);
	}	
	XSetForeground (one .dis, one .textgc, color);
	XSetFont(one .dis, one .textgc, one .fontInfo ->fid);
}

void clearScreen (int col)
{
	XSetForeground (one .dis, one .currentgc, col);
	XFillRectangle (one .dis, one .pixmap, one .currentgc, 0, 0,
						 one .width, one .height);
	XFillRectangle (one .dis, XtWindow (one .mainW), one .currentgc, 0, 0,
						 one .width, one .height);
}

void putColorScale (int N, float * x)
{
	static XImage * put = 0;
	static width = 0, height = 0;
	static fH = 0;
	int i, j, wid, hei, len, clen;
	if (one .horizontalBar){
		hei = one .ColorBarWidth;
		wid = one .width - 50;
		len = wid-50;
		clen = hei;
	}else{
		hei = one .height;
		wid = one .ColorBarWidth;
		len = hei;
		clen = wid;
	}
	if (!put || width != one .width || height != one .height){
		width = one .width;
		height = one .height;
		if (put)
			XDestroyImage (put);
		put = XGetImage (one .dis, one .pixmap, 0, 0,
							 wid, hei, -1, ZPixmap);
		for (i = 0; i < len; i++){
			int col = LinearToThermo (3.0 * (len-i-1)*0xff
											  / len); 
			for (j = 0; j < clen; j++)
				if (one .horizontalBar)
					XPutPixel (put, len-i, j, col);
				else
					XPutPixel (put, j, i, col);
		}
		setTextAtt (one .foreground, one .legendTextAtt);
		fH = one .fontInfo ->max_bounds .ascent + 
			one .fontInfo ->max_bounds .descent;
	}
	
	if (one .PlotColorBar){
		if (one .horizontalBar)
			XPutImage (one .dis, one .pixmap, one .currentgc,
					  put, 0, 0, 25, one .height - hei,
					  wid, hei);
		else
			XPutImage (one .dis, one .pixmap, one .currentgc,
					  put, 0, 0, 0, 0,
					  wid, hei);
	}
	if (one .PlotText){
		if (!one .horizontalBar){
			char buf [100];
			if (one .View < one .data .NDis) 
				for (i = 0; i < N; i++){
					int vPos = (N-i-1.0) * (hei - fH)/(N-1.0) +
						one .fontInfo ->max_bounds .ascent + .5;
					char buf [100];
					sprintf (buf, "%.3f", x [i]);
					text (wid + 3, vPos, buf);
				}
			setTextAtt (one .foreground, "my5");
/*			setTextAtt (one .foreground, "*times*bold*-24-*");*/
			sprintf (buf, "%s", one .data .DisNames [one .View]);
			text (one .MapShiftX,
					one .height - one .MapShiftY,
					buf);
			setTextAtt (one .foreground, "fixed");
		}else{
			char buf [100];
			sprintf (buf, "%.4d %.2d", year (&(one .data)),
						 month (&(one .data))); 	
			setTextAtt (one .foreground, "my10");
			text (one .MapShiftX, one .height - one .MapShiftY - 20,
					buf);
			setTextAtt (one .foreground, one .legendTextAtt);
			sprintf (buf, "%s", one .data .DisNames [one .View]);
			text (one .MapShiftX,
					one .height - one .MapShiftY + 30,
					buf);
			for (i = 0; i < N; i++){
				int vPos = (i) *
					(len - 1.75*one .fontInfo ->max_bounds .width)/(N-1.0);
				if (fabs (x [i]) < 1e-15)
					sprintf (buf, "%1.1f", 0.0);
				else
					sprintf (buf, "%.2f", x [i]);
				text (vPos+25, one .height - hei - fH/3, buf);
				{
					short l [4];
					l [0] = l [2] = (i*(len-3))/(N-1.0) + 25;
					l [1] = one .height - hei;
					l [3] = one .height - hei - 5;
					drawLine (1, 2, l);
					l [0] += 1; l [2] += 1;
					drawLine (1, 2, l);
					l [0] += 1; l [2] += 1;
					drawLine (1, 2, l);
					l [0] += 1; l [2] += 1;
					drawLine (1, 2, l);
				}
			}
		}
	}
}

void putImageExpand (int * image, int atX, int atY, int byX,
							int byY, float subsample)
{
	int i, j;
	XImage * im = 0;
/*	fprintf (stderr, "in Put Image Expand %d %d %d %d %f %d\n",
				atX, atY, byX, byY, subsample,
				(int)one .height-(byY/subsample)-atY);*/
	if (one .depth > 1)
		im = XGetImage (one .dis, one .pixmap, atX,
						 one .height-(byY/subsample)-atY,
						 (byX/subsample), (byY/subsample), -1, ZPixmap);
	else
		im = XGetImage (one .dis, one .pixmap, atX,
						 one .height-(int)(byY/subsample)-atY,
						 byX/subsample, byY/subsample, -1, XYPixmap);
	fprintf (stderr, "%s:%d %d %d %d %d\n",
				__FILE__, __LINE__,
				atX,
				(int)(one .height-(int)(byY/subsample)-atY),
				(int)(byX/subsample), (int)(byY/subsample));
	for (i = 0; i < (int)(byX/subsample); i++)
		for (j = 0; j < (int)(byY/subsample); j++){
			XPutPixel (im, i, (int)(byY/subsample)-1-j,
						  LinearToThermo (image [(int)(i*subsample +
														 byX*(int)(j*subsample))]));
		}
	XPutImage (one .dis, one .pixmap, one .currentgc,
				  im, 0, 0, atX, one .height-(int)(byY/subsample)-atY,
				  byX/subsample, byY/subsample);
	XDestroyImage (im);			
}

void putImage (int * image, int atX, int atY, int byX, int byY)
{
	int i, j;
	XImage * im;
	if (one .depth > 1)
		im = XGetImage (one .dis, one .pixmap, atX,
							 one .height-byY-atY,
							 byX, byY, -1, ZPixmap);
	else
		im = XGetImage (one .dis, one .pixmap, atX,
							 one .height-byY-atY,
							 byX, byY, -1, XYPixmap);
	for (i = 0; i < byX; i++)
		for (j = 0; j < byY; j++)
			XPutPixel (im, i, byY-1-j, LinearToThermo (image [i + byX*j]));
	XPutImage (one .dis, one .pixmap, one .currentgc,
				  im, 0, 0, atX, one .height-byY-atY,
				  byX, byY);
	XDestroyImage (im);
}

void getImage (int * image, int atX, int atY, int byX, int byY)
{
	int i, j;
	XImage * im;
	if (one .depth > 1)
		im = XGetImage (one .dis, one .pixmap, atX,
							 one .height-byY-atY,
							 byX, byY, -1, ZPixmap);
	else
		im = XGetImage (one .dis, one .pixmap, atX,
							 one .height-byY-atY,
							 byX, byY, -1, XYPixmap);
	for (i = 0; i < byX; i++)
		for (j = 0; j < byY; j++)
			image [i + byX*j] =
				ThermoToLinear (XGetPixel (im, i, byY-1-j));
	XDestroyImage (im);
}



void waiting (graph * ooo)
{
	XtVaSetValues (ooo ->mainW,  XtNcursor,
						ooo ->Wait, 0);
}

void doneWaiting (graph * ooo)
{
	XtVaSetValues (ooo ->mainW,  XtNcursor,
						ooo ->Cross, 0);
}
int ThermoToLinear (int i)
{
	fprintf (stderr, "Thermo to Linear is not implemented\n");
	exit (-1);
	return 0;
}

int LinearToThermo (int color)
{
	static int lookup [3*0x100+1];
	static int first = 1;
	if (first){
		int i;
		first = 0;
		for (i = -1; i < 0x300; i ++){
			if (i >= 0){
				int ind = MIN (i/3+1, 0xff); 
				if (one .depth == 24)
					lookup [i+1] =
						(one .color [ind] .red << 16) +
						(one .color [ind] .green << 8) +
						 one .color [ind] .blue;
				else
					lookup [i+1] = ind;
			}else
				lookup [i+1] = one .missingColor;
		}
	}
	return lookup [color + 1];
}

#include <signal.h>
#include <sys/signal.h>
void svHandler (int sig)
{
	if (sig == 0){		
		if (0!=signal (2, svHandler)){
			printf ("could not install signal handler 2\n");
		}
		if (0!=signal (3, svHandler)){
			printf ("could not install signal handler 3\n");
		}
		if (0!=signal (15, svHandler)){
			printf ("could not install signal handler 15\n");
		}
	}else{
		if (0!=signal (sig, svHandler)){
			printf ("could not install signal handler %d\n", sig);
		}
		printf ("in signal %d\n", sig);
	}
}

#include <math.h>

int random_sample (int lineSize, int retSize,
						 int w, int h, short * line,
						 short * ret)
{
	unsigned int  i = 0, x, y, j=0;
	unsigned int 	maxrand = RAND_MAX;
	unsigned int  white;
	short * line1 = malloc (lineSize*2*sizeof(short));
	int wOrig = w, hOrig = h;
	double ratio = MIN(500.0/w, 500.0/h);
	Pixmap pix, pix1;
	XImage * xim, *xim1;
	XGCValues val;
	GC mywhitegc = XCreateGC (one .dis, XtWindow (one .mainW),0,&val);
	GC myblackgc = XCreateGC (one .dis, XtWindow (one .mainW),0,&val);

	white = 255;
	XSetForeground (one .dis, mywhitegc, white);
	XSetForeground (one .dis, myblackgc, !white);
	w = (int)(w*ratio);
	h = (int)(h*ratio);

	for (i = 0; i < lineSize; i++){
		line1 [2*i] = (short)(line [2*i] * ratio);
		line1 [2*i+1] = (short)(line [2*i+1] * ratio);
	}
	fprintf (stderr, "ratio %lf, w%d, h%d\n", ratio, w, h);
	
	if (w+2 >= one .width || h+2 >= one .height){
		fprintf (stderr, "To big region for a display w=%d, h=%d\n", w, h);
		exit (-1);
	}
	
	pix = XCreatePixmap (one .dis, one .pixmap, w+2, h+2, one .depth);

	if (myXerror){
		fprintf (stderr, "pixCreation: w=%d h=%d size=%d\n",
					w, h, retSize);
		exit (-1);
	}

	
   XFillRectangle (one .dis, pix, myblackgc, 0, 0, w+2, h+2);
	XFillPolygon (one .dis, pix, mywhitegc, (XPoint*)line1,
					  lineSize, Nonconvex,
					  CoordModeOrigin);
	if (one .depth > 1)
		xim = XGetImage (one .dis, pix, 0, 0, w+2, h+2,
							  0xffffff, ZPixmap);
	else
		xim = XGetImage (one .dis, pix, 0, 0, w+2, h+2,
							  0xffffff, XYPixmap);
	XPutImage (one .dis, one .pixmap, one .currentgc, xim, 0, 0,
						 0, 0,
						 w+1, h+1);
	flushGraph ();
/*	{
		int ii,jj, kk, p1, p2=!white;
		FILE * o = fopen ("/tmp/xxx", "w");
		char buf [100];
		for(jj =0;jj<h;jj++)
			for(ii=0;ii< w; ii++)
			{
				unsigned char p = XGetPixel (xim, ii,jj);
				fwrite (&p, 1, 1, o);
			}
		fclose (o);
		sprintf (buf, "erode %d %d < /tmp/xxx > /tmp/yyy",
					w, h);
		system (buf);
		o = fopen ("/tmp/yyy", "r");
		for(jj =0;jj<h;jj++)
			for(ii=0;ii< w; ii++)
			{
				unsigned char p;
				fread (&p, 1, 1, o);
				XPutPixel (xim, ii,jj, p);
			}
		fclose (o);
	}						
*/	{
		int ii,jj, kk, p1, p2=!white;
		int cut = 10;
		for(ii=0;ii< w; ii++)
			for(jj =0;jj<h;jj++){
				if((p1=XGetPixel (xim, ii,jj)) == white)
					if (p2 != white){
						for (kk = 0; kk < cut; kk++)
							if (jj+kk < h)
								XPutPixel (xim, ii,jj+kk, !white);
						jj += kk;
					}
				if((p1=XGetPixel (xim, ii,jj)) == !white)
					if (p2 == white){
						for (kk = 0; kk < cut; kk++)
							if (jj-kk >= 0)
								XPutPixel (xim, ii,jj-kk, !white);
					}
				p2 = p1;
			}

		p2=!white;
		for(jj =0;jj<h;jj++)
			for(ii=0;ii< w; ii++){
				if((p1=XGetPixel (xim, ii,jj)) == white)
					if (p2 != white){
						for (kk = 0; kk < cut; kk++)
							if (ii+kk < w)
								XPutPixel (xim, ii+kk,jj, !white);
						ii += kk;
					}
				if((p1=XGetPixel (xim, ii,jj)) == !white)
					if (p2 == white){
						for (kk = 0; kk < cut; kk++)
							if (ii-kk >= 0)
								XPutPixel (xim, ii-kk,jj, !white);
					}
				p2 = p1;
			}
	}

	XPutImage (one .dis, one .pixmap, one .currentgc, xim, 0, 0,
						 0, 0,
						 w+1, h+1);
	flushGraph ();
	if (myXerror){
		fprintf (stderr, "imCreation: w=%d h=%d size=%d\n", w, h, retSize);
		exit (-1);
	}

	XPutImage (one .dis, one .pixmap, one .currentgc, xim, 0, 0,
						 0, 0,
						 w+1, h+1);
	flushGraph ();

	i = 0;
	while (i < retSize){
		int p, k, lef, righ;
		y = MIN(MAX((float)rand () * h / maxrand, 1.0), h-1);
		x = MIN(MAX((float)rand () * w / maxrand, 1.0), w-1);
		p = XGetPixel (xim, x, y);
		if (p == white){
			int ii, jj;
			int r2 = w*h/(double)retSize/3.14;
			int r = sqrt((double)r2);
			ret [i*2] = x;
			ret [i*2+1] = y;
			for (ii = -r; ii < r; ii++)
				for (jj = -r; jj < r; jj++)
					if (ii*ii + jj*jj <= r2){
						if (x+ii < 0 || x+ii >= w || y+jj < 0 || y+jj >= h)
							/*fprintf(stderr, "Out of bounds x%d y%d\n", x, y)*/;
						else
							p = XPutPixel (xim, x+ii, y+jj, !white);
					}
			XPutImage (one .dis, one .pixmap, one .currentgc, xim, 0, 0,
						 0, 0,
						 w+1, h+1);
			flushGraph ();
			i++;
		}
		j++;
		if (j > 100000){
			int ll;
			fprintf(stderr, "error: i=%d w%d, h%d\n", i, w, h);
			if (i == 0){
				ret [0] = w/2;
				ret [1] = h/2;
				i = 1;
				break;
			}else
				break;
			XFillRectangle (one .dis, one .pixmap, myblackgc,
								 0, 0, one .width, one .height);
			XPutImage (one .dis, one .pixmap, one .currentgc, xim,
						  0, 0,
							 0, 0,
							 w+1, h+1);
			flushGraph ();
			fprintf (stderr, "pixel=%d, x=%d, w=%d, y=%d h=%d\n",
						p, x, w, y, h);
			scanf ("%d", &ll);
			ret [i*2] = w/2;
			ret [i*2+1] = h/2;
			i++;
		}
	}
	retSize = i;	
	
	XDestroyImage (xim);
	XFreePixmap (one .dis, pix);

/* test if inside */	
	pix = XCreatePixmap (one .dis, one .pixmap, wOrig+2, hOrig+2,
								one .depth);
	XFillRectangle (one .dis, pix, myblackgc, 0, 0, wOrig+2, hOrig+2);
	XFillPolygon (one .dis, pix, mywhitegc, (XPoint*)line, lineSize,
					  Nonconvex,
					  CoordModeOrigin);
	if (one .depth > 1)
		xim = XGetImage (one .dis, pix, 0, 0, wOrig+1, hOrig+1,
							  0xffffff, ZPixmap);
	else
		xim = XGetImage (one .dis, pix, 0, 0, wOrig+1, hOrig+1,
							  0xffffff, XYPixmap);
	
	for (i = 0; i < retSize; i++){
		if (XGetPixel (xim, (int)(ret [2*i]/ratio),
							(int)(ret [2*i+1]/ratio)) != white){
			int ii, jj;
			int r2 = 64;
			int r = sqrt((double)r2);
			int x = ret [2*i], y = ret [2*i+1]; 
			fprintf (stderr, "Out of Bounds %d %d %d %d\n", wOrig, hOrig, (int)(ret [2*i]/ratio), (int)(ret [2*i+1]/ratio));
			XFillRectangle (one .dis, one .pixmap, myblackgc, 0, 0,
								 w+2, h+2);
			XFillPolygon (one .dis, one .pixmap, mywhitegc,
							  (XPoint*)line1,
							  lineSize, Nonconvex,
					  CoordModeOrigin);
			flushGraph ();
			for (ii = -r; ii < r; ii++)
				for (jj = -r; jj < r; jj++)
					if (ii*ii + jj*jj <= r2){
						if (x+ii < 0 || x+ii >= w || y+jj < 0 || y+jj >= h)
							;
						else
							XDrawPoint (one .dis, one .pixmap, myblackgc,
											x+ii, y+jj);
					}
			flushGraph ();
		}
	}
	XDestroyImage (xim);
	XFreePixmap (one .dis, pix);
	
	for (i = 0; i < retSize*2; i++)
		ret [i] = (short)((ret [i]) / ratio);

	free (line1);
	return retSize;
}






















