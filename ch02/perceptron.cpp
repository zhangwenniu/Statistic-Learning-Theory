// perceptron.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "perceptron.h"
using namespace std;

vector<Data> dt;
int initialData() {
	vector<double> vt(2);
	// 线性可分数据。
	/*
	vt[0] = 3;  vt[1] = 3;
	dt.push_back(Data(vt, 1));
	vt[0] = 4;  vt[1] = 3;
	dt.push_back(Data(vt, 1));
	vt[0] = 1;  vt[1] = 1;
	dt.push_back(Data(vt, -1));
	*/
	// 异或数据。线性不可分。
	vt[0] = 0;  vt[1] = 0;
	dt.push_back(Data(vt, -1));
	vt[0] = 1;  vt[1] = 1;
	dt.push_back(Data(vt, -1));
	vt[0] = 1;  vt[1] = 0;
	dt.push_back(Data(vt, 1));
	vt[0] = 0;  vt[1] = 1;
	dt.push_back(Data(vt, 1));
	return 0;
}

int main()
{
	initialData();
	Perceptron p = Perceptron(dt);
	p.set_Print_Log(1);
	p.justify_Ori_Form(1, 100);
	//p.justify_Dual_Form(1, 100);
	return 0;
}
