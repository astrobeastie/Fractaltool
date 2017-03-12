#include <gtk/gtk.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <cairo-svg.h>
#include <string.h>
#include "global.h"
#include "gui.h"
#include "mandelbrot.h"
#include "julia.h"
#include "helpers.h"

#define WIDTH			550
#define HEIGHT		550
#define LIMIT     16
#define PI				3.141592654
#define GOLDEN_ANGLE 	PI*(3-sqrt(5))
//#define MIN(a,b)  (((a)<(b))?(a):(b))
//#define ITERATIONS(z) (MIN((150*pow((z),1.0/3)),50000))


long double complex center;
gboolean julia = FALSE;
long double zoom = 4;
void rst();
void next_iteration();
int n = 0;
GMutex datamtx;
long double *cplane=NULL;
cairo_surface_t *cst;
gradient *gr;
gboolean draw_callback(GtkWidget * widget, cairo_t * cr, gpointer data)
{
	
	cairo_set_source_surface(cr,cst,0,0);
	cairo_paint(cr);
	
}
static gboolean terminate (GThread *thread)
{
  g_thread_join (thread);

  return FALSE;
}
static gboolean calc_done(long double *data)
{
	RenderData *rd=malloc(sizeof(RenderData));
	rd->data = data;
	
	rd->offset=gtk_adjustment_get_value(adj_offset);
	rd->factor=gtk_adjustment_get_value(adj_factor);
	if(gtk_toggle_button_get_active(logplot))
	{
		if(gtk_combo_box_get_active(GTK_COMBO_BOX(cb_algo))==3)
		{
			rd->mode=2;
		}else{
			rd->mode=1;
		}
	}else{
		rd->mode=0;
	}
	//rd.cst=cst;
	//printf("Calc Done 1\n");
	g_thread_new ("render", (GThreadFunc)render, rd);
	//printf("Calc Done 2\n");
	g_mutex_lock(&datamtx);
	cplane=data;
	g_mutex_unlock(&datamtx);
	return FALSE;
}
static gboolean render_done(cairo_surface_t *s)
{
	cairo_surface_destroy(cst);
	cst=s;
	gtk_widget_queue_draw(panel);
	//printf("Render Done\n");
	return FALSE;
}
static void *calculate(CalcData *cd)
{
	int n = ITERATIONS(cd->zoom);
	long double * data;
	if(cd->julia)
	{
		switch (cd->algo) {
	case 0:
		data = julia_escape_time(cd->center,cd->jseed, WIDTH, HEIGHT, cd->zoom, n);
		break;
	case 1:
		data = julia_frac_escape_time(cd->center,cd->jseed, WIDTH, HEIGHT, cd->zoom, n);
		break;
	case 2:
		data = julia_asm_fet(cd->center,cd->jseed, WIDTH, HEIGHT, cd->zoom, n);
		break;
	default:
		data = julia_exterior_distance(cd->center,cd->jseed, WIDTH, HEIGHT, cd->zoom, n);
	}
	}else{
	switch (cd->algo) {
	case 0:
		data = escape_time(cd->center, WIDTH, HEIGHT, cd->zoom, n);
		break;
	case 1:
		data = frac_escape_time(cd->center, WIDTH, HEIGHT, cd->zoom, n);
		break;
	case 2:
		data = asm_fet(cd->center, WIDTH, HEIGHT, cd->zoom, n);
		break;
	default:
		data = exterior_distance(cd->center, WIDTH, HEIGHT, cd->zoom, n);
	}
	}
	free(cd);
	gdk_threads_add_idle ((GSourceFunc)calc_done, data);
	gdk_threads_add_idle ((GSourceFunc)terminate, g_thread_self ());
	return NULL;
}
static void *render(RenderData *rd)
{
		cairo_surface_t *ncst=cairo_image_surface_create (CAIRO_FORMAT_ARGB32,WIDTH,HEIGHT);
		//printf("Rendering 1\n");
		cairo_t *cr = cairo_create(ncst);
		//		printf("Rendering 2\n");
		g_mutex_lock(&datamtx);
		long double *data = rd->data;
		if (data != NULL) {
		int x, y;

		for (x = 0; x < WIDTH; x++) {
		cairo_set_color_from_gradient
						    (cr,gr, x * rd->factor*4 + rd->offset);
			for (y = 0; y < HEIGHT; y++) {
				long double v = data[x + y * WIDTH];
				if (v == -1) {
					cairo_set_source_rgb(cr, 0, 0, 0);
				} else {
					switch (rd->mode)
					{
					case 1:
						cairo_set_color_from_gradient
						    (cr,gr,
						     logl(v * fabs(rd->factor)+1)*400*SIGN(rd->factor) + rd->offset);
						break;
					case 2:
						cairo_set_color_from_gradient
						    (cr,gr,logl(v)*40*rd->factor + rd->offset);
						break;
					default:
						cairo_set_color_from_gradient
						    (cr,gr, v * rd->factor*400 + rd->offset);
				}
				}
				cairo_rectangle(cr, x, HEIGHT - y, 1, 1);
				cairo_fill(cr);
				}
		}
	}
	  free(rd);
		g_mutex_unlock(&datamtx);
		cairo_destroy(cr);
		gdk_threads_add_idle ((GSourceFunc)render_done, ncst);
		gdk_threads_add_idle ((GSourceFunc)terminate, g_thread_self ());
		return NULL;
}

