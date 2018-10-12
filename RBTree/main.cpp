#include<iostream>
#include "RBTree.h"
#pragma warning(disable:4996)

int main()
{
	RBTree rbTree;
	//插入
	int n[]= { 12, 1, 9, 2, 0, 11, 7, 19, 4, 15, 18, 5, 14, 13, 10, 16, 6, 3, 8, 17 };
	for (int i = 0; i < 20; i++)
	{
		rbTree.InsertData(n[i]);
	}
	rbTree.BFS();
	std::cout << "------------------------------" << std::endl;
	//删除
	for (int i = 0; i <20; i++)
	{
		std::cout << "删除" << n[i] << "后" << std::endl;
		rbTree.DeleteElement(n[i]);
		rbTree.BFS();
	}
	//插入任意序列
	std::cout << "插入任意序列" << std::endl;
	for (int i = 0; i < 100; i++)
	{
		rbTree.InsertData(i);
	}
	//查找3
	std::cout << "查找3" << std::endl;
	std::cout << "结果:" << rbTree.FindElement(3) << std::endl;
	rbTree.BFS();
	std::cout << "------------------------------" << std::endl;
	//删除任意序列,只留三个
	for (int i = 99; i >= 3; i--)
	{
		rbTree.DeleteElement(i);
	}
	//广度遍历
	rbTree.BFS();
	std::cout << "------------------------------" << std::endl;
	system("pause");
	return 0;
}