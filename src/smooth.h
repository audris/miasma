#ifndef SMOOTH_H
#define SMOOTH_H

#include "util.h"

class smooth {
private:
	iarray *data;   // data is an array of values for each object
	iarray *pseudo; // pseudo is an array x points (0, i) - x'es (1, i) - y's
	iarray *random; // random is an array y points (0, i) - x'es (1, i) - y's
	iarray *index;  // index is an array of y indexes indicating the
						 //   region number for i'th point in in random. 
	farray *weights;// weights is an array of weights for each region
	float   lambda; // lambda is a smoothing parameter
	darray *dist;	 // dist is an array of distances between points in
						 //   random and pseudo.
public:
	smooth (iarray* pseudo, iarray* random,
			  iarray* index, farray* weights, float lambda);

	smooth (smooth& s){data=s .data; pseudo=s .pseudo; random=s .random;
							 index = s .index; weights = s .weights;
							 lambda = s .lambda; dist = s .dist;};
	~smooth (void){delete dist;};

	iarray on (iarray& data, int missing);
			// returns array of values corresponding to
	      // points in the array pseudo.
	void changeLambda (float l);
};

#endif
