#pragma once
#include<iostream>
#include<queue>
#include <assert.h>
/*
红黑树是一种自平衡的二叉查找树，典型的用途是实现关联数组。
红黑树由以下5个约束条件限制：
1. 每个节点或者是黑色，或者是红色。
2. 根节点是黑色。
3. 每个叶子节点（空节点）是黑色。为了方便表示叶子节点，设置哨兵节点（Nil）表示空节点，每个叶子节点全都指向它，方便表示。
4. 如果一个节点是红色的，则它的子节点必须是黑色的。
5. 从一个节点到该节点的子孙节点的所有路径上包含相同数目的黑节点。

由约束条件5，可得：从根到所有椰子的最长的路径不多于最短的路径的2倍长，因此这棵树大致上是平衡的。
*/

/*
这个class里，对于两个变量的比较的语句，我把常量值全都放在了左边，防止少写一个等号的情况。
*/
class RBTree
{
private:
	typedef enum
	{
		RBT_COLOR_RED,
		RBT_COLOR_BLACK,
	}RBTColor;
	typedef struct __RBTreeNode
	{
		__RBTreeNode* pParent;
		__RBTreeNode* pLeft;
		__RBTreeNode* pRight;
		RBTColor Color;
		int Value;
	}RBTreeNode, *pRBTreeNode;
private:
	void InsertFixUP(pRBTreeNode pInsertNode);						//插入InsertNode之后，调整红黑树
	void DeleteFixUp(pRBTreeNode pFixNode);						//删除DeleteNode之后，调整红黑树
	void SingleLeft(pRBTreeNode &pNode, pRBTreeNode &newTop);		//左旋转，返回新的顶点
	void SingleRight(pRBTreeNode &pNode, pRBTreeNode &newTop);		//右旋转，返回新的顶点
	void ReplaceParent(pRBTreeNode pBeReplacedNode, pRBTreeNode pReplaceNode);	//把pReplaceNode的父节点修改为pBeReplacedNode的
	bool GetMinNode(pRBTreeNode pNode, pRBTreeNode &pMinNode);					//获取最小的节点
public:
	RBTree();
	~RBTree();
	bool FindElement(int Value);									//查找数据，找到返回true，否则返回false
	void BFS();														//广度遍历
	bool isEmpty();													//判空
	bool GetMax(pRBTreeNode pNode, int &MaxValue);					//获取最大值
	bool GetMin(pRBTreeNode pNode, int &MinValue);					//获取最小值
	void DeleteElement(int DeleteValue);							//删除制定的元素
	void InsertData(int InsertValue);								//插入元素
private:
	pRBTreeNode pRoot;												//根节点指针
	pRBTreeNode pNil;												//哨兵节点
};

