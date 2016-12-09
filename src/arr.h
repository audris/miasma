#ifndef ARRAY_H
#define ARRAY_H


template<class el> class array {
protected:
	unsigned int dim;
	unsigned int size[3];
	el * values;
public:
	array (unsigned int i);
	el& operator() (unsigned int i,unsigned int j=0,unsigned int k=0)
		{return values [i + size[0]*(j) + size[0]*size[1]*(k)];}
	void		write		(FILE * out);
}; 

#endif









