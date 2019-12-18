#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<map>
#include<sstream>
int readFile(){ //return pcb long
	int PCBcount =0;
	std::fstream infile;
	infile.open("test.pin",std::ios::in);
	std::string name;//name
	std::string line;
	std::map<std::string,std::vector<int>>topPCB;//top PCB
	std::map<std::string,std::vector<int>>underPCB;//underlayer
	std::stringstream ss;
	std::stringstream files;
	std::getline(infile,line);
		files.str(line);
		ss.clear();
		ss.str("");
		while(files>>name){
			ss<<name;
		    int placenum;
			ss>>placenum;	   
			topPCB[name].push_back(placenum);	
			PCBcount++;
		}
	std::getline(infile,line);
		files.clear();
		files.str(line);
		ss.clear();
		ss.str("");
		while(files>>name){
			ss<<name;
		    int placenum;
			ss>>placenum;	   
			underPCB[name].push_back(placenum);	
		}	
	
	return PCBcount;
}
int main(int argc, char *argv[]){
	std::cout<<readFile()<<std::endl;
	return 0;
	
}
