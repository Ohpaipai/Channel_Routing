#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<map>
#include<sstream>
#include<algorithm>
#include<gtk/gtk.h>
#include<cairo.h>
typedef struct {
	 int begin;
     int end;
     std::string name;
} Node; 
bool operator<(const Node& s1, const Node& s2)
{
 	  return s1.begin < s2.begin;
}

static gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	cairo_t *cr;
    cr = gdk_cairo_create(widget->window);

    static gboolean xdirection = TRUE;
    static gint counter = 0;

    int width, height;
    gtk_window_get_size(GTK_WINDOW(widget), &width, &height);

    static gdouble rotate = 0;

    static gint bigx = 20;
    static gint bigy = 200;
    static gint delta = 1;


    cairo_set_source_rgb(cr,0,0,0);
	cairo_set_font_size(cr,12.0);
	cairo_move_to(cr,10,10);
	cairo_show_text(cr,"we care");
    cairo_fill(cr);
    cairo_set_source_rgb(cr,0,255, 255);
	cairo_rectangle(cr,10,10,400,30);

    cairo_fill(cr);

    cairo_set_source_rgb(cr,255,0,255 );
	cairo_rectangle(cr,10,60,400,30);
    cairo_fill(cr);
	cairo_destroy(cr);


    return FALSE;
}

static gboolean time_handler (GtkWidget *widget)
{
    if (widget->window == NULL) return FALSE;
    gtk_widget_queue_draw(widget);
    return TRUE;
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

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    g_signal_connect(G_OBJECT(window), "expose-event", G_CALLBACK(on_expose_event), NULL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    gtk_widget_set_app_paintable(window, TRUE);
    gtk_widget_show_all(window);
    g_timeout_add(5, (GSourceFunc) time_handler, (gpointer) window);

    gtk_main();

}
void drawpng(std::map<std::string, Node>& biglong, std::vector<std::string>& top, std::vector<std::string>& tail,std::map<std::string, std::pair<std::vector<Node>, int>>& track){
	
    cairo_t *cr;                                                                                                                                                                                        
    cairo_surface_t *surface;
	gint windowwidth, windowheight;
	windowwidth =top.size()*100;//螢幕長度
	windowheight =track.size()*50;//螢幕寬度
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,windowwidth,windowheight);//設定好螢幕長寬
    cr=cairo_create(surface);//創建畫布
	//畫布上白色
	cairo_set_source_rgb(cr,1,1,1);
    cairo_rectangle(cr,0,0,windowwidth,windowheight);
    cairo_fill(cr);
	//畫上層layer
	cairo_set_source_rgb(cr,0,0,0);
	cairo_rectangle(cr,10,10,windowwidth-20,windowheight/20);
	cairo_fill(cr);
	//畫下層layer
	cairo_set_source_rgb(cr,0,0,0);
	cairo_rectangle(cr,10,windowheight-windowheight*2/20,windowwidth-20,windowheight/20);
	cairo_fill(cr);
	//畫top layer 方塊
	gint diewidgth =(windowwidth-20)/(top.size()*2+1);
	gint initialx =12+diewidgth;
	gint initialy =12;
	for(int i=0;i<top.size();i++){
		cairo_set_source_rgb(cr,0,255,179);
		cairo_rectangle(cr,initialx,initialy,diewidgth,windowheight/20+2);
		cairo_fill(cr);
		//darw num
		cairo_set_source_rgb(cr,0,0,0);
		cairo_set_font_size(cr,windowheight/20+2);
		cairo_move_to(cr,initialx+diewidgth/2,initialy+windowheight/40+5);
		cairo_show_text(cr,top[i].c_str());	
		initialx+=diewidgth*2;
	}
	//畫tail layer 方塊
	initialy =windowheight-(windowheight*2)/20;
	initialx =12+diewidgth;
	for(int i=0;i<top.size();i++){
		cairo_set_source_rgb(cr,0,255,179);
		cairo_rectangle(cr,initialx,windowheight-windowheight*2/20,diewidgth,windowheight/20+2);
		cairo_fill(cr);
		//darw num
		cairo_set_source_rgb(cr,0,0,0);
		cairo_set_font_size(cr,windowheight/20+2);
		cairo_move_to(cr,initialx+diewidgth/2,initialy+windowheight/40+5);
		cairo_show_text(cr,top[i].c_str());	
		initialx+=diewidgth*2;
	}
		

    cairo_select_font_face(cr,"Hello",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr,30.0);
    cairo_move_to(cr,100,400);
    cairo_set_source_rgb(cr,0,1,0);
    cairo_show_text(cr,"Hello World gcc");
    cairo_surface_write_to_png(surface,"image.png");
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
}	
