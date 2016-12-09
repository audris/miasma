#include <stdio.h>
#include <stdlib.h>
#include "malloc.h"

#include "map.h"
#include "graph.h"

map ::~map (void)
{
	if (segments){
		for (int i = 0; i < numberOfSegments; i++)
			delete segments [i];
		delete segments;
	}
//	if (envelopes)
//		delete envelopes;
	if (indexes)
		delete indexes;
	if (selected)
		delete selected;
};

inline int rect ::overlap (rect& p)
{
	return
		! (
		botr .x () < p .topl .x () ||
		botr .y () < p .topl .y () ||
		topl .x () > p .botr .x () ||
		topl .y () > p .botr .y ()
		);
}

line line ::scale (double x, double y) 
{
// Scaling the line by given factors	
	int l = this ->length ();
	line ans = *this;
	for (int i = 0; i < l; i++){
		ans .iarray ::operator() (0, i)  =
			int (ans .iarray ::operator() (0, i) * x + .5); 
		ans .iarray ::operator() (1, i)  =
			int (ans .iarray ::operator() (1, i) * y + .5); 
	}
	return ans;
}


line line ::operator+ (pnt p)
{
// Shifting the line	
	int l = this ->length ();
	line ans = *this;
	for (int i = 0; i < l; i++){
		ans .iarray ::operator() (0, i) += p .x (); 
		ans .iarray ::operator() (1, i) += p .y ();
	}
	return ans;
}



rect line ::envelope (void)
{
// Maximal and minimal values of the line
	int l = this ->length ();
	int maxx, maxy, minx, miny;
	maxx = minx = this ->iarray ::operator() (0, 0);
	maxy = miny = this ->iarray ::operator() (1, 0);
	for (int i = 0; i < l; i++){
		maxx = MAX (this ->iarray ::operator() (0, i), maxx);
		maxy = MAX (this ->iarray ::operator() (1, i), maxy);
		minx = MIN (this ->iarray ::operator() (0, i), minx);
		miny = MIN (this ->iarray ::operator() (1, i), miny);
	}
	pnt tl (minx, miny), br (maxx, maxy);
	rect r (tl, br);
	return r;
}

void  map ::onIndexes (iarray& ind)
{
	(*selected) = 0;
	for (int i = 0; i < ind .length (); i++)
		for (int j = 0; j < this ->size (); j++)
			if ((*indexes) (0,j) == ind (i))
				(*selected) (j) = 1;
	this ->envelope (1);
}
	

rect  map ::envelope (int recalculate)
{
// Calculates the maximal and minimal values of coordinates that
// are achieved in the !!selected!! objects.
	
	if (recalculate){

		int i = 0;
		while (!(*selected) (i)){
			i++;
			if (i == numberOfSegments)
				return view;
		}
		int maxx, maxy, minx, miny;
		minx = envelopes [i] .tl () .x ();
		miny = envelopes [i] .tl () .y ();
		maxx = envelopes [i] .br () .x ();
		maxy = envelopes [i] .br () .y ();

		for (int j = i; j < numberOfSegments; j++)
			if ((*selected)(j)){
				minx = MIN (envelopes [j] .tl () .x (), minx);
				maxx = MAX (envelopes [j] .br () .x (), maxx);
				miny = MIN (envelopes [j] .tl () .y (), miny);
				maxy = MAX (envelopes [j] .br () .y (), maxy);
			}
		pnt tl (minx, miny), br (maxx, maxy);
		view .tl () = tl;
		view .br () = br;
		currentView = view;
	}	
	return view;
}

