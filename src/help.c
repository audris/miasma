#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/Core.h>         /* tourlabels */
#include <X11/CoreP.h>         /* tourlabels */
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/cursorfont.h>
#include <X11/bitmaps/target>

#include <X11/Xaw3d/AsciiText.h>
#include <X11/Xaw3d/AsciiTextP.h>
#include <X11/Xaw3d/Box.h>
#include <X11/Xaw3d/BoxP.h>
#include <X11/Xaw3d/Cardinals.h>
#include <X11/Xaw3d/Command.h>
#include <X11/Xaw3d/CommandP.h>
#include <X11/Xaw3d/Dialog.h>
#include <X11/Xaw3d/DialogP.h>
#include <X11/Xaw3d/Form.h>
#include <X11/Xaw3d/FormP.h>
#include <X11/Xaw3d/Label.h>
#include <X11/Xaw3d/LabelP.h>
#include <X11/Xaw3d/MenuButton.h>
#include <X11/Xaw3d/Paned.h>
#include <X11/Xaw3d/PanedP.h>
#include <X11/Xaw3d/Scrollbar.h>
#include <X11/Xaw3d/ScrollbarP.h>
#include <X11/Xaw3d/SimpleMenu.h>
#include <X11/Xaw3d/Sme.h>
#include <X11/Xaw3d/SmeBSB.h>
#include <X11/Xaw3d/Toggle.h>
#include <X11/Xaw3d/ToggleP.h>

typedef struct {
  Widget hpopup, frame, text, done;
  int helpid;
} HelpData;

#include "graph.h"


XtCallbackProc HelpDoneCback (Widget w,
										XtPointer *dat, void * cback_data)
{
	XtDestroyWidget (XtParent (XtParent (w)));
	return 0;
}

void help (graph * ooo, char * helpfile)
{
	int ok;
	HelpData *data;
	myfopen (helpfile, "r");
	ok = 0;
	data = (HelpData *) XtMalloc (sizeof(HelpData));
	data ->hpopup = XtVaCreatePopupShell ("Help",
		topLevelShellWidgetClass,
      ooo ->toplevel,
      XtNdepth, ooo ->depth, 				  
		XtNcolormap, ooo ->cmap,
		XtNvisual, ooo ->vis,
		XtNtitle, (String) "GRAPH Help Window",
      XtNiconName, (String) "GRAPH Help Window",
		XtNwidth, 1000,
		XtNheight,1000,
		NULL);
	data ->frame = XtVaCreateManagedWidget ("Form",
      panedWidgetClass, data ->hpopup,
      XtNorientation, (XtOrientation) XtorientVertical,
      NULL);
	data ->text = XtVaCreateManagedWidget ("Text",
      asciiTextWidgetClass, data->frame,
      XtNallowResize, (Boolean) True,
      XtNshowGrip, (Boolean) False,
      XtNtype, (XawAsciiType) XawAsciiFile,
      XtNstring, (String) helpfile,
      XtNscrollVertical, (XawTextScrollMode) XawtextScrollWhenNeeded,
      XtNdisplayCaret, (Boolean) False,
		NULL);	
	data ->done = XtVaCreateManagedWidget ("Done",
      commandWidgetClass, data ->frame,
      XtNshowGrip, (Boolean) False,
      XtNskipAdjust, (Boolean) True,
      XtNlabel, (String) "Click here to dismiss",
      NULL);
	XtAddCallback (data->done, XtNcallback,
      (XtCallbackProc) HelpDoneCback, (XtPointer) data);
	XtPopup (data ->hpopup, XtGrabNone);
	XRaiseWindow(ooo ->dis, XtWindow (data->hpopup));
	return;
}











