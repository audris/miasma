#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/keysymdef.h>
#include <X11/cursorfont.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/Xaw3d/Toggle.h>
#include <X11/Xaw3d/Form.h>
#include <X11/Xaw3d/AsciiText.h>

#define XK_MISCELLANY
#include <X11/keysymdef.h>


#include <stdio.h>

#include "graph.h"

static void * res;
static void (*process) (void * res, char * text);

Widget CreateCommand (char * label, Boolean sensitive,
				  Widget href, Widget vref,
				  Widget parent)
{
	Widget ans =
	  XtVaCreateWidget("Command",
     commandWidgetClass, parent,
     XtNlabel, (String) label,
     XtNsensitive, (Boolean) sensitive,
     XtNfromHoriz, (Widget) href,
     XtNfromVert, (Widget) vref,
     NULL);
  return(ans);
}

XtCallbackProc InputCancelCback(Widget w, graph * ooo, XtPointer callback_data)
{
  XtDestroyWidget(XtParent(XtParent(w)));
  return 0;
}

void InputDone (Widget w, graph * ooo, XEvent * event)
{
	XKeyPressedEvent *evnt = (XKeyPressedEvent *) event;
	KeySym key;
	char * name;
	key = XLookupKeysym(evnt, 0);
	if (key == XK_Return){
		XtVaSetValues(XtParent(XtParent(XtParent(w))),
						  XtNstate, (Boolean) False, NULL);
		XtVaGetValues(w, XtNstring, &name, NULL);
		process (res, name);
		XtDestroyWidget(XtParent(XtParent(w)));
		Redraw (ooo);
	}
}

void getInput (Widget W, char * query, char * msg, void * result,
					graph * ooo, void (*proc)(void*, char*))
{
  Widget fpopup, fform, flabel, ftext, fcancel;
  Dimension width, height;
  Position x, y;
  Cursor text_cursor = XCreateFontCursor(ooo ->dis, XC_xterm);
  res = result;
  process = proc;
  XtVaGetValues(W,
    XtNwidth, &width,
    XtNheight, &height,
    NULL);
  XtTranslateCoords(W,
    (Position) (width/2), (Position) (height/2), &x, &y);

  fpopup = XtVaCreatePopupShell("InputPopup",
    transientShellWidgetClass, ooo ->toplevel,
	 XtNvisual, ooo ->vis,
    XtNx, (Position) x,
    XtNy, (Position) y,								  
    XtNinput, (Boolean) True,
    XtNallowShellResize, (Boolean) True,
    XtNtitle, (String) "Enter Input",
    NULL);
  fform = XtVaCreateManagedWidget("InputForm",
    formWidgetClass, fpopup,
    NULL);
  flabel = XtVaCreateManagedWidget("InputText",
    labelWidgetClass, fform,
    XtNleft, (XtEdgeType) XtChainLeft,
    XtNright, (XtEdgeType) XtChainLeft,
    XtNtop, (XtEdgeType) XtChainTop,
    XtNbottom, (XtEdgeType) XtChainTop,
    XtNlabel, (String) query,
    NULL);
  ftext = XtVaCreateManagedWidget("Input",
    asciiTextWidgetClass, fform,
    XtNfromVert, (Widget) flabel,
    XtNleft, (XtEdgeType) XtChainLeft,
    XtNright, (XtEdgeType) XtChainRight,
    XtNtop, (XtEdgeType) XtChainTop,
    XtNbottom, (XtEdgeType) XtChainTop,
    XtNresizable, (Boolean) True,
    XtNeditType, (int) XawtextEdit,
    XtNresize, (XawTextResizeMode) XawtextResizeWidth,
    XtNstring, (String) msg,
    NULL);
  XtAddEventHandler(ftext, KeyPressMask, FALSE,
     (XtEventHandler) InputDone, (XtPointer) ooo);

  fcancel = (Widget) CreateCommand ("Cancel",
    1, (Widget) NULL, (Widget) ftext,
    fform);
  XtManageChild(fcancel);
  XtAddCallback(fcancel, XtNcallback,
    (XtCallbackProc) InputCancelCback, (XtPointer) ooo);

  XtPopup(fpopup, XtGrabExclusive);
  XRaiseWindow(ooo ->dis, XtWindow(fpopup));
  XDefineCursor(ooo ->dis, XtWindow(ftext), text_cursor);
}















