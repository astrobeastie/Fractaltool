#include <cairo.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mandelbrot.h"
#include "julia.h"
#include "helpers.h"
 gradient *gr;
void mandel(char *filename, long double complex center, long double zoom, int it, int width, int height);
void julia(char *filename, long double complex center, long double complex seed, long double zoom, int it, int width, int height);
//replaces first occurance of c with \0 and returns 2nd string; returns null if c not found
char* splitstr(char *str, char c)
{
	while(*str!='\0' && *str!=c)
	{
		str++;
	}
	if(*str=c)
	{
		*str='\0';
		return ++str;
	}
	return NULL;
}

void msusage()
{
	printf("Usage: fractaltool mandel -c center -z zoom -o output [options]\n\n");
	printf("Available options are:\n");
	printf("\t-g : gradientfile\n");
	printf("\t-d : colordensity\n");
	printf("\t-O : gradientoffset\n");
	printf("\t-r : imagesize specified as WIDTHxHEIGHT\n\n");
	printf("All complex numbers must be specified as Re,Im\n");

}
void jsusage()
{
	printf("Usage: fractaltool julia -s seed -c center -z zoom -o output [options]\n\n");
	printf("Available options are:\n");
	printf("\t-g : gradientfile\n");
	printf("\t-d : colordensity\n");
	printf("\t-O : gradientoffset\n");
	printf("\t-r : imagesize specified as WIDTHxHEIGHT\n\n");
	printf("All complex numbers must be specified as Re,Im\n");

}
void mzusage()
{
	printf("Usage: fractaltool mandelzoom -c center -e maxzoom -o outputformat [options]\n\n");
	printf("Available options are:\n");
	printf("\t-g : gradientfile\n");
	printf("\t-d : colordensity\n");
	printf("\t-O : gradientoffset\n");
	printf("\t-r : imagesize specified as WIDTHxHEIGHT\n");
	printf("\t-n : number of frames\n");
	printf("\t-b : minzoom\n\n");
	printf("All complex numbers must be specified as Re,Im\n");

}

void jmusage()
{
	printf("Usage: fractaltool juliamorph -b startingseed -e endseed -o outputformat [options]\n\n");
	printf("Available options are:\n");
	printf("\t-g : gradientfile\n");
	printf("\t-d : colordensity\n");
	printf("\t-O : gradientoffset\n");
	printf("\t-r : imagesize specified as WIDTHxHEIGHT\n");
	printf("\t-n : number of frames\n\n");
	printf("All complex numbers must be specified as Re,Im\n");
}

