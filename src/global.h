#ifndef GLOBAL_H
#define GLOBAL_H
#include <complex.h>
void on_redraw_clicked(GtkWidget * widget, gpointer data);
void reset(GtkWidget * widget, gpointer data);

gboolean draw_callback(GtkWidget * widget, cairo_t * cr, gpointer data);
gboolean on_panel_mouse_down(GtkWidget * widget, GdkEventButton * event,
			     gpointer user_data);
gboolean on_panel_mouse_move(GtkWidget * widget, GdkEventMotion * event,
			     gpointer user_data);
gboolean on_panel_mouse_up(GtkWidget * widget, GdkEventButton * event,
			   gpointer user_data);
gboolean on_panel_scroll(GtkWidget * widget, GdkEventScroll * event,
			 gpointer user_data);
			 
typedef struct {
	long double *data;
	int mode;
	double factor;
	double offset;
}	RenderData;

typedef struct {
	long double complex center;
	long double zoom;
	int algo;
	gboolean julia;
	long double complex jseed;
}	CalcData;

static void *calculate(CalcData *cd);
static void *render(RenderData *rd);
#endif
