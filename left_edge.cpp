#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<map>
#include<sstream>
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
			PCBcount++;
			//find
			it=biglong.find(name);
			if(it!=biglong.end()){
				if(it->second.first>placenum){
					it->second.first=placenum;
				}
				else if(it->second.second<placenum){
					it->second.second=placenum;
				}
			}
			else{
				std::pair<int,int>tem;
				tem.first=placenum;
				tem.second=placenum;
				biglong[name]=tem;
			}

		}
	//second channel
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
				if(it->second.first>placenum){
					it->second.first=placenum;
				}
				else if(it->second.second<placenum){
					it->second.second=placenum;
				}
			}
			else{
				std::pair<int,int>tem;
				tem.first=placenum;
				tem.second=placenum;
				biglong[name]=tem;
			}
		}	
	
	return PCBcount;
}

int main(int argc, char *argv[]){
	std::vector<std::string>top;//top  channel
	std::vector<std::string>tail;//tail chanel
	std::map<std::string,std::pair<int,int>>biglong;//node between longgest
	std::cout<<readFile(biglong,top,tail)<<std::endl;
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
	return 0;
	
}
