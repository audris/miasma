#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "malloc.h"
#include "util.h"
#include "data.h"
#include "readMiasma.h"


void AggBounds (int direction, int periodic, int binWid,
					int& dA,	int& dB, int& dC)
{
	switch (direction){
	case 0:
		if (periodic)
			dA = binWid;
		else{
			if (dA%binWid)
				dA = dA/binWid + 1;
			else
				dA = dA/binWid;
		}
		break;
	case 1:
		if (periodic)
			dB = binWid;
		else{
			if (dB%binWid)
				dB = dB/binWid + 1;
			else
				dB = dB/binWid;
		}
		break;
	case 2:
		if (periodic)
			dC = binWid;
		else{
			if (dC%binWid)
				dC = dC/binWid + 1;
			else
				dC = dC/binWid;
		}
		break;
	default:
		exit (-1);
	}
}

void AggIndex (int direction, int periodic, int binWid,
					int& dA,	int& dB, int& dC)
{
	switch (direction){
	case 0:
		if (periodic)
			dA %= binWid;
		else
			dA /= binWid;
		break;
	case 1:
		if (periodic)
			dB %= binWid;
		else
			dB /= binWid;
		break;
	case 2:
		if (periodic)
			dC %= binWid;
		else
			dC /= binWid;
		break;
	default:
		exit (-1);
	}
}


main (int argc, char * argv [])
{
	int i, j, k, l;
	Data info;
	int binary = 1;
	int binWid = 0, direction = 1, periodic = 0;
	char action;
	switch (argc){
	case 7:
		binary = atoi (argv [6]);
	case 6:
		periodic = atoi (argv [5]);
	case 5:
		direction = atoi (argv [4]);
	case 4:
		binWid = atoi (argv [1]);
		action = argv [2][0];
		info = ReadMiasma (argv [3], binary);
		break;
	case 1:
	case 2:
	case 3:
	default:
		fprintf (stderr,
"Usage %s binWid action filePrefix [direction] [periodic] [binary]\n",
					argv [0]);
		exit (-1);
		break;
	}
	ddataArray * inc = (ddataArray *) info .data;
	double miss = inc ->miss ();

	int dA = info .NDis, dB = info .NMonths, dC = info .NStates;
	AggBounds (direction, periodic, binWid, dA, dB, dC);
	ddataArray results (dA, dB, dC, miss);
	fprintf (stderr, "%d %d %d\n", dA, dB, dC);

	for (i = 0; i < dA; i++)
		for (j = 0; j < dB; j++)
			for (k = 0; k < dC; k++)
				results (i, j, k) = miss;

	for (i = 0; i < info .NDis; i++)
		for (j = 0; j < info .NMonths; j++)
			for (k = 0; k < info .NStates; k++){
				double xx = (*inc) (i, j, k);
				int ii = i, jj = j, kk = k;
				AggIndex (direction, periodic, binWid, ii, jj, kk);
				if (xx != miss){					
					double yy = results (ii, jj, kk);
					if (yy != miss){
						switch (action){
						case 's':/* sum */
							results (ii, jj, kk) = yy + xx;
							break;
						case 'x':/* max */
							results (ii, jj, kk) = MAX (yy, xx);
							break;
						case 'n':/* min */
							results (ii, jj, kk) = MIN (yy, xx);
							break;
						default:
							fprintf (stderr,
										"Unknown aggregation %c\n",
										action);
							exit (-1);
							break;
						}
					}else
						results (ii, jj, kk) = xx;
				}
			}
	
	info .NDis = dA;
	info .NMonths = dB;
	info .NStates = dC;
	if (direction == 1){
		if (!periodic)
			info .YearFraction *= binWid;	
	}
	info .data = &results;
	char buf [1000];
	sprintf (buf, "%s.%d.%c.%d.%d", argv [3], binWid, action,
				direction, periodic);
	WriteMiasma (buf, info, binary);
	exit (0);
}
	












