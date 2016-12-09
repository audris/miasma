#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codes.h"

int codes ::fromIcpsr (int i, int j)
{
	int k;
	if (i >= (*icpsrToMyCode) .length ())
		k = -1;
	else
		k = (*icpsrToMyCode) (i);
	
	while (k >= 0 && cod [icpsrC][k--] != j )
		;
	if (cod [icpsrC][k+1] == j && cod [icpsrS][k+1] == i)
		return k+1;
	else{
		fprintf (stderr, "Unknown icpsr code %d, %d\n", i, j);
		exit (-1);
	}
}

int codes ::fromFips (int i, int j)
{
	int k;
	if (i >= (*fipsToMyCode) .length ())
		k = -1;
	else
		k = (*fipsToMyCode) (i);
	
	while (k >= 0 && cod [fipsC][k--] != j)
		;
	if (cod [fipsC][k+1] == j && cod [fipsS][k+1] == i)
		return k+1;
	else{
		fprintf (stderr, "Unknown fips code %d, %d\n", i, j);
		exit (-1);
	}
}


codes ::~codes (void)
{
	delete icpsrToMyCode;
	delete fipsToMyCode;
	for (int i = 0; i < theSize; i++)
		delete names [i];
	delete names;
}

codes ::codes (char * fileName)
{
		
		FILE * c = myfopen (fileName, "r");
//		if (c == NULL)
//			aerror ("Can not open code file %s\n", fileName);
	
		while (fgetc (c) != '\n');
		int i = 0;
		char buf [100], buf1 [100];
		int maxF = 0, maxI = 0;
		int stId, stCounter = 0, ctCounter = 0;;
		while (!feof (c)){
			int trash;
			if (7 != fscanf (c, "%d %d %d %d %d %s %s", &trash, 
								  &(cod [fipsS][i]),
								  &(cod [fipsC][i]),
								  &(cod [icpsrS][i]),
								  &(cod [icpsrC][i]),
								  buf, buf1))
				break;
			maxI = MAX (maxI, cod [icpsrS][i] + 1);
			maxF = MAX (maxF, cod [fipsS][i] + 1);
			i++;
		}
		theSize = i;
		fipsToMyCode = new iarray (maxF);
		icpsrToMyCode = new iarray  (maxI);
		names = new char * [theSize];
		fseek (c, 0, SEEK_SET);
		while (fgetc (c) != '\n')
			;
		for (i = 0; i < theSize; i++){
			int trash;
			fscanf (c, "%d %d %d %d %d %s %s", &trash, 
								  &trash,
								  &trash,
								  &trash,
								  &trash,
					  buf, buf1);
			names [i] = new char [strlen (buf)+1];
			strcpy (names [i], buf);
		}							
		for (int j = 0; j < maxF; j++)
			(*fipsToMyCode) (j) = -1;
		for (int j = 0; j < maxI; j++)
			(*icpsrToMyCode) (j) = -1;
		
		stId = -1; stCounter=0;
		for (int j = 0; j < i; j++){
			if (stId == -1){
				stId = cod [fipsS][0];
			}
			if (stId != cod [fipsS][j]){
				(*fipsToMyCode) (cod [fipsS][j-1]) = j-1;
				(*icpsrToMyCode) (cod [icpsrS][j-1]) = j-1;
				stId = cod [fipsS][j];
				stCounter++;
			}
  		}
		(*fipsToMyCode) (cod [fipsS][i-1]) = i-1;
		(*icpsrToMyCode) (cod [icpsrS][i-1]) = i-1;		

		fclose (c);
}











