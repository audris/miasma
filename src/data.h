#ifndef DATA_H
#define DATA_H

typedef struct {
	int Disease, Month;
	int NDis, NMonths, NStates;
	double YearFraction; /* what fraction of a year
									are observations apart*/
	double Missing;
	int startYear;
	char ** DisNames;
	char ** StateNames;
	void * data;
} Data;

#ifdef __cplusplus
extern "C" {
#endif
	
	int year (Data * dat);
	int month (Data * dat);

#ifdef __cplusplus
}
#endif

#endif
