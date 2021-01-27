#ifndef _GEB
#include <vector>
#include <cmath>
#include <iostream>
#include <cstdio>
using namespace std;
#endif // !1
#define _GEB
#pragma once

class Loss {
public:
    // 0-1损失函数
    double loss0_1(vector<int>& pred, vector<int>& vexp) {
        int N = pred.size();
        double loss = 0;
        for (int i = 0; i < N; i++) {
            loss += (pred[i] != vexp[i]);
        }
        return loss / N;
    }

    // 平方损失函数
    double loss_quadratic(vector<int>& pred, vector<int>& vexp) {
        int N = pred.size();
        double loss = 0;
        for (int i = 0; i < N; i++) {
            double temp = (pred[i] - vexp[i]);
            loss += temp * temp;
        }
        return loss / N;
    }

    // 绝对值损失函数
    double loss_absolute(vector<int>& pred, vector<int>& vexp) {
        int N = pred.size();
        double loss = 0;
        for (int i = 0; i < N; i++) {
            loss += abs(pred[i] - vexp[i]);
        }
        return loss / N;
    }

    // 对数损失函数，暂时没有。
    double loss_logarithmic() {
        return 0;
    }
};

class Analysis {
public:
    // 计算误差。
    double loss(vector<int>& vpred, vector<int>& vexp) {
        // 这里的误差函数是基于N个样本得到的经验风险值。
        // 经验风险empirical risk
        return Loss().loss0_1(vpred, vexp);
    }

    // 计算epsilon函数。
    double epsilon(int d, int N, double delta) {
        double ans = sqrt(1.0 / (2.0 * N) * (log((double)d) - log(delta)));
        return ans;
    }

    // 泛化误差上界。
    // Generalization Error Bound: geb;
    double geb(vector<int>& vpred, vector<int>& vexp, int d, double prob) {
        // vpred: 由训练得到模型预测的结果
        // vexp:  原始数据希望得到的真实标签
        int N = vpred.size();
        // N表示样本容量。
        // d表示假设空间中模型的数量。
        // prob为置信度，也就是以多大概率相信该经验风险上界的分析结果。
        double delta = 1 - prob;
        return loss(vpred, vexp) + epsilon(d, N, delta);
    }
};