void RBTree::InsertFixUP(pRBTreeNode pInsertNode)
{
	/*
	插入节点后，对红黑树的调整操作：
	根据被插入节点的父节点的情况，可以将情况分三类来讨论
	1. 被插入的节点的父节点是红色
	这个时候会产生冲突，不满足约束条件5。
	这种情况下，被插入节点是一定存在非空祖父节点的；进一步的讲，被插入节点也一定存在叔叔节点(即使叔叔节点为空，我们也视之为存在，空节点本身就是黑色节点)。
	依据"叔叔节点的情况"，将这种情况进一步划分为3种情况进行讨论。处理的核心思路是：将红色的节点移到根节点，然后将根节点设置为黑色。

	**以当前节点的父节点是祖父节点的左孩子为例**
	**对应的，若当前节点的父节点是祖父节点的右孩子时，把对应的right和left互换即可**

	1.1 当前节点的父节点是红色，且当前节点的祖父节点的另一个子节点（叔叔节点）也是红色。
	1. 将“父节点”设为黑色。
	2. 将“叔叔节点”设为黑色。
	3. 将“祖父节点”设为“红色”。
	4. 将“祖父节点”设为“当前节点”(红色节点)；即，之后继续对“当前节点”进行操作。
	1.2 当前节点的父节点是红色，叔叔节点是黑色，且当前节点是其父节点的右孩子。
	1. 将“父节点”作为“新的当前节点”。
	2. 以“新的当前节点”为支点进行左旋。
	1.3 当前节点的父节点是红色，叔叔节点是黑色，且当前节点是其父节点的左孩子。
	1. 将“父节点”设为“黑色”。
	2. 将“祖父节点”设为“红色”。
	3. 以“祖父节点”为支点进行右旋。


	2. 被插入的节点的父节点是黑色
	什么也不需要做。节点被插入后，仍然是红黑树。
	3. 被插入的节点是根节点：
	直接把此节点涂为黑色。
	*/

	pRBTreeNode pFixNode = pInsertNode;;
	//如果父节点是红色。（根节点的父节点是Nil，为黑色。）
	while (RBT_COLOR_RED == pFixNode->pParent->Color)
	{
		//1. 如果调整节点的父亲为祖父节点的左孩子
		if (pFixNode->pParent == pFixNode->pParent->pParent->pLeft)
		{
			//获取叔叔节点(祖父节点的右孩子)
			pRBTreeNode pUncle = pFixNode->pParent->pParent->pRight;
			//1.1 如果叔叔节点为红色，情况1.1.1
			if (RBT_COLOR_RED == pUncle->Color)
			{
				//把父节点和叔叔节点都改为黑色
				pFixNode->pParent->Color = RBT_COLOR_BLACK;
				pUncle->Color = RBT_COLOR_BLACK;
				//把祖父节点改为红色
				pFixNode->pParent->pParent->Color = RBT_COLOR_RED;
				//重新计算调整节点为祖父节点
				pFixNode = pFixNode->pParent->pParent;
			}
			//1.2 叔叔节点不为红色，且调整节点为祖父节点的右孩子。这个时候，稍加处理变会转化为情况1.3
			else if (pFixNode == pFixNode->pParent->pRight)
			{
				//从调整节点的父节点开始旋转
				pFixNode = pFixNode->pParent;
				//记录下新的顶点
				pRBTreeNode pNewTop = nullptr;
				SingleLeft(pFixNode->pParent->pLeft, pNewTop);
				//重新设置调整节点
				pFixNode = pNewTop->pLeft;
			}
			//1.3 叔叔节点为黑，且调整节点为父节点的左孩子
			else if (pFixNode == pFixNode->pParent->pLeft)
			{
				//把父节点变成黑色
				pFixNode->pParent->Color = RBT_COLOR_BLACK;
				//把祖父节点变成红色
				pFixNode->pParent->pParent->Color = RBT_COLOR_RED;
				//以祖父节点右旋转(注意到为根节点的情况)
				pFixNode = pFixNode->pParent->pParent;
				//记录下新的顶点
				pRBTreeNode pNewTop = nullptr;
				if (pRoot == pFixNode)
				{
					SingleRight(pRoot, pNewTop);
				}
				else if (pFixNode == pFixNode->pParent->pLeft)
				{
					SingleRight(pFixNode->pParent->pLeft, pNewTop);
				}
				else if (pFixNode == pFixNode->pParent->pRight)
				{
					SingleRight(pFixNode->pParent->pRight, pNewTop);
				}
				//重新设置调整点
				pFixNode = pNewTop->pLeft;
			}
		}
		//2. 如果调整节点的父节点为祖父节点的右孩子，相对于1中，把left和right互换即可
		else if (pFixNode->pParent == pFixNode->pParent->pParent->pRight)
		{
			//获取叔叔节点（祖父节点的左孩子）
			pRBTreeNode pUncle = pFixNode->pParent->pParent->pLeft;
			//2.1 如果叔叔节点为红色
			if (RBT_COLOR_RED == pUncle->Color)
			{
				//把父节点和叔叔节点都改为黑色
				pFixNode->pParent->Color = RBT_COLOR_BLACK;
				pUncle->Color = RBT_COLOR_BLACK;
				//把祖父节点改为红色
				pFixNode->pParent->pParent->Color = RBT_COLOR_RED;
				//重新计算调整节点为祖父节点
				pFixNode = pFixNode->pParent->pParent;
			}
			//2.2 叔叔节点为黑色，且调整节点为父亲节点的左孩子，对应情况1.2（左右互换）
			else if (pFixNode == pFixNode->pParent->pLeft)
			{
				//从调整节点的父节点开始旋转
				pFixNode = pFixNode->pParent;
				//记录下新的顶点
				pRBTreeNode pNewTop = nullptr;
				SingleRight(pFixNode->pParent->pRight, pNewTop);
				//重新设置调整节点
				pFixNode = pNewTop->pRight;
			}
			//2.3 叔叔节点为黑色，且调整节点为父节点的右孩子，对应情况1.3（左右互换）
			else if (pFixNode == pFixNode->pParent->pRight)
			{
				//把父节点变为黑色
				pFixNode->pParent->Color = RBT_COLOR_BLACK;
				//把祖父节点变为红色
				pFixNode->pParent->pParent->Color = RBT_COLOR_RED;
				//对祖父节点进行左旋转（注意到为根节点的情况）
				pFixNode = pFixNode->pParent->pParent;
				//记录下新节点
				pRBTreeNode pNewTop = nullptr;
				if (pRoot == pFixNode)
				{
					SingleLeft(pRoot, pNewTop);
				}
				else if (pFixNode == pFixNode->pParent->pLeft)
				{
					SingleLeft(pFixNode->pParent->pLeft, pNewTop);
				}
				else if (pFixNode == pFixNode->pParent->pRight)
				{
					SingleLeft(pFixNode->pParent->pRight, pNewTop);
				}
				//重新设置调整节点
				pFixNode = pNewTop->pRight;
			}
		}
	}
	//最后把根节点设置为黑色
	pRoot->Color = RBT_COLOR_BLACK;
}

