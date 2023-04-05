// bitmap.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<stdbool.h>

typedef struct _bitmap {
	char* mem;
	int size;
}bitmap, * p_bitmap;

//初始化
void bitmap_init(p_bitmap pbm, int size) {
	pbm->size = size;
	pbm->mem = (char*)malloc(sizeof(char) * (size / 8 + 1));
	memset(pbm->mem, 0, sizeof(char) * (size / 8 + 1));
}

//释放内存
void bitmap_uninit(p_bitmap pbm) {
	assert(pbm);
	free(pbm->mem);
}

//设置
bool bitmap_set(p_bitmap pbm, int n) {
	if (n > pbm->size) {
		return false;
	}
	int byteindex = n / 8;
	int bitindex = n % 8;
	pbm->mem[byteindex] |= 1 << bitindex;
	return true;
}
//清除
bool bitmap_unset(p_bitmap pbm, int n) {
	if (n > pbm->size) {
		return false;
	}
	int byteindex = n / 8;
	int bitindex = n % 8;
	pbm->mem[byteindex] &= ~(1 << bitindex);
	return true;
}

//是否存在
bool bitmap_get(p_bitmap pbm, int n) {
	if (n > pbm->size) {
		return false;
	}
	int byteindex = n / 8;
	int bitindex = n % 8;
	bool ret = pbm->mem[byteindex] & 1 << bitindex;
	return ret;
}


int main()
{
	bitmap bm;
	bitmap_init(&bm, 10);

	bool b = bitmap_set(&bm, 5);
	if (b) {
		printf("设置5成功\n");
	}
	else
	{
		printf("设置5失败\n");
	}
	b = bitmap_get(&bm, 5);
	if (b) {
		printf("获取5成功\n");
	}
	else
	{
		printf("获取5失败\n");
	}
	b = bitmap_unset(&bm, 5);
	if (b) {
		printf("清除设置5成功\n");
	}
	else
	{
		printf("清除设置5失败\n");
	}

	bitmap_uninit(&bm);
	printf("Hello World!\n");
}

