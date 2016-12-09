#ifndef ANOVA_H
#define ANOVA_H

#include "util.h"

typedef struct {
	ddataArray * rowEff, * colEff,
	* rowLqr, * colLqr,* rowUqr, * colUqr,
	* res;
} TwoWay;

typedef struct {
	ddataArray * eff, * lqr, * uqr, * res;
	double constant;
} OneWay;


OneWay  oneWay (ddataArray& a, int doRow=1);
TwoWay  twoWay (const ddataArray& a);

#endif
