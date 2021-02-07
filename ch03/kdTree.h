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

// KD���еĽڵ�
struct node {
	int index;
	int depth;
	node* left, *right, *father;
	node() {
		index = depth = -1;
		left = right = father = NULL;
	}
};

// ����K���������Ĺ����У����������ȶ����еĽڵ㡣
struct knode {
	// KD���еĽڵ�������
	int index;
	// ��ǰ�ڵ���Ŀ��������ݵľ��롣
	double distance;
	// �����ȶ��б�֤����ѵĶѶ�Ԫ���Ǿ������ֵ��
	// ��������Ԫ������С��K��ʱ����������Ԫ�ء�
	// ������Ԫ�ش���K��ʱ�򣬱Ƚϵ�ǰ�ڵ�ľ����Ƿ��K����С���뻹ҪС������ǵĻ�����ȡ��Ϊ�����λ�á�
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
		// n�����ݵ�ά�ȡ�
		n = data[0].size();
		vector<int> index;
		for (int i = 0; i < data.size(); i++)index.push_back(i);
		// ���������������ԭʼ������������KDTree.
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
		// ������ֵ�������������飬����ѡ���׼����λ����
		for (auto id : index) {
			vt.push_back(pair<double, int>(data[id][dim], id));
		}
		sort(vt.begin(), vt.end());
		int num = vt.size();
		// ������ż��������λ���Ľڵ�Ϊ��n/2����
		root->index = vt[num / 2].second;
		vector<int> left;
		for (int i = 0; i < num / 2; i++) {
			left.push_back(vt[i].second);
		}
		vector<int> right;
		for (int i = num / 2 +1; i < num; i++) {
			right.push_back(vt[i].second);
		}
		// �ݹ���������KD����
		root->left = createKDTree(left, depth + 1);
		root->right = createKDTree(right, depth + 1);

