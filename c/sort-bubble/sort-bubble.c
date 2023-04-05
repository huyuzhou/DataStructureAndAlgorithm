// sort-bubble.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include<stdlib.h>

/*
时间复杂度：O(n2)
空间复杂度：0
稳定性：稳定的
*/

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

int main()
{
	int a[] = { 2,9,4,6,1,5,7 };
	bubble_sort(a, 7);
	for (int i = 0; i < 7; i++) {
		printf("%d ", a[i]);
	}
	printf("\nHello World!\n");
}
