// string_KMP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>

#define MAX_NEXT 100

void get_next(char partern[], int next[], int n) {
	int j, k;
	j = 0;
	k = -1;
	next[0] = -1;
	while (j < n)
	{
		if (k == -1) {
			next[j + 1] = 0;
			k = 0;
			j++;
		}
		else if (partern[j] == partern[k]) {
			next[j + 1] = k + 1;
			j++;
			k++;
		}
		else
		{
			k = next[k];
		}
	}
}

void next_print(int next[], int len) {
	for (int i = 0; i < len; i++)
	{
		printf("%d ", next[i]);
	}
	printf("\n");
}

int KMP(char s[], char p[], int pos) {
	int i, j;
	i = pos;
	j = 0;
	int next[MAX_NEXT];
	memset(next, 0, MAX_NEXT * sizeof(int));
	int slen = strlen(s);
	int plen = strlen(p);
	get_next(p, next, plen);

	next_print(next, plen + 1);

	while (i < slen && j < plen)
	{
		if (j == -1) {
			i++;
			j = 0;
		}
		if (s[i] == p[j]) {
			i++;
			j++;
		}
		else
		{
			j = next[j];
		}
	}
	if (j == plen) {
		return i - j + 1;
	}

	return -1;
}

int main()
{
	char a[] = "abcdabcdababab";
	char b[] = "ababab";
	int pos = KMP(a, b,0);
	printf("%d\n", pos);
	printf("Hello World!\n");
}