		// Ϊ�˻���Ѱ������㣬��Ҫ���游�ڵ㡣
		if (root->left)root->left->father = root;
		if (root->right)root->right->father = root;
		return root;
	}

	int NearestSearch(vector<double> x) {
		// ����ĳ���ڵ㣬Ѱ��������ڵ㡣
		if (x.size() != n) {
			printf("Input data error. Dimension not match.\n");
			exit(5);
		}
		// ����ά������λ���Ĵ�С��ϵ��ѡ����ײ�Ҷ�ڵ�Ϊ��ʼ����ڵ㡣
		stack<node*> st;
		// ��Ѱ��Ҷ�ڵ�Ĺ����У���ӽ���·����
		node* nearest = findLeaf(x, head, st);
		Distance d = Distance(2);

		// ��Ҷ�ڵ���Ϊ��ǰ����ڵ㡣
		double minD = d.Minkowski(x, data[nearest->index]);
		node* minNode = nearest;

		// ʹ�ò���������д����ֹ�ظ�����ͬһ���ڵ��֧��
		vector<bool> vis(data.size(), 0);
		while (!st.empty()) {
			// ����·����
			node* top = st.top();
			vis[top->index] = 1;

			// �����ǰ���ʽڵ㡣
			/*
			cout << top->index << " " << top->depth << " Data: ";
			for (auto dt : data[top->index])cout << dt << " ";
			cout << endl;
			*/

			st.pop();

			// ���㵱ǰ�ڵ��Ƿ���ţ��������Ŀ��ڵ���������������ڵ������Լ�������롣
			double tempD = d.Minkowski(x, data[top->index]);
			if (tempD < minD) {
				minD = tempD;
				minNode = top;
			}

			// �������֮���������롣
			/*
			cout << minD << endl;
			*/

			// �Ƚϵ�ǰ�ڵ����ڵ�ά���£���Ŀ��ڵ�����̰뾶���ɵĳ�Բ�ܷ������ӽڵ��ཻ��
			int dim = top->depth % n;

			// �ܹ����������ཻ����˵��Ŀ��ڵ��ڸ�ά��������ƫ�ƺ�����ֵ��ƫ�Ƴ���Ϊ�뾶����С�ڵ�ǰ�ڵ��ڵ�ǰ�ָ�ά�ȵ���ֵ��
			// �ܹ����������ཻ����˵��Ŀ��ڵ��ڸ�ά��������ƫ�ƺ�����ֵ��ƫ�Ƴ���Ϊ�뾶�������ڵ�ǰ�ڵ��ڵ�ǰ�ָ�ά�ȵ���ֵ��
			int left = x[dim] - minD, right = x[dim] + minD;

			// ͬʱ��֤�ڵ㲻Ϊ�ա�δ���ʹ����Ž��ڵ����·��ջ�н��м�����
			if (left < data[top->index][dim] && top->left && !vis[top->left->index]) {
				st.push(top->left);
			}
			if (right > data[top->index][dim] && top->right && !vis[top->right->index]) {
				st.push(top->right);
			}
		}
		// �����̾����Լ����Ӧ�Ľڵ㡣
		printf("minD = %lf. \n", minD);
		printf("minNode index is %d, depth is %d, data is: ", minNode->index, minNode->depth);
		for (int i = 0; i < n; i++) {
			printf("%0.lf ", data[minNode->index][i]);
		}
		printf("\n");
		// ��������ڵ��Ӧ���±ꡣ
		return minNode->index;
	}


	vector<int> knSearch(vector<double> x, int k) {
		// ����ĳ���ڵ㣬Ѱ��������ڵ㡣
		if (x.size() != n) {
			printf("Input data error. Dimension not match.\n");
			exit(5);
		}
		if (k > data.size()) {
			printf("You're searching %d nearest data, while there is only %d data. \n", k, n);
			printf("Adjusted k to %d. \n", n);
			k = n;
		}
		// ����ά������λ���Ĵ�С��ϵ��ѡ����ײ�Ҷ�ڵ�Ϊ��ʼ����ڵ㡣
		stack<node*> st;
		// ��Ѱ��Ҷ�ڵ�Ĺ����У���ӽ���·����
		node* nearest = findLeaf(x, head, st);
		Distance d = Distance(2);

		// ��Ҷ�ڵ���Ϊ��ǰ����ڵ㣬�������Ѱ��K��������ڵĽڵ㡣
		// ����һ��������Ϊ�������K��������ڵĽڵ�һ���ھ�����ҽڵ���������Χ�ڡ�
		double maxMinKD = 1e8;

		// ʹ�ò���������д����ֹ�ظ�����ͬһ���ڵ��֧��
		vector<bool> vis(data.size(), 0);

		// ����һ�������ѯ�ڵ�֮�����Ĵ���ѣ���֤K���ڵ㶼ҪС�ڵ��ڴ���ѵĶѶ����롣
		priority_queue<knode> q;
		q.emplace(knode(-1, maxMinKD));

		while (!st.empty()) {
			// ����·����
			node* top = st.top();
			vis[top->index] = 1;

			// �����ǰ���ʽڵ㡣
			/*
			cout << top->index << " " << top->depth << " Data: ";
			for (auto dt : data[top->index])cout << dt << " ";
			cout << endl;
			*/
			st.pop();

			// ���㵱ǰ�ڵ��Ƿ���ţ�����Ǿ���Ŀ��ڵ������K����Χ֮�ڣ�������������С�
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

			// �������֮���K������롣
			/*
			cout << maxMinKD << endl;
			*/
			// �Ƚϵ�ǰ�ڵ����ڵ�ά���£���Ŀ��ڵ�����̰뾶���ɵĳ�Բ�ܷ������ӽڵ��ཻ��
			int dim = top->depth % n;

			// �ܹ����������ཻ����˵��Ŀ��ڵ��ڸ�ά��������ƫ�ƺ�����ֵ��ƫ�Ƴ���Ϊ�뾶����С�ڵ�ǰ�ڵ��ڵ�ǰ�ָ�ά�ȵ���ֵ��
			// �ܹ����������ཻ����˵��Ŀ��ڵ��ڸ�ά��������ƫ�ƺ�����ֵ��ƫ�Ƴ���Ϊ�뾶�������ڵ�ǰ�ڵ��ڵ�ǰ�ָ�ά�ȵ���ֵ��
			int left = x[dim] - maxMinKD, right = x[dim] + maxMinKD;

			// ͬʱ��֤�ڵ㲻Ϊ�ա�δ���ʹ����Ž��ڵ����·��ջ�н��м�����
			if (left < data[top->index][dim] && top->left && !vis[top->left->index]) {
				st.push(top->left);
			}
			if (right > data[top->index][dim] && top->right && !vis[top->right->index]) {
				st.push(top->right);
			}
		}
		// �����̾����Լ����Ӧ�Ľڵ㡣
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
		// ����K������ڵ��Ӧ���±����顣
		return ans;
	}

	int vote(vector<double> x, int k) {
		// ��ȡK���ڽڵ���±ꡣ
		vector<int> kNearest = knSearch(x, k);
		// ͶƱ��ȡ����K���ڽڵ�õ������ֵ��
		unordered_map<int, int> mp;
		for (int& id : kNearest) {
			mp[label[id]]++;
		}
		int maxCount = 0, maxLabel = -1;
		// ͶƱ
		for (auto it : mp) {
			if (it.second > maxCount) {
				maxCount = it.second;
				maxLabel = it.first;
			}
		}
		// ������ݼ��䱻ͶƱ�õ��Ľ����
		printf("Data: ");
		for (int i = 0; i < x.size(); i++) {
			printf("%.0lf ", x[i]);
		}
		printf("is labeled as %d, poll num is  %d.", maxLabel, maxCount);
		return maxLabel;
	}

	void DFS(node* root, double& dis, node*& minnode, vector<double>& x) {
		// ��������������ҵ�һ����С�ĵ�ַ��ʱ�临�Ӷ������º��п��ܴﵽO(n)��
		// �����������������ȶ�ʱ�临�Ӷ�ʹ�õģ���ʵ�ʵ��ù����н������ñ�������
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
		// ���ݵ�ǰά�Ⱥͽڵ���λ���Ĵ�С��ϵ���Ƚ�������²��Ҷ�ڵ㡣
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
		// �и����б���kd����
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
		// ��α���������kd����
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
			// ����������
			if (front->left) {
				q.push(front->left);
				printf("index is %d. depth is %d. Data: ", front->left->index, front->left->depth);
				for (auto& d : data[front->left->index]) {
					printf("%.0lf ", d);
				}
				printf("\n");
			}
			// ����������
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