void RBTree::DeleteFixUp(pRBTreeNode pFixNode)
{
	/*
	恢复红黑树的约束条件。
	*/
	while (pFixNode != pRoot&&RBT_COLOR_BLACK == pFixNode->Color)
	{
		//1. 如果调整点是父节点的左孩子
		if (pFixNode == pFixNode->pParent->pLeft)
		{
			pRBTreeNode pBrotherNode = pFixNode->pParent->pRight;			//兄弟节点
																			//1.1 如果兄弟节点为红色，则只能调整成其他的情况
			if (RBT_COLOR_RED == pBrotherNode->Color)
			{
				//把兄弟节点的颜色改为黑色
				pBrotherNode->Color = RBT_COLOR_BLACK;
				//把父节点的颜色改为红色
				pFixNode->pParent->Color = RBT_COLOR_RED;
				//以父节点为根节点左旋
				pRBTreeNode pPivot = pFixNode->pParent;
				pRBTreeNode pNewTop = nullptr;
				//备份parent的指针，因为旋转操作会修改parent指针
				pRBTreeNode pBackParent = pFixNode->pParent;
				if (pRoot = pPivot)
				{
					SingleLeft(pRoot, pNewTop);
				}
				else if (pPivot == pPivot->pParent->pLeft)
				{
					SingleLeft(pPivot->pParent->pLeft, pNewTop);
				}
				else if (pPivot == pPivot->pParent->pRight)
				{
					SingleLeft(pPivot->pParent->pRight, pNewTop);
				}
				//恢复pFixNode的parent指针
				pFixNode->pParent = pBackParent;
			}
			//1.2 如果兄弟节点是黑色且兄弟节点的左右孩子均为黑色
			else if (RBT_COLOR_BLACK == pBrotherNode->pLeft->Color&&RBT_COLOR_BLACK == pBrotherNode->pRight->Color)
			{
				//把兄弟节点染成红色
				pBrotherNode->Color = RBT_COLOR_RED;
				//FixUpNode修改为父亲节点
				pFixNode = pFixNode->pParent;
			}
			//1.3 如果兄弟节点是黑色并且兄弟节点左孩子为红色，右孩子为黑色
			else if (RBT_COLOR_RED == pBrotherNode->pLeft->Color&&RBT_COLOR_BLACK == pBrotherNode->pRight->Color)
			{
				//交换颜色
				RBTColor tempNodeColor = pBrotherNode->pLeft->Color;
				pBrotherNode->pLeft->Color = pBrotherNode->Color;
				pBrotherNode->Color = tempNodeColor;
				//备份parent的指针，因为旋转操作会修改parent指针
				pRBTreeNode pBackParent = pFixNode->pParent;
				//以pBrotherNode进行右旋
				pRBTreeNode pNewTop = nullptr;
				SingleRight(pBrotherNode->pParent->pRight, pNewTop);
				//恢复pFixNode的parent指针
				pFixNode->pParent = pBackParent;
			}
			//1.4 如果兄弟节点是黑色，且兄弟节点的右孩子为红色，兄弟节点的左孩子可以是任意颜色
			else if (RBT_COLOR_RED == pBrotherNode->pRight->Color)
			{
				pBrotherNode->Color = pFixNode->pParent->Color;
				pFixNode->pParent->Color = RBT_COLOR_BLACK;
				pBrotherNode->pRight->Color = RBT_COLOR_BLACK;
				//以pFixNode->pParent左旋
				pRBTreeNode pPivot = pFixNode->pParent;
				pRBTreeNode pNewTop = nullptr;
				if (pRoot == pPivot)
				{
					SingleLeft(pPivot, pNewTop);
				}
				else if (pPivot == pPivot->pParent->pLeft)
				{
					SingleLeft(pPivot->pParent->pLeft, pNewTop);
				}
				else if (pPivot == pPivot->pParent->pRight)
				{
					SingleLeft(pPivot->pParent->pRight, pNewTop);
				}
				pFixNode = pRoot;
			}
		}
		//2. 如果调整节点是父节点的右孩子
		else if (pFixNode == pFixNode->pParent->pRight)
		{
			pRBTreeNode pBrotherNode = pFixNode->pParent->pLeft;
			//2.1 如果兄弟节点为红色节点，则只能通过选择调整成其他的情况，再来处理
			if (RBT_COLOR_RED == pBrotherNode->Color)
			{
				//把兄弟节点的颜色改为黑色
				pBrotherNode->Color = RBT_COLOR_BLACK;
				//把父节点的颜色改为红色
				pFixNode->pParent->Color = RBT_COLOR_RED;
				//以父节点为根，进行右旋
				pRBTreeNode pPivot = pFixNode->pParent;
				pRBTreeNode pNewTop = nullptr;
				//同样备份父节点指针
				pRBTreeNode pBackParent = pFixNode->pParent;
				//
				if (pRoot == pPivot)
				{
					SingleRight(pRoot, pNewTop);
				}
				else if (pPivot == pPivot->pParent->pLeft)
				{
					SingleRight(pPivot->pParent->pLeft, pNewTop);
				}
				else if (pPivot == pPivot->pParent->pRight)
				{
					SingleRight(pPivot->pParent->pRight, pNewTop);
				}

				//恢复pFixNode的Parent指针
				pFixNode->pParent = pBackParent;
			}
			//2.2 如果兄弟节点是黑色且兄弟节点的左右孩子均为黑色
			else if (RBT_COLOR_BLACK == pBrotherNode->pLeft->Color&&RBT_COLOR_BLACK == pBrotherNode->pRight->Color)
			{
				//兄弟节点的颜色改为红色
				pBrotherNode->Color = RBT_COLOR_RED;
				//FixUpNode 修改为父节点
				pFixNode = pFixNode->pParent;
			}
			//2.3 如果兄弟节点是黑色，并且兄弟节点右孩子为红色，左孩子为黑色
			else if (RBT_COLOR_BLACK == pBrotherNode->pLeft->Color&&RBT_COLOR_RED == pBrotherNode->pRight->Color)
			{
				//交换颜色
				RBTColor tempNodeColor = pBrotherNode->pRight->Color;
				pBrotherNode->pRight->Color = pBrotherNode->Color;
				pBrotherNode->Color = tempNodeColor;

				//备份parent指针
				pRBTreeNode pBackParent = pFixNode->pParent;
				//以pBrotherNode进行右旋
				pRBTreeNode pNewTop = nullptr;
				SingleLeft(pBrotherNode->pParent->pLeft, pNewTop);
				//恢复pFixNode的parent指针
				pFixNode->pParent = pBrotherNode;
			}
			//2.4. 如果兄弟节点是黑色,且兄弟节点左孩子是红色,右孩子颜色不限
			else if (RBT_COLOR_RED == pBrotherNode->pLeft->Color)
			{
				pBrotherNode->Color = pFixNode->pParent->Color;
				pFixNode->pParent->Color = RBT_COLOR_BLACK;
				pBrotherNode->pLeft->Color = RBT_COLOR_BLACK;
				pRBTreeNode pPivot = pFixNode->pParent;
				pRBTreeNode pNewTop = nullptr;
				if (pRoot == pPivot)
				{
					SingleRight(pPivot, pNewTop);
				}
				else if (pPivot == pPivot->pParent->pLeft)
				{
					SingleRight(pPivot->pParent->pLeft, pNewTop);
				}
				else if (pPivot == pPivot->pParent->pRight)
				{
					SingleRight(pPivot->pParent->pRight, pNewTop);
				}
				//在执行完这一步后，一定会调整好，直接设置为根节点下次退出
				pFixNode = pRoot;
			}
		}
	}
	pFixNode->Color = RBT_COLOR_BLACK;
}

