// graph-matrix-dg_udg-shortestpath-dijkstra.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<assert.h>
#include"liststack.h"


#define MATRIX_VNODE_NUM 100
#define INF_DIST 0X3FFFFFFF //这里增加最大距离，并在初始化时，将邻接矩阵全部初始化为该值，不再使用0进行初始化

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
	for (int i = 0; i < MATRIX_VNODE_NUM; i++) {
		for (int j = 0; j < MATRIX_VNODE_NUM; j++) {
			pmdg->edges[i][j] = INF_DIST;
		}
	}
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
	 A—-5-—>B
	 |   \4
  2 |     >
	 |       D
	 |     >  \3
	 > / 1     >
	 C             E
*/
//例子
p_matrix_dg matrix_dg_example() {
	p_matrix_dg pmdg = (p_matrix_dg)malloc(sizeof(matrix_dg));
	int vnodes[] = { 'A','B','C','D','E' };
	int edges[][3] = { {'A','B',5},{'A','C',2},{'A','D',4},{'C','D',1},{'D','E',3} };
	matrix_dg_init(pmdg, 5, 5);
	for (int i = 0; i < 5; i++) {
		pmdg->vnodes[i].key = vnodes[i];
	}
	int m, n;
	for (int i = 0; i < 5; i++) {
		m = matrix_dg_vnode_index(pmdg, edges[i][0]);
		n = matrix_dg_vnode_index(pmdg, edges[i][1]);
		pmdg->edges[m][n] = edges[i][2];
	}
	return pmdg;
}

//打印邻接矩阵表示的有向图
void matrix_dg_print(p_matrix_dg pmdg) {
	printf("无向图顶点数：%d 边数：%d\n", pmdg->v_num, pmdg->e_num);
	for (int i = 0; i < pmdg->v_num; i++) {
		printf("%2c", pmdg->vnodes[i]);
	}
	printf("\n");
	for (int i = 0; i < pmdg->v_num; i++) {
		printf("%c", pmdg->vnodes[i]);
		for (int j = 0; j < pmdg->v_num; j++) {
			printf(" 0x%x ", pmdg->edges[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

typedef struct _vnodedist {
	int dist;
	int flag;
	int preidx;
}vnodedist, * p_vnodedist;

//打印
void matrix_dg_dijkstra_print(p_matrix_dg pmdg, int startkey, p_vnodedist pvdist) {
	printf("dijkstra：源点为：%c\n", startkey);
	int startidx = matrix_dg_vnode_index(pmdg, startkey);
	list_stack mystack;
	list_stack_init(&mystack);
	for (int i = 0; i < pmdg->v_num; i++) {
		if (i == startidx) {
			continue;
		}
		list_stack_clear(&mystack);
		int pre = i;
		while (pre != startidx)
		{
			list_stack_push(&mystack, pre, 0);
			pre = pvdist[pre].preidx;
		}
		list_stack_push(&mystack, startidx, 0);
		while (!list_stack_is_empty(&mystack))
		{
			int v1 = list_stack_pop(&mystack);
			printf(" %c ", pmdg->vnodes[v1].key);
			int len = list_stack_lenth(&mystack);
			if (len >= 1) {
				printf("==>");
			}
		}
		printf(" lenth：0x%x\n", pvdist[i].dist);
	}
	list_stack_uninit(&mystack);
}


void matrix_dg_dijkstra(p_matrix_dg pmdg, int startkey) {
	int startidx = matrix_dg_vnode_index(pmdg, startkey);
	//分配辅助内存
	p_vnodedist pvdist = (p_vnodedist)malloc(sizeof(vnodedist) * pmdg->v_num);
	//初始化
	for (int i = 0; i < pmdg->v_num; i++) {
		pvdist[i].dist = pmdg->edges[startidx][i];
		pvdist[i].preidx = startidx;
		pvdist[i].flag = 1;
	}

	//将startidx顶点加入已选集合
	pvdist[startidx].flag = 0;

	//开始遍历
	for (int i = 0; i < pmdg->v_num; i++) {
		//如果当前顶点已经入选，则跳过
		if (pvdist[i].flag == 0) {
			continue;
		}
		//从未入选的顶点中找到距离最短的那个顶点
		int min = INF_DIST;
		int minidx = -1;
		for (int j = 0; j < pmdg->v_num; j++) {
			if (pvdist[j].flag == 0) {
				continue;
			}
			if (pvdist[j].dist < min) {
				min = pvdist[j].dist;
				minidx = j;
			}
		}
		//异常处理下，如果已经找不到距离最短的顶点了，结束
		if (minidx == -1) {
			break;
		}
		//将找到的顶点加入已选集合
		pvdist[minidx].flag = 0;
		//用当前入选的顶点，更新其他没有入选的顶点到源点的距离
		for (int j = 0; j < pmdg->v_num; j++) {
			if (pvdist[j].flag == 0) {
				continue;
			}
			if ((pmdg->edges[minidx][j] + pvdist[minidx].dist < pvdist[j].dist)) {
				pvdist[j].dist = pmdg->edges[minidx][j] + pvdist[minidx].dist;
				pvdist[j].preidx = minidx;
			}
		}
	}

	matrix_dg_dijkstra_print(pmdg, startkey, pvdist);

	free(pvdist);
}





int main()
{
	p_matrix_dg pmdg = matrix_dg_example();
	matrix_dg_print(pmdg);

	matrix_dg_dijkstra(pmdg, 'B');

	matrix_dg_dijkstra(pmdg, 'A');

	matrix_dg_uninit(pmdg);
	printf("\nHello World!\n");
}
