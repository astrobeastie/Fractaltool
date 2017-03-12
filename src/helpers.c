#include "helpers.h"




void cairo_set_source_hsv(cairo_t * cr, double h, double s, double v)
{
	double c = v * s / 10000;
	double x = c * (1 - fabs(fmod(3 * h / PI, 2) - 1));
	double m = v / 100 - c;
	double r, g, b;
	r = 0;
	g = 0, b = 0;
	if (0 <= h && h < PI / 3) {
		r = c + m;
		g = x + m;
		b = m;
	}
	if (PI / 3 <= h && h < 2 * PI / 3) {
		r = x + m;
		g = c + m;
		b = m;
	}
	if (2 * PI / 3 <= h && h < PI) {
		r = m;
		g = c + m;
		b = x + m;
	}
	if (PI <= h && h < 4 * PI / 3) {
		r = m;
		g = x + m;
		b = c + m;
	}
	if (4 * PI / 3 <= h && h < 5 * PI / 3) {
		r = x + m;
		g = m;
		b = c + m;
	}
	if (5 * PI / 3 <= h && h < 2 * PI) {
		r = c + m;
		g = m;
		b = x + m;
	}

	cairo_set_source_rgb(cr, r, g, b);

}

long double *mr=NULL;
long double *mg=NULL;
long double *mb=NULL;
void init_gradient(gradient *gr)
{
	if(mr!=NULL);
		free(mr);
	if(mg!=NULL);
		free(mg);
	if(mb!=NULL);
		free(mb);
	long double *dr =malloc(gr->n*sizeof(long double));
	long double *dg =malloc(gr->n*sizeof(long double));
	long double *db =malloc(gr->n*sizeof(long double));
	mr=malloc(gr->n*sizeof(long double));
	mg=malloc(gr->n*sizeof(long double));
	mb=malloc(gr->n*sizeof(long double));
	int i,j;
	for(i=0;i<gr->n;i++)
	{
		int j=i+1;
		dr[i]=(gr->r[j]-gr->r[i])/(gr->p[j]-gr->p[i]);
		dg[i]=(gr->g[j]-gr->g[i])/(gr->p[j]-gr->p[i]);
		db[i]=(gr->b[j]-gr->b[i])/(gr->p[j]-gr->p[i]);
	//	printf("i: %d j: %d a: %.3g b: %.3g d: %.3g\n",i,j,gr->r[i],gr->r[j],(double)dr[i]);
	}
	dr[gr->n-1]=(gr->r[0]-gr->r[gr->n-1])/(400-gr->p[gr->n-1]);
	dg[gr->n-1]=(gr->g[0]-gr->g[gr->n-1])/(400-gr->p[gr->n-1]);
	db[gr->n-1]=(gr->b[0]-gr->b[gr->n-1])/(400-gr->p[gr->n-1]);
	for(i=0;i<gr->n;i++)
	{
		if(SIGN(dr[i])*SIGN(dr[(i+1)%gr->n])==-1)
		{
			mr[(i+1)%gr->n]=0;
			//printf("sign: %d\n",i);
		}else{
			mr[(i+1)%gr->n]=(dr[i]+dr[(i+1)%gr->n])/2;
		}
		if(SIGN(dg[i])*SIGN(dg[(i+1)%gr->n])==-1)
		{
			mg[(i+1)%gr->n]=0;
		}else{
			mg[(i+1)%gr->n]=(dg[i]+dg[(i+1)%gr->n])/2;
		}
		if(SIGN(db[i])*SIGN(db[(i+1)%gr->n])==-1)
		{
			mb[(i+1)%gr->n]=0;
		}else{
			mb[(i+1)%gr->n]=(db[i]+db[(i+1)%gr->n])/2;
		}
	}
	long double ar=mr[0]/dr[0];
		long double ag=mg[0]/dg[0];
		long double ab=mb[0]/db[0];
		
		long double br=mr[0]/dr[gr->n-1];
		long double bg=mg[0]/dg[gr->n-1];
		long double bb=mb[0]/db[gr->n-1];
		
		if(ar<0 || br < 0)
			mr[gr->n-1]=0;
		if(ab<0 || bb < 0)
			mb[gr->n-1]=0;
		if(ag<0 || bg < 0)
			mg[gr->n-1]=0;
	for(i=1;i<gr->n;i++)
	{
		ar=mr[i]/dr[i];
		ag=mg[i]/dg[i];
		ab=mb[i]/db[i];
		
		br=mr[i]/dr[i-1];
		bg=mg[i]/dg[i-1];
		bb=mb[i]/db[i-1];
		
		if(ar<0 || br < 0)
			mr[i]=0;
		if(ab<0 || bb < 0)
			mb[i]=0;
		if(ag<0 || bg < 0)
			mg[i]=0;
	 
	}
		
	for(i=0;i<gr->n-1;i++)
	{
	  ar=mr[i]/dr[i];
		ag=mg[i]/dg[i];
		ab=mb[i]/db[i];
		
		br=mr[i+1]/dr[i];
		bg=mg[i+1]/dg[i];
		bb=mb[i+1]/db[i];
		if(ar*ar+br*br>9)
		{
			long double t = 3l/sqrtl(ar*ar+br*br);
			mr[i]=t*ar*dr[i];
			mr[i+1]=t*br*dr[i];
		}
		if(ag*ag+bg*bg>9)
		{
			long double t = 3l/sqrtl(ag*ag+bg*bg);
			mg[i]=t*ag*dg[i];
			mg[i+1]=t*bg*dg[i];
		}
		if(ab*ab+bb*bb>9)
		{
			long double t = 3l/sqrtl(ab*ab+bb*bb);
			mb[i]=t*ab*db[i];
			mb[i+1]=t*bb*db[i];
		}
	}
		ar=mr[gr->n-1]/dr[gr->n-1];
		ag=mg[gr->n-1]/dg[gr->n-1];
		ab=mb[gr->n-1]/db[gr->n-1];
		
		br=mr[0]/dr[gr->n-1];
		bg=mg[0]/dg[gr->n-1];
		bb=mb[0]/db[gr->n-1];
		
			if(ar*ar+br*br>9)
		{
			long double t = 3l/sqrtl(ar*ar+br*br);
			mr[gr->n-1]=t*ar*dr[gr->n-1];
			mr[0]=t*br*dr[gr->n-1];
		}
		if(ag*ag+bg*bg>9)
		{
			long double t = 3l/sqrtl(ag*ag+bg*bg);
			mg[gr->n-1]=t*ag*dg[gr->n-1];
			mg[0]=t*bg*dg[gr->n-1];
		}
		if(ab*ab+bb*bb>9)
		{
			long double t = 3l/sqrtl(ab*ab+bb*bb);
			mb[gr->n-1]=t*ab*db[gr->n-1];
			mb[0]=t*bb*db[gr->n-1];
		}
	/*mr[0]=(dr[gr->n-1]+dr[0])/2;
	mg[0]=(dg[gr->n-1]+dg[0])/2;
	mb[0]=(db[gr->n-1]+db[0])/2;*/
	free(dr);
	free(db);
	free(dg);
	//printf("mr4: %.3g\n", (double)mr[4]);
}

