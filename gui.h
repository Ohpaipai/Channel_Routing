#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<map>
#include<sstream>
#include<algorithm>
#include<gtk/gtk.h>
#include<cairo.h>
#include <cairo-svg.h>
#include <gdk/gdkkeysyms.h> 
typedef struct {
	 int begin;
     int end;
     std::string name;
} Node;
typedef struct {
	gint x1;
	gint x2;
	GdkRectangle rec;
	std::string windowname;
}Windownode;
typedef struct {
	gint x;
	gint y;
	std::string name;
}pinnum;
bool operator<(const Node& s1, const Node& s2)
{
 	  return s1.begin < s2.begin;
}
typedef struct {
	gdouble x;
	gdouble y;
	gdouble width;
	gdouble height;
}MyRectangle;
/*my global parameter*/
int firstx;//初始x座標
int firsty;//初始y座標
int fontsize;//pin腳名font size
std::vector<std::pair<MyRectangle,std::string>>topleg;//上層pin腳 leg位置
std::vector<std::pair<MyRectangle,std::string>>tailleg;//下層pin腳 leg位置
std::vector<Windownode>topwnode;//上層pin腳位置
std::vector<Windownode>tailwnode;//下層pin腳位置
std::map<std::string,Windownode>outnode;//track x座標位置
std::vector<pinnum>topPnum;//top pin腳名
std::vector<pinnum>tailPnum;//tail pin腳名
int screenshotcount=0;
gint limitwindowx=0;
gint limitwindowy=0;
double scalesize=1;
GtkWidget *window ;
/*__________________*/
static gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	//cairo_surface_t *cr;
	cairo_t *cr;
    cr = gdk_cairo_create(widget->window);

//		 cairo_translate(cr, -100, 30);

    static gboolean xdirection = TRUE;
    static gint counter = 0;

    int width, height;
    gtk_window_get_size(GTK_WINDOW(widget), &width, &height);	
//	cairo_translate(cr, -1*width/2, -1*height/2);

    static gdouble rotate = 0;

    static gint bigx = 20;
    static gint bigy = 200;
    static gint delta = 1;
