#include "graph.hpp"

bool operator==(VNode head1, VNode head2){
	return head1.stationName == head2.stationName;
}

bool operator!=(VNode head1, VNode head2) {
	return !(head1 == head2);
}

bool operator <(VNode head1, VNode head2) {
	return head1.stationName < head2.stationName;
}

bool operator==(ArcNode node1, ArcNode node2){
	return node1.stationName == node2.stationName;
}

Graph::Graph(string start, string end, Mode mode){
	startStation.stationName = start;
	endStation.stationName = end;
	this->mode = mode;
}

void Graph::InitGraph() {
	cout << "===============初始化图中===============" << endl;
	//读取busroute.txt文件
	ifstream file;
	char buf[50];
	map<string, VNode> m;
	file.open("busroute.txt", ios::in);
	if (!file.is_open()) {
		cout << "文件打开失败!" << endl;
		exit(1);
	}
	ArcNode station;			//保存路线编号和存储上一站点
	VNode head;
	bool isStartStation = true;	//判断当前是否是起点站，起点站没有上一站点。因为是双向的，所以上一站点也应放在边结点集中
	bool setRoute = false;
	while (file >> buf) {	//将路线转换为邻接表存储的图
		//获取路线编号
		if (buf[0] == '#') {
			char* p = buf;
			station.busNo = p + 1;
			isStartStation = true;
			continue;
		}

		//遇到一个新结点，就将其设置为头结点
		head.stationName = "";
		head.stationName += buf;	//头结点需要一个站点名和一个空的边结点集
		head.ArcList.clear();
		if (m.find(head.stationName) == m.end()) {
			m[head.stationName] = head;
		}
		
		//设置route
		route[head.stationName].push_back(station.busNo);
		sta[station.busNo].push_back(head.stationName);

		//获取站点名
		if (isStartStation) {
			station.stationName = "";	//起点站没有上一站，直接改为当前站点
			station.stationName += buf;
			isStartStation = false;
		}
		else {
			//如果不是起点站，则将当前结点存为上一结点的边结点，同时将上一结点存为本节点的边结点
			m[head.stationName].ArcList.push_back(station);
			ArcNode tmp;
			tmp.stationName = head.stationName;
			tmp.busNo = station.busNo;
			m[station.stationName].ArcList.push_back(tmp);

			station.stationName = "";		//station改为当前站点
			station.stationName += buf;
		}
	}
	for (auto it = m.begin(); it != m.end(); it++) {
		headlist.push_back(it->second);
	}
}

void Graph::ShowAdjacencyList(){
	for (int i = 0; i < headlist.size(); i++) {
		cout << headlist[i].stationName << '\t';
		for (int j = 0; j < headlist[i].ArcList.size(); j++) {
			cout << headlist[i].ArcList[j].busNo << " " << headlist[i].ArcList[j].stationName << " → ";
		}
		cout << endl;
	}
}

void Graph::ShowRoute() {
	for (auto it = route.begin(); it != route.end(); it++) {
		cout << it->first << ": ";
		for (int j = 0; j < it->second.size(); j++) {
			cout << it->second[j] << "\t";
		}
		cout << endl;
	}
}

void Graph::FuzzySearch(){
	cout << "===============正在查询站点===============" << endl;
	auto s = find(headlist.begin(), headlist.end(), startStation);
	auto e = find(headlist.begin(), headlist.end(), endStation);
	if (s != headlist.end() && e != headlist.end()) {
		return;
	}
	if (s == headlist.end()) {
		cout << "未查找到" << startStation.stationName << "，正在为您匹配站点……" << endl;
		map<VNode, int> start;
		int  startCount = 0;
		for (int i = 0; i < headlist.size(); i++) {
			for (int j = 0; j < startStation.stationName.length(); j += 2) {
				string sub = startStation.stationName.substr(j, 2);	//所以使用2
				size_t index = headlist[i].stationName.find(sub);
				if (index % 2 == 1) {
					continue;
				}
				if (index != startStation.stationName.npos) {
					startCount++;
				}
			}
			if (startCount > (startStation.stationName.length() / 2) / 2) {
				VNode tmp;
				tmp.stationName = headlist[i].stationName;
				start[tmp] = startCount;
			}
			startCount = 0;
		}
		if (start.size() == 0) {
			cout << "未能匹配到" << startStation.stationName << endl;
			exit(0);
		}
		cout << "您想选择的起点站可能是：" << endl;
		int maxCount = 0;
		VNode maxNode;
		for (auto it = start.begin(); it != start.end(); it++) {
			if (maxCount < it->second) {
				maxCount = it->second;
				maxNode = it->first;
			}
			cout << it->first.stationName << "\t";
		}
		startStation.stationName = maxNode.stationName;
		cout << endl << "已为您匹配" << startStation.stationName << endl;
	}
	if (e == headlist.end()) {
		cout << "未查找到" << endStation.stationName << "，正在为您匹配站点……" << endl;
		map<VNode, int> end;
		int  endCount = 0;
		for (int i = 0; i < headlist.size(); i++) {
			for (int j = 0; j < endStation.stationName.length(); j += 2) {
				string sub = endStation.stationName.substr(j, 2);
				size_t index = headlist[i].stationName.find(sub);
				if (index % 2 == 1) {
					continue;
				}
				if (index != endStation.stationName.npos) {
					endCount++;
				}
			}
			if (endCount > (endStation.stationName.length() / 2) / 2) {
				VNode tmp;
				tmp.stationName = headlist[i].stationName;
				end[tmp] = endCount;
			}
			endCount = 0;
		}
		if (end.size() == 0) {
			cout << "未能匹配到" << endStation.stationName << endl;
			exit(0);
		}
		cout << "您想选择的起点站可能是：" << endl;
		int maxCount = 0;
		VNode maxNode;
		for (auto it = end.begin(); it != end.end(); it++) {
			if (maxCount < it->second) {
				maxCount = it->second;
				maxNode = it->first;
			}
			cout << it->first.stationName << "\t";
		}
		endStation.stationName = maxNode.stationName;
		cout << endl << "已为您匹配" << endStation.stationName << endl;
	}
}