void unload_gradient()
{
	free(mr);
	free(mg);
	free(mb);
}

void gradient_add_stop(gradient *gr, double p, double r, double g, double b)
{
		double *p_new=malloc((gr->n+1)*sizeof(double));
		double *r_new=malloc((gr->n+1)*sizeof(double));
		double *g_new=malloc((gr->n+1)*sizeof(double));
		double *b_new=malloc((gr->n+1)*sizeof(double));
		
		int i=0;
		while(i<gr->n && gr->p[i]<p)
		{
			p_new[i]=gr->p[i];
			r_new[i]=gr->r[i];
			g_new[i]=gr->g[i];
			b_new[i]=gr->b[i];
			i++;
		}
		p_new[i]=p;
		r_new[i]=r;
		g_new[i]=g;
		b_new[i]=b;
		i++;
		while(i<gr->n+1)
		{
			p_new[i]=gr->p[i-1];
			r_new[i]=gr->r[i-1];
			g_new[i]=gr->g[i-1];
			b_new[i]=gr->b[i-1];
			i++;
		}
		if(gr->n>0)
		{
			free(gr->p);
			free(gr->r);
			free(gr->g);
			free(gr->b);
		}
		gr->n++;
		gr->p=p_new;
		gr->r=r_new;
		gr->g=g_new;
		gr->b=b_new;
}
void strip_whitespace(char* source)
{
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ' && *i!='\t')
      i++;
  }
  *i = 0;
}
int gethexdigit(char c)
{
	if(c>='0' && c<='9')
		return c-'0';
	else if(c>='A' && c<='F')
		return c-'A'+10;
	else if(c>='a' && c<='f')
		return c-'a'+10;
	else
		return -1;
}
gradient *load_gradient_from_file(char* filename)
{
	gradient *gr=malloc(sizeof(gradient));
	gr->n=0;
	FILE *fp =fopen(filename,"r");
	char line [128];
	while(fgets(line, 128, fp)!=NULL)
	{
		strip_whitespace(line);
		if(strncmp("factor:", line, 7)==0)
		{
			gr->factor = atof(&(line[7]));
		}else if(strncmp("offset:", line, 7)==0) {
			gr->offset = atof(&(line[7]));
		}else{
			double p=0;
			double r=0;
			double g=0;
			double b=0;
			char c=line[0];
			int i=0;
			while(i<128 && c >= '0' && c<='9')
			{
				p=10*p+c-'0';
				i++;
				c=line[i];
			}
			if(i==0)
			{
				printf("error parsing line: %s\n ignoring\n",line);
				continue;
			}
			if(c=='.')
			{
				i++;
				c=line[i];
				int j=10;
				while(i<128 && c >= '0' && c<='9')
				{
					p+=(c-'0')/j;
					i++;
					j*=10;
					c=line[i];
				}
			}
			if(c==':')
			{
				
				int r1, r2, g1, g2, b1,b2;
				i++;
				r1=gethexdigit(line[i++]);
				r2=gethexdigit(line[i++]);
				g1=gethexdigit(line[i++]);
				g2=gethexdigit(line[i++]);
				b1=gethexdigit(line[i++]);
				b2=gethexdigit(line[i++]);
				if(r1>=0 && r2>=0 && g1>=0 && g2>=0 && b1>=0 && b2>=0 )
				{
					r=16*r1+r2;
					g=16*g1+g2;
					b=16*b1+b2;
					gradient_add_stop(gr,p,r,g,b);
				}else{
					printf("error parsing line: %s\n ignoring\n",line);
					continue;
				}
			}else{
				printf("error parsing line: %s\n ignoring\n",line);
			}
		}
	}
	init_gradient(gr);
	return gr;
}
gradient *load_std_gradient()
{
	gradient *gr=malloc(sizeof(gradient));
	gr->n=5;
	gr->r=malloc(5*sizeof(double));
	gr->g=malloc(5*sizeof(double));
	gr->b=malloc(5*sizeof(double));
	gr->p=malloc(5*sizeof(double));
	
	gr->r[0]=0;
	gr->g[0]=7;
	gr->b[0]=100;
	gr->p[0]=0;
	
	gr->r[1]=32;
	gr->g[1]=107;
	gr->b[1]=203;
	gr->p[1]=64;
	
	gr->r[2]=237;
	gr->g[2]=255;
	gr->b[2]=255;
	gr->p[2]=168;
	
	gr->r[3]=255;
	gr->g[3]=170;
	gr->b[3]=0;
	gr->p[3]=257;
	
	gr->r[4]=49;
	gr->g[4]=2;
	gr->b[4]=48;
	gr->p[4]=343;
	gr->factor=0.42;
	gr->offset=330.0;
	init_gradient(gr);
	
	return gr;
	
}

