#include "GeneralizationErrorBound.h"

int main()
{
    vector<int> vexp(10), vpred(10);
    // ������ɲ���������
    // ǰ�������Ԥ��ֵ����ʵֵ��ͬ�����������Ԥ��ֵ����ʵֵ��ͬ��
    for (int i = 0; i < 10; i++) {
        if (i < 5) {
            vexp[i] = vpred[i] = i % 2;
        }
        else {
            vexp[i] = i % 2;
            vpred[i] = (i + 1) % 2;
        }
    }

    int d = 5;
    double prob = 1;
    double geb;
    for (int i = 0; i < 20; i++) {
        prob -= 0.05;
        geb = Analysis().geb(vpred, vexp, d, prob);
        printf("prob = %lf, geb = %lf. \n", prob, geb);
    }
    return 0;
}
