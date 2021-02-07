// knn.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <string>
#include <fstream>
#include "knn.h"
using namespace std;


int initialData(string path, vector<vector<double> >& data, vector<int>& label, int& k, vector<double>& x) {
	// 生成流数据。
	ifstream fin(path);
	// 输入文件地址非法。
	if (!fin) {
		printf("Error occurs when opening: %s.\n", path.c_str());
		exit(2);
	}
	// Data sample num is n;
	// Each data dimension is m;
	int n, m, id = 0;
	double t;
	// 文件为空
	if (!(fin >> n >> m)) {
		printf("Wrong data input. \n");
		printf("First line is sample num and data dimension size. \n");
		exit(3);
	}
	data = vector<vector<double> >(n, vector<double>(m));
	// 获取N行M列的已有数据。
	while (fin >> t) {
		data[id / m][id % m] = t;
		id++;
		if (id >= m * n) {
			break;
		}
	}
	int lb;
	label = vector<int>{};
	// 读取已有数据的标签。
	while (fin >> lb) {
		label.push_back(lb);
		id++;
		if (id >= m * n + n) {
			break;
		}
	}
	// 读取K近邻的K值，复用了lb。
	while (fin >> lb) {
		k = lb;
		id++;
		break;
	}
	x = vector<double>{};
	// 读取待查询的坐标值。
	while (fin >> t) {
		if (id >= m * n + n + 1 + m) {
			printf("Wrong data input. out of range m * n + n + 1 + m size.");
			break;
		}
		x.push_back(t);
		id++;
	}
	// 输入数据与自己定义的数据规模不符。
	if (id < m * n + n + 1 + m) {
		printf("Wrong input data. Data num should equal to n * m + n. \n");
		exit(4);
	}
	else {
		printf("input done. \n");
	}
	return 1;
}

int main()
{
	string path = "./data.txt";
	// data是输入的数据。
	vector<vector<double> > data;
	// label为每个数据对应的类别。
	vector<int> label;
	// x是待查询节点的位置向量。
	vector<double> x;
	// k是K近邻的数量限制。
	int k;
	// 从文件中初始化读入数据。
	initialData(path, data, label, k, x);
	// 生成kd树的实例。
	KDTree kdt = KDTree(data, label);

	// 通过K近邻方法投票产生查询数据的预测类别。
	kdt.vote(x, k);
	// 绘制graphviz的.dot图，采用了中根遍历的方法实现。
	kdt.drawKDTree(kdt.head, "./out.dot");
	return 0;
}
