#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/StringDefs.h>
#include <X11/cursorfont.h>
#include <X11/Intrinsic.h> 
#include <X11/Xaw3d/Simple.h>
#include "graph.h"
#include "map.h"
#include "smooth.h"
#include "codes.h"

codes * c;
ddataArray * incidence;
darray * scales;
sdataArray * colors;
map theMap;
smooth * theSmooth;

extern "C" void  ColorLegend ()
{
	float * x = new float [one .NLabels];
	for (int i = 0; i < one .NLabels; i++)
		x [i] = (*scales) (one .View, i);
	putColorScale (one .NLabels, x);
	flushGraph ();
	delete x;
}

void reorder (iarray& dataSrc, iarray& dataDst,
				  iarray& indexes)
{
	int i, size = dataDst .length ();
	for (i = 0; i < size; i++)
		dataDst (i) = dataSrc ((*c) .fromIcpsr
									  (indexes (0, i), indexes (1, i)));
}

extern "C" void ChangeLambda (double lambda)
{
	if (theSmooth)
		(*theSmooth) .changeLambda (lambda /
											 (one .width + one.height));
}

int createGrid (map * m)
{
// to create grids
	grid g (*m);
	(*m) .fill ();
	flushGraph ();
  
	int horp, verp, ra, mp, ma;
	fprintf (stderr, 
				"enter the pseudo grid horizontal number, vertical number,\n%s"
				, "random grid for each object, and minimal and maximal number of points\n");
	fflush (stderr);
	scanf ("%d %d %d %d %d", &horp, &verp, &ra, &mp, &ma);
	g .on (*m, horp, verp, ra, mp, ma);
	(*m) .fill ();
	(*m) .fill ();
	g .show (150, 0);
	flushGraph ();
//	sleep (10);
	g .write ();
	return 1;
}

void PlotTimeSeries (graph * ooo, Window win, int disease, int state,
							int wid, int hei)
{
	sdataArray val = (*colors) .column (disease) .row (state);
//	fprintf (stderr, "%d %d %d\n", disease, state, val .length ());
	short * data = new short [ooo ->data .NMonths * 2];
	int j= 0;
	int wid1 = wid - 60, hei1 = hei - 20;
	for (int i = 0; i < ooo ->data .NMonths; i++){
		if (val (i) != MISSING){
			data [2*j+1] =
				short (hei1 * (1.0 - float(val  (i)) / 0x2ff));
			data [2*j] = short(float(i)/ooo ->data .NMonths * wid1);
//			fprintf (stderr, "%d %d %d\n", j, data [2*j], data [2*j+1]);
			j++;
		}
	}
	int len = j;
	XSetForeground (ooo ->dis, ooo ->currentgc, ooo ->background);
	XFillRectangle (ooo ->dis, win, ooo ->currentgc, 0, 0,
						 wid, hei);						
	XSetForeground (ooo ->dis, ooo ->currentgc, ooo ->foreground);
	XDrawLines (ooo ->dis, win,
					ooo ->currentgc, (XPoint *)data,
					len, CoordModeOrigin);
	short * data1 = new short [len * 8];
	int sZ = wid1/ooo ->data .NMonths + 1;
	sZ = MIN (6, sZ);
	for (int k = 0; k < len; k++){
		data1 [4*k] = data [2*k] - sZ/2;
		data1 [4*k+1] = data [2*k+1] - sZ/2;
		data1 [4*k+2] = sZ;
		data1 [4*k+3] = sZ;
	}
	XDrawRectangles (ooo ->dis, win,
					ooo ->currentgc, (XRectangle *)data1,
							 len);
	delete data;
	delete data1;

	int fH = ooo ->fontInfo ->max_bounds .ascent + 
			ooo ->fontInfo ->max_bounds .descent;
	char buf [100];

	for (int i = 0; i < ooo ->NLabels; i++){
		int x = wid1, y = int ((ooo ->NLabels-i-1.0) *
								  (hei - fH)/(ooo ->NLabels-1.0) +
								  ooo ->fontInfo ->max_bounds .ascent + .5);
		sprintf (buf, "%.3e", (*scales) (ooo ->View, i));
		XDrawString(ooo ->dis,
						win,
						ooo ->textgc, x, y, buf, strlen(buf));
	}
	int tmp = ooo ->data .Month;
	for (int i = 1; i < 4; i++){
		ooo ->data .Month = ooo ->data .NMonths * i / 4; 
		sprintf (buf, "%.2d/%.2d", year (&(ooo ->data))-1900,
					month (&(ooo ->data)));
		XDrawString (ooo ->dis, win,
					 ooo ->textgc, wid1 * i/4,
					 hei-fH/2,
					 buf, strlen(buf));
	}
	ooo ->data .Month = tmp;
}

