#ifndef MAP_H
#define MAP_H

#include "util.h"
	
class pnt {
private:
	int xx, yy;

public:
	pnt (void) {xx = 0; yy = 0;};
	pnt (int x, int y) {xx = x; yy = y;};
	pnt (const pnt& p) {(*this) = p;};
	~pnt (void) {};

	
	pnt& operator= (const pnt& p) {xx = p .xx; yy = p .yy; return *this;}; 
	pnt operator* (double p) {pnt an; an .xx = (int) (xx * p);
									an .yy = (int) (yy * p); return an;};
	pnt operator/ (double p) {pnt an; an .xx = (int) (xx / p);
									an .yy = (int) (yy / p); return an;};
	pnt operator+ (double p) {pnt an; an .xx = (int) (xx + p);
									an .yy = (int) (yy + p); return an;};
	pnt operator- (double p) {pnt an; an .xx = (int) (xx - p);
									an .yy = (int) (yy - p); return an;};

	pnt operator* (pnt& p) {pnt an; an .xx = xx * p .xx;
									an .yy = yy * p .yy; return an;};
	pnt operator/ (pnt& p) {pnt an; an .xx = xx / p .xx;
									an .yy = yy / p .yy; return an;};
	pnt operator+ (pnt& p) {pnt an; an .xx = xx + p .xx;
									an .yy = yy + p .yy; return an;};
	pnt operator- (pnt& p) {pnt an; an .xx = xx - p .xx;
									an .yy = yy - p .yy; return an;}; 

	int operator> (pnt& p) {return (xx >= p .xx) && (yy >= p .yy);};
	int operator< (pnt& p) {return (xx <= p .xx) && (yy <= p .yy);};

	int& x (void) {return xx;};
	int& y (void) {return yy;};

	void write (FILE * out = stdout) {//printf ("point:");
												 fprintf (out, "%d %d\n", xx, yy);};
	void read (FILE * in = stdin) {fscanf (in, "%d %d\n", &xx, &yy);};
};

class rect {
private:
	pnt  topl, botr;

public:
	rect (void) {;};
	rect (pnt tl, pnt br) {if (!(tl < br))
		         aerror ("Rectangle top left corner should be smaller");
										topl = tl; botr = br;};
	rect (const rect& r) {topl = r .topl;  botr = r .botr;};
	~rect (void) {};
	
	rect& operator= (const rect& p) {topl = p .topl; botr = p .botr; return *this;}; 

	int inside (rect& p) {return (topl > p .topl) && (botr < p .botr);}; 
	int contains (rect& p) {return (topl < p .topl) && (botr > p .botr);}; 
	int contains (pnt& p) {return (topl < p) && (botr > p);};
	int overlap (rect& p);
	
	pnt& tl (void) {return topl;};
	pnt& br (void) {return botr;};
	int area (void) {return (botr - topl) .x () * (botr - topl) .y ();};

	rect operator* (pnt& p) {return rect (topl * p, botr * p);};
	rect operator/ (pnt& p) {return rect (topl / p, botr / p);};
	rect operator+ (pnt& p) {return rect (topl + p, botr + p);};
	rect operator- (pnt& p) {return rect (topl - p, botr - p);};
	
	void write (FILE * out = stdout) {//printf ("rectangle:\n");
							 topl .write (out); botr .write (out);};
	void read (FILE * in = stdin) {topl .read (in); botr .read (in);};
};


enum action {POINTS, DRAW, FILL};

class line : public iarray {
	void graph (int color = 1, enum action a = FILL, int pattern = 0);
friend class map;
	
public:
	line (int length) : iarray (2, length) {};
	line (const line& l) : iarray (l) {};
	~line (void) {};

	int length (void) {return this ->iarray ::length ()/2;};

	line operator+ (pnt p); 
	line scale (double x, double y); 
	rect envelope (void);
	line unique (); //return the same line without repeated points 
	
