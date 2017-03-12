#ifndef MANDELBROT_H
#define MANDELBROT_H

long double *frac_escape_time(long double complex center, int width, int height,
			      long double zoom, int max_it);
long double *escape_time(long double complex center, int width, int height,
			 long double zoom, int max_it);
long double *stars(long double complex center, int width, int height,
		   long double zoom, int max_it);
long double *exterior_distance(long double complex center, int width,
			       int height, long double zoom, int max_it);
long double *asm_fet(long double complex center, int width, int height,
		       long double zoom, int max_it);
#endif
