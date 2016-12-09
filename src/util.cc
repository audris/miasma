#include <errno.h>
#include <stdlib.h>

#include "util.h"

#define el short
#define array sarray
#define dataArray sdataArray
#define FORMAT "%d "
#include "util.gen"
#undef FORMAT
#undef array
#undef dataArray
#undef el	

#define el int
#define array iarray
#define dataArray idataArray
#define FORMAT "%d "
#include "util.gen"
#undef FORMAT
#undef array
#undef dataArray
#undef el	

#define el char
#define array carray
#define dataArray cdataArray
#define FORMAT "%c "
#include "util.gen"
#undef FORMAT
#undef array
#undef dataArray
#undef el	

#define el float
#define array farray
#define dataArray fdataArray
#define FORMAT "%f "
#include "util.gen"
#undef FORMAT
#undef array
#undef dataArray
#undef el

#define el double
#define array darray
#define dataArray ddataArray
#define FORMAT "%lf "
#include "util.gen"
#undef FORMAT
#undef array
#undef dataArray
#undef el	

extern "C" void aerror (char * text)
{
 /* My subroutine for handling errors */
 /* 	void exit (int status); */
	fprintf (stderr, "Error: %s\n", text);
	exit (1);
}

extern "C" FILE *  myfopen (char * fName, char * mode)
{
	FILE * ans = fopen (fName, mode);
	if (ans == NULL){
		fprintf (stderr, "Can not open file %s in mode %s\n",
					fName, mode);
		exit (-1);
	}
	return ans;
}

