// liststack.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#ifndef _LIST_STACK_H_
#define _LIST_STACK_H_


#include <stdio.h>
#include<stdlib.h>
#include<assert.h>
#include <stdbool.h>

typedef struct _list_stack_node {
	int key;
	int value;//未使用，如果不需要可以忽略
	struct _list_stack_node* next;
}list_stack_node, * p_list_stack_node;

typedef struct _list_stack {
	int lenth;
	p_list_stack_node head;
}list_stack, * p_list_stack;

p_list_stack_node list_stack_node_create(int key, int value);
bool list_stack_node_delete(p_list_stack_node pnode);
void list_stack_clear(p_list_stack liststack);
void list_stack_init(p_list_stack liststack);
void list_stack_uninit(p_list_stack liststack);
bool list_stack_push(p_list_stack liststack, int key, int value);
bool list_stack_is_empty(p_list_stack liststack);
int list_stack_pop(p_list_stack liststack);
int list_stack_lenth(p_list_stack liststack);
void list_stack_display(p_list_stack liststack);
int list_stack_top(p_list_stack liststack);



//清空栈
void list_stack_clear(p_list_stack liststack) {
	while (liststack->lenth > 0)
	{
		list_stack_pop(liststack);
	}
}

//初始化分配内存空间
void list_stack_init(p_list_stack liststack) {
	assert(liststack);
	liststack->lenth = 0;
	liststack->head = NULL;//无头结点
}
void list_stack_uninit(p_list_stack liststack) {
	list_stack_clear(liststack);
}
//创建node结点
p_list_stack_node list_stack_node_create(int key, int value) {
	p_list_stack_node newnode = (p_list_stack_node)malloc(sizeof(list_stack_node));
	newnode->key = key;
	newnode->value = value;
	newnode->next = NULL;
	return newnode;
}

//删除node结点
bool list_stack_node_delete(p_list_stack_node pnode) {
	assert(pnode);
	free(pnode);
	return true;
}


//获取栈顶数据
int list_stack_top(p_list_stack liststack) {
	int ret;
	if (list_stack_is_empty(liststack)) {
		return -1;
	}
	ret = liststack->head->key;
	return ret;
}

//入栈
bool list_stack_push(p_list_stack liststack, int key, int value) {
	p_list_stack_node newnode = list_stack_node_create(key, value);
	assert(newnode);
	newnode->next = liststack->head;
	liststack->head = newnode;
	liststack->lenth++;
	return true;
}

//栈判空
bool list_stack_is_empty(p_list_stack liststack) {
	return liststack->head == NULL ? true : false;
}

//出栈
int list_stack_pop(p_list_stack liststack) {
	int ret;
	if (list_stack_is_empty(liststack)) {
		return -1;
	}
	p_list_stack_node tmp = liststack->head;
	ret = tmp->key;
	liststack->head = tmp->next;
	liststack->lenth--;
	list_stack_node_delete(tmp);
	return ret;
}

//栈长度
int list_stack_lenth(p_list_stack liststack) {
	return liststack->lenth;
}



void list_stack_display(p_list_stack liststack) {
	p_list_stack_node tmp = liststack->head;
	while (tmp)
	{
		printf("%d", tmp->key);
		printf(" ");
		tmp = tmp->next;
	}
	printf("\n");
}

void stackmain()
{
	list_stack liststack;
	list_stack_init(&liststack);
	list_stack_push(&liststack, 1, 0);
	list_stack_push(&liststack, 2, 0);
	list_stack_push(&liststack, 3, 0);
	list_stack_push(&liststack, 4, 0);
	list_stack_push(&liststack, 5, 0);
	printf("链栈的初始内容为：");
	list_stack_display(&liststack);
	printf("弹出栈顶元素后内容为：");
	list_stack_pop(&liststack);
	list_stack_display(&liststack);
	printf("当前栈元素个数为：");
	int lenth = list_stack_lenth(&liststack);
	printf("%d\n", lenth);

	list_stack_clear(&liststack);

	printf("Hello World!\n");
}

#endif // !_LIST_STACK_H_
