#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "helpers.h"
#define BAILOUT 100000000
#define MIN(a,b)  (((a)<(b))?(a):(b))
long double *julia_frac_escape_time(long double complex center,long double complex seed, int width, int height,
			      long double zoom, int max_it)
{
	long double *cplane = malloc(width * height * sizeof(long double));
	memset(cplane, 0, width * height * sizeof(long double));
	long double res = (zoom * ((width < height) ? width : height)) / 4;
	long double xs = creal(center) - width / (2 * res);
	long double ys = cimag(center) - height / (2 * res);
	long double s_re=creall(seed);
	long double s_im=cimagl(seed);
	long double llb = log2l(logl(BAILOUT));
	long double im, re;
	long double x, y;
	int idx = 0;
	for (y = 0; y < height; y++) {
		im = ys + y / res;

		for (x = 0; x < width; x++) {
			int i = 0;
			re = xs + x / res;
			long double c_im = im;
			long double c_re = re;
			int N = 0;

			while (i < max_it
			       && (c_re * c_re + c_im * c_im) < BAILOUT) {
				N++;
				//    c = c*c + re + im * I ;
				long double c_re_temp =
				    c_re * c_re - c_im * c_im + s_re;
				c_im = 2l * c_re * c_im + s_im;
				c_re = c_re_temp;
				i++;
			}

			if (N < max_it)
				cplane[idx] =0.05*(i-log2l(logl(c_re*c_re+c_im*c_im))+llb+1);
			else
				cplane[idx] = -1;
			//     printf("%d",i);
			//  sn=(sn<i)?sn:i;
			//     hist[i]++;
			idx++;

		}
	}
	return cplane;
}

long double *julia_escape_time(long double complex center,long double complex seed, int width, int height,
			      long double zoom, int max_it)
{
	long double *cplane = malloc(width * height * sizeof(long double));
	memset(cplane, 0, width * height * sizeof(long double));
	long double res = (zoom * ((width < height) ? width : height)) / 4;
	long double xs = creal(center) - width / (2 * res);
	long double ys = cimag(center) - height / (2 * res);
	long double im, re;
	long double x, y;
	long double s_re=creall(seed);
	long double s_im=cimagl(seed);
	int idx = 0;
	for (y = 0; y < height; y++) {
		im = ys + y / res;

		for (x = 0; x < width; x++) {
			int i = 0;
			re = xs + x / res;
			long double c_im = im;
			long double c_re = re;
			int N = 0;

			while (i < max_it
			       && (c_re * c_re + c_im * c_im) < BAILOUT) {
				N++;
				//    c = c*c + re + im * I ;
				long double c_re_temp =
				    c_re * c_re - c_im * c_im + s_re;
				c_im = 2l * c_re * c_im + s_im;
				c_re = c_re_temp;
				i++;
			}

			if (N < max_it)
				cplane[idx] = N;
			else
				cplane[idx] = -1;
			//     printf("%d",i);
			//  sn=(sn<i)?sn:i;
			//     hist[i]++;
			idx++;

		}
	}
	return cplane;
}

long double *julia_exterior_distance(long double complex center,long double complex seed, int width, int height,
			      long double zoom, int max_it)
{
	long double *cplane = malloc(width * height * sizeof(long double));
	memset(cplane, 0, width * height * sizeof(long double));
	long double res = (zoom * ((width < height) ? width : height)) / 4;
	long double xs = creal(center) - width / (2 * res);
	long double ys = cimag(center) - height / (2 * res);
	long double im, re;
	long double x, y;
	long double s_re=creall(seed);
	long double s_im=cimagl(seed);
	int idx = 0;
	for (y = 0; y < height; y++) {
		im = ys + y / res;

		for (x = 0; x < width; x++) {
			int i = 0;
			re = xs + x / res;
			long double c_im = im;
			long double c_re = re;
			long double d = c_im * c_im + c_re * c_re;
			//int N=0;
			double d_im = 0, d_re = 1;
			while (i < max_it && d < BAILOUT) {
				//      N++;
				//    c = c*c + re + im * I ;
				long double c_re_temp =
				    c_re * c_re - c_im * c_im + s_re;
				long double c_im_temp = 2 * c_re * c_im + s_im;
				long double d_re_temp =
				    2 * (c_re * d_re - c_im * d_im);
				long double d_im_temp =
				    2 * (c_re * d_im + c_im * d_re);
				c_re = c_re_temp;
				c_im = c_im_temp;
				d_re = d_re_temp;
				d_im = d_im_temp;
				d = c_im * c_im + c_re * c_re;
				i++;
			}

			if (i < max_it) {
				long double mz =
				    sqrtl(c_re * c_re + c_im * c_im);
				long double md =
				    sqrtl(d_re * d_re + d_im * d_im);
				cplane[idx] = mz * logl(mz) / md;
			} else {
				cplane[idx] = -1;
			}
			//     printf("%d",i);
			//  sn=(sn<i)?sn:i;
			//     hist[i]++;
			idx++;

		}
	}
	return cplane;
}

long double *julia_asm_fet(long double complex center,long double complex seed, int width, int height,
			      long double zoom, int max_it)
{
	//printf("2. x: %.5g, y: %.5g, zoom: %.5g \n", (double)creall(center),
	//       (double)cimagl(center), (double)zoom);
	long double *cplane = malloc(width * height * sizeof(long double));
	memset(cplane, 0, width * height * sizeof(long double));
	long double res = (long double)(zoom * ((width < height) ? width-1 : height-1)) / 4;
	long double xs = creal(center) - (width-1) / (2 * res);
	long double ys = cimag(center) - (height-1) / (2 * res);
	//long double il2 = 1l/logl(2l); // Inverse logl 2 (precalc).
	long double lp = log2l(logl(BAILOUT));	// logl(logl bailout) (precalc).
	long double im, re;
	int x, y, px, py;
	int idx = 0l;
	int chunksize=16;
	int i;
	for(i=0;i<5;i++) {
		
		for (y = 0l; y < height; y+=chunksize) {
			
				im = ys + y / res;
		
				for (x = 0; x < width; x+=chunksize) {
					if(x%(2*chunksize)==0)
				px=x;
			else
				px=x-chunksize;
			if(y%(2*chunksize)==0)
				py=y;
			else
				py=y-chunksize;
			
			int eval;
			if(i==0) {
				eval=1;
			}else if (y==py && x==px) {
				eval=0;
			}else if(same_neighbours(cplane,px,py, chunksize*2, width, height)==1) {
				cplane[y*width+x]=cplane[py*width+px];
				eval=0;
			}else{
				eval=1;
			}
			if (eval==1) {
					re = xs + x / res;
					long double res = 0l;	
					_asm_fet(creal(seed), cimag(seed),re,im, max_it, BAILOUT, &res);
					
					#ifdef FRACTALTOOL
						if(res==-1)
							cplane[y*width+x]=max_it;
						else
							cplane[y*width+x]=res;
					#else
						cplane[y*width+x]=res;
					#endif
				}
			}
		}
		chunksize=chunksize/2;
	}
	//printf("\n");
	return cplane;
}
