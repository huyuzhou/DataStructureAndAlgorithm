// hashtable.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include<stdlib.h>
#include<assert.h>
#include <string.h>
#include<stdbool.h>
#define HASH_TABLE_SIZE 13

typedef struct _hashtable_node {
	int key;
	int value;
	struct  _hashtable_node* next;
}hashtable_node, * p_hashtable_node;

typedef struct _hashtable {
	int maxsize;
	int count;
	int* mem;
}hashtable, * p_hashtable;

//创建结点
p_hashtable_node hashtable_node_create(int key, int value) {
	p_hashtable_node pnode = (p_hashtable_node)malloc(sizeof(hashtable_node));
	pnode->key = key;
	pnode->value = value;
	pnode->next = NULL;
	return pnode;
}
//释放结点
bool hashtable_node_delete(p_hashtable_node pnode) {
	assert(pnode);
	free(pnode);
	return true;
}


//hash表初始化
void hashtable_init(p_hashtable phashtable) {
	assert(phashtable);
	phashtable->maxsize = HASH_TABLE_SIZE;
	phashtable->count = 0;
	phashtable->mem = (int*)malloc(sizeof(int) * HASH_TABLE_SIZE);
	memset(phashtable->mem, 0, sizeof(int) * HASH_TABLE_SIZE);
}

//hash表释放内存
void hashtable_uninit(p_hashtable phashtable) {
	assert(phashtable);

	for (int i = 0; i < phashtable->maxsize; i++)
	{
		p_hashtable_node list = (p_hashtable_node)phashtable->mem[i];
		if ((int)list == 0) {
			continue;
		}
		while (list)
		{
			p_hashtable_node del = list;
			list = list->next;
			hashtable_node_delete(del);
		}
	}

	free(phashtable->mem);
}

//hash函数
int hashtable_hash_fuction(p_hashtable phashtable, int key) {
	int add = key % HASH_TABLE_SIZE;
	return add;
}



//插入
bool hashtable_insert(p_hashtable phashtable, int key, int value) {
	int add = hashtable_hash_fuction(phashtable, key);
	p_hashtable_node pnode = hashtable_node_create(key, value);
	if ((phashtable->mem[add]) == 0) {
		phashtable->mem[add] = (int)pnode;
	}
	else
	{
		pnode->next = (p_hashtable_node)phashtable->mem[add];
		phashtable->mem[add] = (int)pnode;
	}
	phashtable->count++;
	return true;
}

//删除
bool hashtable_delete(p_hashtable phashtable, int key) {
	bool ret = false;
	int add = hashtable_hash_fuction(phashtable, key);
	p_hashtable_node list = (p_hashtable_node)phashtable->mem[add];
	if ((int)list == 0) {
		return ret;
	}
	p_hashtable_node pre = NULL;

	while (list->next != NULL)
	{
		if (list->key == key) {
			pre->next = list->next;
			hashtable_node_delete(list);
			phashtable->count--;
			ret = true;
			break;
		}
		pre = list;
		list = list->next;
	}
	return ret;
}
//查找
int hashtable_search(p_hashtable phashtable, int key) {
	int ret = -1;
	int add = hashtable_hash_fuction(phashtable, key);
	p_hashtable_node list = (p_hashtable_node)phashtable->mem[add];
	if ((int)list == 0) {
		return ret;
	}
	p_hashtable_node pre;

	while (list->next != NULL)
	{
		if (list->key == key) {
			ret = list->value;
			break;
		}
		pre = list;
		list = list->next;
	}
	return ret;
}

//打印
void hashtable_print(p_hashtable phashtable) {
	printf("当前hash表中有结点：共%d个\n", phashtable->count);
	for (int i = 0; i < phashtable->maxsize; i++)
	{
		p_hashtable_node list = (p_hashtable_node)phashtable->mem[i];
		if ((int)list == 0) {
			continue;
		}
		int k = 0;
		while (list)
		{
			k++;
			printf("[hash位置%d]：第%d个元素key(%d)--value(%d) \n",
				hashtable_hash_fuction(phashtable, list->key), k, list->key, list->value);
			list = list->next;

		}
		printf("[hash位置%d]：共%d个\n", i, k);
	}
}


int main()
{
	hashtable ht;
	hashtable_init(&ht);
	hashtable_insert(&ht, 1, 11);
	hashtable_insert(&ht, 1, 12);
	hashtable_insert(&ht, 1, 13);
	hashtable_insert(&ht, 1, 14);
	hashtable_insert(&ht, 1, 15);
	hashtable_insert(&ht, 4, 44);
	hashtable_insert(&ht, 4, 45);
	hashtable_print(&ht);

	bool b = hashtable_delete(&ht, 3);
	if (b) {
		printf("删除成功\n");
	}
	else
	{
		printf("删除失败\n");
	}

	b = hashtable_search(&ht, 4);
	if (b) {
		printf("查找成功\n");
	}
	else
	{
		printf("查找失败\n");
	}


	hashtable_uninit(&ht);
	printf("Hello World!\n");
}

