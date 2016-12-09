#include <stdio.h>
#include "array.h"


extern "C" void aerror (char * text)
{
 /* My subroutine for handling errors */
	/* 	void exit (int status); */
	void exit (int);
	fprintf (stderr, "Error: %s\n", text);
	exit (1);
}

extern "C" FILE *  myfopen (char * fName, char * mode)
{
	FILE * ans = fopen (fName, mode);
	void exit (int);
	if (ans == NULL){
		fprintf (stderr, "Can not open file %s in mode %s\n",
					fName, mode);
		exit (-1);
	}
	return ans;
}



template<class el> void sort(int n, el *ra)
{
	int l,j,ir,i;
	el rra;

	if (n <= 1)
		return;
	
	l=(n >> 1)+1;
	ir=n;
	for (;;) {
		if (l > 1)
			rra=ra[--l - 1];
		else {
			rra=ra[ir - 1];
			ra[ir - 1]=ra[1 - 1];
			if (--ir == 1) {
				ra[1 - 1]=rra;
				return;
			}
		}
		i=l;
		j=l << 1;
		while (j <= ir) {
			if (j < ir && ra[j - 1] < ra[j+1 - 1]) ++j;
			if (rra < ra[j - 1]) {
				ra[i - 1]=ra[j - 1];
				j += (i=j);
			}
			else j=ir+1;
		}
		ra[i - 1]=rra;
	}
}


template<class el> void sort2 (int n, el * x, int * y)
{
	static int i, j, k, l, m, p, q;
	static el s, t;
	int * ww = new int [n];
	int * w = ww;

	/* Parameter adjustments */
	--x;
	--y;
	--w;

	/* Function Body */
	i = 1;
L10:
	k = i;
L20:
	j = i;
	y[i] = i;
	++i;
	if (j == n) {
		goto L30;
	}
	if (x[i] >= x[j]) {
		goto L20;
	}
	w[k] = i;
	goto L10;
L30:
	if (k == 1) {
		y [1] = 0;
		return;
	}
	w[k] = (n + 1);
L40:
	m = 1;
	l = 1;
L50:
	i = l;
	if (i > n) {
		goto L120;
	}
	p = y[i];
	s = x[p];
	j = w[i];
	k = j;
	if (j > n) {
		goto L100;
	}
	q = y[j];
	t = x[q];
	l = w[j];
	y[i] = l;
L60:
	if (s > t) {
		goto L70;
	}
	w[m] = p;
	++m;
	++i;
	if (i == k) {
		goto L80;
	}
	p = y[i];
	s = x[p];
	goto L60;
L70:
	w[m] = q;
	++m;
	++j;
	if (j == l) {
		goto L110;
	}
	q = y[j];
	t = x[q];
	goto L60;
L80:
	w[m] = q;
	k = m + l - j;
	i = j - m;
L90:
	++m;
	if (m == k) {
		goto L50;
	}
	w[m] = y[m + i];
	goto L90;
L100:
	y[i] = j;
	l = j;
L110:
	w[m] = p;
	k = m + k - i;
	i -= m;
	goto L90;
L120:
	i = 1;
L130:
	k = i;
	j = y[i];
L140:
	y[i] = w[i];
	++i;
   if (i < j) {
		goto L140;
	}
	w[k] = i;
	if (i <= n) {
		goto L130;
	}
	if (k > 1) {
		goto L40;
	}
	for (i = 1; i <= n; i++)
		y [i] --;
	delete ww;
	return;
} 
























