#include <stdio.h>
#include "arr.h"

array<double> y (0);

template<class el> void array<el> ::write (FILE * out)
{
	for (int k = 0; k < size [2]; k++){
		for (int j = 0; j < size [1]; j++){
			for (int i = 0; i < size [0]; i++)
				fprintf (out, "%lf", (*this)(i,j,k));
			fprintf (out, "\n");
		}
		fprintf (out, "\n");
	}
}

template<class el> array<el> ::array (unsigned int i)
{
	size [0] = i;
	size [1] = 1;
	size [2] = 1;
	dim = 1;
	if ((values  = new el [i]) == NULL)
		;
}
