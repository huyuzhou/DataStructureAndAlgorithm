// graph-matrix-dg_dg-shortestpath-DFS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<assert.h>



//DFS处理单源最短路径


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

//访问顶点
void visit_vnode(p_vnode pvnode) {
	assert(pvnode);
	pvnode->value = 1;
	printf(" %c ", pvnode->key);
}

//访问顶点
void unvisit_vnode(p_vnode pvnode) {
	assert(pvnode);
	pvnode->value = 0;
	printf(" %c ", pvnode->key);
}
//与顶点vnodeindex相邻的顶点下标索引，返回相邻的个数
int matrix_dg_adjnode(p_matrix_dg pmdg, int vnodeindex, int indexs[]) {
	int ret = 0;
	if (vnodeindex<0 || vnodeindex>pmdg->v_num) {
		return ret;
	}
	for (int i = 0; i < pmdg->v_num; i++) {
		if (pmdg->edges[vnodeindex][i] != INF_DIST) {
			indexs[ret] = i;
			ret++;
		}
	}
	return ret;
}

//从一个顶点出发递归的遍历
void DFSinner(p_matrix_dg pmdg, int startidx, int endidx, int dist, int* min) {

	if (startidx == endidx) {
		if (dist < *min) {
			*min = dist;
		}
		return;
	}

	visit_vnode(&pmdg->vnodes[startidx]);
	//获取startidx的邻接边
	int adjnodes[MATRIX_VNODE_NUM];
	memset(adjnodes, 0, sizeof(int) * MATRIX_VNODE_NUM);
	int adjnum = matrix_dg_adjnode(pmdg, startidx, adjnodes);

	for (int i = 0; i < adjnum; i++) {
		if (pmdg->vnodes[adjnodes[i]].value == 0)
		{
			//visit_vnode(&pmdg->vnodes[adjnodes[i]]);
			DFSinner(pmdg, adjnodes[i], endidx, pmdg->edges[startidx][adjnodes[i]] + dist, min);
			unvisit_vnode(&pmdg->vnodes[adjnodes[i]]);
		}
	}
}

//这里一层主要是处理，图中有多个连通分量的情况
void DFS(p_matrix_dg pmdg, int keystart, int keyend) {
	printf("DFS：");
	int min = INF_DIST;//保存最短距离
	int dist = 0;//遍历到一个顶点时的距离
	int startidx = matrix_dg_vnode_index(pmdg, keystart);
	int endidx = matrix_dg_vnode_index(pmdg, keyend);
	DFSinner(pmdg, startidx, endidx, dist, &min);
	printf("\n%c==>%c dist：0x%x\n", keystart, keyend, min);
}



int main()
{
	p_matrix_dg pmdg = matrix_dg_example();
	matrix_dg_print(pmdg);


	DFS(pmdg, 'A', 'E');
	//DFS(pmdg, 'B', 'E');
	//DFS(pmdg, 'E', 'E');


	matrix_dg_uninit(pmdg);
	printf("Hello World!\n");
}