void mandelsingle(int argc, char **argv)
{
		//usage: fractaltool mandel filename center_re center_im zoom width height
	int width = 800;
	int height = 600;
	char *filename;
	long double z = 1;
	long double complex center = -0.5;
	
	int i=2;
	char *a;
	while(i<argc)
	{
		a=argv[i];
		if(strncmp("-g", a,2)==0)
		{
			if(strlen(a)>2)
			{
				a+=2;
				gr=load_gradient_from_file(a);
			}else{
				a=argv[++i];
				gr=load_gradient_from_file(a);
			}
		} else if(strncmp("-d", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				gr->factor=atof(a);
			}else{
				a=argv[++i];
				gr->factor=atof(a);
			}
		} else if(strncmp("-O", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				gr->offset=atof(a);
			}else{
				a=argv[++i];
				gr->offset=atof(a);
			}
		}  else if(strncmp("-z", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				z=powl(2,atof(a));
			}else{
				a=argv[++i];
				z=powl(2,atof(a));
			}
		} else if(strncmp("-o", a,2)==0) {
			if(strlen(a)>2)
			{
				filename=a+2;
			}else{
				filename=argv[++i];
			}
		} else if(strncmp("-c", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				char *s2=splitstr(a,',');
				if(s2!=NULL)
				{
					center=atof(a)+atof(s2)*I;
				}else{
					msusage();
					return;
				}
			}else{
				a=argv[++i];
				char *s2=splitstr(a,',');
				if(s2!=NULL)
				{
					center=atof(a)+atof(s2)*I;
				}else{
					msusage();
					return;
				}
			}
		}else if(strncmp("-r", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				char *s2=splitstr(a,'x');
				if(s2!=NULL)
				{
					width=atof(a);
					height=atof(s2);
				}else{
					msusage();
					return;
				}
			}else{
				a=argv[++i];
				char *s2=splitstr(a,'x');
				if(s2!=NULL)
				{
					width=atof(a);
					height=atof(s2);
				}else{
					msusage();
					return;
				}
			}
		} else {
			msusage();
			return;
		}
		i++;
	}
	long double res = (z * ((width < height) ? width : height)) / 4;
	mandel(filename, center,z,(int)ITERATIONS(res),width,height);

}
void juliasingle(int argc, char **argv)
{
		//usage: fractaltool mandel filename center_re center_im zoom width height
	int width = 800;
	int height = 600;
	char *filename;
	long double z = 1;
	long double complex center = 0;
	long double complex seed =0;
	int i=2;
	char *a;
	while(i<argc)
	{
		a=argv[i];
		if(strncmp("-g", a,2)==0)
		{
			if(strlen(a)>2)
			{
				a+=2;
				gr=load_gradient_from_file(a);
			}else{
				a=argv[++i];
				gr=load_gradient_from_file(a);
			}
		} else if(strncmp("-d", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				gr->factor=atof(a);
			}else{
				a=argv[++i];
				gr->factor=atof(a);
			}
		} else if(strncmp("-O", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				gr->offset=atof(a);
			}else{
				a=argv[++i];
				gr->offset=atof(a);
			}
		}  else if(strncmp("-z", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				z=powl(2,atof(a));
			}else{
				a=argv[++i];
				z=powl(2,atof(a));
			}
		} else if(strncmp("-o", a,2)==0) {
			if(strlen(a)>2)
			{
				filename=a+2;
			}else{
				filename=argv[++i];
			}
		} else if(strncmp("-c", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				char *s2=splitstr(a,',');
				if(s2!=NULL)
				{
					center=atof(a)+atof(s2)*I;
				}else{
					jsusage();
					return;
				}
			}else{
				a=argv[++i];
				char *s2=splitstr(a,',');
				if(s2!=NULL)
				{
					center=atof(a)+atof(s2)*I;
				}else{
					jsusage();
					return;
				}
			}
		}else if(strncmp("-s", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				char *s2=splitstr(a,',');
				if(s2!=NULL)
				{
					seed=atof(a)+atof(s2)*I;
				}else{
					jsusage();
					return;
				}
			}else{
				a=argv[++i];
				char *s2=splitstr(a,',');
				if(s2!=NULL)
				{
					seed=atof(a)+atof(s2)*I;
				}else{
					jsusage();
					return;
				}
			}
		}else if(strncmp("-r", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				char *s2=splitstr(a,'x');
				if(s2!=NULL)
				{
					width=atof(a);
					height=atof(s2);
				}else{
					jsusage();
					return;
				}
			}else{
				a=argv[++i];
				char *s2=splitstr(a,'x');
				if(s2!=NULL)
				{
					width=atof(a);
					height=atof(s2);
				}else{
					jsusage();
					return;
				}
			}
		} else {
			jsusage();
			return;
		}
		i++;
	}
	long double res = (z * ((width < height) ? width : height)) / 4;
	julia(filename, center, seed,z,(int)ITERATIONS(res),width,height);

}
long double antialias(long double *cplane, long double x, long double y, long double r, int width, int height)
{
		int xi,yi;
		long double res=-1, i=0;
		long double r2 = sqrtl(2.0l)*r/2;
		int m = round(r)*round(r)/2;
		for(yi=y;yi<round(y+r);yi++) {
			for(xi=x;xi<round(x+r);xi++) {
				/*if(xi>=0 && xi < width && yi>=0 && yi< height) {

					if(i<m) {
						res=MAX(cplane[xi+yi*width],res);
						i++;
					}else{
						res=MIN(cplane[xi+yi*width],res);
						i++;
					}*/
					long double w = 40-((xi/r2-x+r/2)*(xi/r2-x+r/2)+(yi/r2-y+r/2)*(yi/r2-y+r/2));
					res+=w*cplane[xi+yi*width];
					i+=w;
					//W+=w;
				}
			}

		return res/i;
}
void mandelzoom(int argc, char **argv)
{
	//usage: fractaltool mandelzoom prefix center_re center_im zoom_start zoom_end [width] [height]
	int width = 800;
	int height = 600;
	long double z = 0;
	long double ze = 25;
	long double complex center;
	long double res;
	int n_frames = -1;
	char *fmt;
	int i=2;
	char *a;
	while(i<argc)
	{
		a=argv[i];
		if(strncmp("-g", a,2)==0)
		{
			if(strlen(a)>2)
			{
				a+=2;
				gr=load_gradient_from_file(a);
			}else{
				a=argv[++i];
				gr=load_gradient_from_file(a);
			}
		} else if(strncmp("-d", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				gr->factor=atof(a);
			}else{
				a=argv[++i];
				gr->factor=atof(a);
			}
		} else if(strncmp("-O", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				gr->offset=atof(a);
			}else{
				a=argv[++i];
				gr->offset=atof(a);
			}
		} else if(strncmp("-n", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				n_frames=(int)atof(a);
			}else{
				a=argv[++i];
				n_frames=(int)atof(a);
			}
		} else if(strncmp("-b", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				z=atof(a);
			}else{
				a=argv[++i];
				z=atof(a);
			}
		} else if(strncmp("-e", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				ze=atof(a);
			}else{
				a=argv[++i];
				ze=atof(a);
			}
		} else if(strncmp("-o", a,2)==0) {
			if(strlen(a)>2)
			{
				fmt=a+2;
			}else{
				fmt=argv[++i];
			}
		} else if(strncmp("-c", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				char *s2=splitstr(a,',');
				if(s2!=NULL)
				{
					center=atof(a)+atof(s2)*I;
				}else{
					mzusage();
					return;
				}
			}else{
				a=argv[++i];
				char *s2=splitstr(a,',');
				if(s2!=NULL)
				{
					center=atof(a)+atof(s2)*I;
				}else{
					mzusage();
					return;
				}
			}
		}else if(strncmp("-r", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				char *s2=splitstr(a,'x');
				if(s2!=NULL)
				{
					width=atof(a);
					height=atof(s2);
				}else{
					mzusage();
					return;
				}
			}else{
				a=argv[++i];
				char *s2=splitstr(a,'x');
				if(s2!=NULL)
				{
					width=atof(a);
					height=atof(s2);
				}else{
					mzusage();
					return;
				}
			}
		} else {
			mzusage();
			return;
		}
		i++;
	}

	char *filename = malloc(200);
	i = 0;
	long double m;
	if(n_frames==-1)
		m=0.025;
	else
	 m = (ze-z)/n_frames;

	int length=(width<height)?width:height;
	//z=powl(2,z);
	//ze=powl(2,ze);
	cairo_surface_t *surface;
	cairo_t *cr;
	int s = SIGN(gr->factor);
	double mf=fabs(gr->factor);
	surface =
	    cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
	cr = cairo_create(surface);
	//printf("m: %.5g\n",(double)m);
	while (z < ze) {

		long double zoom = exp2l(z);
		long double d = 1/(zoom*zoom);
		long double complex cn = (1-d)*center -0.5*d;

		res = (zoom * length)*2;
		int it = ITERATIONS(res);
		long double *cplane = asm_fet(cn, 8*width, 8*height, zoom, it);

		res = (zoom * (8*length-1)) / 4;
		long double xs = creal(cn) - (8*width-1) / (2 * res);
		long double ys = cimag(cn) - (8*height-1) / (2 * res);
		//long double xe = xs + 8*width/res;
		//long double ye = ys + 8*height/res;
		long double z2=z;
		z=0;
		long double cnx = creal(cn);
		long double cny = cimag(cn);
		long double mx1 = (cnx-xs)*res;
		long double my1 = (cny-ys)*res;
		while(z<1) {
			zoom=exp2l(z+z2);
			printf("Zoom: 2^%.4g\t", (double)(z+z2));

			d=1/(zoom*zoom);

			long double cx = ((1-d)*creal(center)-0.5*d);
			long double cy = ((1-d)*cimag(center));
			long double mx2 = (cx-xs)*res;
			long double my2 = (cy-ys)*res;
			printf("center: (%.10g,%.10g)\n",(double)cx,(double)cy);

			int x, y;
			long double zfactor = exp2l(-z);
			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					long double v = antialias(cplane,(8*x-mx1)*zfactor+mx2,(8*y-my1)*zfactor+my2,8*zfactor,8*width,8*height);

					if (v >= it) {
						cairo_set_source_rgb(cr, 0, 0, 0);
					} else {
						cairo_set_color_from_gradient(cr,gr,logl(1+mf*v)*s *400+gr->offset);
					}

					cairo_rectangle(cr, x, height-y-1, 1, 1);
					cairo_fill(cr);

				}
			}

			sprintf(filename, fmt, i);
			cairo_status_t st = cairo_surface_write_to_png(surface, filename);

			i++;
			z += m;
		}
		free(cplane);
		z=z+z2;
	}
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
}

