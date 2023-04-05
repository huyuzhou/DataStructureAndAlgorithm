// listqueue.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#ifndef _LIST_QUEUE_H_
#define _LIST_QUEUE_H_


#include <stdio.h>
#include<stdlib.h>
#include<assert.h>

typedef struct _list_queue_node {
	int key;
	int value;//未使用，如果不需要可以忽略
	struct _list_queue_node* next;
}list_queue_node, * p_list_queue_node;

typedef struct _list_queue {
	int lenth;
	p_list_queue_node head;
	p_list_queue_node tail;
}list_queue, * p_list_queue;



bool list_queue_node_delete(p_list_queue_node pnode);
p_list_queue_node list_queue_node_create(int key, int value);
void list_queue_init(p_list_queue listqueue);
void list_queue_uninit(p_list_queue listqueue);
void list_queue_clear(p_list_queue listqueue);
bool list_queue_is_empty(p_list_queue listqueue);
bool list_queue_in(p_list_queue listqueue, int key, int value);
int list_queue_out(p_list_queue listqueue);
int list_queue_lenth(p_list_queue listqueue);
void list_queue_display(p_list_queue listqueue);

//清空队列
void list_queue_clear(p_list_queue listqueue) {
	while (listqueue->lenth > 0)
	{
		list_queue_out(listqueue);
	}
}


//初始化
void list_queue_init(p_list_queue listqueue) {
	assert(listqueue);
	listqueue->lenth = 0;
	listqueue->head = NULL;//无头结点
	listqueue->tail = NULL;
}

//队列销毁
void list_queue_uninit(p_list_queue listqueue) {
	list_queue_clear(listqueue);
}

//队列判空
bool list_queue_is_empty(p_list_queue listqueue) {
	return listqueue->lenth == 0 ? true : false;
}

//创建node结点
p_list_queue_node list_queue_node_create(int key, int value) {
	p_list_queue_node newnode = (p_list_queue_node)malloc(sizeof(list_queue_node));
	newnode->key = key;
	newnode->value = value;
	newnode->next = NULL;
	return newnode;
}

//删除node结点
bool list_queue_node_delete(p_list_queue_node pnode) {
	assert(pnode);
	free(pnode);
	return true;
}

//入队
bool list_queue_in(p_list_queue listqueue, int key, int value) {
	p_list_queue_node newnode = list_queue_node_create(key, value);
	if (list_queue_is_empty(listqueue)) {
		listqueue->head = listqueue->tail = newnode;
		listqueue->lenth++;
		return true;
	}
	else
	{
		listqueue->tail->next = newnode;
		listqueue->tail = newnode;
		listqueue->lenth++;
		return true;
	}
}

//出队
int list_queue_out(p_list_queue listqueue) {
	if (list_queue_is_empty(listqueue))
	{
		printf("队列为空\n");
		return -1;
	}
	p_list_queue_node outnode = listqueue->head;
	int key = outnode->key;
	listqueue->head = outnode->next;
	list_queue_node_delete(outnode);
	listqueue->lenth--;
	return key;
}



//队列长度
int list_queue_lenth(p_list_queue listqueue) {
	return listqueue->lenth;
}

void list_queue_display(p_list_queue listqueue) {
	p_list_queue_node tmp = listqueue->head;
	while (tmp)
	{
		printf("%d", tmp->key);
		printf(" ");
		tmp = tmp->next;
	}
	printf("\n");
}

void  queuemain()
{
	list_queue listqueue;
	list_queue_init(&listqueue);
	list_queue_in(&listqueue, 1, 0);
	list_queue_in(&listqueue, 2, 0);
	list_queue_in(&listqueue, 3, 0);
	list_queue_in(&listqueue, 4, 0);
	list_queue_in(&listqueue, 5, 0);
	list_queue_in(&listqueue, 6, 0);
	printf("初始队列为：");
	list_queue_display(&listqueue);
	printf("弹出队列三个元素后为：");
	list_queue_out(&listqueue);
	list_queue_out(&listqueue);
	list_queue_out(&listqueue);
	list_queue_display(&listqueue);
	printf("再入队2个元素：");
	list_queue_in(&listqueue, 11, 0);
	list_queue_in(&listqueue, 12, 0);
	list_queue_display(&listqueue);
	printf("当前队列长度为：");
	int lenth = list_queue_lenth(&listqueue);
	printf("%d\n", lenth);

	list_queue_clear(&listqueue);

	printf("Hello World!\n");
}


#endif // !_LIST_QUEUE_H_