for (int i=0;i<topwnode.size();i++){
	if(limitwindowx<=topwnode[i].rec.x<(width+limitwindowx) && limitwindowy<=topwnode[i].rec.y<(height+limitwindowy)){
		 
		cairo_set_source_rgb(cr,0,255,179);
		cairo_rectangle(cr,topwnode[i].rec.x,topwnode[i].rec.y,topwnode[i].rec.width,topwnode[i].rec.height);
		cairo_fill(cr);
		cairo_set_source_rgb(cr,0,0,0);
		cairo_set_font_size(cr,fontsize);
		cairo_move_to(cr,topPnum[i].x,topPnum[i].y);
		cairo_show_text(cr,topPnum[i].name.c_str());	
	}
	else if(limitwindowx<=topwnode[i].rec.x+topwnode[i].rec.width<(width+limitwindowx) && limitwindowy<=topwnode[i].rec.y<(height+limitwindowy)){
			
		cairo_set_source_rgb(cr,0,255,179);
		cairo_rectangle(cr,topwnode[i].rec.x,topwnode[i].rec.y,topwnode[i].rec.width,topwnode[i].rec.height);
		cairo_fill(cr);
		cairo_set_source_rgb(cr,0,0,0);
		cairo_set_font_size(cr,fontsize);
		cairo_move_to(cr,topPnum[i].x,topPnum[i].y);
		cairo_show_text(cr,topPnum[i].name.c_str());	
	}
	else{
		break;
	}
}
for (int i=0;i<topwnode.size();i++){
	if(limitwindowx<=tailwnode[i].rec.x<width+limitwindowx&&limitwindowy<=tailwnode[i].rec.y<height+limitwindowy){
		cairo_set_source_rgb(cr,0,255,179);
		cairo_rectangle(cr,tailwnode[i].rec.x,tailwnode[i].rec.y,tailwnode[i].rec.width,tailwnode[i].rec.height);
		cairo_fill(cr);
		cairo_set_source_rgb(cr,0,0,0);
		cairo_set_font_size(cr,fontsize);
		cairo_move_to(cr,tailPnum[i].x,tailPnum[i].y);
		cairo_show_text(cr,tailPnum[i].name.c_str());	
	}
	else if(limitwindowx<=tailwnode[i].rec.x+tailwnode[i].rec.width<(width+limitwindowx) && limitwindowy<=tailwnode[i].rec.y<(height+limitwindowy)){
		
		cairo_set_source_rgb(cr,0,255,179);
		cairo_rectangle(cr,tailwnode[i].rec.x,tailwnode[i].rec.y,tailwnode[i].rec.width,tailwnode[i].rec.height);
		cairo_fill(cr);
		cairo_set_source_rgb(cr,0,0,0);
		cairo_set_font_size(cr,fontsize);
		cairo_move_to(cr,tailPnum[i].x,tailPnum[i].y);
		cairo_show_text(cr,tailPnum[i].name.c_str());	
	}
	else{
		break;
	}
}
std::map<std::string,Windownode>::iterator ittrack;
//std::cout<<outnode.size()<<std::endl;
for(ittrack = outnode.begin(); ittrack != outnode.end(); ittrack++){
	//std::cout <<ittrack->first<<std::endl;	
	if(ittrack->first =="0") continue;
	if(limitwindowx<=ittrack->second.rec.x<width+limitwindowx&&limitwindowy<=ittrack->second.rec.y<height+limitwindowy){
		cairo_set_source_rgb(cr,255,0,0);
		cairo_rectangle(cr,ittrack->second.rec.x,ittrack->second.rec.y,ittrack->second.rec.width,ittrack->second.rec.height);
		cairo_fill(cr);
	}
	else if(limitwindowx<=ittrack->second.rec.x+ittrack->second.rec.width<width+limitwindowx&&limitwindowy<=ittrack->second.rec.y<height+limitwindowy){
		cairo_set_source_rgb(cr,255,0,0);
		cairo_rectangle(cr,ittrack->second.rec.x,ittrack->second.rec.y,ittrack->second.rec.width,ittrack->second.rec.height);
		cairo_fill(cr);
	}
	else{
	}
	
}
for(int i=0; i<topleg.size();i++){
	if(limitwindowx<=topleg[i].first.x<width+limitwindowx && limitwindowy<=topleg[i].first.y<height+limitwindowy){
		cairo_set_source_rgb(cr,0,255,0);
		cairo_rectangle(cr,topleg[i].first.x,topleg[i].first.y,topleg[i].first.width,topleg[i].first.height);
		cairo_fill(cr);	
	}
	else if(limitwindowx<=topleg[i].first.x+topleg[i].first.width<width+limitwindowx && limitwindowy<=topleg[i].first.y<height+limitwindowy){
		cairo_set_source_rgb(cr,0,255,0);
		cairo_rectangle(cr,topleg[i].first.x,topleg[i].first.y,topleg[i].first.width,topleg[i].first.height);
		cairo_fill(cr);
	}	
	else{
//		break;
	}
}
for(int i=0; i<tailleg.size();i++){
	if(limitwindowx<=tailleg[i].first.x<width+limitwindowx && limitwindowy<=tailleg[i].first.y<height+limitwindowy){
		cairo_set_source_rgb(cr,0,0,255);
		cairo_rectangle(cr,tailleg[i].first.x,tailleg[i].first.y,tailleg[i].first.width,tailleg[i].first.height);
		cairo_fill(cr);	
	}
	else if(limitwindowx<=tailleg[i].first.x+tailleg[i].first.width<width+limitwindowx && limitwindowy<=tailleg[i].first.y<height+limitwindowy){
		cairo_set_source_rgb(cr,0,0,255);
		cairo_rectangle(cr,tailleg[i].first.x,tailleg[i].first.y,tailleg[i].first.width,tailleg[i].first.height);
		cairo_fill(cr);	
	}
	else{
//		break;
	}
}

	cairo_destroy(cr);


    return FALSE;
}

