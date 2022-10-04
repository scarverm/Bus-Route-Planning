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
	cout << "===============��ʼ��ͼ��===============" << endl;
	//��ȡbusroute.txt�ļ�
	ifstream file;
	char buf[50];
	map<string, VNode> m;
	file.open("busroute.txt", ios::in);
	if (!file.is_open()) {
		cout << "�ļ���ʧ��!" << endl;
		exit(1);
	}
	ArcNode station;			//����·�߱�źʹ洢��һվ��
	VNode head;
	bool isStartStation = true;	//�жϵ�ǰ�Ƿ������վ�����վû����һվ�㡣��Ϊ��˫��ģ�������һվ��ҲӦ���ڱ߽�㼯��
	bool setRoute = false;
	while (file >> buf) {	//��·��ת��Ϊ�ڽӱ�洢��ͼ
		//��ȡ·�߱��
		if (buf[0] == '#') {
			char* p = buf;
			station.busNo = p + 1;
			isStartStation = true;
			continue;
		}

		//����һ���½�㣬�ͽ�������Ϊͷ���
		head.stationName = "";
		head.stationName += buf;	//ͷ�����Ҫһ��վ������һ���յı߽�㼯
		head.ArcList.clear();
		if (m.find(head.stationName) == m.end()) {
			m[head.stationName] = head;
		}
		
		//����route
		route[head.stationName].push_back(station.busNo);
		sta[station.busNo].push_back(head.stationName);

		//��ȡվ����
		if (isStartStation) {
			station.stationName = "";	//���վû����һվ��ֱ�Ӹ�Ϊ��ǰվ��
			station.stationName += buf;
			isStartStation = false;
		}
		else {
			//����������վ���򽫵�ǰ����Ϊ��һ���ı߽�㣬ͬʱ����һ����Ϊ���ڵ�ı߽��
			m[head.stationName].ArcList.push_back(station);
			ArcNode tmp;
			tmp.stationName = head.stationName;
			tmp.busNo = station.busNo;
			m[station.stationName].ArcList.push_back(tmp);

			station.stationName = "";		//station��Ϊ��ǰվ��
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
			cout << headlist[i].ArcList[j].busNo << " " << headlist[i].ArcList[j].stationName << " �� ";
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
	cout << "===============���ڲ�ѯվ��===============" << endl;
	auto s = find(headlist.begin(), headlist.end(), startStation);
	auto e = find(headlist.begin(), headlist.end(), endStation);
	if (s != headlist.end() && e != headlist.end()) {
		return;
	}
	if (s == headlist.end()) {
		cout << "δ���ҵ�" << startStation.stationName << "������Ϊ��ƥ��վ�㡭��" << endl;
		map<VNode, int> start;
		int  startCount = 0;
		for (int i = 0; i < headlist.size(); i++) {
			for (int j = 0; j < startStation.stationName.length(); j += 2) {
				string sub = startStation.stationName.substr(j, 2);	//����ʹ��2
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
			cout << "δ��ƥ�䵽" << startStation.stationName << endl;
			exit(0);
		}
		cout << "����ѡ������վ�����ǣ�" << endl;
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
		cout << endl << "��Ϊ��ƥ��" << startStation.stationName << endl;
	}
	if (e == headlist.end()) {
		cout << "δ���ҵ�" << endStation.stationName << "������Ϊ��ƥ��վ�㡭��" << endl;
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
			cout << "δ��ƥ�䵽" << endStation.stationName << endl;
			exit(0);
		}
		cout << "����ѡ������վ�����ǣ�" << endl;
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
		cout << endl << "��Ϊ��ƥ��" << endStation.stationName << endl;
	}
}

void Graph::Search() {
	cout << "===============��������===============" << endl;
	if (mode == LESS_STATION) {
		cout << "��ǰģʽ��-b" << endl;
		BFS();
	}
	else {
		cout << "��ǰģʽ��-c" << endl;
		BFSRoute();
	}
}

void Graph::BFS() {
	queue<VNode> q;
	vector<VNode> yVisited;	//�������Ľ�㼯
	map<VNode, Vnode> lastNode;	//��һ��VNode��ǰһ��VNode��ǰ�������ڱ���·��
	map<VNode, string> lastBus;	//��һվ����վ��·��

	//����
	VNode start = startStation;
	q.push(start);
	yVisited.push_back(start);
	while (!q.empty() && q.back() != endStation) {
		//�����ڽӱ����
		auto adj = find(headlist.begin(), headlist.end(), q.front());

		for (int i = 0; i < adj->ArcList.size(); i++) {
			//��ǰ����Ǳ߽�㣬ת��Ϊͷ���
			VNode vnode;
			vnode.stationName = adj->ArcList[i].stationName;

			//�����δ�������Ľ�㣬�����Щ�����������
			if (find(yVisited.begin(), yVisited.end(), vnode) == yVisited.end()) {
				q.push(vnode);
				yVisited.push_back(vnode);
				lastNode[vnode] = q.front();
				lastBus[vnode] = adj->ArcList[i].busNo;
			}
		}

		//�ڶ�����ɾ���ý��
		q.pop();
	}

	//�ѽ������result��
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
	//��ȡ���վ���յ�վ���ڵ�·��
	vector<string> start = route[startStation.stationName];
	vector<string> end = route[endStation.stationName];

	//����map����Ӳ�ͬ·�߳����ĳɱ�(���˴���)������һ·��
	map<string, string> lastRoute;
	int minCost = 1000;
	string startBus;
	string endBus;

	//��ʼ��busToBus
	InitBusToBus();

	//��������յ���ͬһ·����
	for (int i = 0; i < start.size(); i++) {
		if (find(end.begin(), end.end(), start[i]) != end.end()) {
			vector<ArcNode> tmp = singleBus(startStation.stationName, endStation.stationName, start[i]);
			result = tmp;
			return;
		}
	}

	//�ֱ�Ӳ�ͬ�����·�߳��������Ƚ�·���ɱ�
	for (auto s = start.begin(); s != start.end(); s++) {

		//���ö��С��ѱ������ϡ���һ·�߼���
		queue<string> q;
		vector<string> visited;
		map<string, string> last;

		string sb;	//startBus
		sb = *s;
		string eb;	//endBus

		//����ʼ·�߷��������
		q.push(*s);

		//����ʼ·�߷���visited
		visited.push_back(*s);

		//����
		while (!q.empty()) {
			//����ͷ����Ϊ�������
			string begin = q.front();

			//���ڽ�·�߷������
			for (int i = 0; i < busToBus[begin].size(); i++) {
				if (find(visited.begin(), visited.end(), busToBus[begin][i]) == visited.end()) {
					q.push(busToBus[begin][i]);
					visited.push_back(busToBus[begin][i]);
					last[busToBus[begin][i]] = begin;
				}
			}

			//����ǰ·�߳���
			q.pop();

			//����յ��ѱ����������˳�ѭ��
			bool out = false;
			for (int i = 0; i < end.size(); i++) {
				if (find(visited.begin(), visited.end(), end[i]) != visited.end()) {
					eb = end[i];
					out = true;
				}
			}
			if (out) break;
		}

		//����ӵ�ǰ·���������յ��·���ɱ�
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

	//��������·�߽����֮���·�ߣ�����·�ߴ���result��
	string midStartStation = endStation.stationName;	//�Ӻ���ǰ��
	string midEndStation;
	string midStartRoute = endBus;
	string midEndRoute;
	while (midEndStation != startStation.stationName) {
		//��ȡ�������ڵ�·������
		midEndRoute = lastRoute[midStartRoute];

		//�����һ��·��Ϊ�գ����������վ����·��
		if (midEndRoute == "") {
			vector<ArcNode> midRoute;
			midRoute = singleBus(midStartStation, startStation.stationName, midStartRoute);
			result.insert(result.end(), midRoute.begin(), midRoute.end());
			break;
		}

		//�ҵ�����·�ߵĽ��㣬�����ͻ����ͬһ·���ϵ�������յ�վ
		midEndStation = FindCrossStation(midStartRoute, midEndRoute);
		string thisRoute = midStartRoute;
		
		//ͨ��ͬһ·���ϵ�������յ�վѰ������֮���·��
		vector<ArcNode> midRoute;
		midRoute = singleBus(midStartStation, midEndStation, thisRoute);

		//midRoute������������յ�վ����������ټ���������һ·�ߣ���ʱ���յ�վ���Ϊ���վ���ᵼ��result��·���ظ�
		//ɾȥ�յ�վ
		if (midRoute.size() > 1) {
			ArcNode tmp;
			tmp.stationName = midEndStation;
			tmp.busNo = thisRoute;
			auto it = find(midRoute.begin(), midRoute.end(), tmp);
			midRoute.erase(it);
			result.insert(result.end(), midRoute.begin(), midRoute.end());
		}

		//�������·�ߺ����վ
		midStartRoute = midEndRoute;
		midStartStation = midEndStation;
	}
}

void Graph::InitBusToBus(){
	for (auto it = sta.begin(); it != sta.end(); it++) {
		string zhan;	//վ��
		for (int i = 0; i < it->second.size(); i++) {
			zhan = it->second[i];
			for (int j = 0; j < route[zhan].size(); j++) {
				if (find(busToBus[it->first].begin(), busToBus[it->first].end(), route[zhan][j]) == busToBus[it->first].end()) {
					busToBus[it->first].push_back(route[zhan][j]);
				}
			}
		}
	}
	//��ʾ�ڽӱ�busToBus
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
	vector<ArcNode> head;	//��ǰ����
	vector<ArcNode> back;	//������
	ArcNode tmp;
	tmp.busNo = busNo;
	bool headFind = false, backFind = false;
	auto it = find(sta[busNo].begin(), sta[busNo].end(), start);
	auto itHead = it, itBack = it;	//ǰ��ָ��
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
		cout << "δ�õ���ȷ���" << endl;
		return;
	}
	cout << result.size();
	reverse(result.begin(), result.end());
	string busNo = result[1].busNo;
	for (auto it = result.begin(); it != result.end(); it++) {
		if (it->busNo != busNo && it->busNo != "") {
			busNo = it->busNo;
			cout << "����" << it->busNo << "·������";
			cout << endl << it->stationName;
			//cout << it->stationName << "����" << it->busNo << "·������" << endl;
		}
		else {
			cout << endl << it->stationName;
		}
	}
	cout << endl;
}