void RBTree::SingleLeft(pRBTreeNode & pNode, pRBTreeNode & newTop)
{
	/*
	k2									k1
	\							 /     \
	k1				      k2		  N
	\
	N
	*/

	pRBTreeNode k2 = pNode;
	pRBTreeNode k1 = pNode->pRight;
	k2->pRight = k1->pLeft;
	//修改k1左孩子的父节点
	if (k1->pLeft)
	{
		k1->pLeft->pParent = k2;
	}
	k1->pLeft = k2;
	//修改k1的父节点
	k1->pParent = pNode->pParent;
	//修改k2的父节点
	k2->pParent = k1;
	//返回结果
	pNode = k1;
	newTop = k1;
}

void RBTree::SingleRight(pRBTreeNode & pNode, pRBTreeNode & newTop)
{
	/*
	k2									 k1
	/							   /     \
	k1								 N         k2
	/
	N
	*/

	pRBTreeNode k2 = pNode;
	pRBTreeNode k1 = pNode->pLeft;
	k2->pLeft = k1->pRight;
	//修改k1右孩子的父节点
	if (k1->pRight)
	{
		k1->pRight->pParent = k2;
	}
	k1->pRight = k2;
	//修改k1的父节点
	k1->pParent = pNode->pParent;
	//修改k2的父节点
	k2->pParent = k1;
	//返回结果
	pNode = k1;
	newTop = k1;
}

