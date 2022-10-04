#pragma once
#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include "mode.hpp"

using namespace std;

//第一个邻接表保存站点信息
typedef struct ANode{
	string stationName;		//站点名称
	string busNo;			//路线编号（边权值）
}ArcNode;	//边结点

typedef struct Vnode {
	string stationName;
	vector<ArcNode> ArcList;		//边结点
}VNode;		//头结点

class Graph {
private:
	vector<ArcNode> result;
	vector<VNode> headlist;
	map<string, vector<string>> route;	//string是站点，vector<string>是路线
	map<string, vector<string>> sta;	//string是路线，vector<string>是站点
	map<string, vector<string>> busToBus;//从路线到路线
	VNode startStation;
	VNode endStation;
	Mode mode;
public:
	//设置起点和终点站
	Graph(string start, string end, Mode mode);

	//初始化图，创建邻接表
	void InitGraph();
	
	//显示邻接表内容
	void ShowAdjacencyList();
	void ShowRoute();

	//模糊查询
	void FuzzySearch();
	
	//搜索路径
	void Search();
	void BFS();
	void BFSRoute();
	void InitBusToBus();	//初始化从路线到路线
	string FindCrossStation(string busNo1, string busNo2);	//找到两条路线的交汇点，返回站点名称
	vector<ArcNode> singleBus(string start, string end, string busNo);	//在一条路线中查找路径

	//显示结果路径
	void ShowResult();

	friend bool operator==(VNode head1, VNode head2);
	friend bool operator!=(VNode head1, VNode head2);
	friend bool operator <(VNode head1, VNode head2);
	friend bool operator==(ArcNode node1, ArcNode node2);
};


#endif // !__GRAPH_HPP__
