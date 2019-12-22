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
typedef struct {
	gint x1;
	gint x2;
	std::string windowname;
}Windownode;
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
	std::map<std::string,Windownode>outnode;//window中 上node 資訊
	std::vector<Windownode>topwnode;
	std::vector<Windownode>tailwnode;
	std::map<std::string,Windownode>::iterator it;
	topwnode.resize(top.size());
	tailwnode.resize(tail.size());
    cairo_t *cr;                                                                                                                                                                                        
    cairo_surface_t *surface;
	gint windowwidth, windowheight;
	int wsize=100;
	int hsize=50;
	int firstx=wsize/5;
	int firsty=hsize/5;
	windowwidth =top.size()*wsize;//螢幕長度
	windowheight =track.size()*hsize;//螢幕寬度
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,windowwidth,windowheight);//設定好螢幕長寬
    cr=cairo_create(surface);//創建畫布
	//畫布上白色
	cairo_set_source_rgb(cr,1,1,1);
    cairo_rectangle(cr,0,0,windowwidth,windowheight);
    cairo_fill(cr);
	//畫上層layer
	cairo_set_source_rgb(cr,0,0,0);
	cairo_rectangle(cr,firstx,firsty,windowwidth-firstx*2,windowheight/hsize);
	cairo_fill(cr);
	//畫下層layer
	cairo_set_source_rgb(cr,0,0,0);
	cairo_rectangle(cr,firstx,windowheight-firsty-windowheight/hsize,windowwidth-firstx*2,windowheight/hsize);
	cairo_fill(cr);
	std::vector<std::pair<GdkRectangle,std::string>>topleg;//上層方塊座標位置
	topleg.resize(top.size());
	std::vector<std::pair<GdkRectangle,std::string>>tailleg;//下層方塊座標位置
	tailleg.resize(tail.size());
	//畫top layer 方塊
	gint diewidgth =(windowwidth-hsize)/(top.size()*2+1);
	gint initialx =firstx+diewidgth;
	gint initialy =firsty;
	gint trackheight=(windowheight-firsty-(windowheight/hsize)-firsty-(windowheight/hsize))/(track.size()*2+1); //track寬度
	std::cout<<trackheight<<std::endl;
	for(int i=0;i<top.size();i++){
		cairo_set_source_rgb(cr,0,255,179);
		cairo_rectangle(cr,initialx,initialy,diewidgth,windowheight/(hsize)+2);
		//紀錄上層座標
		topwnode[i].x1=initialx;
		topwnode[i].windowname=top[i];
		//紀錄leg retangle位置
		topleg[i].first.x=initialx;
		topleg[i].first.y=initialy+(windowheight/hsize+2);
		topleg[i].first.width=diewidgth;
		topleg[i].second=top[i];
		//
		it=outnode.find(top[i]);
		if(it==outnode.end()){
			Windownode tem;
			tem.x1=initialx;
			tem.x2=initialx+diewidgth;
			tem.windowname=top[i];	
			outnode[top[i]]=tem;
		}
		else{
			if(it->second.x1>initialx){
				it->second.x1=initialx;
			}
			else if(it->second.x2<initialx+diewidgth){
				it->second.x2=initialx+diewidgth;
			}
		}
		cairo_fill(cr);
		//darw num
		cairo_set_source_rgb(cr,0,0,0);
		cairo_set_font_size(cr,windowheight/(hsize)+2);
		cairo_move_to(cr,initialx,initialy+windowheight/(hsize*2)+5);
		cairo_show_text(cr,top[i].c_str());	
		initialx+=diewidgth*2;
	}
	//畫tail layer 方塊
	initialy =windowheight-firsty-windowheight/hsize;
	initialx =firstx+diewidgth;
	for(int i=0;i<tail.size();i++){
		cairo_set_source_rgb(cr,0,255,179);
		cairo_rectangle(cr,initialx,initialy,diewidgth,windowheight/(hsize)+2);
		//紀錄leg retangle位置
		tailleg[i].first.x=initialx;
		tailleg[i].first.y=initialy+(windowheight/hsize+2);
		tailleg[i].first.width=diewidgth;
		tailleg[i].second=tail[i];
		//紀錄下層座標
		tailwnode[i].x1=initialx;
		tailwnode[i].windowname=tail[i];
		it=outnode.find(tail[i]);
		if(it==outnode.end()){
			Windownode tem;
			tem.x1=initialx;
			tem.x2=initialx+diewidgth;
			tem.windowname=tail[i];	
			outnode[tail[i]]=tem;
		}
		else{
			if(it->second.x1>initialx){
				it->second.x1=initialx;
			}
			else if(it->second.x2<initialx+diewidgth){
				it->second.x2=initialx+diewidgth;
			}
		}
		cairo_fill(cr);
		//darw num
		cairo_set_source_rgb(cr,0,0,0);
		cairo_set_font_size(cr,windowheight/(hsize)+2);
		cairo_move_to(cr,initialx,initialy+windowheight/(hsize*2)+5);
		cairo_show_text(cr,tail[i].c_str());	
		initialx+=diewidgth*2;
	}
	//畫tarck 
	gint tracky=firsty+windowheight/(hsize)+trackheight;
	std::map<std::string,std::pair<std::vector<Node>, int>>::iterator ittrack;
	int tc=1;
	for(ittrack = track.begin(); ittrack != track.end(); ittrack++,tc+=2) {
		for(int i=0;i<ittrack->second.first.size();i++){
			if(ittrack->second.first[i].name=="0") continue;//0為空node不畫	
			it=outnode.find(ittrack->second.first[i].name);
			cairo_set_source_rgb(cr,255,0,0);

			cairo_rectangle(cr,it->second.x1,tracky,it->second.x2-it->second.x1,trackheight);
			cairo_fill(cr);
			for(int j=0;j<topleg.size();j++){
				if(ittrack->second.first[i].name==topleg[j].second){
					topleg[j].first.height=trackheight*tc+trackheight;
				}

				if(ittrack->second.first[i].name==tailleg[j].second){
					//tailleg[j].first.height=(track.size()*2+1-tc)*trackheight;
					tailleg[j].first.height=tailleg[j].first.y-tracky-(windowheight/hsize);
					tailleg[j].first.y=tracky;
				}
			}
			
			
		}	
		
		tracky+=2*trackheight;

	}
	GdkRectangle a;
	//判斷是否有intersect
	for(int i=0;i<topleg.size();i++){
		for(int j=0;j<tailleg.size();j++){
			gboolean intersectrec=gdk_rectangle_intersect(&topleg[i].first,&tailleg[j].first,&a);
			if(intersectrec)
			{
				topleg[i].first.width/=2;
				tailleg[j].first.x+=(diewidgth/2);		
				tailleg[j].first.width/=2;
				break;
			}
		}
	}
	//畫出leg
	for(int i=0;i<topleg.size();i++){
		if(topleg[i].second!="0"){
			cairo_set_source_rgb(cr,0,255,0);
			cairo_rectangle(cr,topleg[i].first.x,topleg[i].first.y,topleg[i].first.width,topleg[i].first.height);
			cairo_fill(cr);
		}
		if(tailleg[i].second!="0"){
			cairo_set_source_rgb(cr,0,0,255);
			cairo_rectangle(cr,tailleg[i].first.x,tailleg[i].first.y,tailleg[i].first.width,tailleg[i].first.height);
			cairo_fill(cr);
		}
	}

    cairo_surface_write_to_png(surface,"image.png");
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
}	