void RBTree::ReplaceParent(pRBTreeNode pBeReplacedNode, pRBTreeNode pReplaceNode)
{
	//替换节点并修改父节点的操作
	//1. 如果pBeReplacedNode是根节点
	if (pBeReplacedNode == pRoot)
	{
		pRoot = pReplaceNode;
	}
	//2. 如果pBeReplacedNode是父节点的左孩子
	else if (pBeReplacedNode == pBeReplacedNode->pParent->pLeft)
	{
		pBeReplacedNode->pParent->pLeft = pReplaceNode;
	}
	//3. 如果pBeReplacedNode是父节点的右孩子
	else if (pBeReplacedNode == pBeReplacedNode->pParent->pRight)
	{
		pBeReplacedNode->pParent->pRight = pReplaceNode;
	}
	//4. 修改pReplaceNode的父亲节点
	pReplaceNode->pParent = pBeReplacedNode->pParent;
}

bool RBTree::GetMinNode(pRBTreeNode pNode, pRBTreeNode & pMinNode)
{
	if (nullptr == pNode || pNode == pNil)
		return false;
	pRBTreeNode pPreNode = pNode->pParent;
	while (pNil != pNode)
	{
		pPreNode = pNode;
		pNode = pNode->pLeft;
	}
	pMinNode = pPreNode;
	return true;
}

RBTree::RBTree()
{
	pRoot = nullptr;

	pNil = new RBTreeNode();
	pNil->pLeft = nullptr;
	pNil->pRight = nullptr;
	pNil->pParent = nullptr;
	pNil->Color = RBT_COLOR_BLACK;
	pNil->Value = 0xFFFFFFFF;
}

RBTree::~RBTree()
{
	if (!isEmpty())
	{
		std::queue<pRBTreeNode>nodeQue;
		nodeQue.push(pRoot);								//根节点入队
		while (!nodeQue.empty())							//队列非空
		{
			pRBTreeNode pNode = nodeQue.front();
			pRBTreeNode pLeft = pNode->pLeft;
			pRBTreeNode pRight = pNode->pRight;
			//出队释放资源
			nodeQue.pop();
			if (pLeft != pNil)nodeQue.push(pLeft);
			if (pRight != pNil)nodeQue.push(pRight);
		}
	}
	if (pNil)												//如果是空节点
	{
		delete pNil;
		pNil = nullptr;
	}
}

bool RBTree::FindElement(int Value)
{
	if (isEmpty())
	{
		return false;
	}
	pRBTreeNode pCurrent = pRoot;
	while (pNil != pCurrent)
	{
		if (Value<pCurrent->Value)
		{
			pCurrent = pCurrent->pLeft;
		}
		else if (Value>pCurrent->Value)
		{
			pCurrent = pCurrent->pRight;
		}
		else
		{
			return true;
		}
	}
	return false;
}

