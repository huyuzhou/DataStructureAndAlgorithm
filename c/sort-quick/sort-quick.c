// sort-quick.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include<stdlib.h>

/*
时间复杂度：O(nlogn)
空间复杂度：o
稳定性：不稳定
*/
void swap(int* a, int* b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int partition(int s[], int start, int end) {
	int select = end;
	int pos = start;
	for (int i = start; i <= end - 1; i++) {
		if (s[i] < s[select]) {
			swap(&s[i], &s[pos]);
			pos++;
		}
	}
	swap(&s[select], &s[pos]);
	return pos;
}


void quick_sort(int s[], int start, int end) {
	if (start >= end) {
		return;
	}
	int selectpos = partition(s, start, end);
	quick_sort(s, start, selectpos - 1);
	quick_sort(s, selectpos + 1, end);
}

int main()
{
	int a[] = { 2,9,4,6,1,5,7 };
	quick_sort(a, 0, 6);
	for (int i = 0; i < 7; i++) {
		printf("%d ", a[i]);
	}
	printf("\nHello World!\n");
}
