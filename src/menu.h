#ifndef MENU_H
#define MENU_H

String MenuItems [] = {
	"Number of frames",
	"Set Missing Values",
	"Set subsampling rate",
	"Select Colormap",
	"Print"
};

void MenuSelect(Widget w, XtPointer paneNum,  XtPointer pIII);
void playCback (Widget w, graph * ooo);
void recordCback (Widget w, graph * ooo);
void fillCback (Widget w, graph * ooo);

#endif
