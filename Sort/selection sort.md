直接选择排序(selection sort)

基本思想：每一趟从待排序的记录中选出关键字最小的记录，顺序放在已排好序的子文件的最后，直到全部记录排序完毕。常用的选择排序方法有直接选择排序和堆排序。

具体步骤：

1. 初始状态：无序区为 R[1..n]，有序区为空。
2. 第 1 趟排序：在无序区 R[1..n]中选出关键字最小的记录 R[k]，将它与无序区的第 1 个记录 R[1]交换，使 R[1..i]和 R[2..n]分别变为记录个数增加 1 个的新有序区和记录个数减少 1 个的新无序区。 ……
3. 第 i 趟排序：第i趟排序开始时，当前有序区和无序区分别为 R[1..i-1]和 R[i..n][1≤i≤n-1]。该趟排序从当前无序区中选出关键字最小的记录 R[k]，将它与无序区的第 i 个记录 R[i]交换，使 R[1..i]和 R[i+1..n]分别变为记录个数增加 1 个的新有序区和记录个数减少 1 个的新无序区。

这样，n 个记录的文件的直接选择排序可经过 n-1 趟直接选择排序得到有序结果。

代码（C++)：

```cpp
#include<iostream>
#include<vector>
#pragma warning(disable:4996)
using namespace std;

vector<int> v = { 2, 4, 6, 8, 10, 1, 3, 5, 9, 7 };

void selectionSort()
{
	for (int i = 0; i < v.size(); i++)
	{
		int min = i;
		for (int j = i+1; j < v.size(); j++)
		{
			if (v[j] < v[min])
				min = j;
		}
		swap(v[i], v[min]);
	}
}

int main()
{
	selectionSort();

	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i] << ' ';
	}
	cout << endl;
	system("pause");
	return 0;
}
```



