// sort-select.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <stdio.h>
#include<stdlib.h>
/*
时间复杂度：O(n2)
空间复杂度：o
稳定性：不稳定
*/
void swap(int* a, int* b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void select_sort(int s[], int n) {
	for (int i = 0; i < n; i++) {
		int minindex = i;
		for (int j = i; j < n; j++) {
			if (s[j] < s[minindex]) {
				minindex = j;
			}
		}
		swap(&s[i], &s[minindex]);
	}
}

int main()
{
	int a[] = { 2,9,4,6,1,5,7 };
	select_sort(a, 7);
	for (int i = 0; i < 7; i++) {
		printf("%d ", a[i]);
	}
	printf("\nHello World!\n");
}

