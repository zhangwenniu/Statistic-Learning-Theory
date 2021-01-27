# 泛化误差上界的C++代码实现

具体证明及解释，可以参见我的CSDN博客[机器学习中泛化误差上界的实现及分析](https://blog.csdn.net/ProfSnail/article/details/113100000?spm=1001.2014.3001.5502 "机器学习中泛化误差上界的实现及分析")。
本章代码实现了：
+ 0-1损失函数误差计算、平方损失函数误差计算、绝对值损失函数误差计算，包含在头文件[GeneralizationErrorBound.h](./GeneralizationErrorBound.h)下的类Loss中。
+ 实现了泛化误差上界的计算方法，包含在同一个头文件[GeneralizationErrorBound.h](./GeneralizationErrorBound.h)下的Analysis类中。
+ 主函数为[test-geb-main.cpp](./test-geb-main.cpp)，用于生成测试用例，在其中自定义正确预测分类的个数、置信概率、假设空间容量。
