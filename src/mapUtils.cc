#include <stdlib.h>
#include "lvr.h"
#include "map.h"
#include "graph.h"
#include "codes.h"


static int *image = 0, *image1 = 0, *image2 = 0;
static char * id = 0;


extern "C" void Record (void);


void fillSmooth (char * id, int * image, int hor, int ver,
					  iarray& regular,
					  int w, int h, iarray& dat);
void fillSmoothGl (char * id, int hor, int ver,
					  iarray& regular,
					  int atX, int atY, 
		int w, int h, iarray& dat);

int fill_usa (int col, float subsample = 1)
{
	map usa;
	char buf [255];
	sprintf (buf, "%s/usa.map", getenv ("MIASMA"));
 	usa .read (buf);
	iarray usaColorData (1);
	usaColorData = col;
	usa .fill (&usaColorData, subsample);

/*	map states;
	states .read ("state.map");
	iarray stateColorData (states .size ());
	stateColorData = (int)(!col);
	states .draw (&stateColorData);
*/	flushGraph ();
	return 0;
}

char * makeIdImage (float subsample=1)
{
	char * id;
	int w = int(one .MapWidth*subsample),
		h = int(one .MapHeight*subsample);
	XImage * idIm;

	clearScreen (0);
	fill_usa (1, subsample);
	idIm = XGetImage
		(one .dis, one .pixmap, one .MapShiftX,
		 one .height - h - one .MapShiftY,
		 w, h, 0xffff, ZPixmap);
	{
		int i, j;
		if ((id = new char [w * h])==0){
		 	perror ("Not enough memory");
			exit (1);
		}
		for (i = 0; i < w; i++)
			for (j = 0; j < h; j++)
				if (XGetPixel (idIm, i, j))
					id [i+(h-1-j)*w] =  1;
				else
					id [i+(h-1-j)*w] = 0;
	}
	XDestroyImage (idIm);
	clearScreen (one .missingColor);
	return id;		 
}


void map ::interpolateFill (int times, int fillMissing,
									 iarray& dataA, iarray& dataB)
{
	iarray outline = dataA;
	outline = 0;

	for (int i = 0; i < times; i++){
		double s = double (i)/times;
		for (int j = 0; j < outline .length (); j++)
			if (dataA (j) >=0 && dataB (j) >= 0)
				outline (j) = int ((1.0-s) * dataA (j) + 
										 s * dataB (j) + .5);
			else{
				if (dataA (j) > 0)
					outline (j) = int (-(1.0-s) * dataA (j) +
											 s * dataB (j)  - .5);
				else{
					if (dataB (j) > 0)
						outline (j) = int ((1.0-s) * dataA (j) -
												s *  dataB (j) - .5);
					else
						outline (j) = int ((1.0-s) * dataA (j) +
												s *  dataB (j) - .5);
				}
			}
		fill (&outline);
		Record ();
	}
}

static void createImages (int size, float subsample)
{
	if (id)
		delete id;
	if (image)
		delete image;
	if (image2)
		delete image2;
	if (image1)
		delete image1;
	id = makeIdImage (subsample);
	image  = new int [size];
	image1 = new int [size];
	image2 = new int [size];
	if (id == NULL || image == NULL ||
		 image1 == NULL || image2 == NULL)
		aerror ("Not enough memory to allocate the image");
}

static void refresh (int w, int h, float subsample)
{
	static int wOld = 0, hOld = 0;
	static float subsamp = 0;
	if (subsamp != subsample ||
		 wOld != w || hOld != h){
		wOld = w;
		hOld = h;
		subsamp = subsample;
		createImages (w*h, subsample);
	}
}

void map ::interpolateSmooth (int& times, int hor, int ver,
										iarray& pseudo,
										iarray dataA, iarray dataB,
									float subsample)
{
	int w = int(one .MapWidth*subsample),
		h = int(one .MapHeight*subsample);
	line reg (pseudo .length ()/ 2);
	reg. iarray ::operator= (pseudo);
	line transPseudo0 (transform (reg, subsample));
	pnt pp (-one .MapShiftX, one .height - one .MapShiftY);
	line transPseudo (transPseudo0 .scale (1, -1) + pp);
	
	refresh (w, h, subsample);

	::fillSmooth (id, image1, hor, ver, transPseudo, w, h, dataA);
	::fillSmooth (id, image2, hor, ver, transPseudo, w, h, dataB);
	
	for (int i = 0; i < times; i++){
		float s = double (i) / times;
//		printf ("%f\n", s);
		for (int j = 0; j < w*h; j++){
			image [j] = int((1.0-s) * image1 [j] + s * image2 [j]);
		}	
		putImageExpand (image, one .MapShiftX, one .MapShiftY, w, h, subsample);
		Record ();
	}
/*  Gl stuff
	iarray datC = dataA;
	for (int i = 0; i < times; i++){
		float s = double (i) / times;
		for (int j = 0; j < dataA .length (); j++)
			datC (j) =  int((1.0-s) * dataA (j) + s * dataB (j));
		
//		clearScreen (one .missingColor);
		::fillSmoothGl (id, hor, ver, transPseudo, 
			one .MapShiftX, one .MapShiftY, w, h, 
							 datC);
		Record ();
		}
*/		
}


