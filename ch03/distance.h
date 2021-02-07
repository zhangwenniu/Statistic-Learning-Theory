#pragma once
#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#define INF 0x7fffffff
using namespace std;

class Distance {
public:
	int k;
	Distance(int _k):k(_k) {}
	double Minkowski(vector<double> x, vector<double> y) {
		int n = x.size();
		if (x.size() != y.size()) {
			printf("Error Message: in Distance.Minkowski(). \n");
			printf("x.size() = %d, y.size() = %d. They are guaranteed to be same. ", x.size(), y.size());
			exit(-1);
		}
		double dis = 0;
		if (k == INF) {
			// k趋于无穷的时候，就应该是所有坐标距离的最大值。
			for (int i = 0; i < n; i++) {
				dis = max(dis, abs(x[i] - y[i]));
			}
		}
		else {
			for (int i = 0; i < n; i++) {
				dis += pow(fabs(x[i] - y[i]), k);
			}
			dis = powf(dis, 1.0 / k);
		}
		return dis;
	}
};
