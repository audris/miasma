class array {
protected:
	unsigned int dim;
	unsigned int size[3];
	el * values;
public:
	array (unsigned int i, unsigned int j=1, unsigned int k=1);
	array (const array& s);
	array (el * s, unsigned int i) {dim = 1; values = s; size [0] = i;
											  size [1] = size [2] = 1;};
	~array (void) {delete values;}

	array& operator= (array& a);

	array& operator= (el i);

	double 		sum ();
	
	el& operator() (unsigned int i,unsigned int j=0,unsigned int k=0)
		{return values [i + size[0]*(j) + size[0]*size[1]*(k)];}

	el get (unsigned int i,unsigned int j=0,unsigned int k=0) const
		{return values [i + size[0]*(j) + size[0]*size[1]*(k)];}

	int length (void) const {return size[0]*size[1]*size[2];}
	int width (void) const {return size[0];}
	int height (void) const {return size[1];}
	int depth (void) const {return size[2];}

	void		read		(FILE * in = stdin);	
	void		write		(FILE * out = stdout);
}; 

class dataArray : public array {
private:
	el MISS;
	dataArray scalarOperation (int which, double arg) const; 
	dataArray arrayOperation (int which, const dataArray& arg) const; 
	
public:	
	dataArray (unsigned int i, unsigned int j=1,
				  unsigned int k=1, el miss = -1) :
		array (i, j, k) {MISS = miss;}
    //dataArray (dataArray s) : array (s) {MISS = s .MISS;}
	dataArray (const dataArray& s) : array (s) {MISS = s .MISS;}
	dataArray (const array& s, el miss) : array (s) {MISS = miss;}
	dataArray (el * s, unsigned int i, el miss) : array (s, i) {
											  MISS = miss;}
	~dataArray (void) {}

	el miss () const {return MISS;}
	
	dataArray& operator= (dataArray& a) {this -> array ::operator=(a);
													 MISS = a .MISS; return *this;}

//	dataArray& operator= (array& a, el miss)
//		{this -> array ::operator=(a); MISS = miss; return *this;}


	dataArray operator+ (double i) const {return scalarOperation (0, i);}
	dataArray operator- (double i) const {return scalarOperation (1, i);}
	dataArray operator* (double i) const {return scalarOperation (2, i);}
	dataArray operator/ (double i) const {return scalarOperation (3, i);}
	
	dataArray operator+ (const dataArray& i) const {return arrayOperation (0, i);}
	dataArray operator- (const dataArray& i) const {return arrayOperation (1, i);}
	dataArray operator* (const dataArray& i) const {return arrayOperation (2, i);}
	dataArray operator/ (const dataArray& i) const {return arrayOperation (3, i);}

	dataArray  transform (double (*f) (double)) const;
	void  transformSelf (double (*f) (double));

								
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













