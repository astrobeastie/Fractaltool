#ifndef JULIA_H
#define JULIA_H

long double *julia_frac_escape_time(long double complex center,long double complex seed, int width, int height,
			      long double zoom, int max_it);
long double *julia_escape_time(long double complex center,long double complex seed, int width, int height,
			 long double zoom, int max_it);
long double *julia_exterior_distance(long double complex center,long double complex seed, int width,
			       int height, long double zoom, int max_it);
long double *julia_asm_fet(long double complex center,long double complex seed, int width, int height,
		       long double zoom, int max_it);
#endif
