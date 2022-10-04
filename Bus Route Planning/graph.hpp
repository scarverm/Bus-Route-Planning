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

//��һ���ڽӱ���վ����Ϣ
typedef struct ANode{
	string stationName;		//վ������
	string busNo;			//·�߱�ţ���Ȩֵ��
}ArcNode;	//�߽��

typedef struct Vnode {
	string stationName;
	vector<ArcNode> ArcList;		//�߽��
}VNode;		//ͷ���

class Graph {
private:
	vector<ArcNode> result;
	vector<VNode> headlist;
	map<string, vector<string>> route;	//string��վ�㣬vector<string>��·��
	map<string, vector<string>> sta;	//string��·�ߣ�vector<string>��վ��
	map<string, vector<string>> busToBus;//��·�ߵ�·��
	VNode startStation;
	VNode endStation;
	Mode mode;
public:
	//���������յ�վ
	Graph(string start, string end, Mode mode);

	//��ʼ��ͼ�������ڽӱ�
	void InitGraph();
	
	//��ʾ�ڽӱ�����
	void ShowAdjacencyList();
	void ShowRoute();

	//ģ����ѯ
	void FuzzySearch();
	
	//����·��
	void Search();
	void BFS();
	void BFSRoute();
	void InitBusToBus();	//��ʼ����·�ߵ�·��
	string FindCrossStation(string busNo1, string busNo2);	//�ҵ�����·�ߵĽ���㣬����վ������
	vector<ArcNode> singleBus(string start, string end, string busNo);	//��һ��·���в���·��

	//��ʾ���·��
	void ShowResult();

	friend bool operator==(VNode head1, VNode head2);
	friend bool operator!=(VNode head1, VNode head2);
	friend bool operator <(VNode head1, VNode head2);
	friend bool operator==(ArcNode node1, ArcNode node2);
};


#endif // !__GRAPH_HPP__
