// graph-matrix-dg.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<assert.h>

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
	 A—-—-—>B
	 |   \
	 |     >
	 |       D
	 |     >  \
	 > /        >
	 C             E
*/
//例子
p_matrix_dg matrix_dg_example() {
	p_matrix_dg pmdg = (p_matrix_dg)malloc(sizeof(matrix_dg));
	int vnodes[] = { 'A','B','C','D','E' };
	int edges[][2] = { {'A','B'},{'A','C'},{'A','D'},{'C','D'},{'D','E'} };
	matrix_dg_init(pmdg, 5, 5);
	for (int i = 0; i < 5; i++) {
		pmdg->vnodes[i].key = vnodes[i];
	}
	int m, n;
	for (int i = 0; i < 5; i++) {
		m = matrix_dg_vnode_index(pmdg, edges[i][0]);
		n = matrix_dg_vnode_index(pmdg, edges[i][1]);
		pmdg->edges[m][n] = 1;
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


int main()
{
	p_matrix_dg pmdg = matrix_dg_example();
	matrix_dg_print(pmdg);
	matrix_dg_uninit(pmdg);
	printf("Hello World!\n");
}
