#include "GeneralizationErrorBound.h"

int main()
{
    vector<int> vexp(10), vpred(10);
    // 随机生成测试用例。
    // 前五个数据预测值与真实值相同，后五个数据预测值和真实值不同。
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
