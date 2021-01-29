#ifndef _PERCEPTRON
#define _PERCEPTRON
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
#endif // !_PERCEPTRON

#pragma once

struct Data{
public:
	vector<double> x; // feature vector;
	int y; // instance label. 
	Data(){}
	Data(vector<double> _x, int _y): x(_x), y(_y){}
};

class Perceptron {
private:
	vector<Data> data; // train data;
	int N, n; // N: number of input data; n: vector size of each data;
	bool printLog;
	vector<double> w; // weight vector; 
	vector<double> alpha; // alpha vector: 对偶形式下的修正次数。
	vector<vector<double> > Gram;
	double b; // bias;

public:
	Perceptron(vector<Data> _data) :data(_data), N(_data.size()), n(_data[0].x.size()), w(n, 0), b(0), printLog(0), Gram(N) {}
	Perceptron(vector<Data> _data, vector<double> _w) :data(_data), N(_data.size()), n(_data[0].x.size()), w(_w), b(0), printLog(0), Gram(N) {}
	Perceptron(vector<Data> _data, vector<double> _w, double _b) :data(_data), N(_data.size()), n(_data[0].x.size()), w(_w), b(_b), printLog(0), Gram(N) {}
	Perceptron(vector<Data> _data, double _b) :data(_data), N(_data.size()), n(_data[0].x.size()), w(n, 0), b(_b), printLog(0), Gram(N) {}
	void set_Print_Log(bool _printLog) {
		printLog = _printLog;
	}
	double dotMulti(vector<double>& a, vector<double>& b) {
		// 两个向量的点乘（内积）
		double sum = 0;
		int nn = a.size();
		for (int i = 0; i < nn; i++) {
			sum += a[i] * b[i];
		}
		return sum;
	}
	double distance(int i) {
		// calculate distance from data[i] to hyperplane.
		double sum = b;
		for (int j = 0; j < n; j++) {
			sum += w[j] * data[i].x[j];
		}
		return sum;
	}

	int signx(double x) {
		// sign function.  
		if (x >= 0)return 1;
		else return -1;
	}
	
	int judgeWrong(int i, double& sum) {
		// judge whether each point got wrong classification. 
		sum = distance(i) * data[i].y;
		if (sum <= 0)return 1;
		else return 0;
	}

	double loss_hyper_distance() {
		double loss = 0;
		for (int i = 0; i < N; i++) {
			double eachLoss = 0;
			if (judgeWrong(i, eachLoss)) {
				loss += -eachLoss;
			}
		}
		return loss;
	}

	int justify_Ori_Form(double lt, int trainLimit) {
		// lt: learning rate; 0 < lt <= 1;
		int flag = 1, wrongNum=0;
		double eachLoss = 0;
		while (flag) {
			flag = 0;
			for (int i = 0; i < N; i++) {
				if (judgeWrong(i, eachLoss)) {
					for (int j = 0; j < n; j++) {
						w[j] += lt * data[i].y * data[i].x[j];
					}
					b += lt * data[i].y;
					if (printLog) {
						printf("WrongNum = %d. Data[%d] judge Wrong. w: ", ++wrongNum, i+1);
						for (int j = 0; j < n; j++) {
							printf("%.1lf ", w[j]);
						}
						printf("b: %.1lf\n", b);
						printf("w·x + b: ");
						for (int j = 0; j < n; j++) {
							printf("%.1lfx^{(%d)}+", w[j], j + 1);
						}
						printf("%.1lf\n", b);
					}
					flag = 1;
				}
			}
			/*
			一般来说，只要点集是线性可分的，就一定可以在有限次数内通过上面的随机梯度算法产生一个超平面，使得正类和
			负类完全分开。判断点集的收敛性需要用到正集和负集，两个向量组是否存线性相关，计算量较大，暂时不做预处理
			讨论，仅仅通过设置迭代次数上限进行迭代次数的限制。如果在迭代次数上限内，超平面没有完成收敛，则简单断定
			为收敛失败，检查学习速率、收敛上限或点集是否具有线性可分性。
			*/
			if (!--trainLimit) {
				return 0;
			}
		}
		if (printLog) {
			printf("Justify to accurate. w: ");
			for (int j = 0; j < n; j++) {
				printf("%.1lf ", w[j]);
			}
			printf("b: %.1lf\n", b);
		}
		printf("w·x + b: \n$");
		for (int j = 0; j < n; j++) {
			printf("%.1lfx^{(%d)}+", w[j], j + 1);
		}
		printf("%.1lf$\n", b);

		return 1;
	}

	int justify_Dual_Form(double lt, int trainLimit) {
		for(int i = 0; i < N; i++)Gram[i].resize(N);
		alpha.resize(N);
		for (int i = 0; i < N; i++) {
			for (int j = i; j < N; j++) {
				Gram[i][j] = Gram[j][i] = dotMulti(data[i].x, data[j].x);
			}
		}
		int flag = 1, wrongNum = 0;
		while (flag) {
			flag = 0;
			for (int i = 0; i < N; i++) {
				double eachCheack = b;
				for (int j = 0; j < N; j++) {
					eachCheack += alpha[j] * data[j].y * Gram[j][i];
				}
				eachCheack *= data[i].y;
				if (eachCheack <= 0) {
					alpha[i] += lt;
					b += lt * data[i].y;
					flag = 1;
					if (printLog) {
						printf("WrongNum = %d. Data[%d] judge Wrong. alpha: ", ++wrongNum, i+1);
						for (int j = 0; j < N; j++) {
							printf("%.0lf ", alpha[j]);
						}
						printf("b: %.0lf\n", b);
					}

				}
			}
			/*
			同上。
			一般来说，只要点集是线性可分的，就一定可以在有限次数内通过上面的随机梯度算法产生一个超平面，使得正类和
			负类完全分开。判断点集的收敛性需要用到正集和负集，两个向量组是否存线性相关，计算量较大，暂时不做预处理
			讨论，仅仅通过设置迭代次数上限进行迭代次数的限制。如果在迭代次数上限内，超平面没有完成收敛，则简单断定
			为收敛失败，检查学习速率、收敛上限或点集是否具有线性可分性。
			*/
			if (!--trainLimit) {
				return 0;
			}
		}
		if (printLog) {
			printf("Justify to accurate. w: ");
			for (int j = 0; j < N; j++) {
				printf("%.0lf ", alpha[j]);
			}
			printf("b: %.0lf\n", b);
		}
		printf("w·x + b: \n$");
		for (int j = 0; j < n; j++) {
			printf("%.1lfx^{(%d)}+", w[j], j + 1);
		}
		printf("%.1lf$\n", b);

		return 1;
	}

};
