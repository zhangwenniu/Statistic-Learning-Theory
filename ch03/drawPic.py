import numpy as np
from graphviz import Digraph
from matplotlib import pyplot as plt
from matplotlib.pyplot import MultipleLocator
#data = [[2,3],[6, 4],[9, 6],[4, 7],[8, 1],[7, 2], [8,2], [10,4], [6,6]]
data = [[7,2], [5,4], [9,6], [2,3], [4,7], [8,1]]
data = np.array(data)

# 节点
class node:
	def __init__(self, _data=None, _left=None, _right=None, _father=None, _dim=None, _index=None, _visiable=True):
		self.data = _data
		self.left = _left
		self.right = _right
		self.father = _father
		self.dim = _dim
		self.index = _index
		self.visiable = _visiable
	def getData(self):
		s = "("
		for i in range(self.data.size):
			if i!=0:
				s += ','
			s+=str(self.data[i])
		s += ")"
		return s
	def __str__(self):
		if(self.visiable):
			return str(self.index)
		else:
			return "_invis"+str(self.index)

# 生成简要二叉树，并返回最大深度
visIdx = 0
def creatKDT(data, depth, k, maxDepth=0):
	dim = depth % k
	length = data.shape[0]
	if(length==0):
		return None, max(depth, maxDepth)
	index = []
	for i in range(length):
		index.append([data[i][dim], i])
	index.sort()
	root = data[index[length//2][1]]
	left = [data[index[i][1]] for i in range(length//2)]
	left = np.array(left)
	right = [data[index[i][1]] for i in range(length//2+1, length)]
	right = np.array(right)
	global visIdx
	root_node = node(_data=root, _index=visIdx)
	visIdx += 1

	root_node.left, maxL=creatKDT(left, depth+1, k, maxDepth)
	root_node.right, maxR=creatKDT(right, depth+1, k, maxDepth)
	if(root_node.left):
		root_node.left.father=root_node
	if(root_node.right):
		root_node.right.father=root_node
	maxDepth = max(max(maxDepth, maxL), maxR)
	return root_node, max(depth, maxDepth)

# 生成完全二叉树
invisNum = 0
def toComKDT(root, depth, maxDepth):
	if(depth>=maxDepth):
		return None
	global invisNum
	if(root.left==None):
		root.left=node(_data=root.data, _visiable=False, _index=invisNum)
		invisNum += 1
	if(root.right==None):
		root.right=node(_data=root.data, _visiable=False, _index=invisNum)
		invisNum += 1
	root.left=toComKDT(root.left, depth+1, maxDepth)
	root.right=toComKDT(root.right, depth+1, maxDepth)
	return root

# 绘制完全二叉树。
def drawComKDT(root, dot):
	if(root == None):
		return dot

	if(root.visiable==False):
		dot.node(str(root), root.getData())#, style="invis")
	else:
		dot.node(str(root), root.getData())

	if root.left:
		if(root.left.visiable==False):
			dot.edge(str(root), str(root.left), style="invis")
		else:
			dot.edge(str(root), str(root.left))

	dot = drawComKDT(root.left, dot)
	if root.right:
		if(root.right.visiable==False):
			dot.edge(str(root), str(root.right), style="invis")
		else:
			dot.edge(str(root), str(root.right))

	dot = drawComKDT(root.right, dot)
	return dot
'''
root, maxDepth = creatKDT(data, 0, 2, 0)
root = toComKDT(root, 0, maxDepth)
dot = Digraph(node_attr={'shape': 'circle'})
dot = drawComKDT(root, dot)
dot.view()
print(dot.source)
'''


# 绘制KD树对齐形式。
dataIndex = 1
def drawKDTree(data, depth, k, dot):
	dim = depth % k
	length = data.shape[0]
	if(length==0):
		return None, dot
	index = []
	for i in range(length):
		index.append([data[i][dim], i])
	index.sort()
	root = data[index[length//2][1]]
	left = [data[index[i][1]] for i in range(length//2)]
	left = np.array(left)
	right = [data[index[i][1]] for i in range(length//2+1, length)]
	right = np.array(right)
	global dataIndex
	root_node = node(_data=root, _dim=dim, _index=dataIndex)
	dataIndex+=1

	dot.node(str(root_node.index), root_node.getData())

	root_node.left, dot=drawKDTree(left, depth+1, k, dot)
	if(root_node.left==None):
		pass
		dot.node("_left"+str(root_node.index), root_node.getData(), style="invis")
		dot.edge(str(root_node.index), "_left"+str(root_node.index), style="invis")
	else:
		dot.edge(str(root_node.index), str(root_node.left.index))

	dot.node("_middle"+str(root_node.index), root_node.getData(), style="invis")
	dot.edge(str(root_node.index), "_middle"+str(root_node.index), style="invis", weight="10")

	root_node.right, dot=drawKDTree(right, depth+1, k, dot)

	if(root_node.right==None):
		pass
		dot.node("_right"+str(root_node.index), root_node.getData(), style="invis")
		dot.edge(str(root_node.index), "_right"+str(root_node.index), style="invis")
	else:
		dot.edge(str(root_node.index), str(root_node.right.index))

	if(root_node.left):
		root_node.left.father=root_node
	if(root_node.right):
		root_node.right.father=root_node
	
	return root_node, dot

dot = Digraph(node_attr={'shape': 'circle'})
_, dot = drawKDTree(data, 0, 2, dot)
#dot.view()
print(dot.source)


# 生成KD树，并绘制一个完整的平面图形。
def createTree(data, depth, k, l, r, d, u):
	dim = depth % k
	length = data.shape[0]
	if(length==0):
		return None
	index = []
	for i in range(length):
		index.append([data[i][dim], i])
	index.sort()
	root = data[index[length//2][1]]
	left = [data[index[i][1]] for i in range(length//2)]
	left = np.array(left)
	right = [data[index[i][1]] for i in range(length//2+1, length)]
	right = np.array(right)
	root_node = node(_data=root, _dim=dim)

	if(dim == 0):
		plt.plot([root[0]]*(u-d+1), range(d, u+1))
		root_node.left=createTree(left, depth+1, k, l, root[0], d, u)
		root_node.right=createTree(right, depth+1, k, root[0], r, d, u)
		if(root_node.left):
			root_node.left.father=root_node
		if(root_node.right):
			root_node.right.father=root_node
	else:
		plt.plot(range(l, r+1), [root[1]]*(r-l+1))
		root_node.left=createTree(left, depth+1, k, l, r, d, root[1])
		root_node.right=createTree(right, depth+1, k, l, r, root[1], u)
		if(root_node.left):
			root_node.left.father=root_node
		if(root_node.right):
			root_node.right.father=root_node
	return root_node

# 绘制分类超平面
def drawOri(data):
	fig, ax = plt.subplots()
	fig.set_size_inches(5, 5)
	data = np.array(data)
	mmax = np.max(data)+1
	mmin = np.min(data)-1
	major_locator=MultipleLocator(1)
	plt.scatter(data[:,0], data[:,1])
	plt.xlim(mmin, mmax)
	plt.ylim(mmin, mmax)
	ax = plt.gca()
	ax.xaxis.set_major_locator(major_locator)
	ax.yaxis.set_major_locator(major_locator)
	return createTree(data, 0, 2, mmin, mmax, mmin, mmax)

# 绘制标记点及分类超平面
def drawPic(x, data):
	fig, ax = plt.subplots()
	fig.set_size_inches(5, 5)
	data = np.array(data)
	mmax = np.max(data)+1
	mmin = np.min(data)-1
	major_locator=MultipleLocator(1)
	plt.scatter(data[:,0], data[:,1])
	plt.scatter([x[0]], [x[1]], marker='x')
	plt.xlim(mmin, mmax)
	plt.ylim(mmin, mmax)
	ax = plt.gca()
	ax.xaxis.set_major_locator(major_locator)
	ax.yaxis.set_major_locator(major_locator)
	return createTree(data, 0, 2, mmin, mmax, mmin, mmax)

# 计算两点间的欧式距离
def distance(a, b):
	return ((a[0]-b[0])**2+(a[1]-b[1])**2)**0.5

# 寻找叶节点
def findLeaf(root, x, stack):
	if(root==None):
		return stack
	stack.append(root)
	if(x[root.dim]<=root.data[root.dim]):
		return findLeaf(root.left, x, stack)
	else:
		return findLeaf(root.right, x, stack)


# 寻找最近邻节点，并绘制图形
def searchNearest(root, x, differt_pic=True, show=False):
	plt.scatter([x[0]], [x[1]], marker='x')
	stack = []
	stack = findLeaf(root, x, stack)
	nearN = stack[-1]
	minD = distance(stack[-1].data, x)
	visted = set()
	path = 1
	while(stack):
		top = stack[-1]
		visted.add(top)
		stack.pop()
		dis = distance(top.data, x)
		if(dis < minD):
			minD = dis
			nearN = top
		if show:
			plt.show()
		if differt_pic:
			# 重新绘制一张底图。
			drawPic(x, data)
		ax = plt.gca()
		ax.scatter(top.data[0], top.data[1], marker='x', s=200)
		ax.plot([x[0], nearN.data[0]], [x[1], nearN.data[1]])
		theta = np.arange(0, 2*np.pi, 0.01)
		xx = x[0] + minD * np.cos(theta)
		yy = x[1] + minD * np.sin(theta)
		plt.plot(xx, yy)
		plt.savefig("{0}.png".format(path))
		path += 1

		left = x[top.dim] - minD
		right = x[top.dim] + minD
		if(left <= top.data[top.dim] and top.left != None and top.left not in visted):
			stack.append(top.left)
		if(right >= top.data[top.dim] and top.right != None and top.right not in visted):
			stack.append(top.right)
	return nearN


x = [4, 3]
drawOri(data)
plt.savefig("0.png")
root = drawPic(x, data, differt_pic=True, show=False)
searchNearest(root, x)





