#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "malloc.h"
#include "util.h"
#include "data.h"
#include "anova.h"
#include "readMiasma.h"

void outputAnovas (char * name, Data info, TwoWay * anovas,
			  int binary = 1)
{
	char fName [1000];
	Data info1 = info;
	ddataArray res (info .NDis,info .NMonths,
						 info .NStates, info .Missing);
  	info1 .data = (void *) &res;	
	
// Effects //////////////////////////////////////////////	
	int i, k, j;
	for (i = 0; i < info .NDis; i++){
		TwoWay anova = anovas [i];
		for (k = 0; k < info .NMonths; k++)
			for (j = 0; j < info .NStates; j++){
				double tmp;
				if (anova .colEff ->operator() (k) != info .Missing &&
					 anova .rowEff ->operator() (j)  != info .Missing)
					tmp = anova .colEff ->operator() (k)
					+	anova .rowEff ->operator() (j);
				else
					tmp = info .Missing;
				res (i, k, j) = tmp;
			}
	}
	sprintf (fName, "%sEff", name);
	WriteMiasma (fName, info1, binary);

// Residuals	///////////////////////////////////
	for (i = 0; i < info .NDis; i++)
		for (int k = 0; k < info .NMonths; k++)
			for (int j = 0; j < info .NStates; j++)
				res (i, k, j) = anovas [i] .res ->operator() (k, j);
	sprintf (fName, "%sRes", name);
	WriteMiasma (fName, info1, binary);
}

main (int argc, char * argv [])
{
	int i, j, k;
	Data info;
	int binary = 1;
	switch (argc){
	case 3:
		binary = atoi (argv [2]);
	case 2:
		info = ReadMiasma (argv [1], binary);
		break;
	default:
		fprintf (stderr, "Usage: %s file [binary]\n", argv [0]); 
		exit (-1);
		break;
	}
	
	ddataArray * incidence = (ddataArray *) info .data;
	double miss = incidence ->miss ();
	TwoWay * ans = new TwoWay [info .NDis];
	for (i = 0; i < info .NDis; i++){
//		fprintf (stderr, "%le %le\n", (*incidence) .column (i) .min (),
//					(*incidence) .column (i) .max ());
		ans [i] =  twoWay ((*incidence) .column (i));
	}
	
	outputAnovas (argv [1], info, ans, binary);	
}
	
	
	











