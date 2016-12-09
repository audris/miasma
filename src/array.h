#ifndef ARRAY_H
#define ARRAY_H

extern "C" void aerror (char * c);
extern "C" FILE * myfopen (char * f, char * m);
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((b) > (a) ? (b) : (a))
template<class el> void sort (int n, el *ra);
template<class el> void sort2 (int n, el *x, int * ind);


template<class el> class array {
protected:
	unsigned int dim;
	unsigned int size[3];
	el * values;
public:
	array (unsigned int i, unsigned int j=1, unsigned int k=1);
	array (array& s);
	~array (void) {delete (void*) values;}

	array& operator= (array& a);

	array& operator= (el i);

	double 		sum ();
	
	el& operator() (unsigned int i,unsigned int j=0,unsigned int k=0)
		{return values [i + size[0]*(j) + size[0]*size[1]*(k)];}
	int length (void) {return size[0]*size[1]*size[2];}
	int width (void) {return size[0];}
	int height (void) {return size[1];}
	int depth (void) {return size[2];}


	void		read		(FILE * in = stdin);	
	void		write		(FILE * out = stdout);
}; 

template<class el> class dataArray : public array<el> {
private:
	el MISS;
	dataArray scalarOperation (int which, double arg); 
	dataArray arrayOperation (int which, dataArray& arg); 
	
public:	
	dataArray (unsigned int i, unsigned int j=1,
				  unsigned int k=1, el miss = -9999) :
		array<el> (i, j, k) {MISS = miss;}
	dataArray (dataArray& s) : array<el> (s) {MISS = s .MISS;}
	dataArray (array<el>& s, el miss) : array<el> (s) {MISS = miss;}
	~dataArray (void) {}

	el& miss () {return MISS;}
	
	dataArray& operator= (dataArray& a) {this -> array<el> ::operator=(a);
													 MISS = a .MISS; return *this;}

//	dataArray& operator= (array& a, el miss)
//		{this -> array<el> ::operator=(a); MISS = miss; return *this;}


	dataArray operator+ (double i) {return scalarOperation (0, i);}
	dataArray operator- (double i) {return scalarOperation (1, i);}
	dataArray operator* (double i) {return scalarOperation (2, i);}
	dataArray operator/ (double i) {return scalarOperation (3, i);}
	
	dataArray operator+ (dataArray& i) {return arrayOperation (0, i);}
	dataArray operator- (dataArray& i) {return arrayOperation (1, i);}
	dataArray operator* (dataArray& i) {return arrayOperation (2, i);}
	dataArray operator/ (dataArray& i) {return arrayOperation (3, i);}

	dataArray  transform (double (*f) (double));

								
	dataArray  column (int i);
	dataArray  row    (int i);
	dataArray  cbind  (dataArray& b);
	dataArray  rbind  (dataArray& b);
	dataArray  selectRows    (int from, int to);
	dataArray  selectColumns (int from, int to);
	dataArray  selectRows    (int len, int * ind);
	dataArray  selectColumns (int len, int * ind);
	
	double 		sum ();
	el 			max ();
	el 			min ();
	double		mean ();
	double		var ();
	double 		median ();
	void	 		quantile (int n, double * p);
	dataArray&	sort ();
	dataArray&	histogram (el from, el to);
	dataArray&	rescale (el from, el to);
};


template<class el> void	array<el> ::read (FILE * in)
{
//	printf ((size[2]!=1?"%u %u %u\n":(size[1]!=1?"%u %u\n":"%u\n")),
//			  size [0], size [1], size[2]);

	double dummy;
	for (int k = 0; k < size [2]; k++)
		for (int j = 0; j < size [1]; j++)
			for (int i = 0; i < size [0]; i++){
				if (1 != fscanf (in, "%lf", &dummy))
					aerror ("Error reading array");
				(*this)(i,j,k) = (el) dummy;
			}
}

template<class el> void array<el> ::write (FILE * out)
{
//	printf ((size[2]!=1?"%u %u %u\n":(size[1]!=1?"%u %u\n":"%u\n")),
//			  size [0], size [1], size[2]);

	for (int k = 0; k < size [2]; k++){
		for (int j = 0; j < size [1]; j++){
			for (int i = 0; i < size [0]; i++)
				fprintf (out, FORMAT, (*this)(i,j,k));
			fprintf (out, "\n");
		}
		fprintf (out, "\n");
	}
}

