#include"gui.h"
int readFile(std::map<std::string, Node>& biglong, std::vector<std::string>& top, std::vector<std::string>& tail,char *argv[]) { //return pcb long
	int PCBcount = 0;
	std::fstream infile;
	infile.open(argv[1], std::ios::in);
	std::string name;//name
	std::string line;

	std::map<std::string, Node>::iterator it;//iterator

	//stringstream
	std::stringstream ss;
	std::stringstream files;

	//first chaneel
	std::getline(infile, line);
	files.str(line);
	ss.clear();
	ss.str("");
	while (files >> name) {
		ss << name;
		int placenum;
		ss >> placenum;
		top.push_back(name);
		//find
		it = biglong.find(name);
		if (it != biglong.end()) {
			if (it->second.begin > PCBcount) {
				it->second.begin = PCBcount;
			}
			else if (it->second.end < PCBcount) {
				it->second.end = PCBcount;
			}
		}
		else {
			Node tem;
			tem.begin = PCBcount;
			tem.end = PCBcount;
			tem.name=name;
			biglong[name] = tem;
		}
		PCBcount++;

	}
	//second channel
	PCBcount = 0;
	std::getline(infile, line);
	files.clear();
	files.str(line);
	ss.clear();
	ss.str("");
	while (files >> name) {
		ss << name;
		int placenum;
		ss >> placenum;
		tail.push_back(name);
		it = biglong.find(name);
		if (it != biglong.end()) {
			if (it->second.begin > PCBcount) {
				it->second.begin = PCBcount;
			}
			else if (it->second.end < PCBcount) {
				it->second.end = PCBcount;
			}
		}
		else {
			Node tem;
			tem.begin = PCBcount;
			tem.end = PCBcount;
			tem.name = name;
			biglong[name] = tem;
		}
		PCBcount++;
	}

	return PCBcount;
}


int main(int argc, char* argv[]) {
	std::vector<std::string>top;//top  channel
	std::vector<std::string>tail;//tail chanel
	std::map<std::string, Node>biglong;//node between longgest
	std::cout << readFile(biglong, top, tail,argv) << std::endl;
	//sort
	std::map<std::string, Node>::iterator it;//iterator
	std::vector<Node>sortNode;
	for(it=biglong.begin();it!=biglong.end();it++)
	{
		sortNode.push_back(it->second);
	}
	std::sort(sortNode.begin(),sortNode.end());
	//left edge
	
	std::map<std::string, std::pair<std::vector<Node>, int>>track;
	int trackcount = 0;
	std::map<std::string, std::pair<std::vector<Node>, int>>::iterator ittrack;
	for (int i = 0; i < sortNode.size(); i++)//Node sort
	{
		if(sortNode[i].name =="0") continue;
		bool isfind = false;
		for (ittrack = track.begin(); ittrack != track.end(); ittrack++) {
			if (ittrack->second.second < sortNode[i].begin) {
				ittrack->second.first.push_back(sortNode[i]);
				ittrack->second.second = sortNode[i].end;
				isfind = true;
				break;
			}
		}
		if (isfind == false) {
			std::stringstream ss;
			ss<<trackcount;
			std::string tracklayer;
			ss>>tracklayer;
			std::pair<std::vector<Node>, int>tem;
			tem.first.push_back(sortNode[i]);
			tem.second = sortNode[i].end;
			track[tracklayer] = tem;
			trackcount++;
		}
	}

	//
/*	debug
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
	std::cout << trackcount << std::endl;
	for (ittrack = track.begin(); ittrack != track.end(); ittrack++) {
		std::cout << "track   " << ittrack->first << "   -->\n";
		for (int i = 0; i < ittrack->second.first.size(); i++) {
			std::cout <<ittrack->second.first[i].name<<"->" <<ittrack->second.first[i].begin << "~" << ittrack->second.first[i].end << std::endl;
		}
	}


	//gtk+cairo
	drawpng(biglong, top, tail,track);
	drawgui(argc, argv);

	return 0;

}