void Graph::Search() {
	cout << "===============正在搜索===============" << endl;
	if (mode == LESS_STATION) {
		cout << "当前模式：-b" << endl;
		BFS();
	}
	else {
		cout << "当前模式：-c" << endl;
		BFSRoute();
	}
}

void Graph::BFS() {
	queue<VNode> q;
	vector<VNode> yVisited;	//遍历过的结点集
	map<VNode, Vnode> lastNode;	//后一个VNode是前一个VNode的前趋，用于保存路径
	map<VNode, string> lastBus;	//上一站到本站的路线

	//遍历
	VNode start = startStation;
	q.push(start);
	yVisited.push_back(start);
	while (!q.empty() && q.back() != endStation) {
		//设置邻接表起点
		auto adj = find(headlist.begin(), headlist.end(), q.front());

		for (int i = 0; i < adj->ArcList.size(); i++) {
			//当前结点是边结点，转换为头结点
			VNode vnode;
			vnode.stationName = adj->ArcList[i].stationName;

			//如果是未遍历过的结点，则把这些结点放入队列中
			if (find(yVisited.begin(), yVisited.end(), vnode) == yVisited.end()) {
				q.push(vnode);
				yVisited.push_back(vnode);
				lastNode[vnode] = q.front();
				lastBus[vnode] = adj->ArcList[i].busNo;
			}
		}

		//在队列中删除该结点
		q.pop();
	}

	//把结果存入result中
	ArcNode res;
	VNode tmp = endStation;
	res.stationName = tmp.stationName;
	res.busNo = lastBus[tmp];
	result.push_back(res);
	while (res.stationName != startStation.stationName) {
		tmp = lastNode[tmp];
		res.stationName = tmp.stationName;
		res.busNo = lastBus[tmp];
		result.push_back(res);
	}
}

void Graph::BFSRoute() {
	//获取起点站和终点站所在的路线
	vector<string> start = route[startStation.stationName];
	vector<string> end = route[endStation.stationName];

	//定义map保存从不同路线出发的成本(换乘次数)，和上一路线
	map<string, string> lastRoute;
	int minCost = 1000;
	string startBus;
	string endBus;

	//初始化busToBus
	InitBusToBus();

	//如果起点和终点在同一路线上
	for (int i = 0; i < start.size(); i++) {
		if (find(end.begin(), end.end(), start[i]) != end.end()) {
			vector<ArcNode> tmp = singleBus(startStation.stationName, endStation.stationName, start[i]);
			result = tmp;
			return;
		}
	}

	//分别从不同的起点路线出发，最后比较路径成本
	for (auto s = start.begin(); s != start.end(); s++) {

		//设置队列、已遍历集合、上一路线集合
		queue<string> q;
		vector<string> visited;
		map<string, string> last;

		string sb;	//startBus
		sb = *s;
		string eb;	//endBus

		//将起始路线放入队列中
		q.push(*s);

		//将起始路线放入visited
		visited.push_back(*s);

		//遍历
		while (!q.empty()) {
			//将队头设置为搜索起点
			string begin = q.front();

			//将邻近路线放入队列
			for (int i = 0; i < busToBus[begin].size(); i++) {
				if (find(visited.begin(), visited.end(), busToBus[begin][i]) == visited.end()) {
					q.push(busToBus[begin][i]);
					visited.push_back(busToBus[begin][i]);
					last[busToBus[begin][i]] = begin;
				}
			}

			//将当前路线出队
			q.pop();

			//如果终点已被遍历，则退出循环
			bool out = false;
			for (int i = 0; i < end.size(); i++) {
				if (find(visited.begin(), visited.end(), end[i]) != visited.end()) {
					eb = end[i];
					out = true;
				}
			}
			if (out) break;
		}

		//计算从当前路径出发到终点的路径成本
		int count = 0;
		string tmp;
		tmp = eb;
		while (tmp != sb) {
			count++;
			tmp = last[tmp];
		}
		count++;
		if (minCost > count) {
			minCost = count;
			lastRoute.swap(last);
			startBus = sb;
			endBus = eb;
		}
	}

	//查找两条路线交叉点之间的路线，并将路线存入result中
	string midStartStation = endStation.stationName;	//从后往前找
	string midEndStation;
	string midStartRoute = endBus;
	string midEndRoute;
	while (midEndStation != startStation.stationName) {
		//获取两条相邻的路线名称
		midEndRoute = lastRoute[midStartRoute];

		//如果上一条路线为空，即到了起点站所在路线
		if (midEndRoute == "") {
			vector<ArcNode> midRoute;
			midRoute = singleBus(midStartStation, startStation.stationName, midStartRoute);
			result.insert(result.end(), midRoute.begin(), midRoute.end());
			break;
		}

		//找到两条路线的交点，这样就获得了同一路线上的起点与终点站
		midEndStation = FindCrossStation(midStartRoute, midEndRoute);
		string thisRoute = midStartRoute;
		
		//通过同一路线上的起点与终点站寻找两者之间的路线
		vector<ArcNode> midRoute;
		midRoute = singleBus(midStartStation, midEndStation, thisRoute);

		//midRoute包含了起点与终点站，这是如果再继续查找下一路线，此时的终点站会成为起点站，会导致result中路线重复
		//删去终点站
		if (midRoute.size() > 1) {
			ArcNode tmp;
			tmp.stationName = midEndStation;
			tmp.busNo = thisRoute;
			auto it = find(midRoute.begin(), midRoute.end(), tmp);
			midRoute.erase(it);
			result.insert(result.end(), midRoute.begin(), midRoute.end());
		}

		//更新起点路线和起点站
		midStartRoute = midEndRoute;
		midStartStation = midEndStation;
	}
}

