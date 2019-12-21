#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<map>
#include<sstream>
#include<algorithm>
#include<gtk/gtk.h>
#include<cairo.h>
static gboolean on_expose_event (GtkWidget *widget, GdkEventExpose * event, gpointer data)
{
    cairo_t *cr;

    cr = gdk_cairo_create (widget->window);

    cairo_set_source_rgb (cr, 0.627, 0, 0);
    cairo_select_font_face (cr, "ohpaipaip", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size (cr, 24.0);

    cairo_move_to (cr, 10.0, 34.0);
    cairo_show_text (cr, "编程艺术");

    cairo_destroy (cr);

    return FALSE;
}
void drawgui(int argc, char * argv[]){
   /* cairo_t *cr;                                                                                                                                                                                        
    cairo_surface_t *surface;
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,400,800);
    cr=cairo_create(surface);
	cairo_set_source_rgb(cr,1,0,0);
    cairo_rectangle(cr,0,0,400,800);
    cairo_fill(cr);
    cairo_select_font_face(cr,"Hello",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr,30.0);
    cairo_move_to(cr,100,400);
    cairo_set_source_rgb(cr,0,1,0);
    cairo_show_text(cr,"Hello World gcc");
    cairo_surface_write_to_png(surface,"image.png");
	cairo_destroy(cr);
	cairo_surface_destroy(surface);*/
	GtkWidget *window;

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    g_signal_connect (window, "expose-event", G_CALLBACK (on_expose_event), NULL);
    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size (GTK_WINDOW (window), 320, 48);
    gtk_widget_set_app_paintable (window, TRUE);

    gtk_widget_show_all (window);

    gtk_main ();

}