template<class el> array<el> ::array (unsigned int i,unsigned int j,unsigned int k)
{
	size [0] = i;
	size [1] = j;
	size [2] = k;
	if (j == 1)
		dim = 1;
	else
		if (k == 1)
			dim = 2;
		else
			dim = 3;
	if ((values  = new el [i*j*k]) == NULL)
		aerror ("CAN NOT ALLOCATE ARRAY");
}

template<class el> array<el> ::array (array<el>& s)
{
	dim = s .dim;
	for (int i = 0; i < 3; i++)
		size [i] = s .size[i];
	values = new el [size[0]*size[1]*size[2]];
	for (i = 0; i < size [0]; i++)
		for (int j = 0; j < size [1]; j++)
			for (int k = 0; k < size [2]; k++)
				(*this) (i, j, k) = s (i, j, k);
}

template<class el> array<el>& array<el> ::operator= (array<el>& s)
{
	if (&s == this)
		return *this;
	int i = s .size [0], j = s .size [1], k = s .size [2];

	if (size [0] != i || size [1] != j || size [2] != k)
		aerror ("Can not assign arrays of different dimension");
  
	for (i = 0; i < size [0]; i++)
		for (j = 0; j < size [1]; j++)
			for (k = 0; k < size [2]; k++)
				(*this) (i, j, k) = s (i, j, k);
	return *this;
}

template<class el> array<el>& array<el> ::operator= (el s)
{
	int i = size [0] * size [1] * size [2];
  
	for (int j = 0; j < i; j++)
		(*this) .values [j] = s;
	return *this;
}



// ***************************************************************
template<class el> double array<el> ::sum ()
{
	int i = size [0] * size [1] * size [2];
	double s = 0;
	for (int j = 0; j < i; j++)
		s += (double) ((*this) .values [j]);
	return s;
}


template<class el> dataArray<el>  dataArray<el> ::cbind
	(dataArray<el>& b)
{
	if (size [0] != b .size [0] || size [2] != b .size [2])
		aerror ("Wrong shape of the arrays to be column cat");
	dataArray<el> c (size [0], size [1] + b .size [1],
						  size [2], MISS);
	register i, j, k;
	int limi=size [0], limk = size [2], limja=size [1],
			limjb=b .size [1];
	for (i = 0; i < limi; i++)
		for (k = 0; k < limk; k++){
			for (j = 0; j < limja; j++)
				c .operator() (i, j, k) = this ->operator() (i, j, k);
			for (j = 0; j < limjb; j++)
				c .operator() (i, j+limja, k) = b .operator() (i, j, k);
		}
	return c;
}

template<class el> dataArray<el>  dataArray<el> ::rbind (dataArray<el>& b)
{
	if (size [1] != b .size [1] || size [2] != b .size [2])
		aerror ("Wrong shape of the dataArrays to be row cat");
	dataArray<el> c (size [0]+b .size [0], size [1], size [2], MISS);
	register i, j, k;
	int limi=size [1], limk = size [2], limja=size [0],
			limjb=b .size [0];
	for (i = 0; i < limi; i++)
		for (k = 0; k < limk; k++){
			for (j = 0; j < limja; j++)
				c .operator() (j, i, k) = this ->operator() (j, i, k);
			for (j = 0; j < limjb; j++)
				c .operator() (j+limja, i, k) = this ->operator() (j, i, k);
		}
	return c;
}

template<class el> dataArray<el> dataArray<el> ::column (int k)
{
	if (k >= size [0] ||  k < 0)
		aerror ("Index out of bounds");
	int n = size [1], m = size[2];
	register i, j;
	dataArray<el> a (n, m, 1, MISS);
	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++)
			a .operator() (i, j) = this ->operator() (k, i, j);
	return a;
}

template<class el> dataArray<el> dataArray<el> ::row (int k)
{
	if (k >= size [1] ||  k < 0)
		aerror ("Index out of bounds");
	int n = size [0], m = size[2];
	register i, j;
	dataArray<el> a (n, m, 1, MISS);
	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++)
			a .operator() (i, j) = this ->operator() (i, k, j);
	return a;
}

template<class el> dataArray<el> dataArray<el> ::selectRows (int from, int to)
{
	int * ind = new int [to-from+1];
	for (int i = 0; i <to-from+1; i++)
		ind [i] = from + i;
	dataArray<el> ans =  selectRows (to-from+1, ind);
	delete ind;
	return ans;
}

template<class el> dataArray<el> dataArray<el> ::selectColumns (int from, int to)
{
	int * ind = new int [to-from+1];
	for (int i = 0; i <to-from+1; i++)
		ind [i] = from + i;
	dataArray<el> ans =  selectColumns (to-from+1, ind);
	delete ind;
	return ans;
}

