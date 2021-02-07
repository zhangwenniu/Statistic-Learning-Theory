#pragma once
#include <set>
#include <vector>
#include <algorithm>
#include <utility>
#include <queue>
#include <stack>
#include <map>
#include <unordered_map>
using namespace std;

// KD树中的节点
struct node {
	int index;
	int depth;
	node* left, *right, *father;
	node() {
		index = depth = -1;
		left = right = father = NULL;
	}
};

// 进行K近邻搜索的过程中，保存在优先队列中的节点。
struct knode {
	// KD树中的节点索引。
	int index;
	// 当前节点与目标检索数据的距离。
	double distance;
	// 用优先队列保证大根堆的堆顶元素是距离最大值。
	// 当队列内元素数量小于K的时候，向堆中添加元素。
	// 当队列元素大于K的时候，比较当前节点的距离是否比K个最小距离还要小，如果是的话则将其取代为顶点的位置。
	knode() {}
	knode(int _id, double _d) :index(_id), distance(_d) {}
	bool operator < (const knode& nd) const {
		return distance < nd.distance;
	}
};


class KDTree {
public:
	node* head;
	vector<vector<double> > data;
	vector<int> label;
	int n, searchTime = 0;
	KDTree(vector<vector<double> > _data, vector<int> _label):data(_data), label(_label){
		// n是数据的维度。
		n = data[0].size();
		vector<int> index;
		for (int i = 0; i < data.size(); i++)index.push_back(i);
		// 利用索引数组而非原始数组用以生成KDTree.
		head = createKDTree(index, 0);
	}
	node* createKDTree(vector<int>& index, int depth) {
		if (index.empty()) {
			return NULL;
		}
		node* root = new node();
		root->depth = depth;
		int dim = depth % n;
		vector<pair<double, int> > vt;
		// 根据数值和索引生成数组，用于选择标准的中位数。
		for (auto id : index) {
			vt.push_back(pair<double, int>(data[id][dim], id));
		}
		sort(vt.begin(), vt.end());
		int num = vt.size();
		// 无论奇偶，设置中位数的节点为第n/2个。
		root->index = vt[num / 2].second;
		vector<int> left;
		for (int i = 0; i < num / 2; i++) {
			left.push_back(vt[i].second);
		}
		vector<int> right;
		for (int i = num / 2 +1; i < num; i++) {
			right.push_back(vt[i].second);
		}
		// 递归生成左右KD树。
		root->left = createKDTree(left, depth + 1);
		root->right = createKDTree(right, depth + 1);

		// 为了回溯寻找最近点，需要保存父节点。
		if (root->left)root->left->father = root;
		if (root->right)root->right->father = root;
		return root;
	}

	int NearestSearch(vector<double> x) {
		// 根据某个节点，寻找最近相邻点。
		if (x.size() != n) {
			printf("Input data error. Dimension not match.\n");
			exit(5);
		}
		// 根据维度与中位数的大小关系，选择最底层叶节点为初始最近节点。
		stack<node*> st;
		// 在寻找叶节点的过程中，添加进来路径。
		node* nearest = findLeaf(x, head, st);
		Distance d = Distance(2);

		// 以叶节点作为当前最近节点。
		double minD = d.Minkowski(x, data[nearest->index]);
		node* minNode = nearest;

		// 使用布尔数组进行打表，防止重复访问同一个节点分支。
		vector<bool> vis(data.size(), 0);
		while (!st.empty()) {
			// 回溯路径。
			node* top = st.top();
			vis[top->index] = 1;

			// 输出当前访问节点。
			/*
			cout << top->index << " " << top->depth << " Data: ";
			for (auto dt : data[top->index])cout << dt << " ";
			cout << endl;
			*/

			st.pop();

			// 计算当前节点是否更优，如果距离目标节点更近，则更新最近节点索引以及最近距离。
			double tempD = d.Minkowski(x, data[top->index]);
			if (tempD < minD) {
				minD = tempD;
				minNode = top;
			}

			// 输出更新之后的最近距离。
			/*
			cout << minD << endl;
			*/

			// 比较当前节点所在的维度下，由目标节点与最短半径构成的超圆能否与其子节点相交。
			int dim = top->depth % n;

			// 能够与左子树相交，就说明目标节点在该维度下向左偏移后的最大值（偏移长度为半径），小于当前节点在当前分割维度的数值。
			// 能够与右子树相交，就说明目标节点在该维度下向右偏移后的最大值（偏移长度为半径），大于当前节点在当前分割维度的数值。
			int left = x[dim] - minD, right = x[dim] + minD;

			// 同时保证节点不为空、未访问过，才将节点放入路径栈中进行检索。
			if (left < data[top->index][dim] && top->left && !vis[top->left->index]) {
				st.push(top->left);
			}
			if (right > data[top->index][dim] && top->right && !vis[top->right->index]) {
				st.push(top->right);
			}
		}
		// 输出最短距离以及其对应的节点。
		printf("minD = %lf. \n", minD);
		printf("minNode index is %d, depth is %d, data is: ", minNode->index, minNode->depth);
		for (int i = 0; i < n; i++) {
			printf("%0.lf ", data[minNode->index][i]);
		}
		printf("\n");
		// 返回最近节点对应的下标。
		return minNode->index;
	}


