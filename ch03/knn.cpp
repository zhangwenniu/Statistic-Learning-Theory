// knn.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <string>
#include <fstream>
#include "knn.h"
using namespace std;


int initialData(string path, vector<vector<double> >& data, vector<int>& label, int& k, vector<double>& x) {
	ifstream fin(path);
	if (!fin) {
		printf("Error occurs when opening: %s.\n", path.c_str());
		exit(2);
	}
	// Data sample num is n;
	// Each data dimension is m;
	int n, m, id = 0;
	double t;
	if (!(fin >> n >> m)) {
		printf("Wrong data input. \n");
		printf("First line is sample num and data dimension size. \n");
		exit(3);
	}
	data = vector<vector<double> >(n, vector<double>(m));
	while (fin >> t) {
		data[id / m][id % m] = t;
		id++;
		if (id >= m * n) {
			break;
		}
	}
	int lb;
	label = vector<int>{};
	while (fin >> lb) {
		label.push_back(lb);
		id++;
		if (id >= m * n + n) {
			break;
		}
	}
	while (fin >> lb) {
		k = lb;
		id++;
		break;
	}
	x = vector<double>{};
	while (fin >> t) {
		if (id >= m * n + n + 1 + m) {
			printf("Wrong data input. out of range m * n + n + 1 + m size.");
			break;
		}
		x.push_back(t);
		id++;
	}
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
	vector<vector<double> > data;
	vector<int> label;
	vector<double> x;
	int k;
	initialData(path, data, label, k, x);
	KDTree kdt = KDTree(data, label);

	kdt.vote(x, k);

	return 0;
}
