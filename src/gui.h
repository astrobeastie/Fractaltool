#ifndef GUI_H
#define GUI_H
GtkWindow *window;
GtkWidget *btn_redraw;
GtkWidget *panel;
GtkAdjustment *adj_re;
GtkAdjustment *adj_im;
GtkAdjustment *adj_zoom;
GtkAdjustment *adj_factor;
GtkAdjustment *adj_offset;
GtkToggleButton *logplot;
GtkToggleButton *tg_julia;
GtkComboBoxText *cb_algo;
GtkWidget *btn_reset;

void create_gui();
gboolean delete_event(GtkWidget * widget, GdkEvent * event, gpointer data);
void destroy(GtkWidget * widget, gpointer data);
#endif
