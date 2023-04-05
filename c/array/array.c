// array.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include<stdlib.h>
#include<assert.h>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//顺序表存储及增删改查
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _array {
	int* head;
	int lenth;
	int size;
}array, * parray;

//初始化分配内存
void array_init(parray a, int memsize) {
	assert(a);
	a->head = (int*)malloc(sizeof(int) * memsize);
	assert(a->head);
	a->lenth = 0;
	a->size = memsize;
}

//释放内存
void array_uninit(parray a) {
	assert(a);
	assert(a->head);
	free(a->head);
}

//在pos位置插入key元素
int array_insert(parray a, int pos, int key) {
	assert(a->lenth < a->size);
	assert((pos <= a->lenth) && (pos >= 0));

	for (int i = a->lenth - 1; i >= pos; i--)
	{
		a->head[i + 1] = a->head[i];
	}
	a->head[pos] = key;
	a->lenth++;
	return pos;
}

//尾插入元素
int array_inserttail(parray a, int key) {
	return array_insert(a, a->lenth, key);
}

//头插入元素
int array_inserthead(parray a, int key) {
	return array_insert(a, 0, key);
}

//删除pos位置元素
int array_delete(parray a, int pos) {
	assert(pos >= 0 && pos < a->lenth);

	for (int i = pos; i < a->lenth; i++)
	{
		a->head[i] = a->head[i + 1];
	}
	a->lenth--;
	return pos;
}

//查找key元素在array中出现的位置（首次位置）
int array_search(parray a, int key) {
	for (int i = 0; i < a->lenth; i++)
	{
		if (a->head[i] == key) {
			return i;
		}
	}
	return -1;
}

void array_display(parray a) {
	for (int i = 0; i < a->lenth; i++)
	{
		printf("%d", a->head[i]);
		printf(" ");
	}
	printf("\n");
}


//修改pos位置的元素值为newkey，返回旧值
int array_modify(parray a, int pos, int newkey) {
	assert(pos >= 0 && pos < a->lenth);
	int oldkey = a->head[pos];
	a->head[pos] = newkey;
	return oldkey;
}

//array元素个数
int array_lenth(parray a) {
	return a->lenth;
}

int main()
{
	array myarray;
	array_init(&myarray, 30);
	array_inserttail(&myarray, 11);
	array_inserttail(&myarray, 12);
	array_inserttail(&myarray, 13);
	array_inserttail(&myarray, 14);
	array_inserttail(&myarray, 15);
	array_insert(&myarray, 5, 1);
	array_insert(&myarray, 5, 2);
	array_insert(&myarray, 5, 3);
	array_insert(&myarray, 5, 4);
	array_insert(&myarray, 5, 5);
	array_inserthead(&myarray, 21);
	array_inserthead(&myarray, 22);
	array_inserthead(&myarray, 23);
	array_inserthead(&myarray, 24);
	array_inserthead(&myarray, 25);

	printf("初始array内容：");
	array_display(&myarray);

	printf("删除pos=10后内容：");
	array_delete(&myarray, 10);
	array_display(&myarray);

	int oldkey = array_modify(&myarray, 0, 88);
	printf("修改pos=0(旧值：%d)为88内容：", oldkey);
	array_display(&myarray);

	printf("查找元素为88位置：");
	int pos = array_search(&myarray, 88);
	printf("%d\n", pos);

	printf("当前顺序表长度为：");
	int lenth = array_lenth(&myarray);
	printf("%d\n", lenth);

	array_uninit(&myarray);

	printf("Hello World!\n");
}