void redraw()
{
	if(cplane != NULL)
	{
		RenderData *rd=malloc(sizeof(RenderData));
		rd->data = cplane;
		rd->offset=gtk_adjustment_get_value(adj_offset);
		rd->factor=gtk_adjustment_get_value(adj_factor);
		if(gtk_toggle_button_get_active(logplot))
	{
		if(gtk_combo_box_get_active(GTK_COMBO_BOX(cb_algo))==3)
		{
			rd->mode=2;
		}else{
			rd->mode=1;
		}
	}else{
		rd->mode=0;
	}
		g_thread_new ("render", (GThreadFunc)render, rd);
	}
}
void recalc()
{
	CalcData *cd=malloc(sizeof(CalcData));
	cd->center=gtk_adjustment_get_value(adj_re)+gtk_adjustment_get_value(adj_im)*I;
	cd->zoom=powl(2,gtk_adjustment_get_value(adj_zoom));
	cd->algo=gtk_combo_box_get_active(GTK_COMBO_BOX(cb_algo));
	cd->julia=julia;
	cd->jseed=center;
	g_thread_new ("calc", (GThreadFunc)calculate, cd);
}
void on_redraw_clicked(GtkWidget * widget, gpointer data)
{
	//center=gtk_adjustment_get_value(adj_re)+gtk_adjustment_get_value(adj_im)*I;
	//zoom=powl(2,gtk_adjustment_get_value(adj_zoom));
	recalc();
}
int lg=0;
void choose_gradient(GtkWidget * widget, gpointer data)
{
	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;

	dialog = gtk_file_chooser_dialog_new ("Open File",
	                                      window,
	                                      action,
	                                      "_Cancel",
	                                      GTK_RESPONSE_CANCEL,
	                                      "_Open",
	                                      GTK_RESPONSE_ACCEPT,
	                                      NULL);
	GtkFileFilter *filter = gtk_file_filter_new ();
	gtk_file_filter_add_pattern (filter, "*.gr");
	gtk_file_chooser_add_filter (dialog, filter);

	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT)
	  {
	    char *filename;
	    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
	    filename = gtk_file_chooser_get_filename (chooser);
	    gr=load_gradient_from_file(filename);
	    lg=1;
	    gtk_adjustment_set_value(adj_offset,gr->offset);
			gtk_adjustment_set_value(adj_factor,gr->factor);
	    redraw();
	    lg=0;
	  }

	gtk_widget_destroy (dialog);
	
}
void density_changed(GtkWidget * widget, gpointer data)
{
 	if(lg==0)
		redraw();
}

void offset_changed(GtkWidget * widget, gpointer data)
{
	if(lg==0)
		redraw();
}

void log_changed(GtkWidget * widget, gpointer data)
{
	redraw();
}

void reset(GtkWidget * widget, gpointer data)
{
	rst();
	gtk_widget_queue_draw(panel);
}

void rst()
{
	if(julia)
	{
		gtk_adjustment_set_value(adj_re, 0);
		gtk_adjustment_set_value(adj_im, 0);
	}else{
		gtk_adjustment_set_value(adj_re, -0.74364388703715900153);
		gtk_adjustment_set_value(adj_im, 0.13182590420532999675);
	}
	gtk_adjustment_set_value(adj_zoom, 0);
	//zoom=1;
	//center=-0.743643887037151+0.131825904205330*I;
	recalc();
	//memset(cplane, 0, sizeof(int) * WIDTH *HEIGHT);

	//iteration(n);

}

