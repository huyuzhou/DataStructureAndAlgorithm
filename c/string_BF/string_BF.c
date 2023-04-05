// string_BF.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include<stdlib.h>
#include<string.h>

int BF(char s[], char p[], int pos) {
	int slen = strlen(s);
	int plen = strlen(p);
	int i = pos;
	int j = 0;
	while (i < slen && j < plen)
	{
		if (s[i] == p[j]) {
			i++;
			j++;
		}
		else
		{
			i = i - j + 1;
			j = 0;
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
	int pos = BF(a, b, 0);
	printf("%d\n", pos);
	printf("Hello World!\n");
}


