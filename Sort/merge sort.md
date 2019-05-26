归并排序（merge sort）

归并排序(Merge Sort)是利用"归并"技术来进行排序。归并是指将若干个已排序的子文件合并成一个有序的文件。

空间复杂度：O(N)​，时间复杂度​O(log N)​，稳定的。

这里只讲二路归并排序。

合并的过程

比较二个数列的第一个数，谁小就先取谁，取了后就在对应数列中删除这个数。然后再进行比较，如果有数列为空，那直接将另一个数列的数据依次取出即可。

代码（C++）

```cpp
void merge(int low, int mid, int high)
{
	int* temp = new int[high - low + 1];
	int cnt = 0;
	int left_low = low, left_high = mid, right_low = mid + 1, right_high = high;
	while (left_low<=left_high&&right_low<=right_high)
	{
		if (v[left_low] <= v[right_low])
			temp[cnt++] = v[left_low++];
		else
			temp[cnt++] = v[right_low++];
	}
	if (left_low <= left_high)		//第一个有剩余
	{
		for (int i = left_low; i <= left_high; i++)
			temp[cnt++] = v[i];
	}
	if (right_low <= right_high)		//第一个有剩余
	{
		for (int i = right_low; i <= right_high; i++)
			temp[cnt++] = v[i];
	}
	for (int i = 0; i < high-low+1; i++)
		v[low + i] = temp[i];
	delete temp;
}
```

归并排序有两种实现方式，一种是自顶向下，一种是自底向上。

自顶向下：

1. 分解，将当前区间一分为二。
2. 求解，进行归并排序。
3. 组合，将两个区间归并为一个有序的区间。

递归的终止条件：子区间长度为1。

代码（C++）：

```cpp
void mergeSort(int low, int high)
{
	int mid = (low + high) / 2;
	if (low < high)
	{
		mergeSort(low, mid);
		mergeSort(mid + 1, high);
		merge(low, mid, high);
	}
}
```

自底向上：

在自顶向下部分，一开始是要把一个大区间逐步划分，直到每个子区间的长度为1，现考虑直接对每个子区间进行操作，将他们逐渐归并。这就是自底向上的思想。

代码（C++）：

```cpp
void mergeLength(int length)
{
	int i = 0;
	for (i = 0; i +2*length< v.size(); i=i+2*length)
	{
		merge(i, i + length - 1, i + 2 * length - 1);
	}
	if (i + length - 1 < v.size() - 1)
		merge(i, i + length - 1, v.size() - 1);
	return;
}
void mergeSort2()
{
	for (int i = 1; i < v.size(); i*=2)
	{
		mergeLength(i);
	}
	return;
}
```

完整代码（C++）：

```cpp
#include<iostream>
#include<vector>
#pragma warning(disable:4996)
using namespace std;

vector<int> v = { 2, 4, 6, 8, 10, 1, 3, 5, 9, 7 };

void merge(int low, int mid, int high)
{
	int* temp = new int[high - low + 1];
	int cnt = 0;
	int left_low = low, left_high = mid, right_low = mid + 1, right_high = high;
	while (left_low<=left_high&&right_low<=right_high)
	{
		if (v[left_low] <= v[right_low])
			temp[cnt++] = v[left_low++];
		else
			temp[cnt++] = v[right_low++];
	}
	if (left_low <= left_high)		//第一个有剩余
	{
		for (int i = left_low; i <= left_high; i++)
			temp[cnt++] = v[i];
	}
	if (right_low <= right_high)		//第一个有剩余
	{
		for (int i = right_low; i <= right_high; i++)
			temp[cnt++] = v[i];
	}
	for (int i = 0; i < high-low+1; i++)
		v[low + i] = temp[i];
	delete temp;
}

//自顶向下
void mergeSort(int low, int high)
{
	int mid = (low + high) / 2;
	if (low < high)
	{
		mergeSort(low, mid);
		mergeSort(mid + 1, high);
		merge(low, mid, high);
	}
}

//自底向上
void mergeLength(int length)
{
	int i = 0;
	for (i = 0; i +2*length< v.size(); i=i+2*length)
	{
		merge(i, i + length - 1, i + 2 * length - 1);
	}
	if (i + length - 1 < v.size() - 1)
		merge(i, i + length - 1, v.size() - 1);
	return;
}
void mergeSort2()
{
	for (int i = 1; i < v.size(); i*=2)
	{
		mergeLength(i);
	}
	return;
}

int main()
{
	//mergeSort(0, v.size() - 1);
	mergeSort2();
	
	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i] << ' ';
	}
	cout << endl;
	system("pause");
	return 0;
}
```

