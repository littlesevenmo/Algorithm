冒泡排序。

基本思想：从无序序列头部开始，进行两两比较，根据大小交换位置，直到最后将最大（小）的数据元素交换到了无序队列的队尾，从而成为有序序列的一部分；下一次继续这个过程，直到所有数据元素都排好序。

每一趟只能确定将一个数归位，核心是双重循环，算法是**稳定**的。

时间复杂度最好情况下是$O(N)$，在正好有序的情况下。最坏情况下是$O(N^2)$，在完全反序的情况下。平均时间复杂度是$O(N^2)$。

因此在内层循环里，是`v.size()-i`，因为最后的`i`都已经有序了。

代码（C++）

```cpp
#include<iostream>
#include<vector>
#pragma warning(disable:4996)
using namespace std;

vector<int> v = { 2, 4, 6, 8, 10, 1, 3, 5, 9, 7 };

int main()
{
	for (int i = 0; i < v.size(); i++)
	{
		for (int j = 0; j < v.size()-i-1; j++)
		{
			if (v[j] > v[j + 1])				//  > 表示升序，反之为降序
				swap(v[j], v[j + 1]);
		}
	}

	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i] << ' ';
	}
	cout << endl;
	system("pause");
	return 0;
}
```



