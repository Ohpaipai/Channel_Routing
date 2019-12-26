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
//pin腳結構
typedef struct {
	 int begin;
     int end;
     std::string name;
} Node;
//window上的點資訊
typedef struct {
	gint x1;
	gint x2;
	GdkRectangle rec;
	std::string windowname;
}Windownode;
//數字位置結構
typedef struct {
	gdouble x;
	gdouble y;
	std::string name;
}pinnum;
//比大小
bool operator<(const Node& s1, const Node& s2)
{
 	  return s1.begin < s2.begin;
}
//自定義長方形結構
typedef struct {
	gdouble x;
	gdouble y;
	gdouble width;
	gdouble height;
}MyRectangle;
//判斷是否有交集
gboolean Myrecintersec(MyRectangle r1,MyRectangle r2){
//	std::cout<<r1.x<<"  "<<r1.y<<"  "<<r1.width<<"  "<<r1.height<<r2.x<<"  "<<r2.y<<" "<<r2.width<<"  "<<r2.height<<std::endl;
//	std::cout<<r1.x<<"  "<<r1.y<<"  "<<r1.width<<"  "<<r1.height<<r2.x<<"  "<<r2.y<<" "<<r2.width<<"  "<<r2.height<<std::endl;
	return !( (r1.x+r1.width)<=r2.x || r1.x>=(r2.x+r2.width) || (r1.y+r1.height)<=r2.y || r1.y>=(r2.y+r2.height));
}
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
int screenshotcount=0;//節圖count
gdouble fFontsize;//字形大小
gdouble limitwindowx=0;//windowsx邊界x左
gdouble mousex=0;//滑鼠x位置
gdouble mousey=0;//滑鼠y位置
gdouble limitwindowy=0;//windows邊界y左
double scalesize=1;//放大倍率
std::vector<MyRectangle>topvia;//上層的via
std::vector<MyRectangle>tailvia;//下層的via
GtkWidget *window; //gtkweight window視窗

/*__________________視窗變動event*/
static gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	//創建cairo
	cairo_t *cr;
    cr = gdk_cairo_create(widget->window);
    static gboolean xdirection = TRUE;
    static gint counter = 0;
    int width, height;
	//得到視窗大小
    gtk_window_get_size(GTK_WINDOW(widget), &width, &height);	
	//畫上下的pin位置