void map ::fillSmooth (int hor, int ver, iarray pseudo, iarray dat,
						  float subsample)
{
	line reg (pseudo .length () / 2);
	reg. iarray ::operator= (pseudo);
	line transPseudo0 = transform (reg, subsample);	
	pnt pp (-one .MapShiftX, one .height - one .MapShiftY);
	line transPseudo (transPseudo0 .scale (1, -1) + pp);

	int w = int(one .MapWidth*subsample),
		h = int(one .MapHeight*subsample);

	refresh (w, h, subsample);

	::fillSmooth (id, image, hor, ver, transPseudo, w, h, dat);
	putImageExpand (image,
						 one .MapShiftX, one .MapShiftY,
						 w, h,
						 subsample);

/*
	::fillSmoothGl (id, hor, ver, transPseudo, 
		one .MapShiftX, one .MapShiftY, w, h, 
		dat);
*/		
}

#include <math.h>
			 

int map ::generateRandom (int averageNumber,
								  int minNumber, int maxNumber,
								  iarray ** random, iarray ** index)
{
	
	sarray ii ((maxNumber) * numberOfSegments); 
	sarray iiC ((maxNumber) * numberOfSegments); 
   iarray rr (2, (maxNumber) * numberOfSegments);

	double area = view .area ();

	int current = 0;
	for (int i = 0; i < numberOfSegments; i++){
		rect env = envelopes [i];
		double smallArea = env .area ();

		line * cc = segments [i];
		int len = cc ->length ();
		short * points = new short [len*2];
		int x0 = env .tl () .x (); 
		int y0 = env .tl () .y (); 
		for (int j = 0; j < len; j++){
			points [2*j] = (*cc) (0, j) - x0; 
			points [2*j+1] = (*cc) (1, j) - y0;
		}

		int all = MIN(MAX ((int)(averageNumber * numberOfSegments *
									smallArea / area), minNumber), maxNumber);
	 	short * ret = new short [all*2];
		all = random_sample (len, all, env .br () .x () - x0,
									env .br () .y () - y0,
							points, ret);
		delete points;
		if (all + current >=  (averageNumber+1) * numberOfSegments)
			aerror ("Problem generating random grid");
		for (int j = current; j < all + current; j++){
			rr (0, j) = ret [2*(j-current)] + x0;
			rr (1, j) = ret [2*(j-current)+1] + y0;
			ii (j) = (*indexes) (0, i);
			iiC (j) = (*indexes) (1, i);
		}
		delete ret;
		current += all;
	}
	*index = new iarray (current);
	*random = new iarray (2, current);
	for (int i = 0; i < current; i++){
		(**random) (0, i) = rr (0, i);
		(**random) (1, i) = rr (1, i);
	}

// fiddling to get the indexes into my ordering:	
	codes code ("stateTable");
	for (int i = 0; i < current; i++){
//		fprintf (stderr, "%d %d %d ", i, ii(i), code .fromIcpsr (ii (i)));
//		fprintf (stderr, "%d %d\n", (**random) (0, i), (**random) (1, i));
		(**index) (i) = code .fromIcpsr (ii (i), iiC (i));
	}

	return current;
}
	  

// Algorithm to fill from the regular grid using id as an indicator where
// to put nonbackground collor and place the resulting image into
// array image. 
// dat is array of hor times ver of the data points;
// image and id are arrays of size w times h
// id should be upside down !!!!
// !!!!!!!! The filling is done upside down to appear correctly on the
//  X window !!!!!!!!!!!

void fillSmooth (char * id, int * image, int hor, int ver,
					  iarray& regular,
					  int w, int h, iarray& dat)
{
	for (int i = 0; i < w*h; i++)
		image [i] = MISSING;
	for (int i = 0; i < hor-1; i++)
		for (int j = 0; j < ver-1; j++){
			int bl = dat (i*ver + j);
			int br = dat ((i+1)*ver + j);
			int tl = dat (i*ver + j + 1);
			int tr = dat ((i+1)*ver + j + 1);
  			int lX = regular (0, i*ver+j);  
			int rX = regular (0, (i+1)*ver+j+1);  
			int bY = regular (1, i*ver+j);  
			int tY = regular (1, (i+1)*ver+j+1);
			if ((rX-lX) < 0 || (tY-bY) < 0 )
				fprintf (stderr, "%s:%d %d %d %d %d\n",
							__FILE__, __LINE__, lX, rX, tY, bY);
 			for (int k = lX; k < rX; k++)
				for (int l = bY; l < tY; l++){
					int ind = l;
					if (k + ind*w < 0 || k + ind*w >= w*h){
//						fprintf (stderr, "%s:%d %d %d %d %d\n", __FILE__, __LINE__, k, ind, k+ind*w, w*h);
					}else
						if (id [k + ind*w]){
							image [k + ind*w] = 
								int((tl * (rX-k) * (l-bY) +
											 tr * (k-lX) * (l-bY) + 
											 bl * (rX-k) * (tY-l) +
											br * (k-lX) * (tY-l)) /
								  		(float) (rX-lX) / (tY-bY) + .5);
						}
				}
		}

}