template<class el> dataArray<el> dataArray<el> ::selectRows (int le, int * ind)
{
	int n = size [0], m = size[2];
	dataArray<el> a (n, le*m, 1, MISS);
  	register i, j, k;
	for (k = 0; k < le; k++)
		for (i = 0; i < n; i++)
			for (j = 0; j < m; j++)
				a .operator() (i, (j+1)*k) = this ->operator() (i, ind [k], j);
	return a;
}

template<class el> dataArray<el> dataArray<el> ::selectColumns (int le, int * ind)
{
	int n = size [1], m = size[2];
	dataArray<el> a (le*m, n, 1, MISS);
  	register i, j, k;
	for (k = 0; k < le; k++)
		for (i = 0; i < m; i++)
			for (j = 0; j < n; j++)
				a .operator() ((i+1)*k, j) = this ->operator() (ind [k], j, i);
	return a;
}


template<class el> double dataArray<el> ::sum ()
{
	int i = size [0] * size [1] * size [2];
	double s = 0;
	int k = 0;
	for (int j = 0; j < i; j++)
		if (values [j] != MISS){
			k++;
			s += (double) ((*this) .values [j]);
		}
	if (k)
		return s;
	else
		return MISS;
}


template<class el> dataArray<el> dataArray<el> :: scalarOperation (int which, double arg)
{
	int i = size [0] * size [1] * size [2];
	dataArray<el> res = (*this);
	int j;
	switch (which){
	case 0:
		for (j = 0; j < i; j++)
			if (values [j] != MISS)
				res .values [j] = el (values [j] + arg);
		break;
	case 1:
		for (j = 0; j < i; j++)
			if (values [j] != MISS)
				res .values [j] = el (values [j] - arg);
		break;
	case 2:
		for (j = 0; j < i; j++)
			if (values [j] != MISS)
				res .values [j] = el (values [j] * arg);
		break;
	case 3:
		for (j = 0; j < i; j++)
			if (values [j] != MISS)
				res .values [j] = el (values [j] / arg);
		break;
	}		

	return res;
}

template<class el> dataArray<el> dataArray<el> :: arrayOperation (int which, dataArray<el>& arg)
{
	int i = size [0] * size [1] * size [2];
	dataArray<el> res = (*this);
	int j;
	switch (which){
	case 0:
		for (j = 0; j < i; j++)
			if (values [j] != MISS && arg .values [j] != MISS)
				res .values [j] = el (values [j] + arg .values [j]);
			else
				res .values [j] = MISS;
		break;
	case 1:
		for (j = 0; j < i; j++)
			if (values [j] != MISS && arg .values [j] != MISS)
				res .values [j] = el (values [j] - arg .values [j]);
			else
				res .values [j] = MISS;
		break;
	case 2:
		for (j = 0; j < i; j++)
			if (values [j] != MISS && arg .values [j] != MISS)
				res .values [j] = el (values [j] * arg .values [j]);
			else
				res .values [j] = MISS;
		break;
	case 3:
		for (j = 0; j < i; j++)
			if (values [j] != MISS && arg .values [j] != MISS)
				res .values [j] = el (values [j] / arg .values [j]);
			else
				res .values [j] = MISS;
		break;
	}		

	return res;
}

template<class el> dataArray<el> dataArray<el> ::transform  (double (*f) (double))
{
	int i = size [0] * size [1] * size [2];
	dataArray<el> res = (*this);
	int j;
	for (j = 0; j < i; j++)
		if (values [j] != MISS)
			res .values [j] = el (f(double(values [j])));
		else
			res .values [j] = MISS;
	return res;
}


template<class el> el dataArray<el> ::max ()
{
	int i = size [0] * size [1] * size [2];
	int first = 1;
	el m = MISS;
	for (register j = 0; j < i; j++)
		if (values [j] != MISS){
			if (first){
				m = values [j];
				first = 0;
			}else
				m = MAX (m, values [j]);
			
		}
	return m;
}

template<class el> el dataArray<el> ::min ()
{
	int i = size [0] * size [1] * size [2];
	int first = 1;
	el m = MISS;
	for (register j = 0; j < i; j++)
		if (values [j] != MISS){
			if (first){
				m = values [j];
				first = 0;
			}else
				m = MIN (m, values [j]);
		}
	return m;
}

template<class el> double dataArray<el> ::mean ()
{
	int i = size [0] * size [1] * size [2];
	double m = 0;
	int k = 0;
	for (int j = 0; j < i; j++)
		if (values [j] != MISS){
			k++;
			m += (double) ((*this) .values [j]);
		}
	if (k != 0)
		return (m/k);
	else
		return MISS;
}