	vector<int> knSearch(vector<double> x, int k) {
		// 根据某个节点，寻找最近相邻点。
		if (x.size() != n) {
			printf("Input data error. Dimension not match.\n");
			exit(5);
		}
		if (k > data.size()) {
			printf("You're searching %d nearest data, while there is only %d data. \n", k, n);
			printf("Adjusted k to %d. \n", n);
			k = n;
		}
		// 根据维度与中位数的大小关系，选择最底层叶节点为初始最近节点。
		stack<node*> st;
		// 在寻找叶节点的过程中，添加进来路径。
		node* nearest = findLeaf(x, head, st);
		Distance d = Distance(2);

		// 以叶节点作为当前最近节点，逐层向上寻找K个最近相邻的节点。
		// 设置一个最大距离为无穷大，则K个最近相邻的节点一定在距离查找节点的无穷大球范围内。
		double maxMinKD = 1e8;

		// 使用布尔数组进行打表，防止重复访问同一个节点分支。
		vector<bool> vis(data.size(), 0);

		// 设置一个与待查询节点之间距离的大根堆，保证K个节点都要小于等于大根堆的堆顶距离。
		priority_queue<knode> q;
		q.emplace(knode(-1, maxMinKD));

		while (!st.empty()) {
			// 回溯路径。
			node* top = st.top();
			vis[top->index] = 1;

			// 输出当前访问节点。
			/*
			cout << top->index << " " << top->depth << " Data: ";
			for (auto dt : data[top->index])cout << dt << " ";
			cout << endl;
			*/
			st.pop();

			// 计算当前节点是否更优，如果是距离目标节点最近的K个范围之内，则更新索引队列。
			double tempD = d.Minkowski(x, data[top->index]);
			if (q.size() < k) {
				q.emplace(knode(top->index, tempD));
			}
			else {
				if (tempD <= q.top().distance) {
					q.pop();
					q.emplace(knode(top->index, tempD));
				}
			}
			maxMinKD = q.top().distance;

			// 输出更新之后的K最近距离。
			/*
			cout << maxMinKD << endl;
			*/
			// 比较当前节点所在的维度下，由目标节点与最短半径构成的超圆能否与其子节点相交。
			int dim = top->depth % n;

			// 能够与左子树相交，就说明目标节点在该维度下向左偏移后的最大值（偏移长度为半径），小于当前节点在当前分割维度的数值。
			// 能够与右子树相交，就说明目标节点在该维度下向右偏移后的最大值（偏移长度为半径），大于当前节点在当前分割维度的数值。
			int left = x[dim] - maxMinKD, right = x[dim] + maxMinKD;

			// 同时保证节点不为空、未访问过，才将节点放入路径栈中进行检索。
			if (left < data[top->index][dim] && top->left && !vis[top->left->index]) {
				st.push(top->left);
			}
			if (right > data[top->index][dim] && top->right && !vis[top->right->index]) {
				st.push(top->right);
			}
		}
		// 输出最短距离以及其对应的节点。
		vector<int> ans;
		printf("maxMinKD = %lf. \n", maxMinKD);
		while (!q.empty()) {
			int idx = q.top().index;
			double dis = q.top().distance;
			ans.push_back(idx);
			q.pop();
			printf("Each node index is %d, distance is %lf. data is: ", idx, dis);
			for (int i = 0; i < n; i++) {
				printf("%0.lf ", data[idx][i]);
			}
			printf("\n");
		}
		// 返回K个最近节点对应的下标数组。
		return ans;
	}