void map ::read (char * name)
{
	FILE * in = fopen (name, "r");
	if (in == NULL){
		char  buf [100];
		sprintf (buf, "Can not open map file:%s", name);
		aerror (buf);
	}

	fscanf (in, "%d", &numberOfSegments);
	if (NULL == (segments = new line * [numberOfSegments]))
		aerror ("No memory");
	if (NULL == (envelopes = new rect [numberOfSegments]))
		aerror ("No memory");
	if (NULL == (selected = new carray (numberOfSegments)))
		aerror ("No memory");
	if (NULL == (indexes = new iarray (2, numberOfSegments)))
		aerror ("No memory");
	
	view .read (in);

	for (int i = 0; i < numberOfSegments; i++){
		int len;
		(*selected)(i) = 1;
		fscanf (in, "%d %d %d",
				  &((*indexes) (0, i)), &((*indexes) (1, i)), &len);
		segments [i] = new line (len);
		segments [i] ->read (in);
		envelopes [i] = segments [i] ->envelope ();
	}
	this ->envelope (1);
	currentView = view;
}


void map ::write (char * name)
{
// writes the !!selected!! object of the map
  	
	FILE * out = fopen (name, "w");
	if (out == NULL)
		aerror ("Can not open map file"); 

	fprintf (out, "%d\n\n", this ->visibleSize ());
	this ->envelope (1) .write (out);
	fprintf (out, "\n");

	for (int i = 0; i < numberOfSegments; i++)
		if ((*selected) (i)){
			fprintf (out, "%d %d\n%d\n", (*indexes) (0, i),
						(*indexes) (1, i), segments [i] ->length ());
			segments [i] ->write (out);
		}
}


typedef void (*gr) (int, int, short*, int pattern);

extern gr act [3];

void line ::graph (int col, enum action a, int pattern)
{
// General (draws points lines and fills) graphing for a line	
	int len = this ->length ();
	short * points = new short [len*2];
	for (int i = 0; i < len; i++){
		points [2*i] = (*this) (0, i); 
		points [2*i+1] = (*this) (1, i);
	}
   (act [a]) (col, len, points, pattern);
	delete points;
}

line	map ::scale (line& lin)
{
	int width, height;
   width = one .MapWidth;
	height = one .MapHeight;

	pnt dest (width, height);
	pnt origin = pnt () - currentView .tl ();
	pnt sizes = currentView .br ()  - currentView .tl ();

	double scaleX = (double) dest .x () / sizes .x ();
	double scaleY = (double) dest .y () / sizes .y ();
//	scaleY = MIN ((RATIO*scaleX), scaleY);
//	scaleX = IRATIO*scaleY;
 	return (lin + origin) .scale (scaleX, scaleY);
}
	
void map ::graph (enum action a,
						iarray * data, float subsample)
{
	for (int i = 0; i < numberOfSegments; i++)
		if ((*selected) (i))
			if ((envelopes [i]) .overlap (currentView)){
				// alternative:
				// (envelopes [i]) .inside (currentView)
				if (data == 0)
					transform (*(segments [i]), subsample)
						.graph (i, a);
				else{
					if ((*data) (i) >= 0)
						transform (*(segments [i]), subsample)
		 					.graph ((*data)(i), a);
					else{
//						fprintf (stderr, "%d\n", (*data)(i));
						transform (*(segments [i]), subsample)
		 					.graph (-(*data)(i), a, 1);
					}
				}
//			flush_graph ();
			}

//	flush_graph ();
}

line map ::transform (line& a, float subsample)
{
	int width, height;
   width = int(one .MapWidth*subsample+.5);
	height = int(one .MapHeight*subsample+.5);
	pnt dest (width, height);
	pnt upside = pnt (one .MapShiftX, one .height - one .MapShiftY);
	pnt origin = pnt () - currentView .tl ();
	pnt sizes = currentView .br ()  - currentView .tl ();

	double scaleX = (double) dest .x () / sizes .x (); 
	double scaleY = (double) dest .y () / sizes .y ();
//	printf ("\n%lf %lf-", scaleX, scaleY);
//	scaleY = MIN ((RATIO*scaleX), scaleY);
//	scaleX = IRATIO*scaleY;
//	printf ("%lf %lf\n", scaleX, scaleY);

	return ((a + origin) .scale (scaleX, -scaleY)+upside);
}

void map ::draw (iarray * data)
{
	this ->graph (DRAW, data);
}

void map ::fill (iarray * data, float subsample)
{
	this ->graph (FILL, data, subsample);
}


