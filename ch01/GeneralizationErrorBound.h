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
    // 0-1��ʧ����
    double loss0_1(vector<int>& pred, vector<int>& vexp) {
        int N = pred.size();
        double loss = 0;
        for (int i = 0; i < N; i++) {
            loss += (pred[i] != vexp[i]);
        }
        return loss / N;
    }

    // ƽ����ʧ����
    double loss_quadratic(vector<int>& pred, vector<int>& vexp) {
        int N = pred.size();
        double loss = 0;
        for (int i = 0; i < N; i++) {
            double temp = (pred[i] - vexp[i]);
            loss += temp * temp;
        }
        return loss / N;
    }

    // ����ֵ��ʧ����
    double loss_absolute(vector<int>& pred, vector<int>& vexp) {
        int N = pred.size();
        double loss = 0;
        for (int i = 0; i < N; i++) {
            loss += abs(pred[i] - vexp[i]);
        }
        return loss / N;
    }

    // ������ʧ��������ʱû�С�
    double loss_logarithmic() {
        return 0;
    }
};

class Analysis {
public:
    // ������
    double loss(vector<int>& vpred, vector<int>& vexp) {
        // ����������ǻ���N�������õ��ľ������ֵ��
        // �������empirical risk
        return Loss().loss0_1(vpred, vexp);
    }

    // ����epsilon������
    double epsilon(int d, int N, double delta) {
        double ans = sqrt(1.0 / (2.0 * N) * (log((double)d) - log(delta)));
        return ans;
    }

    // ��������Ͻ硣
    // Generalization Error Bound: geb;
    double geb(vector<int>& vpred, vector<int>& vexp, int d, double prob) {
        // vpred: ��ѵ���õ�ģ��Ԥ��Ľ��
        // vexp:  ԭʼ����ϣ���õ�����ʵ��ǩ
        int N = vpred.size();
        // N��ʾ����������
        // d��ʾ����ռ���ģ�͵�������
        // probΪ���Ŷȣ�Ҳ�����Զ��������Ÿþ�������Ͻ�ķ��������
        double delta = 1 - prob;
        return loss(vpred, vexp) + epsilon(d, N, delta);
    }
};


