# Statistic-Learning-Theory
动手实现李航《统计学习方法》的C++版本。

----
2021/1/27更新内容
+ 在ch01中添加了第一章中的损失函数（经验风险）计算方法，以及有限假设空间下的泛化误差上界计算方法，二者的C++代码实现。
----
2021/1/30更新内容
+ 在ch02中添加了第二章中的感知机模型的C++实现，包括随机梯度下降算法的实现及其对偶形式算法的实现，可以在有限次数内将线性可分的数据集依据收敛后的超平面划分成正负两类。同时，如果两类在迭代周期上限内无法收敛，可能存在线性不可分的情形，判定在周期内无法收敛。
----