int main(int argc, char *argv[])
{

	/* This is called in all GTK applications. Arguments are parsed
	 * from the command line and are returned to the application. */

	//cplane = malloc(sizeof(double) * WIDTH * HEIGHT);
	//hist = malloc(30*sizeof(int));
	//printf("Mallocd\n");
	gr=load_std_gradient();
	cst=cairo_image_surface_create (CAIRO_FORMAT_ARGB32,WIDTH,HEIGHT);
	gtk_init(&argc, &argv);
	create_gui();
	gtk_adjustment_set_value(adj_offset,gr->offset);
	gtk_adjustment_set_value(adj_factor,gr->factor);
	rst();
	/* All GTK applications must have a gtk_main(). Control ends here
	 * and waits for an event to occur (like a key press or
	 * mouse event). */

	gtk_main();
	
	free(cplane);
	cairo_surface_destroy(cst);
	free_gradient(gr);
//      free(hist);
	return 0;
}
void mode_changed(GtkToggleButton *togglebutton, gpointer user_data)
{
		if(julia){
			gtk_adjustment_set_value(adj_re,creall(center));
			gtk_adjustment_set_value(adj_im,cimagl(center));
			gtk_adjustment_set_value(adj_zoom,zoom);
			julia=FALSE;
		}else{
			center=gtk_adjustment_get_value(adj_re)+gtk_adjustment_get_value(adj_im)*I;
			zoom=gtk_adjustment_get_value(adj_zoom);
			gtk_adjustment_set_value(adj_re,0);
			gtk_adjustment_set_value(adj_im,0);
			gtk_adjustment_set_value(adj_zoom,0);
			julia=TRUE;
		}
		recalc();
}
int md = 0;
gboolean on_panel_mouse_down(GtkWidget * widget, GdkEventButton * event,
			     gpointer user_data)
{
	md = 1;
}

double pointer_x, pointer_y;
gboolean on_panel_mouse_move(GtkWidget * widget, GdkEventMotion * event,
			     gpointer user_data)
{
	pointer_x = event->x;
	pointer_y = event->y;
	
		//memset(cplane, 0, sizeof(int) * WIDTH *HEIGHT);
		//iteration(no);
		gtk_widget_queue_draw(panel);

}

gboolean on_panel_mouse_up(GtkWidget * widget, GdkEventButton * event,
			   gpointer user_data)
{
	if(md==1)
	{
			long double c_re=	gtk_adjustment_get_value(adj_re);
	long double c_im=	gtk_adjustment_get_value(adj_im);
	long double z=gtk_adjustment_get_value(adj_zoom);
	long double zp=powl(2,z);
	long double xn = c_re - 2 / zp + pointer_x / (WIDTH * zp / 4);
	long double yn = c_im + 2 / zp - pointer_y / (HEIGHT * zp / 4);
		gtk_adjustment_set_value(adj_re, xn);
		gtk_adjustment_set_value(adj_im, yn);
		recalc();
	}
	md = 0;

}

gboolean on_panel_scroll(GtkWidget * widget, GdkEventScroll * event,
			 gpointer user_data)
{
	long double c_re=	gtk_adjustment_get_value(adj_re);
	long double c_im=	gtk_adjustment_get_value(adj_im);
	long double z=gtk_adjustment_get_value(adj_zoom);
	long double zp=powl(2,z);
	long double xn = c_re - 2 / zp + pointer_x / (WIDTH * zp / 4);
	long double yn = c_im + 2 / zp - pointer_y / (HEIGHT * zp / 4);

	

	switch (event->direction) {
	case GDK_SCROLL_UP:
		z++;

		break;
	case GDK_SCROLL_DOWN:
		z--;

		break;
	default:
		break;
	}

	//printf("Center: %.20g + %.20g * i\t\t\t Zoom: 2^%.3g\n", xn,yn, log(zoom)/log(2));
	n = ITERATIONS(zoom);
	//memset(plane, 0, sizeof(double complex) * WIDTH * HEIGHT);
	//memset(cplane, 0, sizeof(int) * WIDTH *HEIGHT);
	//iteration(nn);
	gtk_adjustment_set_value(adj_re, xn);
	gtk_adjustment_set_value(adj_im, yn);
	gtk_adjustment_set_value(adj_zoom, z);
	recalc();

}

