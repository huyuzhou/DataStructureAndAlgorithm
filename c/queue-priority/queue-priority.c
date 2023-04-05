// queue-priority.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//



#include <stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"minheap.h"
/*
各种底层实现
1、数组、链表实现 一般需要O(n)的时间复杂的
2、红黑树、AVL树 插入 删除 查找 logn
3、完全二叉树（堆）logn（插入，删除logn，查找O(1)）
4、本文实现方式为完全二叉树（堆）
*/

typedef struct _priority_queue_node {
	int key;
	int value;
}priority_queue_node, * p_priority_queue_node;

typedef struct _priority_queue {
	min_heap m_heap;
}priority_queue, * p_priority_queue;


void priority_queue_init(p_priority_queue pq, int maxsize) {
	min_heap_init(&pq->m_heap, maxsize);
}

void priority_queue_uninit(p_priority_queue pq) {
	min_heap_uninit(&pq->m_heap);
}

bool priority_queue_insert(p_priority_queue  pq, priority_queue_node pqnode) {
	min_heap_node hnode;
	hnode.key = pqnode.key;
	hnode.value = pqnode.value;
	return min_heap_insert(&pq->m_heap, hnode);
}

bool priority_queue_delete(p_priority_queue  pq, priority_queue_node pqnode) {
	min_heap_node hnode;
	hnode.key = pqnode.key;
	hnode.value = pqnode.value;
	return min_heap_delete(&pq->m_heap, hnode);
}

priority_queue_node priority_queue_gettop(p_priority_queue  pq) {
	min_heap_node hnode = min_heap_gettop(&pq->m_heap);
	priority_queue_node ret;
	ret.key = hnode.key;
	ret.value = hnode.value;
	return ret;
	//return (priority_queue_node)hnode;
}

int priority_queue_size(p_priority_queue pq) {
	return min_heap_size(&pq->m_heap);
}

void priority_queue_print(p_priority_queue pq) {
	min_heap_print(&pq->m_heap);
}


int main()
{

	int key[] = { 1,4,2,8,5,7 };
	int value[] = { 11,14,12,18,15,17 };
	priority_queue pq;
	priority_queue_init(&pq, 10);
	for (int i = 0; i < 6; i++)
	{
		priority_queue_node pqnode;
		pqnode.key = key[i];
		pqnode.value = value[i];
		priority_queue_insert(&pq, pqnode);
	}
	priority_queue_print(&pq);
	priority_queue_node pqnode;
	pqnode.key = 20;
	pqnode.value = 30;
	priority_queue_insert(&pq, pqnode);
	priority_queue_print(&pq);

	pqnode.key = 20;
	pqnode.value = 30;
	priority_queue_delete(&pq, pqnode);
	priority_queue_print(&pq);

	pqnode.key = 40;
	pqnode.value = 50;
	priority_queue_delete(&pq, pqnode);
	priority_queue_print(&pq);

	priority_queue_node top = priority_queue_gettop(&pq);
	printf("top key %d--value%d\n", top.key, top.value);

	priority_queue_uninit(&pq);

	printf("Hello World!\n");

}