static gboolean time_handler (GtkWidget *widget)
{
    if (widget->window == NULL) return FALSE;
    gtk_widget_queue_draw(widget);
    return TRUE;
}
gboolean deal_key_press(GtkWidget *widget, GdkEventKey  *event, gpointer data)  
{  
      
  
    int key = event->keyval; // 得當傲鍵盤的值 
   // printf("keyval = %d\n", key);  
	if(key==32){//節圖功能
    int width, height;
    gtk_window_get_size(GTK_WINDOW(widget), &width, &height);

	GdkPixbuf *pixbuf = NULL;
	pixbuf = gdk_pixbuf_get_from_drawable(NULL, widget->window, NULL,
            0,0, 0, 0, width, height);  //當前視窗
	if(NULL == pixbuf){	// 失敗,則輸出錯誤
		printf("gdk_pixbuf_get_from_drawable failed\n");
		return	false;
	}
	

	cairo_surface_t *surface = NULL;
	cairo_t *cr;
	surface = cairo_image_surface_create ( CAIRO_FORMAT_ARGB32,width,height) ;
	cr = cairo_create( surface) ; 
	
	gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);
	cairo_paint(cr);
	std::stringstream ss;
	ss<<screenshotcount;
	std::string tem;
	ss>>tem;

	std::string outscreenshot="screenshot";
	outscreenshot+=tem;
	outscreenshot+=".png";
	cairo_surface_write_to_png ( surface,outscreenshot.c_str() ) ;
	
	
	cairo_destroy(cr) ;
	cairo_surface_destroy( surface) ;
	screenshotcount++;	
	printf("gdk_pixbuf screenshot over\n");
	}	
    return TRUE;  
}
void scalePinTrackAndLeg(gint x, gint y,gboolean bigorsmall){
	if(bigorsmall)
		scalesize*=1.1;
	else
		scalesize/=1.1;
	fontsize*=scalesize;
	for (int i=0;i<topwnode.size();i++){
		topwnode[i].rec.x-=x;
		topwnode[i].rec.y-=y;
		topwnode[i].rec.x*=scalesize;
		topwnode[i].rec.y*=scalesize;
		topwnode[i].rec.x+=x;
		topwnode[i].rec.y+=y;
		topwnode[i].rec.width*=scalesize;
		topwnode[i].rec.height*=scalesize;
		topPnum[i].x-=x;
		topPnum[i].y-=y;
		topPnum[i].x*=scalesize;
		topPnum[i].y*=scalesize;
		topPnum[i].x+=x;
		topPnum[i].y+=y;
		/////
		tailwnode[i].rec.x-=x;
		tailwnode[i].rec.y-=y;
		tailwnode[i].rec.x*=scalesize;
		tailwnode[i].rec.y*=scalesize;
		tailwnode[i].rec.x+=x;
		tailwnode[i].rec.y+=y;
		tailwnode[i].rec.width*=scalesize;
		tailwnode[i].rec.height*=scalesize;
		tailPnum[i].x-=x;
		tailPnum[i].y-=y;
		tailPnum[i].x*=scalesize;
		tailPnum[i].y*=scalesize;
		tailPnum[i].x+=x;
		tailPnum[i].y+=y;
	}
	std::map<std::string,Windownode>::iterator ittrack;
	for(ittrack = outnode.begin(); ittrack != outnode.end(); ittrack++){
		ittrack->second.rec.x -=x;
		ittrack->second.rec.y -=y;
		ittrack->second.rec.x*=scalesize;
		ittrack->second.rec.y*=scalesize;
		ittrack->second.rec.x+=x;
		ittrack->second.rec.y+=y;
		ittrack->second.rec.width*=scalesize;	
		ittrack->second.rec.height*=scalesize;
		
	}
	////
	for(int i=0; i<topleg.size();i++){
		topleg[i].first.x -=x;
		topleg[i].first.y -=y;
		topleg[i].first.x*=scalesize;
		topleg[i].first.y*=scalesize;
		topleg[i].first.x+=x;
		topleg[i].first.y+=y;
		topleg[i].first.width*=scalesize;
		topleg[i].first.height*=scalesize;
		///
		tailleg[i].first.x -=x;
		tailleg[i].first.y -=y;
		tailleg[i].first.x*=scalesize;
		tailleg[i].first.y*=scalesize;
		tailleg[i].first.x+=x;
		tailleg[i].first.y+=y;
		tailleg[i].first.width*=scalesize;
		tailleg[i].first.height*=scalesize;
	}
	
}
gboolean deal_mouse_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
				gint i ;
				gint j ;
	switch(event->button){	// 判斷滑鼠點選的型別
		case 1:
			//printf("Left Button!!\n");
			std::cout<<scalesize<<std::endl;
			if(event->type == GDK_2BUTTON_PRESS){
				 i = event->x;
				 j = event->y;
				scalePinTrackAndLeg(i,j,false);
				gtk_widget_queue_draw(window);
				printf("縮小\n");
			}
			break;
		case 2:
			break;
		case 3://右鍵

			std::cout<<scalesize<<std::endl;
			if(event->type == GDK_2BUTTON_PRESS){
				 i = event->x;
				 j = event->y;
				scalePinTrackAndLeg(i,j,true);
				gtk_widget_queue_draw(window);
				printf("放大\n");
			}
			break;
		default:
			printf("Unknown Button!!\n");
	}



	return TRUE;
}