void free_gradient(gradient *gr)
{
		free(gr->r);
		free(gr->g);
		free(gr->b);
		free(gr->p);
		free(gr);
}
void cairo_set_color_from_gradient(cairo_t * cr, gradient *gr, long double f)
{
	//if (f < -360)
	//	f = -360;
	
	f = fmodl(f, 400);
	if(f<0)
		f=400-abs(f);
	
	long double r=0, g=0, b=0;
	int i;
	//int found=0;
	for(i=0;i<gr->n;i++)
	{
		if(i<gr->n-1)
		{
			if(f >= gr->p[i] && f<gr->p[i+1])
			{
		//	found=1;
			r=interpolate(f,gr->p[i],gr->p[i+1],gr->r[i],gr->r[i+1],mr[i],mr[i+1]);
				g=interpolate(f,gr->p[i],gr->p[i+1],gr->g[i],gr->g[i+1],mg[i],mg[i+1]);
				b=interpolate(f,gr->p[i],gr->p[i+1],gr->b[i],gr->b[i+1],mb[i],mb[i+1]);
				
				break;
			}
		}else{
			if(f >=gr->p[i])
			{
		//	found=1;
			r=interpolate(f,gr->p[i],gr->p[0]+400,gr->r[i],gr->r[0],mr[i],mr[0]);
				g=interpolate(f,gr->p[i],gr->p[0]+400,gr->g[i],gr->g[0],mg[i],mg[0]);
				b=interpolate(f,gr->p[i],gr->p[0]+400,gr->b[i],gr->b[0],mb[i],mb[0]);
				
				break;
			}
		}
	}
//	if(!found)
	//	printf("not found: %.3g\n", (double)f);
	cairo_set_source_rgb(cr, r/255 , g/255 , b/255 );
}
/* CUBIC INTERPOLATION */
#define H_00(t)	(2*(t)*(t)*(t)-3*(t)*(t)+1)		// 2t³-3t²+1
#define H_10(t) ((t)*(t)*(t)-2*(t)*(t)+(t))		// t³-2t²+t
#define H_01(t) (-2*(t)*(t)*(t)+3*(t)*(t))		// -2t³+3t²
#define H_11(t) ((t)*(t)*(t)-(t)*(t))					// t³-t²

