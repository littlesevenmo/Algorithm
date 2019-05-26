一种玩笑式的排序算法，原理是开多个线程，根据排序的值进行Sleep，到时就输出这个数。

C++：

```cpp
#include <iostream> 
#include <windows.h> 
#include <process.h> 
#include <vector>

using namespace std;

void routine(void *a)
{
	int n = *(int *)a;
	Sleep(n);
	cout << n << ' ';
	return;
}

void sleepSort(vector<int> &v)
{
	HANDLE* threads= (HANDLE*)malloc(sizeof(HANDLE)*v.size());
	for (int i = 0; i < v.size(); i++)
		threads[i] = (HANDLE)_beginthread(&routine, 0, &v[i]);
	WaitForMultipleObjects(v.size(), threads, TRUE, INFINITE);
	return;
}

int main()
{
	vector<int> arr = { 50, 10, 100, 1 };
	sleepSort(arr);
	cout << endl;
	system("pause");
	return(0);
}
```

说下`WaitForMultipleObjects`函数，函数原型是

```cpp
DWORD WaitForMultipleObjects(  

  DWORD nCount,             // number of handles in the handle array  

  CONST HANDLE *lpHandles,  // pointer to the object-handle array  

  BOOL fWaitAll,            // wait flag  

  DWORD dwMilliseconds      // time-out interval in milliseconds  

);  
```

参数解析：

- `DWORD`就是Double Word，每个Word长为2字节。
- `nCount`指定列表中句柄的数量。最大值为64（MAXIMUM_WAIT_OBJECTS）。
- `*lpHandles`表示指向句柄数组的指针。lpHandles为指定对象句柄组合中的第一个元素，HANDLE类型可以为（Event，Mutex，Process，Thread，Semaphore）数组。我这里是Thread类型，由`_beginthread`创建。
- `bWaitAll`为等待的类型，如果为TRUE，表示除非对象都发出信号，否则就一直等待下去；如果FALSE，表示任何对象发出信号即可。
- `dwMilliseconds`指定要等候的毫秒数。如设为零，表示立即返回。如指定常数INFINITE，则可根据实际情况无限等待下去。

这里没用到函数的返回值，就不在这里啰嗦了。