void calcMapDims (graph& one)
{
	one .mapAspect = 1.7;
	one .MapWidth =  int (one .width - 100);
	one .MapHeight = int (one .height * 8 / 10.);
	one .MapShiftX = int (100);
	one .MapShiftY = int (one .height * 1 / 10.);
	if (one .MapHeight * one .mapAspect > one .MapWidth){
		one .MapShiftY = int(one .height * 1 / 10. + 
			(one .MapHeight - one .MapWidth / one .mapAspect) / 2.);
		one .MapHeight =  int(one .MapWidth / one .mapAspect);
	}
	if (one .MapHeight * one .mapAspect < one .MapWidth){
		one .MapShiftX = int (100 +
			(one .MapWidth - one .MapHeight * one .mapAspect) / 2.);
		one .MapWidth =  int (one .MapHeight * one .mapAspect);
	}
}

void RedrawFill (int fill)
{
	static iarray * indexes = 0;
	static grid * g;
	int size = theMap .size ();
	static farray * weights;

// initialize 	
	if (!indexes){
		char buf [100];
		indexes = theMap .getIndexes ();
		sprintf (buf, "%s/state.map.grid", getenv ("MIASMA"));
		FILE * gg = fopen (buf, "r");
		if (gg == 0)
			aerror (buf);
		g = new grid (theMap);
		(*g) .read (gg);
		fclose (gg); 
		weights = new farray (one .data .NStates);
		(*weights) = 1;
		theSmooth = new smooth (&((*g) .p ()), &((*g) .r ()),
							 &((*g) .i ()), weights, one .Lambda);

	}


// Check the window
	static unsigned short wid, hei, sX, sY;
	XtVaGetValues (one .mainW,
						XtNwidth, &wid,
						XtNheight, &hei,
						XtNx, &sX,
						XtNy, &sY,
						NULL);
	if (one .width != wid || one .height != hei){
		one .width = wid;
		one .height = hei;
		fprintf (stderr, "- %d %d %d %d %d %d %d %d-\n",
					wid, hei, sX, sY,
					one .MapWidth,
					one .MapHeight,
					one .MapShiftX, one .MapShiftY);
		Pixmap pixmap = XCreatePixmap (one .dis, XtWindow (one .mainW),
									  one .width + 2, one .height + 2,
									  one .depth);
		XtVaSetValues (one .mainW, XtNbitmap, pixmap,
			NULL);
		XFreePixmap (one .dis, one .pixmap);
		one .pixmap = pixmap;
		clearScreen (one .missingColor);
	}
		


	
// Prepare the data	
	iarray datA (size);
	iarray datB (size);
	idataArray tmpA (one .data .NStates);
	idataArray tmpB (one .data .NStates);
	int whereA = (one .data .Month - one .direction)%
		one .data .NMonths;
	int whereB = one .data .Month;
  	whereA = whereA > 0 ? whereA : 0;
	fprintf (stderr, "%d %d %d\n", whereA, whereB, one .data .NMonths);
	for (int i = 0; i < one .data .NStates; i ++){
		tmpA (i) = (*colors) (one .View, whereA, i);
		tmpB (i) = (*colors) (one .View, whereB, i);
	}

	ColorLegend ();
// Do the drawing	
	if (fill){
		int medValA = int(tmpA .median () + .5);
		int medValB = int(tmpB .median () + .5);
		fprintf (stderr, "%d %d\n",  medValA,  medValB);
		for (int i = 0; i < one .data .NStates; i ++){
			if (tmpA (i) != MISSING){
				tmpA (i) = LinearToThermo (tmpA (i));
			}else{
				if (one .direction && one .interpolate > 1)
					tmpA (i) = - LinearToThermo (medValA);
				else
					tmpA (i) = - LinearToThermo (one .showMiss);
			}					
			if (tmpB (i) != MISSING){
				tmpB (i) = LinearToThermo (tmpB (i));
			}else{
				if (one .direction && one .interpolate > 1)
					tmpB (i) = - LinearToThermo (medValB);
				else
					tmpB (i) = - LinearToThermo (one .showMiss);
			}					
		}
		reorder (tmpA, datA, *indexes);
		reorder (tmpB, datB, *indexes);
		if (one .direction)
			theMap .interpolateFill (one .interpolate,
									 one .showMiss,		 
									  datA,
									  datB);
		else
			theMap .fill (&datA);
//		(*g) .show (LinearToThermo (3*0xff));
	}else{ //smooth
		if (one .direction){
			theMap .interpolateSmooth (one .interpolate,
				(*g) .horizontal (), (*g) .vertical (),
				(*g) .p (),
				(*theSmooth) .on (tmpA, MISSING),
				(*theSmooth) .on (tmpB, MISSING),
				one .subsample								
			  );
		}else{
			theMap .fillSmooth ((*g) .horizontal (), (*g) .vertical (),
				(*g) .p (),
				(*theSmooth) .on (tmpA, MISSING),
									  one .subsample);
		}
		ColorLegend ();
	}
	flushGraph ();
}


