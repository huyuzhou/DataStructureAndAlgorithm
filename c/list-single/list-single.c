// list-single.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//单向链表存储及增删改查
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
单向链表
表中数据索引从-1开始(包含头结点)，
*/
typedef struct _single_list_node {
	int key;
	struct _single_list_node* next;
}single_list_node, * p_single_list_node;


typedef struct _single_list {
	p_single_list_node head;
	int lenth;
}single_list, * p_single_list;


void single_list_display(p_single_list list) {
	assert(list);
	p_single_list_node tmp;
	tmp = list->head;
	while (tmp->next)
	{
		printf("%d", tmp->next->key);
		tmp = tmp->next;
		printf(" ");
	}
	printf("\n");
}

//初始化一个链表
void single_list_init(p_single_list list) {
	list->lenth = 0;
	list->head = (p_single_list_node)malloc(sizeof(single_list_node));//有头结点
	assert(list->head);
	list->head->key = 0xffff;
	list->head->next = NULL;
}

//链表长度
int single_list_lenth(p_single_list list) {
	return list->lenth;
}

//创建一个node结点
p_single_list_node single_list_node_create(int key) {
	p_single_list_node newnode = (p_single_list_node)malloc(sizeof(single_list_node));
	newnode->key = key;
	newnode->next = NULL;
	return newnode;
}

//删除一个node结点
bool single_list_node_delete(p_single_list_node pnode) {
	assert(pnode);
	free(pnode);
	return true;
}

//获取第pos个位置的结点指针
p_single_list_node single_list_searchnode(p_single_list list, int pos) {
	assert(pos >= -1 && pos < list->lenth);

	p_single_list_node tmp = list->head;
	int i = -1;
	while (tmp)
	{
		if (i == pos) {
			break;
		}
		tmp = tmp->next;
		i++;
	}
	return tmp;
}

//在链表中pos位置插入数据key
bool single_list_insert(p_single_list list, int pos, int key) {
	//pos等于0为头结点插入
	//pos等于lenth为尾结点插入
	assert(pos <= list->lenth && pos >= 0);

	p_single_list_node prenode = single_list_searchnode(list, pos - 1);
	p_single_list_node newnode = single_list_node_create(key);

	newnode->next = prenode->next;
	prenode->next = newnode;

	list->lenth++;
	return true;
}

//头插入元素
bool single_list_inserthead(p_single_list list, int key) {
	return single_list_insert(list, 0, key);
}
//尾插入元素
bool single_list_inserttail(p_single_list list, int key) {
	return single_list_insert(list, list->lenth, key);
}

//找到元素为元素key，返回其在链表中的pos位置（首次出现）
int single_list_search(p_single_list list, int key) {
	int pos = -1;
	p_single_list_node tmp = list->head;
	while (tmp)
	{
		if (tmp->key == key) {
			return pos;
		}
		tmp = tmp->next;
		pos++;
	}
	return -1;
}


//删除pos位置的元素key
bool single_list_delete(p_single_list list, int pos) {
	assert(pos < list->lenth&& pos >= 0);
	p_single_list_node prenode = single_list_searchnode(list, pos - 1);
	p_single_list_node del = prenode->next;
	prenode->next = del->next;
	single_list_node_delete(del);
	list->lenth--;
	return true;
}

//修改pos位置上的元素，返回旧值
int single_list_modify(p_single_list list, int pos, int newkey) {
	assert(pos < list->lenth&& pos >= 0);
	p_single_list_node curnode = single_list_searchnode(list, pos);
	int oldkey = curnode->key;
	curnode->key = newkey;
	return oldkey;
}




int main()
{
	single_list list;
	single_list_init(&list);
	single_list_inserthead(&list, 11);
	single_list_inserthead(&list, 12);
	single_list_inserthead(&list, 13);
	single_list_inserthead(&list, 14);
	single_list_inserthead(&list, 15);
	single_list_inserttail(&list, 1);
	single_list_inserttail(&list, 2);
	single_list_inserttail(&list, 3);
	single_list_inserttail(&list, 4);
	single_list_inserttail(&list, 5);
	single_list_inserthead(&list, 21);
	single_list_inserthead(&list, 22);
	single_list_inserthead(&list, 23);
	single_list_inserthead(&list, 24);
	single_list_inserthead(&list, 25);
	single_list_insert(&list, 5, 33);
	single_list_insert(&list, 5, 34);
	single_list_insert(&list, 5, 35);
	printf("初始单链表内容：");
	single_list_display(&list);

	printf("删除pos=5元素后单链表内容：");
	single_list_delete(&list, 5);
	single_list_display(&list);

	int oldkey = single_list_modify(&list, 4, 88);
	printf("修改pos=4（旧值%d）内容为88后：", oldkey);
	single_list_display(&list);

	printf("查找元素88首次出现位置位置：");
	int pos = single_list_search(&list, 88);
	printf("%d\n", pos);

	printf("当前单链表长度为：");
	int lenth = single_list_lenth(&list);
	printf("%d\n", lenth);

	printf("Hello World!\n");
}


