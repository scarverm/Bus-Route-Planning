#include <iostream>
#include "mode.hpp"
#include "graph.hpp"
using namespace std;

int main(int argc, char** argv) {
	/*命令行参数获取，“hkbus.exe -b station1 station2”
	* -b:经过站点数最少
	* -c:换乘最少
	* 返回经过的站点的个数和路径，如有换乘，列出换乘的线路
	*/
	Mode mode;
	if (argc != 4) {		//参数数量不正确
		cout << "请输入正确的命令行参数!" << endl;
		exit(0);
	}
	if (argv[1][0] != '-' || strlen(argv[1]) != 2) {		//第二个参数(-b、-c)不正确
		cout << "请输入正确的命令行参数!" << endl;
		exit(0);
	}
	if (argv[1][1] == 'b') mode = LESS_STATION;			//获取第二个参数
	else if (argv[1][1] == 'c') mode = LESS_BUS;
	else {
		cout << "请输入正确的命令行参数!" << endl;
		exit(0);
	}
	string startStation(argv[2]);
	string endStation(argv[3]);

	Graph graph(startStation, endStation, mode);
	graph.InitGraph();
	//graph.ShowAdjacencyList();
	//graph.ShowRoute();
	graph.FuzzySearch();
	graph.Search();
	graph.ShowResult();
	
	system("pause");
	return 0;
}