void Redraw (graph * ooo)
{
	RedrawFill (ooo ->fill);
}

extern "C" double sqrtTrans (double x)
{
	if (x >= 0)
		return sqrt (x);
	else
		return sqrt (-x);
}

static double logParameter = 0;
extern "C" double logTrans (double x)
{
	return log (x + logParameter);
}

extern "C" double inverseTrans (double x)
{
	return 1/ (x + DBL_EPSILON);
}


void Rescale (graph * ooo)
{
	for (int i = 0; i < ooo ->data .NDis; i++){
		ddataArray trans = (*incidence) .column (i);
		double theMax = trans .max ();
		double theMin = trans .min ();
//		printf ("%le %le\n", theMax, theMin); 
		int j;
		double * xx = new double [ooo ->NLabels];
		switch (ooo ->Transformation){
		case 0:{
			for (j = 0; j < ooo ->NLabels; j ++)
				xx [j] = double (j)/(ooo ->NLabels-1);
			trans .quantile (ooo ->NLabels, xx);
			for (j = 0; j < ooo ->NLabels; j ++)
				(*scales) (i, j) = xx [j]; 
			trans .histogram (1, 3*0xff);
			printf ("%d %le %le\n", i, trans .max (), trans .min ());
			break;
		}
		case 1:
			logParameter = -theMin + FLT_EPSILON;
			for (j = 0; j < ooo ->NLabels; j ++)
				(*scales) (i, j) =
					exp (double (j)/(ooo ->NLabels-1) *
						  (logTrans (theMax) - logTrans (theMin)) +
						  logTrans (theMin)) - logParameter;
												; 
		  	trans .transformSelf (logTrans);
			trans .rescale (1, 3*0xff);
			break;
		case 2:
			for (j = 0; j < ooo ->NLabels; j ++){
				double tmp = 
					double (j)/(ooo ->NLabels-1) *
					(sqrtTrans (theMax) - sqrtTrans (theMin)) +
					sqrtTrans (theMin);				
				if (tmp >= 0)
					(*scales) (i, j) = tmp * tmp;
				else
					(*scales) (i, j) =  - tmp * tmp;
			}
		  	trans .transformSelf (sqrtTrans);
			trans .rescale (1, 3*0xff);
			break;
		case 3:
			for (j = 0; j < ooo ->NLabels; j ++)
				(*scales) (i, j) = double (j)/(ooo ->NLabels-1) *
					( (theMax) -  (theMin)) +  (theMin); 
			trans .rescale (1, 3*0xff);
			break;			
		case 4:
			for (j = 0; j < ooo ->NLabels; j ++)
				(*scales) (i, j) = inverseTrans(double (j)/
														  (ooo ->NLabels-1) *
					(-inverseTrans(theMax) + inverseTrans(theMin))
						+ inverseTrans(theMax)) - DBL_EPSILON; 
		  	trans .transformSelf (inverseTrans);
			trans .rescale (1, 3*0xff);
			break;			
		}
		for (int k = 0; k < ooo ->data .NMonths; k++)
			for (j = 0; j < ooo ->data .NStates; j++)
				if (trans (k, j) != ooo ->data .Missing)
 					(*colors) (i, k, j) = (int) trans (k, j);
				else
					(*colors) (i, k, j) = MISSING;
		delete xx;
	}
}