for (int i=0;i<topwnode.size();i++){
	//計算scale
	gdouble fx=(topwnode[i].rec.x-mousex)*scalesize+mousex;
	gdouble fy=(topwnode[i].rec.y-mousey)*scalesize+mousey;
	gdouble fwidth=(topwnode[i].rec.width)*scalesize;
	gdouble fheight=(topwnode[i].rec.height)*scalesize;
	gdouble fnamex=(topPnum[i].x-mousex)*scalesize+mousex;
	gdouble fnamey=(topPnum[i].y-mousey)*scalesize+mousey;
	gdouble fFontSize=fontsize*scalesize;
	if(limitwindowx<=fx<(width+limitwindowx) && limitwindowy<=fy<(height+limitwindowy)){
		 
		cairo_set_source_rgb(cr,0,255,179);
		cairo_rectangle(cr,fx,fy,fwidth,fheight);
		cairo_fill(cr);
		cairo_set_source_rgb(cr,0,0,0);
		cairo_set_font_size(cr,fFontsize);
		cairo_move_to(cr,fnamex,fnamey);
		cairo_show_text(cr,topPnum[i].name.c_str());	
		cairo_fill(cr);
	}
	else if(limitwindowx<=fx+fwidth<(width+limitwindowx) && limitwindowy<=fy<(height+limitwindowy)){
			
		cairo_set_source_rgb(cr,0,255,179);
		cairo_rectangle(cr,fx,fy,fwidth,fheight);
		cairo_fill(cr);
		cairo_set_source_rgb(cr,0,0,0);
		cairo_set_font_size(cr,fFontsize);
		cairo_move_to(cr,fnamex,fnamey);
		cairo_show_text(cr,topPnum[i].name.c_str());	
		cairo_fill(cr);
	}
	
	//計算scale
	 fx=(tailwnode[i].rec.x-mousex)*scalesize+mousex;
	 fy=(tailwnode[i].rec.y-mousey)*scalesize+mousey;
	 fwidth=(tailwnode[i].rec.width)*scalesize;
	 fheight=(tailwnode[i].rec.height)*scalesize;
	 fnamex=(tailPnum[i].x-mousex)*scalesize+mousex;
	 fnamey=(tailPnum[i].y-mousey)*scalesize+mousey;
	 fFontSize=fontsize*scalesize;
	if(limitwindowx<=fx<(width+limitwindowx) && limitwindowy<=fy<(height+limitwindowy)){
		 
		cairo_set_source_rgb(cr,0,255,179);
		cairo_rectangle(cr,fx,fy,fwidth,fheight);
		cairo_fill(cr);
		cairo_set_source_rgb(cr,0,0,0);
		cairo_set_font_size(cr,fFontsize);
		cairo_move_to(cr,fnamex,fnamey);
		cairo_show_text(cr,tailPnum[i].name.c_str());	
		cairo_fill(cr);
	}
	else if(limitwindowx<=fx+fwidth<(width+limitwindowx) && limitwindowy<=fy<(height+limitwindowy)){
			
		cairo_set_source_rgb(cr,0,255,179);
		cairo_rectangle(cr,fx,fy,fwidth,fheight);
		cairo_fill(cr);
		cairo_set_source_rgb(cr,0,0,0);
		cairo_set_font_size(cr,fFontsize);
		cairo_move_to(cr,fnamex,fnamey);
		cairo_show_text(cr,tailPnum[i].name.c_str());	
		cairo_fill(cr);
	}
}
//畫track
std::map<std::string,Windownode>::iterator ittrack;
for(ittrack = outnode.begin(); ittrack != outnode.end(); ittrack++){
	//計算scale
	gdouble fx=(ittrack->second.rec.x-mousex)*scalesize+mousex;
	gdouble fy=(ittrack->second.rec.y-mousey)*scalesize+mousey;
	gdouble fwidth=(ittrack->second.rec.width)*scalesize;
	gdouble fheight=(ittrack->second.rec.height)*scalesize;
	if(ittrack->first =="0") continue;
	if(limitwindowx<= fx <width+limitwindowx&&limitwindowy<= fy <height+limitwindowy){
		cairo_set_source_rgb(cr,255,0,0);
		cairo_rectangle(cr,fx,fy,fwidth,fheight);
		cairo_fill(cr);
	}
	else if(limitwindowx<=fx+fwidth<width+limitwindowx&&limitwindowy<=fy<height+limitwindowy){
		cairo_set_source_rgb(cr,255,0,0);
		cairo_rectangle(cr,fx,fy,fwidth,fheight);
		cairo_fill(cr);
	}
	
}
//畫pin跟pin的leg
for(int i=0; i<topleg.size();i++){
	//計算scale
	gdouble fx=(topleg[i].first.x-mousex)*scalesize+mousex;
	gdouble fy=(topleg[i].first.y-mousey)*scalesize+mousey;
	gdouble fwidth=(topleg[i].first.width)*scalesize;
	gdouble fheight=(topleg[i].first.height)*scalesize;
	if(limitwindowx<= fx <width+limitwindowx && limitwindowy<= fy <height+limitwindowy){
		cairo_set_source_rgba(cr,0,255,0,1);
		cairo_rectangle(cr,fx,fy,fwidth,fheight);
		cairo_fill(cr);	
	}
	else if(limitwindowx<= fx+fwidth<width+limitwindowx && limitwindowy<=fy<height+limitwindowy){
		cairo_set_source_rgba(cr,0,255,0,1);
		cairo_rectangle(cr,fx,fy,fwidth,fheight);
		cairo_fill(cr);
	}

	//計算scale
	fx=(tailleg[i].first.x-mousex)*scalesize+mousex;
	fy=(tailleg[i].first.y-mousey)*scalesize+mousey;
	fwidth=(tailleg[i].first.width)*scalesize;
	fheight=(tailleg[i].first.height)*scalesize;

	if(limitwindowx<= fx <width+limitwindowx && limitwindowy<= fy <height+limitwindowy){
		cairo_set_source_rgba(cr,0,0,255,1);
		cairo_rectangle(cr,fx,fy,fwidth,fheight);
		cairo_fill(cr);	
	}
	else if(limitwindowx<=fx+fwidth<width+limitwindowx && limitwindowy<=fy<height+limitwindowy){
		cairo_set_source_rgba(cr,0,0,255,1);
		cairo_rectangle(cr,fx,fy,fwidth,fheight);
		cairo_fill(cr);	
	}


}