void juliamorph(int argc, char **argv)
{
	//usage: fractaltool juliamorph prefix start_re start_im end_re end_im num_frames width, height
	char* prefix = argv[2];
	long double complex s;
	long double complex e;
	int n_frames= 300;
	int width = 800;
	int height = 600;
	char *fmt;
	int i=2;
	char *a;
	while(i<argc)
	{
		a=argv[i];
		if(strncmp("-g", a,2)==0)
		{
			if(strlen(a)>2)
			{
				a+=2;
				gr=load_gradient_from_file(a);
			}else{
				a=argv[++i];
				gr=load_gradient_from_file(a);
			}
		} else if(strncmp("-d", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				gr->factor=atof(a);
			}else{
				a=argv[++i];
				gr->factor=atof(a);
			}
		} else if(strncmp("-O", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				gr->offset=atof(a);
			}else{
				a=argv[++i];
				gr->offset=atof(a);
			}
		} else if(strncmp("-n", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				n_frames=(int)atof(a);
			}else{
				a=argv[++i];
				n_frames=(int)atof(a);
			}
		} else if(strncmp("-b", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				char *s2=splitstr(a,',');
				if(s2!=NULL)
				{
					s=atof(a)+atof(s2)*I;
				}else{
					jmusage();
					return;
				}
			}else{
				a=argv[++i];
				char *s2=splitstr(a,',');
				if(s2!=NULL)
				{
					s=atof(a)+atof(s2)*I;
				}else{
					jmusage();
					return;
				}
			}
		} else if(strncmp("-e", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				char *s2=splitstr(a,',');
				if(s2!=NULL)
				{
					e=atof(a)+atof(s2)*I;
				}else{
					jmusage();
					return;
				}
			}else{
				a=argv[++i];
				char *s2=splitstr(a,',');
				if(s2!=NULL)
				{
					e=atof(a)+atof(s2)*I;
				}else{
					jmusage();
					return;
				}
			}
		} else if(strncmp("-o", a,2)==0) {
			if(strlen(a)>2)
			{
				fmt=a+2;
			}else{
				fmt=argv[++i];
			}
		}else if(strncmp("-r", a,2)==0) {
			if(strlen(a)>2)
			{
				a+=2;
				char *s2=splitstr(a,'x');
				if(s2!=NULL)
				{
					width=atof(a);
					height=atof(s2);
				}else{
					jmusage();
					return;
				}
			}else{
				a=argv[++i];
				char *s2=splitstr(a,'x');
				if(s2!=NULL)
				{
					width=atof(a);
					height=atof(s2);
				}else{
					jmusage();
					return;
				}
			}
		}else{
			jmusage();
			return;
		}
		i++;
	}
	char* filename=malloc(200);
	int n;
	long double res;
	for(n=0;n<=n_frames;n++)
	{
				sprintf(filename, fmt, n);
				res = MIN(width,height) / 4;
				julia(filename,0,s+n*(e-s)/n_frames,1,ITERATIONS(res), width, height);

	}

}

