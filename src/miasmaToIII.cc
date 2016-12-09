#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "util.h"
#include "data.h"
#include "readMiasma.h"
#include "writeIII.h"



main (int argc, char * argv [])
{
	int i, j, k, l;
	Data info;
	int binary = 1;
	switch (argc){
	case 3:
		binary = atoi (argv [2]);
	case 2:
		info = ReadMiasma (argv [1], binary);
		break;
	default:
		fprintf (stderr, "Usage: %s filePrefix [binary]\n",
					argv [0]);
		exit (-1);
		break;
	}
	ddataArray * inc = (ddataArray *) info .data;
	double miss = inc ->miss ();

	char buf [500];
	sprintf (buf, "%s.iii", argv [1]);
	writeIII (XZY, buf,
				 info .NDis,
				 info .NMonths,
				 info .NStates,
				 inc);	
}
	








