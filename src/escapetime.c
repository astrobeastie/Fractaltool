gboolean draw_callback(GtkWidget * widget, cairo_t * cr, gpointer data)
{
	guint width, height;
	GdkRGBA color;
	width = gtk_widget_get_allocated_width(widget);
	height = gtk_widget_get_allocated_height(widget);
	double x, y;
	int idx = 0;
	double hue = 3 * PI / 2;
	int i;
	int sum = 0;
	for (i = sn; i < n; i++)
		sum += hist[i];
	for (i = sn; i <= n; i++) {
		if (i == n) {
			cairo_set_source_rgb(cr, 0, 0, 0);
		} else {
			cairo_set_source_hsv(cr, hue, 100, 90);
			hue = 4 * PI * pow((double)hist[i] / sum, 0.25);
			hue = fmod(hue, 2 * PI);
		}
		idx = 0;
		for (x = 0; x < WIDTH; x++) {

			for (y = 0; y < HEIGHT; y++) {

				if (cplane[idx] == i) {

					cairo_rectangle(cr, x,
							HEIGHT - y, 1, 1);
					cairo_fill(cr);
				}
				idx++;
			}

		}
	}
	//free(hist);

}
