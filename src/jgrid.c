#include <cairo.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <cairo-svg.h>
#include <string.h>
#include <stdio.h>
#include "julia.h"
#define MIN(a,b)  (((a)<(b))?(a):(b))
#define ITERATIONS(z) (MIN((150*pow((z),1.0/3)),100000))
void cairo_set_color_from_gradient(cairo_t * cr, long double f);

int main(int argc, char *argv[])
{

	long double *cplane;
	cairo_surface_t *surface;
	cairo_t *cr;

	surface =
	    cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 10000, 10000);
	cr = cairo_create(surface);
	long double x, y, X,Y;
	for (X=0;X<200;X++) {
		for(Y=0;Y<200;Y++) {
		cplane = julia_potential(0,-2+2*X/100+(-2+2*Y/100)*I, 50, 50, 1, 30);
			for (x = 0; x<50; x++) {
				for (y = 0; y < 50; y++) {
					long double v = cplane[(int)(x + y * 50)];
					if (v == -1) {
						cairo_set_source_rgb(cr, 0, 0, 0);
					} else {
						cairo_set_color_from_gradient(cr,logl(v) *360);
					}
					cairo_rectangle(cr, (50*X+x), 10000 - (50*Y+y), 1, 1);
					cairo_fill(cr);

				}
			}
			free(cplane);
		}
	}
	cairo_destroy(cr);
	cairo_status_t st = cairo_surface_write_to_png(surface, argv[1]);
	//if(st!=CAIRO_STATUS_SUCCESS);
	//{
	//printf("ERROR : %d\n",(int)st);
	//}
	cairo_surface_destroy(surface);
	return 0;
}

void cairo_set_color_from_gradient(cairo_t * cr, long double f)
{
	f = fmodl(f, 360);
	if (f < 0)
		f = 360 + f;
	long double r, g, b;
	if (f < 64) {
		r = f * 32 / 64;
		g = 7 + f * (107 - 7) / 64;
		b = 100 + f * (203 - 100) / 64;
		cairo_set_source_rgb(cr, r / 255, g / 255, b / 255);
		return;
	}
	if (f < 168) {
		r = 32 + (f - 64) * (237 - 32) / 104;
		g = 107 + (f - 64) * (255 - 107) / 104;
		b = 203 + (f - 64) * (255 - 203) / 104;
		cairo_set_source_rgb(cr, r / 255, g / 255, b / 255);
		return;
	}
	if (f < 257) {
		r = 237 + (f - 168) * (255 - 237) / 89;
		g = 255 + (f - 168) * (170 - 255) / 89;
		b = 255 + (f - 168) * (0 - 255) / 89;
		cairo_set_source_rgb(cr, r / 255, g / 255, b / 255);
		return;
	}
	if (f < 343) {
		r = 255 + (f - 257) * (49 - 255) / 86;
		g = 170 + (f - 257) * (2 - 170) / 86;
		b = 0 + (f - 257) * (48 - 0) / 86;
		cairo_set_source_rgb(cr, r / 255, g / 255, b / 255);
		return;
	}
	r = 49 + (f - 343) * (0 - 49) / 17;
	g = 2 + (f - 343) * (7 - 2) / 17;
	b = 48 + (f - 343) * (100 - 48) / 17;
	cairo_set_source_rgb(cr, r / 255, g / 255, b / 255);
}