// 滑鼠移動
gboolean deal_motion_notify_event(GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
	// 獲得移動滑鼠的座標值，距離視窗左頂點

	return TRUE;
}
void drawgui(int argc, char * argv[]){
//	GtkWidget *window;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    g_signal_connect(G_OBJECT(window), "expose-event", G_CALLBACK(on_expose_event), NULL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
		// GDK_BUTTON_PRESS_MASK：滑鼠點選事件
	// GDK_BUTTON_MOTION_MASK：按住滑鼠移動事件
	//GDK_BUTTON_RELEASE_MASK 滑鼠釋放事件    
	//GDK_KEY_PRESS_MASK 鍵盤按住事件
	gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_MOTION_MASK|GDK_BUTTON_RELEASE_MASK|GDK_KEY_PRESS_MASK);	
    g_signal_connect(window, "key-press-event",G_CALLBACK(deal_key_press), NULL); //鍵盤事件
	// "button-press-event" 與 deal_mouse_event 連線，滑鼠點選事件
	g_signal_connect(window, "button-press-event", G_CALLBACK(deal_mouse_press), NULL);
	// "motion-notify-event" 與 deal_motion_notify_event 連線，按住滑鼠移動事件
	g_signal_connect(window, "motion-notify-event", G_CALLBACK(deal_motion_notify_event), NULL);

	
	gtk_widget_set_app_paintable(window, TRUE);
    gtk_widget_show_all(window);
    //g_timeout_add(5, (GSourceFunc) time_handler, (gpointer) window);

    gtk_main();

}
void drawpng(std::map<std::string, Node>& biglong, std::vector<std::string>& top, std::vector<std::string>& tail,std::map<std::string, std::pair<std::vector<Node>, int>>& track){
	//std::map<std::string,Windownode>outnode;//window中 上node 資訊
	//std::vector<Windownode>topwnode;
	//std::vector<Windownode>tailwnode;
	std::map<std::string,Windownode>::iterator it;
	topwnode.resize(top.size());
	tailwnode.resize(tail.size());
    cairo_t *cr;                                                                                                                                                                                        
    cairo_surface_t *surface;
	gint windowwidth, windowheight;
	int wsize=100;
	int hsize=50;
	firstx=wsize/5;
	firsty=hsize/5;
	windowwidth =top.size()*wsize;//螢幕長度
	windowheight =track.size()*hsize;//螢幕寬度
    //surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,windowwidth,windowheight);//設定好螢幕長寬png
    surface = cairo_svg_surface_create("sample.svg",windowwidth,windowheight);//設定好螢幕長寬svg
    cr=cairo_create(surface);//創建畫布
	//畫布上白色
	cairo_set_source_rgb(cr,1,1,1);
    cairo_rectangle(cr,0,0,windowwidth,windowheight);
    cairo_fill(cr);
	//畫上層layer
	/*cairo_set_source_rgb(cr,255,124,134);
	cairo_rectangle(cr,firstx,firsty,windowwidth-firstx*2,windowheight/hsize);
	cairo_fill(cr);*/
	//畫下層layer
/*	cairo_set_source_rgb(cr,255,124,134);
	cairo_rectangle(cr,firstx,windowheight-firsty-windowheight/hsize,windowwidth-firstx*2,windowheight/hsize);
	cairo_fill(cr);*/
	//std::vector<std::pair<GdkRectangle,std::string>>topleg;//上層方塊座標位置
	topleg.resize(top.size());
	//std::vector<std::pair<GdkRectangle,std::string>>tailleg;//下層方塊座標位置
	tailleg.resize(tail.size());
	//畫top layer 方塊
	gint diewidgth =(windowwidth-hsize)/(top.size()*2+1);
	gint initialx =firstx+diewidgth;
	gint initialy =firsty;
	gint trackheight=(windowheight-firsty-(windowheight/hsize)-firsty-(windowheight/hsize))/(track.size()*2+1); //track寬度
//	std::cout<<trackheight<<std::endl;
	fontsize=windowheight/hsize+2;//font 大小	
	topPnum.resize(top.size());
	tailPnum.resize(top.size());
	for(int i=0;i<top.size();i++){
		cairo_set_source_rgb(cr,0,255,179);
		cairo_rectangle(cr,initialx,initialy,diewidgth,windowheight/(hsize)+2);
		//紀錄上層座標
		topwnode[i].x1=initialx;
		topwnode[i].windowname=top[i];
		topwnode[i].rec.x=initialx;
		topwnode[i].rec.y=initialy;
		topwnode[i].rec.width=diewidgth;
		topwnode[i].rec.height=windowheight/(hsize)+2;
		//紀錄leg retangle位置
		topleg[i].first.x=initialx;
		topleg[i].first.y=initialy+(windowheight/hsize+2);
		topleg[i].first.width=diewidgth;
		topleg[i].second=top[i];
		//紀錄好die頭尾方便畫track
		it=outnode.find(top[i]);
		if(it==outnode.end()){
			Windownode tem;
			tem.x1=initialx;
			tem.x2=initialx+diewidgth;
			tem.windowname=top[i];	
			outnode[top[i]]=tem;
			//std::cout<<top[i]<<std::endl;
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
		cairo_move_to(cr,initialx,initialy+windowheight/(hsize*2)+4);
		topPnum[i].x=initialx;
		topPnum[i].y=initialy+windowheight/(hsize*2)+4;
		topPnum[i].name=top[i];
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
		tailwnode[i].rec.x=initialx;
		tailwnode[i].rec.y=initialy;
		tailwnode[i].rec.width=diewidgth;
		tailwnode[i].rec.height=windowheight/(hsize)+2;
		//紀錄好die頭尾方便畫track
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
		cairo_move_to(cr,initialx,initialy+windowheight/(hsize*2)+4);
		tailPnum[i].x=initialx;
		tailPnum[i].y=initialy+windowheight/(hsize*2)+4;
		tailPnum[i].name=tail[i];
		cairo_show_text(cr,tail[i].c_str());	
		initialx+=diewidgth*2;
	}
	//畫tarck 
	gint tracky=firsty+windowheight/(hsize)+trackheight;
	std::map<std::string,std::pair<std::vector<Node>, int>>::iterator ittrack;
	int tc=1;
	for(ittrack = track.begin(); ittrack != track.end(); ittrack++,tc+=2) {
		for(int i=0;i<ittrack->second.first.size();i++){
			it=outnode.find(ittrack->second.first[i].name);
			if(ittrack->second.first[i].name=="0") {
				it->second.rec.x=it->second.x1;
				it->second.rec.y=tracky;
				it->second.rec.width=it->second.x2-it->second.x1;
				it->second.rec.height=trackheight;
				continue;//0為空node不畫	
			}
				it->second.rec.x=it->second.x1;
				it->second.rec.y=tracky;
				it->second.rec.width=it->second.x2-it->second.x1;
				it->second.rec.height=trackheight;
			cairo_set_source_rgb(cr,255,0,0);

			cairo_rectangle(cr,it->second.x1,tracky,it->second.x2-it->second.x1,trackheight);
			cairo_fill(cr);
			//設定好我接下來畫的legx y位置還有長寬大小
			for(int j=0;j<topleg.size();j++){
				//上往下打
				if(ittrack->second.first[i].name==topleg[j].second){
					topleg[j].first.height=trackheight*tc+trackheight;
				}

				if(ittrack->second.first[i].name==tailleg[j].second){
					//下往上打
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

//    cairo_surface_write_to_png(surface,"sample.svg");  //png繪圖
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
}	