//畫via
for(int i=0;i<topvia.size();i++){
	gdouble fx=(topvia[i].x-mousex)*scalesize+mousex;
	gdouble fy=(topvia[i].y-mousey)*scalesize+mousey;
	gdouble fwidth=(topvia[i].width)*scalesize;
	gdouble fheight=(topvia[i].height)*scalesize;
	if(limitwindowx<= fx <width+limitwindowx && limitwindowy<= fy <height+limitwindowy){
		cairo_set_source_rgba(cr,255,0,255,1);
		cairo_rectangle(cr,fx,fy,fwidth,fheight);
		cairo_fill(cr);	
	}
	else if(limitwindowx<= fx+fwidth<width+limitwindowx && limitwindowy<=fy<height+limitwindowy){
		cairo_set_source_rgba(cr,255,0,255,1);
		cairo_rectangle(cr,fx,fy,fwidth,fheight);
		cairo_fill(cr);
	}
	//計算scale
	fx=(tailvia[i].x-mousex)*scalesize+mousex;
	fy=(tailvia[i].y-mousey)*scalesize+mousey;
	fwidth=(tailvia[i].width)*scalesize;
	fheight=(tailvia[i].height)*scalesize;

	if(limitwindowx<= fx <width+limitwindowx && limitwindowy<= fy <height+limitwindowy){
		cairo_set_source_rgba(cr,255,255,0,1);
		cairo_rectangle(cr,fx,fy,fwidth,fheight);
		cairo_fill(cr);	
	}
	else if(limitwindowx<=fx+fwidth<width+limitwindowx && limitwindowy<=fy<height+limitwindowy){
		cairo_set_source_rgba(cr,255,255,0,1);
		cairo_rectangle(cr,fx,fy,fwidth,fheight);
		cairo_fill(cr);	
	}
}
	cairo_destroy(cr);
    return FALSE;
}
//時間函數
static gboolean time_handler (GtkWidget *widget)
{
    if (widget->window == NULL) return FALSE;
    gtk_widget_queue_draw(widget);
    return TRUE;
}
//鍵盤按壓事件
gboolean deal_key_press(GtkWidget *widget, GdkEventKey  *event, gpointer data)  
{  
      
  
    int key = event->keyval; // 得當傲鍵盤的值 
	if(key==32){//是否為空白鍵
    int width, height;
    gtk_window_get_size(GTK_WINDOW(widget), &width, &height);
	GdkPixbuf *pixbuf = NULL;
	pixbuf = gdk_pixbuf_get_from_drawable(NULL, widget->window, NULL,
            0,0, 0, 0, width, height);  //當前視窗
	if(NULL == pixbuf){	// 失敗,則輸出錯誤
		printf("gdk_pixbuf_get_from_drawable failed\n");
		return	false;
	}
	
	//節圖
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
//滑鼠事件
gboolean deal_mouse_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	switch(event->button){	// 判斷滑鼠點選的型別
		case 1://左鍵縮小 且要double click
			if(event->type == GDK_2BUTTON_PRESS){
				scalesize=scalesize/1.1;
				fFontsize/=1.1;
   				 mousex = event->x;
				 mousey = event->y;
				gtk_widget_queue_draw(window); //刷新我的window
			}
			break;
		case 2://中鍵
			break;
		case 3://右鍵放大 且要double click
			if(event->type == GDK_2BUTTON_PRESS){
				scalesize=scalesize*1.1;
				fFontsize*=1.1;
				 mousex = event->x;
				 mousey = event->y;
				gtk_widget_queue_draw(window);//刷新我的window
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
//輸出圖片
void drawpng(std::map<std::string, Node>& biglong, std::vector<std::string>& top, std::vector<std::string>& tail,std::map<std::string, std::pair<std::vector<Node>, int>>& track){
	std::map<std::string,Windownode>::iterator it;
	topwnode.resize(top.size());
	tailwnode.resize(tail.size());
	topvia.resize(top.size());
	tailvia.resize(tail.size());
    cairo_t *cr;                                                                                                                                                                                        
    cairo_surface_t *surface;
	gint windowwidth, windowheight;
	int wsize=50;
	int hsize=20;
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
	//開好leg空間
	topleg.resize(top.size());
	tailleg.resize(tail.size());
	//畫top layer 方塊
	//先定義好die寬度 還有初始座標位置 與track高度
	gint diewidgth =(windowwidth-hsize)/(top.size()*2+1);
	gint initialx =firstx+diewidgth;
	gint initialy =firsty;
	gint trackheight=(windowheight-firsty-(windowheight/hsize)-firsty-(windowheight/hsize))/(track.size()*2+1); //track寬度
	//fontsize=windowheight/hsize+2;//font 大小	
	//fontsize=windowheight/50;//font 大小	
	topPnum.resize(top.size());
	tailPnum.resize(top.size());
	if(diewidgth>((windowheight/hsize)+2))	
		fontsize=((windowheight/hsize)+2);
	else
		fontsize=diewidgth;
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
		//紀錄via
		topvia[i].x=initialx;
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
		cairo_set_font_size(cr,fontsize);
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
		//紀錄via
		tailvia[i].x=initialx;
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
		cairo_set_font_size(cr,fontsize);
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
					//上層via
					topvia[j].y=tracky;
					topvia[j].width=diewidgth;
					topvia[j].height=trackheight;					
				}

				if(ittrack->second.first[i].name==tailleg[j].second){
					//下往上打
					//tailleg[j].first.height=(track.size()*2+1-tc)*trackheight;
					tailleg[j].first.height=tailleg[j].first.y-tracky-(windowheight/hsize);
					tailleg[j].first.y=tracky;
					//下層via
					tailvia[j].y=tracky;
					tailvia[j].width=diewidgth;
					tailvia[j].height=trackheight;					
				}
			}
			
			
		}	
		
		tracky+=2*trackheight;

	}
	GdkRectangle a;
	//判斷是否有intersect
	for(int i=0;i<topleg.size();i++){
		//for(int j=0;j<tailleg.size();j++){
			//gboolean intersectrec=gdk_rectangle_intersect(&topleg[i].first,&tailleg[i].first,&a);
			gboolean intersectrec=Myrecintersec(topleg[i].first,tailleg[i].first);
			if(intersectrec)
			{
			//	std::cout<<i<<std::endl;
				topleg[i].first.width/=2;
				tailleg[i].first.x+=(diewidgth/2);		
				tailleg[i].first.width/=2;
				//via
				topvia[i].width/=2;
				tailvia[i].x+=(diewidgth/2);
				tailvia[i].width/=2;
			}
	}
	//畫出leg
	for(int i=0;i<topleg.size();i++){
		if(topleg[i].second!="0"){
			cairo_set_source_rgba(cr,0,255,0,1);
			cairo_rectangle(cr,topleg[i].first.x,topleg[i].first.y,topleg[i].first.width,topleg[i].first.height);
			cairo_fill(cr);
			cairo_set_source_rgba(cr,255,0,255,1);
			cairo_rectangle(cr,topvia[i].x,topvia[i].y,topvia[i].width,topvia[i].height);
			cairo_fill(cr);
		}
		if(tailleg[i].second!="0"){
			cairo_set_source_rgba(cr,0,0,255,1);
			cairo_rectangle(cr,tailleg[i].first.x,tailleg[i].first.y,tailleg[i].first.width,tailleg[i].first.height);
			cairo_fill(cr);
			cairo_set_source_rgba(cr,255,255,0,1);
			cairo_rectangle(cr,tailvia[i].x,tailvia[i].y,tailvia[i].width,tailvia[i].height);
			cairo_fill(cr);
		}
	}
	fFontsize=fontsize;
//    cairo_surface_write_to_png(surface,"sample.svg");  //png繪圖
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
}	