void ReadData (graph * ooo, char * name, const int binary)
{
	char fName [100];
	FILE * in;
	sprintf (fName, "%s.desc", name);
	in = myfopen (fName, "r");	
	if (6 != fscanf (in, "%d %d %le %d %le %d",
						  &(ooo ->data .NDis),
						  &(ooo ->data .NMonths),
						  &(ooo ->data .YearFraction),
						  &(ooo ->data .NStates),
						  &(ooo ->data .Missing),
						  &(ooo ->data .startYear))){
		fprintf (stderr, "Can not read %s file\n", fName);
		exit (-1);
	}
	ooo ->NViews = ooo ->data .NDis;
	incidence = new ddataArray
		(ooo ->NViews, ooo ->data .NMonths,
		 ooo ->data .NStates, ooo ->data .Missing);
	colors = new sdataArray (ooo ->NViews, 
				 ooo ->data .NMonths, ooo ->data .NStates, MISSING);
	scales = new darray (ooo ->NViews, ooo ->NLabels);
	ooo->data .DisNames  = new char * [ooo ->NViews];
	if (incidence == NULL || colors == NULL
		 || scales == NULL || ooo->data .DisNames == NULL){
		fprintf (stderr, "Can not allocate two %d size arrays\n",
					ooo ->data .NDis *
					ooo ->data .NMonths * ooo ->data .NStates);
		exit (-1);
	}
	for (int i = 0; i < ooo ->NViews; i++)
		for (int j = 0; j < ooo ->NLabels; j++)
			(*scales) (i, j) = 0; 
	for (int i = 0; i < ooo ->data .NDis; i++){
		char buf [100];
		if (1 != fscanf (in, "%s", buf)){
			fprintf (stderr, "Can not read name %d\n", i);
			exit (-1);
		}
		ooo->data .DisNames [i] = new char [strlen (buf) + 1];
		strcpy (ooo->data .DisNames [i], buf);
	}
	fclose (in);

	sprintf (fName, "%s/stateTable", getenv ("MIASMA"));
	c = new codes (fName);
	ooo ->data .StateNames = c ->Names ();

	if (binary)
		sprintf (fName, "%s.bin", name);
	else	
		sprintf (fName, "%s.asc", name);
	in = myfopen (fName, "r");
	for (int i = 0; i < ooo ->data .NDis; i++)
		for (int k = 0; k < ooo ->data .NMonths; k++)
			for (int j = 0; j < ooo ->data .NStates; j++){
				double tmp;
				if (binary)
					fread (&tmp,
							 sizeof (tmp), 1, in);
				else
					fscanf (in, "%le", &tmp);
				if (tmp != ooo ->data .Missing) 
					(*incidence) (i, k, j) = (tmp * 1e5 
			/ ooo ->data .YearFraction);
				else
					(*incidence) (i, k, j) = ooo ->data .Missing;
			}
	fclose (in);
}

main (int argc, char * argv [])
{
	char buf [255];
	int binary = 1;
	char * cMap = "rgb";
	sprintf (buf, "6");
	switch (argc){
	case 4:
		cMap = argv [3];
	case 3:
		binary = atoi (argv [2]);
	case 2:
		sprintf (buf, "%s", argv [1]);
	}		
		
		
// read map
	{
		char buf [255];
		sprintf (buf, "%s/state.map", getenv ("MIASMA"));
		theMap .read (buf);
	}
	
	one .NLabels = 5;
	one .legendTextAtt = "fixed";
	one .horizontalBar = 0;
	one .PlotColorBar = 1;
	one .ColorBarWidth = 20;
	one .PlotText = 1;
	
// read data
	ReadData (&one, buf, binary);
	one .data .Month = 0;

	one .data .Disease = 0;
	one .View = 0;

	one .Transformation = 3; // linearTrans
	one .fill = 1;	
	one .interpolate = 10;
	one .showMiss = -1;
	one .Lambda = .00005;
	one .subsample = 1;

	one .NTransformations = 5;
	one .TransformationNames = (char **) malloc (one .NTransformations *
													sizeof (char *));
	one .TransformationNames [0] = "Rank";
	one .TransformationNames [1] = "Log";
	one .TransformationNames [2] = "Sqrt";
	one .TransformationNames [3] = "Linear";
	one .TransformationNames [4] = "Inverse";

	Rescale (&one);
	one .width = 1100;
	one .height = 650;
	one .width = 700;
	one .height = 400;
	calcMapDims (one);
	initGraph (cMap);
	clearScreen (one .missingColor);

//	createGrid (&theMap);
//	exit (1);

	Redraw (&one);
	XtAppMainLoop (one .appCon);	
}



extern "C" int year (Data * dat)
{
	return int(dat ->Month * dat ->YearFraction +
				  dat ->startYear);
}

extern "C" int month (Data * dat)
{
	int d = int(dat ->Month * dat ->YearFraction);
	return dat ->Month - int(d / dat ->YearFraction) + 1;
}

	



















