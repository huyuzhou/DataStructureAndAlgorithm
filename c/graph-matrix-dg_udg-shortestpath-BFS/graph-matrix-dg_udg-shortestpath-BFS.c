// graph-matrix-dg_udg-shortestpath-BFS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<assert.h>
#include"listqueue.h"


//BFS广度优先搜索查找单源最短路径，适用于有向图 无向图（不带权值）

#define MATRIX_VNODE_NUM 100

typedef struct _vnode {
	int key;
	int value;
}vnode, * p_vnode;

typedef struct _matrix_udg {
	int v_num;
	int e_num;
	vnode vnodes[MATRIX_VNODE_NUM];
	int edges[MATRIX_VNODE_NUM][MATRIX_VNODE_NUM];
}matrix_udg, * p_matrix_udg;

//初始化申请内存
void matrix_udg_init(p_matrix_udg pmudg, int v_num, int e_num) {
	assert(pmudg);
	memset(pmudg, 0, sizeof(matrix_udg));
	pmudg->v_num = v_num;
	pmudg->e_num = e_num;
	return;
}

//释放内存
void matrix_udg_uninit(p_matrix_udg pmudg) {
	assert(pmudg);
	//free(pmudg->vnodes);
	//free(pmudg->edges);
}

//返回结点在数组中的索引下标
int matrix_udg_vnode_index(p_matrix_udg pmudg, int vnodekey) {
	int ret = -1;
	for (int i = 0; i < pmudg->v_num; i++) {
		if (pmudg->vnodes[i].key == vnodekey) {
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
p_matrix_udg matrix_udg_example() {
	p_matrix_udg pmudg = (p_matrix_udg)malloc(sizeof(matrix_udg));
	int vnodes[] = { 'A','B','C','D','E' };
	int edges[][2] = { {'A','B'},{'A','C'},{'A','D'},{'C','D'},{'D','E'} };
	matrix_udg_init(pmudg, 5, 5);
	for (int i = 0; i < 5; i++) {
		pmudg->vnodes[i].key = vnodes[i];
		pmudg->vnodes[i].value = 0;//这里value没有使用，就用来作为是否访问过的标记
	}
	int m, n;
	for (int i = 0; i < 5; i++) {
		m = matrix_udg_vnode_index(pmudg, edges[i][0]);
		n = matrix_udg_vnode_index(pmudg, edges[i][1]);
		pmudg->edges[m][n] = 1;
		pmudg->edges[n][m] = 1;
	}
	return pmudg;
}

//打印邻接矩阵表示的无向图
void matrix_udg_print(p_matrix_udg pmudg) {
	printf("无向图顶点数：%d 边数：%d\n", pmudg->v_num, pmudg->e_num);
	for (int i = 0; i < pmudg->v_num; i++) {
		printf("%2c", pmudg->vnodes[i]);
	}
	printf("\n");
	for (int i = 0; i < pmudg->v_num; i++) {
		printf("%c", pmudg->vnodes[i]);
		for (int j = 0; j < pmudg->v_num; j++) {
			printf("%2d", pmudg->edges[i][j]);
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

//与顶点vnodeindex相邻的顶点下标索引，返回相邻的个数
int matrix_udg_adjnode(p_matrix_udg pmudg, int vnodeindex, int indexs[]) {
	int ret = 0;
	if (vnodeindex<0 || vnodeindex>pmudg->v_num) {
		return ret;
	}
	for (int i = 0; i < pmudg->v_num; i++) {
		if (pmudg->edges[vnodeindex][i]) {
			indexs[ret] = i;
			ret++;
		}
	}
	return ret;
}



//BFS单源最短路径
void BFS(p_matrix_udg pmudg, int key) {
	printf("BFS：");
	int adjnodes[MATRIX_VNODE_NUM];
	int dist[MATRIX_VNODE_NUM];
	memset(dist, 0, sizeof(int) * MATRIX_VNODE_NUM);

	list_queue myqueue;
	list_queue_init(&myqueue);

	int startindex = matrix_udg_vnode_index(pmudg, key);

	visit_vnode(&pmudg->vnodes[startindex]);
	list_queue_in(&myqueue, &pmudg->vnodes[startindex], 0);
	int layer = 1;
	while (!list_queue_is_empty(&myqueue))
	{
		int stacksize = list_queue_lenth(&myqueue);
		for (int i = 0; i < stacksize; i++) {
			p_vnode pvnode = list_queue_out(&myqueue);
			memset(adjnodes, 0, sizeof(int) * MATRIX_VNODE_NUM);
			int curnodeidx = matrix_udg_vnode_index(pmudg, pvnode->key);
			int adjnum = matrix_udg_adjnode(pmudg, curnodeidx, adjnodes);
			for (int j = 0; j < adjnum; j++)
			{
				if (pmudg->vnodes[adjnodes[j]].value == 0)
				{
					visit_vnode(&pmudg->vnodes[adjnodes[j]]);
					list_queue_in(&myqueue, &pmudg->vnodes[adjnodes[j]], 0);
					dist[adjnodes[j]] = layer;
				}
			}
		}
		layer++;
	}
	printf("\n当前原点为：%c\n", key);
	for (int i = 0; i < pmudg->v_num; i++) {
		printf("%c==>%c  dist：%d\n", key, pmudg->vnodes[i].key, dist[i]);
	}

	list_queue_uninit(&myqueue);
}


int main()
{
	p_matrix_udg pmudg = matrix_udg_example();
	matrix_udg_print(pmudg);

	BFS(pmudg, 'A');
	matrix_udg_uninit(pmudg);
	printf("\nHello World!\n");
}









