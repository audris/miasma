#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

extern "C" void aerror (char * c);
extern "C" FILE * myfopen (char * f, char * m);
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((b) > (a) ? (b) : (a))
	
#define el short
#define array sarray
#define dataArray sdataArray
#include "util.gen.h"
#undef array
#undef dataArray
#undef el	

#define el int
#define array iarray
#define dataArray idataArray
#include "util.gen.h"
#undef array
#undef dataArray
#undef el	

#define el char
#define array carray
#define dataArray cdataArray
#include "util.gen.h"
#undef array
#undef dataArray
#undef el	

#define el float
#define array farray
#define dataArray fdataArray
#include "util.gen.h"
#undef dataArray
#undef array
#undef el

#define el double
#define array darray
#define dataArray ddataArray
#include "util.gen.h"
#undef array
#undef dataArray
#undef el	

#endif