long double interpolate(long double x, long double x1, long double x2, long double y1, long double y2, long double m1, long double m2)
{
	long double dx=x2-x1;
	long double t=(x-x1)/dx;
	
	long double r= H_00(t)*y1+H_10(t)*dx*m1+H_01(t)*y2+H_11(t)*dx*m2;
	/*if(r>255 || r<0)
	{
			printf("x1: %.3g x2: %.3g y1: %.3g y2: %.3g r: %.3g \n", (double)x1,(double)x2,(double)y1,(double)y2,(double)r);
	}*/
	//if(r>255) r=255;
	//if(r<0) r=0;
	return r;
}
int same_neighbours(long double *cplane,int x, int y, int chunksize, int width, int height){
	if(x==0 || y==0)
		return 0;
	if(x+chunksize >= width || y+chunksize >= height)
		return 0;
	if(cplane[y*width+x]==cplane[(y-chunksize)*width+x-chunksize]) {
		if(cplane[y*width+x]==cplane[(y-chunksize)*width+x]) {
			if(cplane[y*width+x]==cplane[(y-chunksize)*width+x+chunksize]) {
				if(cplane[y*width+x-chunksize]) {
					if(cplane[y*width+x]==cplane[y*width+x+chunksize]) {
						if(cplane[y*width+x]==cplane[(y+chunksize)*width+x-chunksize]) {
							if(cplane[y*width+x]==cplane[(y+chunksize)*width+x]) {
								if(cplane[y*width+x]==cplane[(y+chunksize)*width+x+chunksize]) {
									return 1;
								}
							}
						}
					}
				}
			}
		}
	}
	return 0;
}
