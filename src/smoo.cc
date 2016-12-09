#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "malloc.h"
#include "util.h"
#include "data.h"
#include "anova.h"
#include "readMiasma.h"

main (int argc, char * argv [])
{
	int i, j, k, l;
	Data info;
	int binary = 1;
	int window = 0;
	char action;
	switch (argc){
	case 5:
		binary = atoi (argv [4]);
	case 4:
		window = atoi (argv [1]);
		action = argv [2][0];
		info = ReadMiasma (argv [3], binary);
		break;
	case 1:
	case 2:
	default:
		fprintf (stderr,
"Usage %s binWid action filePrefix [binary]\n",
					argv [0]);
		exit (-1);
		break;
	}
	ddataArray * inc = (ddataArray *) info .data;
	double miss = inc ->miss ();
	ddataArray results (info .NDis, info .NMonths,
							  info .NStates, miss);
	for (i = 0; i < info .NDis; i++)
		for (j = 0; j < info .NMonths; j++)
			for (k = 0; k < info .NStates; k++){
				int a = MAX (0, j - window); 
				int b = MIN (info .NMonths - 1, j + window);
				ddataArray tmp (b - a + 1, 1, 1, miss);
				for (l = a; l <= b; l++)
					tmp (l - a) = (*inc) (i, l, k);
				double xx;
				switch (action){
				case 'a':
					xx = tmp .mean ();
					break;
				case 'm':
					xx = tmp .median ();
					break;
				}
				results (i, j, k) = xx; 
		}
	(*inc) = results;
	char buf [1000];
	sprintf (buf, "%s.%d.%c", argv [3], window, action);
	WriteMiasma (buf, info, binary);	
}
	
	
	











