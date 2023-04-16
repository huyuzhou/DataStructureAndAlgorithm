// queue-monotonic.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//



#include <stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//单调队列（底层为链式队列）
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


typedef struct _list_queue_monotonic_node {
	int key;
	int value;//未使用
	struct _list_queue_monotonic_node* next;
}list_queue_monotonic_node, * p_list_queue_monotonic_node;

typedef struct _list_queue_monotonic {
	int lenth;
	p_list_queue_monotonic_node head;
	p_list_queue_monotonic_node tail;
}list_queue_monotonic, * p_list_queue_monotonic;




//初始化
void list_queue_monotonic_init(p_list_queue_monotonic listqueue) {
	assert(listqueue);
	listqueue->lenth = 0;
	listqueue->head = NULL;//无头结点
	listqueue->tail = NULL;
}
void list_queue_monotonic_clear(p_list_queue_monotonic listqueue);
//队列销毁
void list_queue_monotonic_uninit(p_list_queue_monotonic listqueue) {
	list_queue_monotonic_clear(listqueue);
}


//队列判空
bool list_queue_monotonic_is_empty(p_list_queue_monotonic listqueue) {
	return listqueue->lenth == 0 ? true : false;
}

//创建node结点
p_list_queue_monotonic_node list_queue_monotonic_node_create(int key) {
	p_list_queue_monotonic_node newnode = (p_list_queue_monotonic_node)malloc(sizeof(list_queue_monotonic_node));
	newnode->key = key;
	newnode->next = NULL;
	return newnode;
}

//删除node结点
bool list_queue_monotonic_node_delete(p_list_queue_monotonic_node pnode) {
	assert(pnode);
	free(pnode);
	return true;
}


//尾入队
bool list_queue_monotonic_push_back(p_list_queue_monotonic listqueue, int key) {
	while (!list_queue_monotonic_is_empty(listqueue) && (key > list_queue_monotonic_back(listqueue)))
	{
		list_queue_monotonic_pop_back(listqueue);
	}
	p_list_queue_monotonic_node newnode = list_queue_monotonic_node_create(key);
	if (list_queue_monotonic_is_empty(listqueue)) {
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



//尾出队
int list_queue_monotonic_pop_back(p_list_queue_monotonic listqueue) {
	if (list_queue_monotonic_is_empty(listqueue))
	{
		printf("队列为空\n");
		return -1;
	}

	p_list_queue_monotonic_node pre = listqueue->head;
	while (pre->next != NULL && pre->next != listqueue->tail)
	{
		pre = pre->next;
	}
	p_list_queue_monotonic_node del = listqueue->tail;
	if (pre == listqueue->tail) {
		listqueue->head = listqueue->tail = NULL;
	}
	else
	{
		pre->next = NULL;
		listqueue->tail = pre;
	}
	int key = del->key;
	list_queue_monotonic_node_delete(del);
	listqueue->lenth--;
	return key;
}



//头出队
int list_queue_monotonic_pop_front(p_list_queue_monotonic listqueue) {
	if (list_queue_monotonic_is_empty(listqueue))
	{
		printf("队列为空\n");
		return -1;
	}
	p_list_queue_monotonic_node outnode = listqueue->head;
	int key = outnode->key;
	listqueue->head = outnode->next;
	list_queue_monotonic_node_delete(outnode);
	listqueue->lenth--;
	return key;
}


void list_queue_monotonic_clear(p_list_queue_monotonic listqueue) {
	while (listqueue->lenth > 0)
	{
		list_queue_monotonic_pop_front(listqueue);
	}
}

//获取头
int list_queue_monotonic_front(p_list_queue_monotonic listqueue) {
	if (list_queue_monotonic_is_empty(listqueue))
	{
		printf("队列为空\n");
		return -1;
	}
	p_list_queue_monotonic_node node = listqueue->head;
	int key = node->key;
	return key;
}

//获取尾
int list_queue_monotonic_back(p_list_queue_monotonic listqueue) {
	if (list_queue_monotonic_is_empty(listqueue))
	{
		printf("队列为空\n");
		return -1;
	}
	p_list_queue_monotonic_node node = listqueue->tail;
	int key = node->key;
	return key;
}


//队列长度
int list_queue_monotonic_lenth(p_list_queue_monotonic listqueue) {
	return listqueue->lenth;
}

void list_queue_monotonic_display(p_list_queue_monotonic listqueue) {
	p_list_queue_monotonic_node tmp = listqueue->head;
	while (tmp)
	{
		printf("%d", tmp->key);
		printf(" ");
		tmp = tmp->next;
	}
	printf("\n");
}

int main()
{
	list_queue_monotonic listqueue;
	list_queue_monotonic_init(&listqueue);
	list_queue_monotonic_push_back(&listqueue, 6);
	list_queue_monotonic_push_back(&listqueue, 5);
	list_queue_monotonic_push_back(&listqueue, 4);
	list_queue_monotonic_push_back(&listqueue, 3);
	list_queue_monotonic_push_back(&listqueue, 2);
	list_queue_monotonic_push_back(&listqueue, 1);
	printf("初始队列为：");
	list_queue_monotonic_display(&listqueue);
	printf("弹出队列三个元素后为：");
	list_queue_monotonic_pop_back(&listqueue);
	list_queue_monotonic_pop_back(&listqueue);
	list_queue_monotonic_pop_back(&listqueue);
	list_queue_monotonic_display(&listqueue);
	printf("再入队2个元素：");
	list_queue_monotonic_push_back(&listqueue, 13);
	list_queue_monotonic_push_back(&listqueue, 12);
	list_queue_monotonic_push_back(&listqueue, 11);
	list_queue_monotonic_display(&listqueue);
	printf("当前队列长度为：");
	int lenth = list_queue_monotonic_lenth(&listqueue);
	printf("%d\n", lenth);
	printf("当前的队头为：");
	int head = list_queue_monotonic_front(&listqueue);
	printf("%d\n", head);
	printf("当前的队尾为：");
	int back = list_queue_monotonic_back(&listqueue);
	printf("%d\n", back);
	list_queue_monotonic_clear(&listqueue);

	printf("Hello World!\n");
}