void map ::points (iarray * data)
{
	this ->graph (POINTS, data);
}


void grid ::show (int pcol, int rcol)
{
	line ps (pseudo ->length ()/2);
	line ra (random ->length ()/2);
	
	ps . iarray ::operator= (*pseudo);
	ra . iarray ::operator= (*random);


	(*m) .transform (ps) .points (pcol);
	(*m) .transform (ra) .points (rcol);
	pnt i10 (1, 0),  i01 (0, 1), i11 (1, 1);
	((*m) .transform (ra) + i10) .points (rcol);
	((*m) .transform (ra) + i01) .points (rcol);
	((*m) .transform (ra) + i11) .points (rcol);
	
//	flushGraph ();
}

void grid ::on (map& mm,
					 int horizontal, int vertical,
					 int nr, int nmin, int nmax)
{
	verp = vertical;
	horp = horizontal;
	int npseudo = verp * horp;
	m = &mm;	
	pseudo = new iarray (2, npseudo);
	pnt tl = mm .envelope () .tl (); 
	pnt br = mm .envelope () .br ();
	int x = tl .x (), y = tl .y ();
	int w = (br - tl) .x (), h = (br - tl) .y ();
	for (int i = 0; i < horp; i++)   
		for (int j = 0; j < verp; j++){   
			(*pseudo) (0, i*verp + j) = (int) (x + (float) i * w / (horp - 1.0));  
			(*pseudo) (1, i*verp + j) = (int) (y + (float) j * h / (verp - 1.0));
		}

	nrandom = mm .generateRandom (nr, nmin, nmax, &random, &indexes);
};

void grid ::read (FILE * in)
{
	if (3 != fscanf (in, "%d %d %d", &horp, &verp, &nrandom))
		aerror ("Can not read grid file");
	if (pseudo)
		delete pseudo;
	pseudo = new iarray (2, horp*verp);
	if (random)
		delete random;
	random = new iarray (2, nrandom);
	if (indexes)
		delete indexes;
	indexes = new iarray (nrandom);
	pseudo ->read (in);
	random ->read (in);
	indexes ->read (in);
//	indexes  ->write (stdout);
}


rect map ::rescaleMap (rect& range)
{
	rect domain = envelope (1);
	pnt Dsize = domain .br () - domain .tl ();
	pnt Rsize = range .br () - range .tl ();
	pnt Offset = pnt (0, 0) - domain .tl ();
	double x = double(Rsize .x ())/Dsize .x ();
	double y = double(Rsize .y ())/Dsize .y ();
	for (int i = 0; i < numberOfSegments; i++){
      line tmp = (*(segments [i]) + Offset) .scale (x, y) + range .tl ();     
		(*(segments [i])) = tmp;
		envelopes [i] = (*(segments [i])) .envelope ();
	}
	return envelope (1);
}


line line ::unique ()
{
// Making the same line without repeated points
	int l = length ();
	int * tmp = (int *) malloc (2*l*sizeof (int));
	int newL = 1;
	int x = iarray ::operator() (0, 0), y = iarray ::operator() (1, 0);
	tmp [0] = x; tmp [1] = y;
	for (int i = 1; i < l; i++){
		int newX = iarray ::operator() (0, i);
		int newY =  iarray ::operator() (1, i);
		if (newX != x || newY != y){
			tmp [2* newL] = newX;
			tmp [2* newL + 1] = newY;
			x = newX;
			y = newY;
			newL ++;
			
		}
	}
	line ans (newL);
	for (int i = 0; i < newL; i++){
		ans .iarray ::operator() (0, i)  =
			tmp [2*i];
		ans .iarray ::operator() (1, i)  =
			tmp [2*i + 1];
	}
	free (tmp);
	return ans;
}


void map ::unique ()
{
	for (int i = 0; i < numberOfSegments; i++){
		line tmp = (*(segments [i])) .unique ();
		delete segments [i];
		segments [i] = new line (tmp .length ());
		*(segments [i]) = tmp;
	}
}
