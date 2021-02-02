// perceptron.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "perceptron.h"
using namespace std;

vector<Data> dt;
int initialData() {
	vector<double> vt(2);
	// 线性可分数据。
	vt[0] = 3;  vt[1] = 3;
	dt.push_back(Data(vt, 1));
	vt[0] = 4;  vt[1] = 3;
	dt.push_back(Data(vt, 1));
	vt[0] = 1;  vt[1] = 1;
	dt.push_back(Data(vt, -1));
	// 异或数据。线性不可分。
	/*
	vt[0] = 0;  vt[1] = 0;
	dt.push_back(Data(vt, -1));
	vt[0] = 1;  vt[1] = 1;
	dt.push_back(Data(vt, -1));
	vt[0] = 1;  vt[1] = 0;
	dt.push_back(Data(vt, 1));
	vt[0] = 0;  vt[1] = 1;
	dt.push_back(Data(vt, 1));
	*/
	return 0;
}

int main()
{
	initialData();
	Perceptron p = Perceptron(dt);
	p.set_Print_Log(1);
	// 调用原始形式的算法
	p.justify_Ori_Form(1, 100);
	// 调用对偶问题的算法。
	//p.justify_Dual_Form(1, 100);
	// 由于初始化的问题，调用一个算法之后会改变内部的超平面位置。所以只能二选一：
	// 在main函数中只能选择调用原始形式或者对偶形式两种方法之一。
	return 0;
}
