// graph-matrix-dag-aov-topolorder.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<assert.h>
#include"listqueue.h"

// Activity On Vertex (AOV图)

#define MATRIX_VNODE_NUM 100

typedef struct _vnode {
	int key;
	int value;
}vnode, * p_vnode;

typedef struct _matrix_dg {
	int v_num;
	int e_num;
	vnode vnodes[MATRIX_VNODE_NUM];
	int edges[MATRIX_VNODE_NUM][MATRIX_VNODE_NUM];
}matrix_dg, * p_matrix_dg;

//初始化申请内存
void matrix_dg_init(p_matrix_dg pmdg, int v_num, int e_num) {
	assert(pmdg);
	memset(pmdg, 0, sizeof(matrix_dg));
	pmdg->v_num = v_num;
	pmdg->e_num = e_num;
	return;
}

//释放内存
void matrix_dg_uninit(p_matrix_dg pmdg) {
	assert(pmdg);
	//free(pmdg->vnodes);
	//free(pmdg->edges);
}

//返回结点在数组中的索引下标
int matrix_dg_vnode_index(p_matrix_dg pmdg, int vnodekey) {
	int ret = -1;
	for (int i = 0; i < pmdg->v_num; i++) {
		if (pmdg->vnodes[i].key == vnodekey) {
			ret = i;
			break;
		}
	}
	return ret;
}


/*
*
*
*
*
				 b                 g
		  6 >    1\         9>    2\
		   /            >   /             >
	   a--4->c-1->e--7->h-4->i
		  \                         >
		   5 >                4/
				 d----2--->f

*/
//例子
p_matrix_dg matrix_dg_example() {
	p_matrix_dg pmdg = (p_matrix_dg)malloc(sizeof(matrix_dg));
	int vnodes[] = { 'A','B','C','D','E','F','G','H','I' };
	int edges[][3] = { {'A','B',6},{'A','C',4},{'A','D',5},{'C','E',1},{'B','E',1},{'E','G',9},{'E','H',7},{'D','F',2},{'F','H',4},{'H','I',4},{'G','I',2} };
	matrix_dg_init(pmdg, 9, 11);
	for (int i = 0; i < 9; i++) {
		pmdg->vnodes[i].key = vnodes[i];
	}
	int m, n;
	for (int i = 0; i < 11; i++) {
		m = matrix_dg_vnode_index(pmdg, edges[i][0]);
		n = matrix_dg_vnode_index(pmdg, edges[i][1]);
		pmdg->edges[m][n] = edges[i][2];
	}
	return pmdg;
}

//打印邻接矩阵表示的有向图
void matrix_dg_print(p_matrix_dg pmdg) {
	printf("有向图顶点数：%d 边数：%d\n", pmdg->v_num, pmdg->e_num);
	for (int i = 0; i < pmdg->v_num; i++) {
		printf("%2c", pmdg->vnodes[i]);
	}
	printf("\n");
	for (int i = 0; i < pmdg->v_num; i++) {
		printf("%c", pmdg->vnodes[i]);
		for (int j = 0; j < pmdg->v_num; j++) {
			printf("%2d", pmdg->edges[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


void matrix_dg_topolorder(p_matrix_dg pmdg) {
	int* indegree = (int*)malloc(sizeof(int) * pmdg->v_num);
	memset(indegree, 0, sizeof(int) * pmdg->v_num);

	int* topolorder = (int*)malloc(sizeof(int) * pmdg->v_num);
	memset(topolorder, 0, sizeof(int) * pmdg->v_num);
	int topolorderidx = 0;

	list_queue myqueue;
	list_queue_init(&myqueue);

	for (int i = 0; i < pmdg->v_num; i++) {
		for (int j = 0; j < pmdg->v_num; j++) {
			if (pmdg->edges[j][i] > 0) {
				indegree[i]++;
			}
		}
	}
	for (int i = 0; i < pmdg->v_num; i++) {
		if (indegree[i] == 0) {
			list_queue_in(&myqueue, i, 0);
		}
	}

	while (!list_queue_is_empty(&myqueue))
	{
		int top = list_queue_out(&myqueue);
		topolorder[topolorderidx++] = top;
		for (int i = 0; i < pmdg->v_num; i++) {
			if (i == top) {
				continue;
			}
			if (pmdg->edges[top][i] > 0) {
				indegree[i]--;
				if (indegree[i] == 0) {
					list_queue_in(&myqueue, i, 0);
				}
			}
		}
	}

	if (topolorderidx == pmdg->v_num) {
		printf("拓扑排序：");
		for (int i = 0; i < topolorderidx; i++) {
			printf("%c ", pmdg->vnodes[i].key);
		}
		printf("\n");
	}
	else
	{
		printf("图中有环\n");
	}

	list_queue_uninit(&myqueue);
	free(indegree);
	free(topolorder);
}



int main()
{
	p_matrix_dg pmdg = matrix_dg_example();
	matrix_dg_print(pmdg);

	matrix_dg_topolorder(pmdg);

	matrix_dg_uninit(pmdg);
	printf("Hello World!\n");
}



