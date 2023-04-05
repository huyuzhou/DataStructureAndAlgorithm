// graph-list-udg-DFS-BFS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<assert.h>
#include<string.h>
#include"listqueue.h"

typedef struct _enode {
	int key;
	int value;
	struct  _enode* next;
}enode, * p_enode;

typedef struct _vnode {
	int key;
	int value;
	p_enode next;
}vnode, * p_vnode;

typedef struct _list_udg {
	int v_num;
	int e_num;
	p_vnode vnodes;
}list_udg, * p_list_udg;

//创建边
p_enode list_udg_enode_create(int v1, int v2) {
	p_enode pe = (p_enode)malloc(sizeof(enode));
	pe->key = v1;
	pe->value = v2;
	pe->next = NULL;
	return pe;
}

//删除边
void list_udg_enode_delete(p_enode pe) {
	assert(pe);
	free(pe);
}

//初始化分配内存
void list_udg_init(p_list_udg pludg, int v_num, int e_num) {
	assert(pludg);
	pludg->v_num = v_num;
	pludg->e_num = e_num;
	pludg->vnodes = (p_vnode)malloc(sizeof(vnode) * v_num);
	memset(pludg->vnodes, 0, sizeof(vnode) * v_num);
}

//释放内存
void list_udg_uninit(p_list_udg pludg) {
	assert(pludg);
	for (int i = 0; i < pludg->v_num; i++) {
		p_enode pe = pludg->vnodes[i].next;
		p_enode del = NULL;
		while (pe)
		{
			del = pe;
			pe = pe->next;
			list_udg_enode_delete(del);
		}
	}
	free(pludg->vnodes);
}


//返回结点在数组中的索引下标
int matrix_udg_vnode_index(p_list_udg pludg, int v) {
	int ret = -1;
	for (int i = 0; i < pludg->v_num; i++) {
		if (pludg->vnodes[i].key == v) {
			ret = i;
			break;
		}
	}
	return ret;
}
/*
	 A———B
	 |   \
	 |     D
	 |   /    \
	 C         E
*/
//例子
p_list_udg list_udg_example() {
	p_list_udg pludg = (p_list_udg)malloc(sizeof(list_udg));
	int vnodes[] = { 'A','B','C','D','E' };
	int edges[][2] = { {'A','B'},{'A','C'},{'A','D'},{'C','D'},{'D','E'} };
	list_udg_init(pludg, 5, 5);
	for (int i = 0; i < pludg->v_num; i++) {
		pludg->vnodes[i].key = vnodes[i];
		pludg->vnodes[i].value = 0;
	}
	int m, n;
	for (int i = 0; i < pludg->e_num; i++) {
		//头顶点索引
		m = matrix_udg_vnode_index(pludg, edges[i][0]);
		//尾顶点索引
		n = matrix_udg_vnode_index(pludg, edges[i][1]);
		p_enode pe1 = list_udg_enode_create(m, n);
		pe1->next = pludg->vnodes[m].next;
		pludg->vnodes[m].next = pe1;

		p_enode pe2 = list_udg_enode_create(n, m);
		pe2->next = pludg->vnodes[n].next;
		pludg->vnodes[n].next = pe2;
	}
}

//打印
void list_udg_print(p_list_udg pludg) {
	printf("无向图顶点数：%d 边数：%d\n", pludg->v_num, pludg->e_num);
	for (int i = 0; i < pludg->v_num; i++) {
		printf("%2c", pludg->vnodes[i].key);
		p_enode pe = pludg->vnodes[i].next;
		while (pe)
		{
			printf("  %c--%c  ", pludg->vnodes[i].key, pludg->vnodes[pe->value].key);
			pe = pe->next;
		}
		printf("\n");
	}
	printf("\n");
}


//访问顶点
void visit_vnode(p_vnode pvnode) {
	assert(pvnode);
	pvnode->value = 1;
	printf(" %c ", pvnode->key);
}

//从一个顶点出发递归的遍历
void DFSinner(p_list_udg pludg, int vnodeindex) {
	if (pludg->vnodes[vnodeindex].value == 1) {
		return;
	}
	visit_vnode(&pludg->vnodes[vnodeindex]);
	p_enode pe = pludg->vnodes[vnodeindex].next;
	while (pe)
	{
		p_vnode pv = &pludg->vnodes[pe->value];
		if (pv->value == 0) {
			DFSinner(pludg, pe->value);
		}
		pe = pe->next;
	}
}

//这里一层主要是处理，无向图中有多个连通分量的情况
void DFS(p_list_udg pludg) {
	printf("DFS：");
	for (int i = 0; i < pludg->v_num; i++) {
		if (pludg->vnodes[i].value == 0) {
			DFSinner(pludg, i);
		}
	}
}

//BFS遍历
void BFS(p_list_udg pludg) {
	printf("BFS：");
	list_queue myqueue;
	list_queue_init(&myqueue);

	for (int i = 0; i < pludg->v_num; i++)
	{
		if (pludg->vnodes[i].value == 0)
		{
			visit_vnode(&pludg->vnodes[i]);
			list_queue_in(&myqueue, &pludg->vnodes[i], 0);
			while (!list_queue_is_empty(&myqueue))
			{
				p_vnode pv = list_queue_out(&myqueue);
				p_enode pe = pv->next;
				while (pe)
				{
					pv = &pludg->vnodes[pe->value];
					if (pv->value == 0) {
						visit_vnode(pv);
						list_queue_in(&myqueue, pv, 0);
					}
					pe = pe->next;
				}
			}
		}
	}
	list_queue_uninit(&myqueue);
}


int main()
{
	p_list_udg pludg = list_udg_example();
	list_udg_print(pludg);

	DFS(pludg);
	list_udg_uninit(pludg);
	printf("\n");

	pludg = list_udg_example();
	BFS(pludg);
	list_udg_uninit(pludg);
	printf("\nHello World!\n");
}