void Graph::InitBusToBus(){
	for (auto it = sta.begin(); it != sta.end(); it++) {
		string zhan;	//站点
		for (int i = 0; i < it->second.size(); i++) {
			zhan = it->second[i];
			for (int j = 0; j < route[zhan].size(); j++) {
				if (find(busToBus[it->first].begin(), busToBus[it->first].end(), route[zhan][j]) == busToBus[it->first].end()) {
					busToBus[it->first].push_back(route[zhan][j]);
				}
			}
		}
	}
	//显示邻接表busToBus
	/*for (auto it = busToBus.begin(); it != busToBus.end(); it++) {
		cout << it->first << ": " << endl;
		for (int i = 0; i < it->second.size(); i++) {
			cout << it->second[i] << "\t";
		}
		cout << endl;
	}*/
}

string Graph::FindCrossStation(string busNo1, string busNo2)
{
	string name;
	for (int i = 0; i < sta[busNo1].size(); i++) {
		if (find(sta[busNo2].begin(), sta[busNo2].end(), sta[busNo1][i]) != sta[busNo2].end()) {
			return sta[busNo1][i];
		}
	}
	return "";
}

vector<ArcNode> Graph::singleBus(string start, string end, string busNo){
	if (start == end) {
		return {};
	}
	vector<ArcNode> head;	//向前查找
	vector<ArcNode> back;	//向后查找
	ArcNode tmp;
	tmp.busNo = busNo;
	bool headFind = false, backFind = false;
	auto it = find(sta[busNo].begin(), sta[busNo].end(), start);
	auto itHead = it, itBack = it;	//前后指针
	tmp.stationName = *itHead;
	head.push_back(tmp);
	while (true) {
		if (*itHead != end && itHead != sta[busNo].begin())
			itHead--;
		else
			break;
		tmp.stationName = *itHead;
		head.push_back(tmp);
		if (*itHead == end) {
			headFind = true;
			break;
		}
	}
	tmp.stationName = *itBack;
	back.push_back(tmp);
	itBack++;
	while (itBack != sta[busNo].end()) {
		tmp.stationName = *itBack;
		back.push_back(tmp);
		if (*itBack == end) {
			backFind = true;
			break;
		}
		itBack++;
	}
	if (headFind) {
		return head;
	}
	else {
		reverse(back.begin(), back.end());
		return back;
	}
}

void Graph::ShowResult(){
	if (result.size() <= 1) {
		cout << "未得到正确结果" << endl;
		return;
	}
	cout << result.size();
	reverse(result.begin(), result.end());
	string busNo = result[1].busNo;
	for (auto it = result.begin(); it != result.end(); it++) {
		if (it->busNo != busNo && it->busNo != "") {
			busNo = it->busNo;
			cout << "换乘" << it->busNo << "路公交车";
			cout << endl << it->stationName;
			//cout << it->stationName << "换乘" << it->busNo << "路公交车" << endl;
		}
		else {
			cout << endl << it->stationName;
		}
	}
	cout << endl;
}
