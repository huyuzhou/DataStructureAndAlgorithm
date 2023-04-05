// sort-bucket.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//



#include <stdio.h>
#include<stdlib.h>
#include<string.h>
/*
时间复杂度：O(nlogn)
空间复杂度：o
稳定性：不稳定
*/


//冒泡排序
void swap(int* a, int* b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}
void bubble_sort(int s[], int n) {
	for (int i = 0; i < n - 1; i++) {//
		for (int j = 0; j < n - 1 - i; j++) {//从0~n-1-i为还没有排好序
			if (s[j] > s[j + 1]) {
				swap(&s[j], &s[j + 1]);//相邻两个元素比较
			}
		}
	}
}


#define MAX_BUCKETS 5
#define MAX_BUCKETS_NODES 10

typedef struct _bucket {
	int bucket_node_num;
	int nodes[MAX_BUCKETS_NODES];//这里就固定大小了，不去动态分配内存了
}bucket, p_bucket;

void bucket_sort(int s[], int n) {
	/*
	  0      1       2      3      4
	[0,2) [2,4) [4,6) [6,8) [8,10)
	*/
	bucket buckets[MAX_BUCKETS];
	memset(buckets, 0, MAX_BUCKETS * sizeof(bucket));
	int buckets_idx;
	for (int i = 0; i < n; i++) {
		buckets_idx = s[i] / 2;
		buckets[buckets_idx].nodes[buckets[buckets_idx].bucket_node_num] = s[i];
		buckets[buckets_idx].bucket_node_num++;
	}
	//对每个桶中的数据进行排序（这里使用冒泡排序）
	for (int i = 0; i < MAX_BUCKETS; i++) {
		if (buckets[i].bucket_node_num) {
			bubble_sort(buckets[i].nodes, buckets[i].bucket_node_num);
		}
	}
	int len = 0;
	//将每个桶中的数据拷贝至s数组
	for (int i = 0; i < MAX_BUCKETS; i++) {
		if (buckets[i].bucket_node_num) {
			memcpy(s + len, buckets[i].nodes, buckets[i].bucket_node_num * sizeof(int));
			len += buckets[i].bucket_node_num;
		}
	}
}

int main()
{
	int a[] = { 2,9,4,6,1,5,7 };
	bucket_sort(a, 7);
	for (int i = 0; i < 7; i++) {
		printf("%d ", a[i]);
	}
	printf("\nHello World!\n");
}

