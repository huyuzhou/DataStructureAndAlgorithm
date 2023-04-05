// graph-matrix-dg_udg-shortestpath-floyd.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<assert.h>

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

//floyd算法实现
void matrix_dg_floyd(p_matrix_dg pmdg) {
	int dist[MATRIX_VNODE_NUM][MATRIX_VNODE_NUM];
	for (int i = 0; i < pmdg->v_num; i++) {
		for (int j = 0; j < pmdg->v_num; j++) {
			dist[i][j] = pmdg->edges[i][j];
		}
	}
	for (int k = 0; k < pmdg->v_num; k++) {
		for (int i = 0; i < pmdg->v_num; i++) {
			for (int j = 0; j < pmdg->v_num; j++) {
				if (dist[i][k] + dist[k][j] < dist[i][j]) {
					dist[i][j] = dist[i][k] + dist[k][j];
				}
			}
		}
	}

	printf("floyd：\n");
	for (int i = 0; i < pmdg->v_num; i++) {
		printf(" %c ", pmdg->vnodes[i].key);
	}
	for (int i = 0; i < pmdg->v_num; i++) {
		printf("\n%c ", pmdg->vnodes[i].key);
		for (int j = 0; j < pmdg->v_num; j++) {
			printf("0x%x ", dist[i][j]);
		}
	}

}



int main()
{
	p_matrix_dg pmdg = matrix_dg_example();
	matrix_dg_print(pmdg);

	matrix_dg_floyd(pmdg);


	matrix_dg_uninit(pmdg);
	printf("\nHello World!\n");
}