void mandel(char *filename, long double complex center, long double zoom, int it, int width, int height)
{
	long double *cplane = asm_fet(center, 4*width, 4*height, zoom, it);
	cairo_surface_t *surface;
	cairo_t *cr;

	surface =
	    cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
	cr = cairo_create(surface);
	int x, y;
	int s = SIGN(gr->factor);
	double a=fabs(gr->factor);
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			long double v = antialias(cplane,4*x,4*y,4,4*width,4*height);

			if (v >= it || v<0) {
				cairo_set_source_rgb(cr, 0, 0, 0);
			} else {
				cairo_set_color_from_gradient(cr,gr,logl(1+a*v)*s *400+gr->offset);
			}

			cairo_rectangle(cr, x, height-y, 1, 1);
			cairo_fill(cr);

		}
	}
	free(cplane);
	cairo_destroy(cr);
	cairo_status_t st = cairo_surface_write_to_png(surface, filename);
	cairo_surface_destroy(surface);

}

void julia(char *filename, long double complex center, long double complex seed, long double zoom, int it, int width, int height)
{
	long double *cplane = julia_asm_fet(center,seed, 4*width, 4*height, zoom, it);
	cairo_surface_t *surface;
	cairo_t *cr;

	surface =
	    cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
	cr = cairo_create(surface);
	int x, y;
	int s = SIGN(gr->factor);
	double a=fabs(gr->factor);
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			long double v = antialias(cplane,4*x,4*y,4,4*width,4*height);

			if (v >= it || v<0) {
				cairo_set_source_rgb(cr, 0, 0, 0);
			} else {
				cairo_set_color_from_gradient(cr,gr,logl(1+a*v)*s *400+gr->offset);
			}

			cairo_rectangle(cr, x, height-y, 1, 1);
			cairo_fill(cr);

		}
	}
	free(cplane);
	cairo_destroy(cr);
	cairo_status_t st = cairo_surface_write_to_png(surface, filename);
	cairo_surface_destroy(surface);

}

void usage() {
		printf("Available commands are:\n");
		printf("\tmandel    \tRender single image of the Mandelbrotset\n");
		printf("\tjulia     \tRender single image of a Juliaset\n");
		printf("\tmandelzoom\tRender set of images zooming into the Mandelbrotset\n");
		printf("\tjuliamorph\tRender set of images of Juliasets with gradually changing parameter\n");

}
int main(int argc, char **argv)
{
	gr = load_std_gradient();
	if(argc < 2) {
		usage();
		exit(EXIT_FAILURE);
	}

	if(!strcmp(argv[1], "mandelzoom"))
	{
		mandelzoom(argc, argv);
	}else if(!strcmp(argv[1], "juliamorph")){
		juliamorph(argc, argv);
	}else if(!strcmp(argv[1], "mandel")){
		mandelsingle(argc, argv);
	}else if(!strcmp(argv[1], "julia")){
		juliasingle(argc, argv);
	}else{
		usage();
	}

	return 0;
}