template<class el> double dataArray<el> ::var ()
{
	int i = size [0] * size [1] * size [2];
	double m = values [0]*values [0], m1 = mean (), mm;
	register j;
	int k = 0;
	for (j = 1; j < i; j++)
		if (values [j] != MISS){
			mm = (values [j] - m1);
			m += mm*mm;
			k ++;
		}
	if (k != 0)
		return (m/k);
	else
		return MISS;
}

template<class el> double dataArray<el> ::median ()
{
	int n = size [0] * size [1] * size [2];
	if (n == 1)
		return values [0];
	el * x = new el [n]; 
	int i = 0, j = 0;
	while (i < n){
		if (values [i] != MISS){
			x [j] = values [i];
			j++;
		}
		i++;
	}
	double answer;
	if (j < 1)
		answer =  (values [0]);
	else
		if (j == 1)
			answer = (x [0]);
		else{
			::sort (j, x);
			int n2, n2p;
			n2p=(n2=j/2)+1;
			answer = ((j % 2) ? (x[n2p-1]) : (0.5*(x[n2-1]+x[n2p-1])));
	}
	delete x;
	return answer;
}

template<class el> void	dataArray<el> ::quantile (int m, double * p)
{
	int n = size [0] * size [1] * size [2];
	if (n == 1){
		for (int k = 0; k < m; k++)
			p [k] = values [0];
		return;
	}
	el * x = new el [n]; 
	int i = 0, j = 0;
	while (i < n){
		if (values [i] != MISS){
			x [j] = values [i];
			j++;
		}
		i++;
	}
	if (j <= 1){
		x [0] = values [0];
		for (int k = 0; k < m; k++)
			p [k] = x [0];
		delete x;
		return;
	}
	::sort (j, x);
	for (int k = 0; k < m; k++){
		for (i = 0; i < j; i++)
			if (double(i+1)/j >= p [k]){
				p [k] = x [i];
				break;
			}
	}
	delete x;
	return;
}

template<class el> dataArray<el>& dataArray<el> ::sort ()
{
	int n = size [0] * size [1] * size [2];
	::sort (n, values); 
	return *this;
}

template<class el> dataArray<el>& dataArray<el> ::rescale (el from, el to)
{
	el up = max ();
	if (up == MISS)
		return *this;
	el low = min ();
	int len = length ();
	double ratio;
	if (up - low > 0)
		ratio = (to - from)/double(up - low);
	else
		ratio = 1;
	for (int i = 0; i < len; i++)
		if (values [i] != MISS)
			values [i] = el((values [i] - low)*ratio + from);
	return *this;
}

template<class el> dataArray<el>& dataArray<el> ::histogram (el from, el to)
{
	int len = length ();
	el up = max ();
	if (up == MISS)
		aerror ("No nonmissing values to make histogram");
	int * hist = new int [len];
	int * indTmToVal = new int [len];
	el * tmpVal = new el [len];
	int nNonMis = 0;
	for (int i = 0; i < len; i++)
		if (values [i] != MISS){
			indTmToVal [nNonMis] = i;
			tmpVal [nNonMis++] = values [i];
		}
  	sort2 (nNonMis, tmpVal, hist);
//	now deal with repeated values
	el val0 = tmpVal [hist [0]];
	int NDistinct = 1;
	for (i = 0; i < nNonMis; i++)
		if (val0 < tmpVal [hist [i]]){
			val0 = tmpVal [hist [i]];
			NDistinct ++;
		}
	printf ("%d %d %d\n", len, nNonMis, NDistinct); 
	if (NDistinct <= 1){
		for (i = 0; i < len; i++)
			if (values [i] != MISS)
				values [i] = el (from);
	}else{
		int index = 0;
		val0 = tmpVal [hist [0]];
		for (i = 0; i < nNonMis; i++){
			if (val0 < tmpVal [hist [i]]){
				val0 = tmpVal [hist [i]];
				index ++;
			}
			values [indTmToVal [hist [i]]] =
				el (double (index) / (NDistinct-1)
								  * (to - from) + from);
		}
	}
	delete hist;
	delete indTmToVal;
	delete tmpVal;
	return *this;
}

typedef array<short> sarray;
typedef array<char> carray;
typedef array<int> iarray;
typedef array<float> farray;
typedef array<double> darray;

typedef dataArray<short> sdataArray;
typedef dataArray<char> cdataArray;
typedef dataArray<int> idataArray;
typedef dataArray<float> fdataArray;
typedef dataArray<double> ddataArray;


	
#endif









