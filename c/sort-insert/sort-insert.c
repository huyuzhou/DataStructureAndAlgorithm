// sort-insert.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include<stdlib.h>
/*
时间复杂度：O(n2)
空间复杂度：0
稳定性：可以稳定的，也可以是不稳定的
*/

void swap(int* a, int* b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void insert_sort(int s[], int n) {
	for (int i = 1; i < n; i++) {
		int key = s[i];//当前待排元素
		int j = i - 1; //0~i-1为已经排好序
		while (j >= 0 && s[j] > key)
		{
			swap(&s[j + 1], &s[j]);
			j--;
		}
		s[j + 1] = key;
	}
}

int main()
{
	int a[] = { 2,9,4,6,1,5,7 };
	insert_sort(a, 7);
	for (int i = 0; i < 7; i++) {
		printf("%d ", a[i]);
	}
	printf("\nHello World!\n");
}

