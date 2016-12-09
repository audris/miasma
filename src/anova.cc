#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "anova.h"



OneWay  oneWay (ddataArray& a, int doRow)
{
	OneWay  ans;
	int w = a .width (), h = a .height ();
	double miss = a .miss ();
	int lenght = (doRow) ? h : w;
	ans .res = new ddataArray (a);
	ans .eff = new ddataArray (lenght, 1, 1, miss);
	ans .lqr = new ddataArray (lenght, 1, 1, miss);
	ans .uqr = new ddataArray (lenght, 1, 1, miss);
	ddataArray	e (lenght, 1, 1, miss);
	(*(ans .eff)) .darray ::operator=(0.0);

	for (int j = 0; j < lenght; j++){
		if (doRow)
			e (j) = ((*(ans .res)) .row (j) .median ());
		else
			e (j) = ((*(ans .res)) .column (j) .median ());
		if ((*(ans .eff)) (j) != miss && e (j) != miss)
			(*(ans .eff)) (j) += e (j);
		else
			(*(ans .eff)) (j) = miss;
	}
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			if ((*(ans .res)) (j, i) != miss)
				(*(ans .res)) (j, i) -= (doRow) ? e (i) : e (j);

	for (int i = 0; i < lenght; i++){
		double x [2];
		x [0] = .25, x[1] = .75;
		if (doRow)
			a .row (i) .quantile (2, x);
		else
			a .column (i) .quantile (2, x);
		if (x[0] != miss){
			(*(ans .lqr)) (i) = x [0];
			(*(ans .uqr)) (i) = x [1];
		}else{
			(*(ans .lqr)) (i) = miss;
			(*(ans .uqr)) (i) = miss;
		}
	}
	return ans;
}


TwoWay  twoWay (const ddataArray& a)
{
	TwoWay  ans;
	int w = a .width (), h = a .height ();
	double miss = a .miss ();
	ans .res = new ddataArray (a);

	ans .colEff = new ddataArray (w, 1, 1, miss);
	ans .rowEff = new ddataArray  (h, 1, 1, miss);
	ans .colLqr = new ddataArray (w, 1, 1, miss);
	ans .rowLqr = new ddataArray  (h, 1, 1, miss);
	ans .colUqr = new ddataArray (w, 1, 1, miss);
	ans .rowUqr = new ddataArray  (h, 1, 1, miss);
	ddataArray	e1 (w, 1, 1, miss), e2 (h, 1, 1, miss);

	e1 .darray ::operator= (0.0);
	e2 .darray ::operator= (0.0);
	(*(ans .colEff)) .darray ::operator= (0.0);
	(*(ans .rowEff)) .darray ::operator= (0.0);

	int i,j, iter = 0;
	double prec = sqrt ((a * a) .sum ()/h/w) * 1e-5;
	do {
		iter++;
		for (j = 0; j < h; j++){
			e2 (j) = ((*(ans .res)) .row (j) .median ());
			if (e2 (j) != miss && (*(ans .rowEff)) (j) != miss)
				(*(ans .rowEff)) (j) += e2 (j);
			else
				(*(ans .rowEff)) (j) = miss;
		}
		for (i = 0; i < h; i++)
				for (j = 0; j < w; j++)
					if ((*(ans .res)) (j, i) != miss)
						(*(ans .res)) (j, i) -= e2 (i);

		for (i = 0; i < w; i++){
			e1 (i) = (*(ans .res)) .column (i) .median ();
			if (e1 (i) != miss && (*(ans .colEff)) (j) != miss)
				(*(ans .colEff)) (i) += e1 (i);
			else
				(*(ans .colEff)) (j) = miss;
		}
		for (i = 0; i < h; i++)
				for (j = 0; j < w; j++)
					if ((*(ans .res)) (j, i) != miss)
						(*(ans .res)) (j, i) -= e1 (j);
		
//		fprintf (stderr, "%d %le %le %le ----\n",
//					iter, e1 .sum (), e2 .sum (), prec);
		if (iter > 1000){
			fprintf (stderr, "%le, %le\n", e1 .max (),
						e1 .min ());
			e1 .write (stderr);
			e2 .write (stderr);
//			ans .colEff .write (stderr);
//			ans .rowEff .write (stderr);
//			(*(ans .res)) .write (stderr);
			exit (-1);
		}
	} while (fabs (e1 .sum () + e2 .sum ()) > prec);
	
	for (int i = 0; i < w; i++){
		double x [2];
		x [0] = .25, x[1] = .75;
		ans .res ->column (i) .quantile (2, x);
		if (x[0] != miss){
			(*(ans .colLqr)) (i) = x [0];
			(*(ans .colUqr)) (i) = x [1];
		}else{
			(*(ans .colLqr)) (i) = miss;
			(*(ans .colUqr)) (i) = miss;
		}
	}
	for (j = 0; j < h; j++){
		double x [2];
		x [0] = .25, x[1] = .75;
		ans .res ->row (j) .quantile (2, x);
		if (x[0] != miss){
			(*(ans .rowLqr)) (j) = x[0];
			(*(ans .rowUqr)) (j) = x[1];
		}else{
			(*(ans .rowLqr)) (j) = miss;
			(*(ans .rowUqr)) (j) = miss;
		}
	}
	return ans;
}






