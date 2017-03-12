#ifndef HELPERS_H
#define HELPERS_H
#include <cairo.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PI				3.141592654
#define GOLDEN_ANGLE 	PI*(3-sqrt(5))
#ifndef MIN
#define MIN(a,b)  (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b)  (((a)>(b))?(a):(b))
#endif
#define ITERATIONS(z) (MIN((150*pow((z),1.0/3)),100000))
//#define ITERATIONS(z) 2
#define SIGN(n) (((n)<0)?-1:1)

typedef struct {
	double *r;
	double *g;
	double *b;
	double *p;
	int n;
	double factor;
	double offset;
} gradient;
extern void _asm_fet(long double re, long double im,long double a_re, long double a_im, int max_it, int bailout, long double *res);
void cairo_set_color_from_gradient(cairo_t * cr,gradient *gr, long double f);
void cairo_set_source_hsv(cairo_t * cr, double h, double s, double v);
long double interpolate(long double x, long double x1, long double x2, long double y1, long double y2, long double m1, long double m2);
void init_gradient(gradient *gr);
void unload_gradient();
gradient *load_std_gradient();
gradient *load_gradient_from_file(char* filename);
void free_gradient(gradient *gr);
int same_neighbours(long double *cplane,int x, int y, int chunksize, int width, int height);
#endif