	int vote(vector<double> x, int k) {
		// 获取K近邻节点的下标。
		vector<int> kNearest = knSearch(x, k);
		// 投票获取根据K近邻节点得到的类别值。
		unordered_map<int, int> mp;
		for (int& id : kNearest) {
			mp[label[id]]++;
		}
		int maxCount = 0, maxLabel = -1;
		// 投票
		for (auto it : mp) {
			if (it.second > maxCount) {
				maxCount = it.second;
				maxLabel = it.first;
			}
		}
		// 输出数据及其被投票得到的结果。
		printf("Data: ");
		for (int i = 0; i < x.size(); i++) {
			printf("%.0lf ", x[i]);
		}
		printf("is labeled as %d, poll num is  %d.", maxLabel, maxCount);
		return maxLabel;
	}

	void DFS(node* root, double& dis, node*& minnode, vector<double>& x) {
		// 深度优先搜索，找到一个最小的地址。时间复杂度最坏情况下很有可能达到O(n)。
		// 本函数仅仅是用来比对时间复杂度使用的，在实际调用过程中将不调用本函数。
		cout << searchTime++ << endl;
		double tempDis = Distance(2).Minkowski(x, data[root->index]);
		if (tempDis < dis) {
			dis = tempDis;
			minnode = root;
		}
		int dim = root->depth % n;
		double left = x[dim] - dis, right = x[dim] + dis;
		if (left < data[root->index][dim]&&root->left) {
			DFS(root->left, dis, minnode, x);
		}
		if (right > data[root->index][dim] && root->right) {
			DFS(root->right, dis, minnode, x);
		}
	}


	node* findLeaf(vector<double>& x, node*& root, stack<node*>& st) {
		// 根据当前维度和节点中位数的大小关系，比较深达最下层的叶节点。
		st.push(root);
		int dim = root->depth % n;
		if (x[dim] < data[root->index][dim]) {
			if (root->left) {
				return findLeaf(x, root->left, st);
			}
			else {
				return root;
			}
		}
		else {
			if (root->right) {
				return findLeaf(x, root->right, st);
			}
			else {
				return root;
			}
		}
	}
	void inOrder(node* root) {
		// 中根序列遍历kd树。
		if (root == NULL)return;
		inOrder(root->left);
		printf("index is %d. depth is %d. Data: ", root->index, root->depth);
		for (auto& d : data[root->index]) {
			printf("%.0lf ", d);
		}
		printf("\n");
		inOrder(root->right);
	}
	void levelOrder(node* root) {
		// 层次遍历法遍历kd树。
		queue<node*> q;
		printf("index is %d. depth is %d. Data: ", root->index, root->depth);
		for (auto& d : data[root->index]) {
			printf("%.0lf ", d);
		}
		printf("\n");
		q.push(root);
		while (!q.empty()) {
			node* front = q.front();
			q.pop();
			// 遍历左子树
			if (front->left) {
				q.push(front->left);
				printf("index is %d. depth is %d. Data: ", front->left->index, front->left->depth);
				for (auto& d : data[front->left->index]) {
					printf("%.0lf ", d);
				}
				printf("\n");
			}
			// 遍历右子树
			if (front->right) {
				q.push(front->right);
				printf("index is %d. depth is %d. Data: ", front->right->index, front->right->depth);
				for (auto& d : data[front->right->index]) {
					printf("%.0lf ", d);
				}
				printf("\n");
			}
		}
	}
};