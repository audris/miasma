#include <stdio.h>
#include <stdlib.h>
#include <float.h>

extern "C" {
	double exp (double);
	double sqrt (double);
};

#include "smooth.h"


smooth ::smooth (iarray* p, iarray* r,
			  iarray* ii, farray* w, float l)
{
	pseudo  = p;
	random  = r;
	index   = ii;
	weights = w;
	lambda  = l;


	int x = (*pseudo) .length() / 2;
	int y = (*random) .length() / 2;
	dist  = new darray (x, y);
	
	for (int i = 0; i < x; i++)
		for (int j = 0; j < y; j++){
			(*dist) (i, j) = (double) exp (- lambda * (
									 ((*random) (0, j) - (*pseudo) (0, i)) *
									 ((*random) (0, j) - (*pseudo) (0, i))  +
									 ((*random) (1, j) - (*pseudo) (1, i)) *
										 ((*random) (1, j) - (*pseudo) (1, i)))); 
		}
	
//	(*index) .write ();	
//	(*dist) .write ();
}

void smooth ::changeLambda (float l)
{
	lambda = l;
	int x = (*pseudo) .length() / 2;
	int y = (*random) .length() / 2;
	
	for (int i = 0; i < x; i++)
		for (int j = 0; j < y; j++){
			(*dist) (i, j) =  (double) exp (- lambda * (
									 ((*random) (0, j) - (*pseudo) (0, i)) *
									 ((*random) (0, j) - (*pseudo) (0, i))  +
									 ((*random) (1, j) - (*pseudo) (1, i)) *
										 ((*random) (1, j) - (*pseudo) (1, i))));
		}
}

iarray smooth ::on (iarray& data, int missing)
{
	int x = (*pseudo) .length() / 2;
	int y = (*random) .length() / 2;
	iarray answer (x);

	for (int i = 0; i < x; i++){
		double num = 0, den = 0;
		int nonmis = 0;
		for (int j = 0; j < y; j++)
			if (data ((*index)(j)) != missing){
				num += (*dist) (i,j) *
							data ((*index)(j)) * (*weights) ((*index) (j));
				den += (*dist) (i,j) * (*weights) ((*index) (j));
				nonmis ++;
			}
		if (nonmis && den > DBL_MIN)
			answer (i) = (int) (num / den);
		else
			answer (i) = 0;
//		else{
//			for (int j = 0; j < y; j++)
//				if (data ((*index)(j)) != missing){
//					fprintf (stderr, "%d %d %d %le %d %le\n", i, j, (*index) (j),
//								(*dist) (i,j),
//								data ((*index)(j)), (*weights) ((*index) (j)));
//			}
//			answer (i) = 0;
//		}
	}
	return answer;
}

