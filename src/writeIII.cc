#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "util.h"
#include "writeIII.h"

void writeIII (int permute, char * name, int X, int Y, int Z, 
			 ddataArray * inc)
{
	double miss = inc ->miss ();
//	inc ->rescale (0, 1);
	char str [500];
	sprintf (str, "%s.plot", name);
	FILE * fP = fopen (str, "w");
	sprintf (str, "%s.desc", name);
	FILE * fD = fopen (str, "w");

	int i, j, k, iWid, iHei;
	switch (permute){
	case 0: // x, y, z
		iWid = MAX (1, 500/X);
		iHei = MAX (1, 500/Y);
		for (i = 0; i < X; i++)
			for (j = 0; j < Y; j++)
				for (k = 0; k < Z; k++){
					double tmp = (*inc) (i, j, k);
					float out = 0;
					if (tmp != miss)
						out = tmp;
					fwrite (&out, sizeof (out), 1, fP);
				}
			fprintf (fD, "%d\n%d\n%d %d\n%d %d\n%d\n",
					X*Y, // Number of Icons
				   Z,   // Length of time series 
					iWid,  // Icon Width
					iHei,  // Icon Height
					X,   // CS Width
					Y,   // SC Height
					0
					);
		break;
	case 1: // x, z, y
		iWid = MIN(MAX (1, 500/X), Y);
		iHei = MAX (1, 500/Z);
		for (i = 0; i < X; i++)
			for (j = 0; j < Z; j++)
				for (k = 0; k < Y; k++){
					double tmp = (*inc) (i, k, j);
					float out = 0;
					if (tmp != miss)
						out = tmp;
					fwrite (&out, sizeof (out), 1, fP);
				}
			fprintf (fD, "%d\n%d\n%d %d\n%d %d\n%d\n",
					X*Z, // Number of Icons
				   Y,   // Length of time series 
					iWid,  // Icon Width
					iHei,  // Icon Height
					X,   // CS Width
					Z,   // SC Height
					0
					);
		break;
	case 2: // y, z, x
		iWid = MIN(MAX (1, 500/Y), X);
		iHei = MAX (1, 500/Z);
		for (i = 0; i < Y; i++)
			for (j = 0; j < Z; j++)
				for (k = 0; k < X; k++){
					double tmp = (*inc) (k, i, j);
					float out = 0;
					if (tmp != miss)
						out = tmp;
					fwrite (&out, sizeof (out), 1, fP);
				}
			fprintf (fD, "%d\n%d\n%d %d\n%d %d\n%d\n",
					Z*Y, // Number of Icons
				   X,   // Length of time series 
					iWid,  // Icon Width
					iHei,  // Icon Height
					Y,   // CS Width
					Z,   // SC Height
					0
					);
		break;
	}	
	fclose (fP);
	fclose (fD);
}		








