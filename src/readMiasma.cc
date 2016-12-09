#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "data.h"

Data ReadMiasma (char * name, const int binary)
{
	Data ans;
	char fName [1000];
	FILE * in;
	sprintf (fName, "%s.desc", name);
	in = myfopen (fName, "r");	
	if (6 != fscanf (in, "%d %d %le %d %le %d",
						  &(ans .NDis),
						  &(ans .NMonths),
						  &(ans .YearFraction),
						  &(ans .NStates),
						  &(ans .Missing),
						  &(ans .startYear))){
		fprintf (stderr, "Can not read %s file\n", fName);
		exit (-1);
	}
	fprintf (stderr, "%d %d %d %d\n",
				ans .NDis, ans .NMonths, ans .NStates,
				ans .startYear);

	ddataArray * incidence;
	incidence = new ddataArray (ans .NDis, 
										 ans .NMonths,
										 ans .NStates,
										 ans .Missing);
	ans .data = (void *) incidence;
	ans .DisNames = new char * [ans .NDis];
	if (incidence == NULL  || ans .DisNames == NULL){
		fprintf (stderr, "Can not allocate two %d size arrays\n",
					ans .NDis *
					ans .NMonths * ans .NStates);
		exit (-1);
	}
	for (int i = 0; i < ans .NDis; i++){
		char buf [1000];
		if (1 != fscanf (in, "%s", buf)){
			fprintf (stderr, "Can not read name %d\n", i);
			exit (-1);
		}
		ans .DisNames [i] = new char [strlen (buf) + 1];
		strcpy (ans .DisNames [i], buf);
	}
	fclose (in);

	if (binary)
		sprintf (fName, "%s.bin", name);
	else	
		sprintf (fName, "%s.asc", name);
	in = myfopen (fName, "r");


	double * sEff = new double [ ans .NStates ];
	int * nsEff = new int [ ans .NStates ];
	for (int i = 0; i < ans .NDis; i++){
		double tmp;
		for (int j = 0; j < ans .NStates; j++){
			nsEff [j] = 0;
			sEff [j] = 0;
		}
		for (int k = 0; k < ans .NMonths; k++)
			for (int j = 0; j < ans .NStates; j++){
				if (binary)
					fread (&tmp,
							 sizeof (tmp), 1, in);
				else
					fscanf (in, "%le", &tmp); 
				(*incidence) (i, k, j) = tmp;
				if (tmp != ans .Missing){
					sEff [j] += tmp;
					nsEff [j] ++;
				}
			}
//	  	for (j = 0; j < ans .NStates; j++){
//			fprintf (stderr, "%le ", sEff [j]/nsEff [j]);
//		}
//		fprintf (stderr, "\n");
	}
	delete nsEff;
	delete sEff;
	fclose (in);
	return ans;
}

void WriteMiasma (char * name, Data& info, int binary)
{
	char fName [1000];
	FILE * in;
	sprintf (fName, "%s.desc", name);
	in = myfopen (fName, "w");	
	fprintf (in, "%d\n%d\n%le\n%d\n%le\n%d\n",
				(info .NDis),
				(info .NMonths),
				(info .YearFraction),
				(info .NStates),
				(info .Missing),
				(info .startYear));
	for (int i = 0; i < info .NDis; i++)
		fprintf (in, "%s\n", info .DisNames [i]);
	fprintf (in, "\n");
	fclose (in);
	
	if (binary)
		sprintf (fName, "%s.bin", name);
	else
		sprintf (fName, "%s.asc", name);
	in = myfopen (fName, "w");
	int j, k;
	for (int i = 0; i < info .NDis; i++)
		for (k = 0; k < info .NMonths; k++){
			for (j = 0; j < info .NStates; j++){
				double tmp = ((ddataArray *)info .data)
					->operator() (i, k, j);
				if (binary)
					fwrite (&tmp, 1, sizeof (tmp), in);
				else
					fprintf (in, "%le ",  tmp);
			}
			if (!binary)
				fprintf (in, "\n");
		}
	fclose (in);
}













