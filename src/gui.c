#include <gtk/gtk.h>
#include "global.h"
#include "gui.h"

void create_gui()
{
	GtkBuilder *builder;
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "./gui.glade", NULL);
	window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
	panel = GTK_WIDGET(gtk_builder_get_object(builder, "panel"));
	btn_reset = GTK_WIDGET(gtk_builder_get_object(builder, "btn_reset"));
	btn_redraw = GTK_WIDGET(gtk_builder_get_object(builder, "bn_redraw"));
	adj_im = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_im"));
	adj_re = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_re"));
	adj_zoom = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_zoom"));
	adj_factor =
	    GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_factor"));
	adj_offset =
	    GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_offset"));
	logplot = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "logplot"));
	tg_julia = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "tg_julia"));
	cb_algo =
	    GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "cb_algo"));
	gtk_builder_connect_signals(builder, NULL);

	g_object_unref(G_OBJECT(builder));

	gtk_widget_show_all(GTK_WIDGET(window));
}

void destroy(GtkWidget * widget, gpointer data)
{
	gtk_main_quit();
}

gboolean delete_event(GtkWidget * widget, GdkEvent * event, gpointer data)
{
	/* If you return FALSE in the "delete-event" signal handler,
	 * GTK will emit the "destroy" signal. Returning TRUE means
	 * you don't want the window to be destroyed.
	 * This is useful for popping up 'are you sure you want to quit?'
	 * type dialogs. */

	// g_print ("delete event occurred\n");

	/* Change TRUE to FALSE and the main window will be destroyed with
	 * a "delete-event". */

	return FALSE;
}