void RBTree::BFS()
{
	if (isEmpty())return;
	std::queue<pRBTreeNode>nodeQue;
	nodeQue.push(pRoot);

	//BFS
	int TreeHeight = 0;
	while (!nodeQue.empty())
	{
		int nCurrentLevelSize = nodeQue.size();						//记录当前层元素的个数
		int nCnt = 0;
		std::cout << "第" << TreeHeight + 1 << "层";
		while (nCnt < nCurrentLevelSize)
		{
			pRBTreeNode acessNode = nodeQue.front();
			nodeQue.pop();
			if (acessNode == pRoot)
			{
				std::cout << acessNode->Value << "根节点，颜色" << acessNode->Color << ")" << ' ';
			}
			else
			{
				if (acessNode->pParent->pLeft == acessNode)
				{
					std::cout << "[(" << acessNode->Value << "颜色" << acessNode->Color << ')' << '(' << acessNode->pParent->Value << "的左孩子)]" << ' ';
				}
				else if (acessNode->pParent->pRight == acessNode)
				{
					std::cout << "[(" << acessNode->Value << "颜色" << acessNode->Color << ')' << '(' << acessNode->pParent->Value << "的右孩子)]" << ' ';
				}
			}

			//下一层的元素
			pRBTreeNode pLeft = acessNode->pLeft;
			pRBTreeNode pRight = acessNode->pRight;
			if (pNil != pLeft)
			{
				nodeQue.push(pLeft);
			}
			if (pNil != pRight)
			{
				nodeQue.push(pRight);
			}
			++nCnt;
		}
		TreeHeight++;
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

bool RBTree::isEmpty()											//判空
{
	return pRoot == nullptr;
}

bool RBTree::GetMax(pRBTreeNode pNode, int & MaxValue)
{
	if (nullptr == pNode)
	{
		return false;
	}
	while (pNode)
	{
		MaxValue = pNode->Value;
		pNode = pNode->pRight;
	}
	return true;

}

bool RBTree::GetMin(pRBTreeNode pNode, int & MinValue)
{
	if (nullptr == pNode)
		return false;
	while (pNode)
	{
		MinValue = pNode->Value;
		pNode = pNode->pLeft;
	}
	return true;
}

void RBTree::DeleteElement(int DeleteValue)
{
	/*
	删除操作
	基本思路如下：
	1. 将红黑树当做一棵二叉查找树，将节点删除。
	分三种情况：
	1. 被删除节点没有儿子，即为叶节点，此时直接删除即可。用空节点顶替它的位置。
	2. 被删除节点只有一个子节点，此时，直接删除该节点，然后用该节点的唯一子节点顶替他。
	3. 被删除节点有两个子节点，则要先找到它的直接后继节点，把值复制到它的位置，然后删除它的后继节点，因为它的后继节点不可能有两个非空子节点，所以这个过程最多只进行一次。
	2. 修正这棵树
	在删除节点后，有可能违反2,4,5三个约束条件，第二步即是调整整棵树，使之满足约束条件。
	*/

	if (isEmpty())
		return;
	//先找到要删除的节点
	pRBTreeNode pCurrent = pRoot;
	pRBTreeNode pDeletedNode = nullptr;
	while (pNil != pCurrent)
	{
		if (DeleteValue<pCurrent->Value)
		{
			pCurrent = pCurrent->pLeft;
		}
		else if (DeleteValue>pCurrent->Value)
		{
			pCurrent = pCurrent->pRight;
		}
		else
		{
			pDeletedNode = pCurrent;
			break;
		}
	}
	//2. 如果未找到，则直接退出
	if (nullptr == pDeletedNode)return;
	//3. 执行删除操作，计算出pDeletedNode,pRealDeleteNode,pFixUpNode.

	pRBTreeNode pRealDeleteNode = nullptr;
	pRBTreeNode pFixUpNode = nullptr;
	RBTColor RealDeleteColor;

	//3.1如果左子树为空
	if (pNil == pDeletedNode->pLeft)
	{
		pRealDeleteNode = pDeletedNode;
		RealDeleteColor = pRealDeleteNode->Color;
		pFixUpNode = pRealDeleteNode->pRight;
		//替换
		ReplaceParent(pRealDeleteNode, pRealDeleteNode->pRight);
	}
	//3.2 如果右子树为空
	else if (pNil == pDeletedNode->pRight)
	{
		pRealDeleteNode = pDeletedNode;
		RealDeleteColor = pRealDeleteNode->Color;
		pFixUpNode = pRealDeleteNode->pLeft;
		//替换
		ReplaceParent(pRealDeleteNode, pRealDeleteNode->pLeft);
	}
	//3.3 如果左右子树都不为空
	else
	{
		/*
		可以用右子树的最小的节点或者左子树的最大的节点来替换，这里选择右子树的最小的节点。这样可以保证二叉搜索树的性质不变
		*/
		//获取准备删除节点的右子树的最小节点，pRealDeleteNode一定不是pNil
		bool GetMinRet = GetMinNode(pDeletedNode->pRight, pRealDeleteNode);
		assert(GetMinRet);
		assert(pRealDeleteNode != pNil);
		RealDeleteColor = pRealDeleteNode->Color;
		//最小的点的左子树已定位pNil,所以pRight就是它的后继节点。
		pFixUpNode = pRealDeleteNode->pRight;
		//思路是用最小点（pRealDeleteNode）来替换需要删除的点（pDeletedNode）的位置，分两种情况
		if (pRealDeleteNode->pParent == pDeletedNode)
		{
			pFixUpNode->pParent = pRealDeleteNode;
		}
		else
		{
			//让pRealDeleteNode父节点指向 pRealDeleteNode->pRight
			ReplaceParent(pRealDeleteNode, pRealDeleteNode->pRight);
			//让pRealDeleteNode的右节点接管原来pDeletedNode的右节点
			pRealDeleteNode->pRight = pDeletedNode->pRight;
			//让pRealDeleteNode的右节点的父节点指向pRealDeleteNode(右子树一定不为pNil)
			pRealDeleteNode->pRight->pParent = pRealDeleteNode;
		}
		//让pDeleteNode父节点指向pRealDeleteNode
		ReplaceParent(pDeletedNode, pRealDeleteNode);
		//让pRealDeleteNode的左节点接管原来pDeleteNode的右节点
		pRealDeleteNode->pLeft = pDeletedNode->pLeft;
		//让pRealDeleteNode的左孩子的父节点指向pRealDeleteNode(左子树一定不为pNil)
		pRealDeleteNode->pLeft->pParent = pRealDeleteNode;
		// 使用pDeleteNode的颜色
		pRealDeleteNode->Color = pDeletedNode->Color;
	}
	//4. 在pFixUpNode点执行调整
	if (RBT_COLOR_BLACK == RealDeleteColor)
	{
		DeleteFixUp(pFixUpNode);
	}
	//5. 处理根节点问题
	if (pRoot == pNil)
		pRoot = nullptr;
	delete pDeletedNode;
}

void RBTree::InsertData(int InsertValue)
{
	/*
	插入节点：
	以二叉查找树的方法增加一个节点。
	一开始要把节点的颜色标记为红色。如果设为黑色，会导致根到叶子的路径上，会有一条路多一个额外的黑色节点。这个是很难调整的。
	另一个理解是，把插入的节点着色为红色，不会违背约束条件1235，只违背一个约束条件，需要处理的情况会很少。
	插入后，通过一系列的旋转、着色等操作，使之重新成为一颗红黑树。
	*/
	//插入的特殊情况：要插入的节点是第一个节点
	if (isEmpty())
	{
		pRoot = new RBTreeNode();
		pRoot->Color = RBT_COLOR_BLACK;
		pRoot->Value = InsertValue;
		pRoot->pLeft = pNil;
		pRoot->pRight = pNil;
		pRoot->pParent = pNil;
		return;
	}
	//2. 找到需要插入的位置
	pRBTreeNode pPreNode = pRoot->pParent;
	pRBTreeNode pCurrent = pRoot;
	while (pNil != pCurrent)
	{
		pPreNode = pCurrent;
		if (InsertValue <= pCurrent->Value)
		{
			pCurrent = pCurrent->pLeft;
		}
		else
		{
			pCurrent = pCurrent->pRight;
		}
	}
	//3. 把数据插入到正确的位置
	pRBTreeNode pInsertNode = new RBTreeNode();
	pInsertNode->Color = RBT_COLOR_RED;
	pInsertNode->Value = InsertValue;
	pInsertNode->pLeft = pNil;
	pInsertNode->pRight = pNil;
	pInsertNode->pParent = pPreNode;
	if (InsertValue <= pPreNode->Value)
	{
		pPreNode->pLeft = pInsertNode;
	}
	else
	{
		pPreNode->pRight = pInsertNode;
	}
	//4. 调整
	InsertFixUP(pInsertNode);
}
