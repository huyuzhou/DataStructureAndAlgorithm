// sort-merge.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include<stdlib.h>
#include<string.h>
/*
时间复杂度：O(nlogn)
空间复杂度：O(n)
稳定性：稳定
*/


void merge(int s[], int start, int mid, int end) {
	int* tmp = (int*)malloc(sizeof(int) * (end - start + 1));
	int i = start;
	int j = mid + 1;
	int k = 0;
	while (i <= mid && j <= end)
	{
		if (s[i] < s[j]) {
			tmp[k] = s[i];
			i++;
			k++;
		}
		else
		{
			tmp[k] = s[j];
			j++;
			k++;
		}
	}
	while (i <= mid)
	{
		tmp[k] = s[i];
		i++;
		k++;
	}
	while (j <= end)
	{
		tmp[k] = s[j];
		j++;
		k++;
	}
	memcpy(s + start, tmp, sizeof(int) * (end - start + 1));
	free(tmp);
}

void merge_sort(int s[], int start, int end) {
	if (start >= end) {
		return;
	}
	int mid = (start + end) / 2;
	merge_sort(s, start, mid);
	merge_sort(s, mid + 1, end);
	merge(s, start, mid, end);
}

int main()
{
	int a[] = { 2,9,4,6,1,5,7 };
	merge_sort(a, 0, 6);
	for (int i = 0; i < 7; i++) {
		printf("%d ", a[i]);
	}
	printf("\nHello World!\n");
}

