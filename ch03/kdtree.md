### 更新时间
2021/2/7 13:53
### 更新内容
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;添加了第三章《K近邻法》的C++实现方法。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;实现的内容包括文本文档数据流的读入操作、计算明可夫斯基向量距离的方法、生成KD树的方法、KD树中寻找最近相邻节点的方法、KD树中寻找最近相邻K个节点的方法、根据K近邻算法投票预测目标节点类别的方法、中根遍历KD树的方法、层次遍历KD树的方法等。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;使用方法：在data.txt文本文档中添加如下数据：第一行两个数字N和M，分别表示数据的个数以及数据的维度。接下来是N行M列的数据，用于表示输入数据在各个维度的坐标。随后输入N个整数，表示对应向量的类别。最后输入K进行K近邻查询，以及一个M维的待查询的数据向量。示例：
```cpp
6 2
2 3
5 4
9 6
4 7
8 1
7 2
1 0 1 1 0 0
3
2 5
```
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;运行knn.cpp，会告知K个最近邻坐标及其对应与当前节点的距离，同时投票产生待查询数据的类别。输出示例如下：
```cpp
input done.
maxMinKD = 3.162278.
Each node index is 1, distance is 3.162278. data is: 5 4
Each node index is 3, distance is 2.828427. data is: 4 7
Each node index is 0, distance is 2.000000. data is: 2 3
Data: 2 5 is labeled as 1, poll num is  2.
```
### 更新时间
2021/2/8 01:49
### 更新内容
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;添加了使用graphviz绘制KD二叉树的代码至kdtree.h，并更新了部分注释内容。使用方法为生成KD树实例kdt之后，调用kdt.drawKDTree(kdt.head, "./out.dot");生成.dot文件。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;使用graphviz需要安装该文件，打开[graphviz安装包](http://www.graphviz.org/download/)，依据不同平台下载最新版本的内容，将graphviz安装后的/bin安装到系统路径下并重启系统以使得Path生效。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;接下来，在.dot文件的所在目录下，使用命令行代码<br>
```cpp
dot out.dot -Tsvg -okdtree.svg
```
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;即可生成svg图像。使用命令行代码<br>
```cpp
cpp dot out2.dot -Tpdf -okdtree.pdf
```
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;即可生成图像对应的pdf文档。<br>
