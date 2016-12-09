#ifndef CODES_H
#define CODES_H

#ifdef __cplusplus

#include "util.h"

enum {icpsrS=2, icpsrC=3, fipsS=0, fipsC=1};

class codes {
	char ** names;
	iarray * icpsrToMyCode, * fipsToMyCode;
	int theSize;
	int cod [4][4000];
	
public:
	codes (char * fileName);	
	~codes (void);

	char ** Names (void) { return names; };
	int size () {return theSize;};
	int fromFips (int i, int j=0);
	int fromIcpsr (int i, int j=0);
	int toFips (int i)
		{if (i < theSize) return cod [fipsS][i]; else return -1;};
	int toIcpsr (int i)
		{if (i < theSize) return cod [icpsrS][i]; else return -1;};
	int toFipsC (int i)
		{if (i < theSize) return cod [fipsC][i]; else return -1;};
	int toIcpsrC (int i)
		{if (i < theSize) return cod [icpsrC][i]; else return -1;};
};

#endif
#endif



