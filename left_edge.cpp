#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<map>
#include<sstream>
typedef struct {
	int begin;
	int end;
	std::string name;
} Node;
int readFile(std::map<std::string,std::pair<int,int>>&biglong ,std::vector<std::string>&top ,std::vector<std::string>&tail){ //return pcb long
	int PCBcount =0;
	std::fstream infile;
	infile.open("test.pin",std::ios::in);
	std::string name;//name
	std::string line;
	
	std::map<std::string,std::pair<int,int>>::iterator it;//iterator
	
	//stringstream
	std::stringstream ss;
	std::stringstream files;

	//first chaneel
	std::getline(infile,line);
		files.str(line);
		ss.clear();
		ss.str("");
		while(files>>name){
			ss<<name;
		    int placenum;
			ss>>placenum;	   
			top.push_back(name);	
			//find
			it=biglong.find(name);
			if(it!=biglong.end()){
				if(it->second.first>PCBcount){
					it->second.first=PCBcount;
				}
				else if(it->second.second<PCBcount){
					it->second.second=PCBcount;
				}
			}
			else{
				std::pair<int,int>tem;
				tem.first=PCBcount;
				tem.second=PCBcount;
				biglong[name]=tem;
			}
			PCBcount++;

		}
	//second channel
	PCBcount=0;
	std::getline(infile,line);
		files.clear();
		files.str(line);
		ss.clear();
		ss.str("");
		while(files>>name){
			ss<<name;
		    int placenum;
			ss>>placenum;	   
			tail.push_back(name);	
			it=biglong.find(name);
			if(it!=biglong.end()){
				if(it->second.first>PCBcount){
					it->second.first=PCBcount;
				}
				else if(it->second.second<PCBcount){
					it->second.second=PCBcount;
				}
			}
			else{
				std::pair<int,int>tem;
				tem.first=PCBcount;
				tem.second=PCBcount;
				biglong[name]=tem;
			}
			PCBcount++;
		}	
	
	return PCBcount;
}
int main(int argc, char *argv[]){
	std::vector<std::string>top;//top  channel
	std::vector<std::string>tail;//tail chanel
	std::map<std::string,std::pair<int,int>>biglong;//node between longgest
	std::cout<<readFile(biglong,top,tail)<<std::endl;
	//left edge
	std::map<std::string,std::pair<int,int>>::iterator it;//iterator
	std::map<std::string,std::vector<Node>>track;
	int trackcount=0;
	std::map<std::string,std::vector<Node>>::iterator itn;
	for(it=biglong.begin();it!=biglong.end();it++){
		if(it->first!="0"){
			bool infind=false;
			for(itn=track.begin();itn!=track.end();itn++){
				for(int i=0;i<=itn->second.size();i++){
					if((it->second.first>=itn->second[i].begin && it->second.first<=itn->second[i].end) || (it->second.second>=itn->second[i].begin && it->second.second<=itn->second[i].end)){
					}
					else{
						infind=true;
						Node tem;
						tem.begin=it->second.first;
						tem.end=it->second.second;
						tem.name=it->first;
						itn->second.push_back(tem);
						break;
					}
				}
			}
			if(infind==false){
				Node tem;
				tem.begin=it->second.first;
				tem.end=it->second.second;
				tem.name=it->first;
				std::stringstream ss;
				ss<<trackcount;
				std::string temname;
				ss>>temname;
				std::vector<Node>t;
				t.push_back(tem);
				track[temname]=t;		
				trackcount++;	
			}
		}
	}
	
	//
	/*debug
	for(int i=0;i<top.size();i++){
		std::cout<<top[i]<<" ";
	}
	std::cout<<std::endl;
	for(int i=0;i<top.size();i++){
		std::cout<<tail[i]<<" ";
	}
	std::cout<<std::endl;
	std::map<std::string,std::pair<int,int>>::iterator it;//iterator
	for(it=biglong.begin();it!=biglong.end();it++){
		std::cout<<it->first<<"->"<<it->second.first<<"~"<<it->second.second<<std::endl;
	}
	*/
	std::cout<<trackcount-1<<std::endl;
	return 0;
	
}