	void draw (int col) {this ->graph (col, DRAW);};
	void fill (int col, int pattern = 0) {this ->graph (col, FILL,
																	pattern);};
	void points (int col) {this ->graph (col, POINTS);};
};

class map {
private:
	line ** segments; // closed or not line segments that constitute a map 
	rect * envelopes;  // envelopes for each line segment
	iarray * indexes;  // two indexes for each line segment
	carray * selected; // indicator which segments are selected
	int 	numberOfSegments;
	rect	view,        // the envelope for the whole map
			currentView; // the part that is being displayed
				
	void  graph (enum action a = FILL,
					 iarray * data = 0, float subsample = 1);
	line	scale (line& lin);
friend class grid;
	int   generateRandom (int averageNumber, int minNumber, int maxNumber,
								 iarray ** r, iarray ** i);
	// returns the size of the random points array.
public:
	map (int n, line ** s, rect * env,
		   iarray * ind, carray * sel) {
		numberOfSegments = n;
		segments = s;
		envelopes = env;
		indexes = ind;
		selected = sel;
	}

	map (void) {numberOfSegments = 0; segments=0; envelopes=0; indexes=0;
						selected = 0;};
//	map (map& m) {};
	~map (void);

	map& operator= (const map& m);
	
	rect  envelope (int recalculate = 0);
	rect& zoom   (void) {return currentView;};
	pnt&  offset (void) {return currentView .tl ();};
	int   size   (void) {return numberOfSegments;};
	int   visibleSize  (void) {return (int)(selected ->sum ());};

	iarray *  getIndexes (void) {return indexes;};
	void  onIndexes (iarray& i); // select by state icpsr indexes
	void  on (carray * select) {delete selected; selected = select;
										 this ->envelope (1);};
			// select only objects indicated by select. 		

	void read (char * name);
	void write (char * name); // writes only selected objects
	
	void draw (iarray * data = 0);
	void points (iarray * data = 0);
	void fill (iarray * data = 0, float subsample = 1);
	void fillSmooth (int horizontal, int vertical, iarray pseudo,
						  iarray data, float  subsample = 1.0f);

	void interpolateFill (int times, int missing,
								 iarray& dataA,
								 iarray& dataB);
	void interpolateSmooth (int& times, int horizontal, int vertical, iarray& pseudo,	iarray dataA, iarray dataB, float subsample = 1);
	line * segment (int which) { return segments[which]; };
	line transform (line& a, float subsample = 1);
	               // transfor an arbitrary line in map coordinates
		            // to a line in map window coordinates
	rect rescaleMap (rect& range); // rescale the map to fit
		// into rectangle range									  
	void unique (); //make the map lines without repeated points
};

class grid
{
	iarray * pseudo;
	iarray * random;
	iarray * indexes;
	// array of indexes for each random point as in the map m. 
	int horp, verp, nrandom;
	map * m;
public:
	grid (map& mm) {m = &mm; pseudo = random = 0;
						indexes = 0; horp = verp = nrandom = 0;};
	~grid (void) {if (pseudo) delete pseudo;
					  if (random) delete random;
					  if (indexes) delete indexes;
				  };

	iarray& p (void) {return *pseudo;};
	iarray& r (void) {return *random;};
	iarray& i (void) {return *indexes;};

	int horizontal (void) {return horp;};
	int vertical (void) {return verp;};
	
	void on (map& mm, int horp = 10, int verp = 10, int nr = 1, int nmin = 1,
				int nmax = 1);
	void show (int pcol = 5, int rcol = 0);
	
	void write (FILE * out = stdout){fprintf (out, "%d %d %d\n\n",
										 horp, verp, nrandom);
										 pseudo ->write (out); fprintf (out, "\n");
										 random ->write (out); fprintf (out, "\n");
										 indexes ->write (out);};
	
	void read (FILE * in = stdin);
};
#endif








