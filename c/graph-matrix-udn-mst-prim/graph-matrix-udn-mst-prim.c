// graph-matrix-udn-mst-prim.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<assert.h>


/*
undirected network 无向网   无向带权图（无向网）
*/


#define MATRIX_VNODE_NUM 100
#define INF_DIST 0X3FFFFFFF //这里增加最大距离，并在初始化时，将邻接矩阵全部初始化为该值，不再使用0进行初始化

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
	for (int i = 0; i < MATRIX_VNODE_NUM; i++) {
		for (int j = 0; j < MATRIX_VNODE_NUM; j++) {
			pmudg->edges[i][j] = INF_DIST;
		}
	}
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
	 A—3—B
	 |   \1
	 4    D
	 |  /6  \7
	C —3—E—9—F
--------生成如下最小生成树---------
	 A—3—B
	 |   \1
	 4    D
	 |
	C —3—E—9—F
*/
//例子
p_matrix_udg matrix_udg_example() {
	p_matrix_udg pmudg = (p_matrix_udg)malloc(sizeof(matrix_udg));
	int vnodes[] = { 'A','B','C','D','E' ,'F' };
	int edges[][3] = { {'A','B',3},{'A','C',4},{'A','D',1},{'C','D',6},{'C','E',3} ,{'D','E',7},{'E','F',9} };
	matrix_udg_init(pmudg, 6, 7);
	for (int i = 0; i < 6; i++) {
		pmudg->vnodes[i].key = vnodes[i];
		pmudg->vnodes[i].value = vnodes[i];
	}
	int m, n;
	for (int i = 0; i < 7; i++) {
		m = matrix_udg_vnode_index(pmudg, edges[i][0]);
		n = matrix_udg_vnode_index(pmudg, edges[i][1]);
		pmudg->edges[m][n] = edges[i][2];
		pmudg->edges[n][m] = edges[i][2];
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
			printf(" 0x%x ", pmudg->edges[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}





//顶点集合结构体
typedef struct _vnode_prim {
	int dist;
	int preindex;
}vnode_prim, * p_vnode_prim;

//增加一个边的结构体，记录
typedef struct _enode {
	int v1;
	int v2;
	int weight;
}enode, * p_enode;

//prim算法选取的边，打印
void matrix_udg_prim_print(p_enode penode, int n) {
	for (int i = 0; i < n; i++) {
		printf("%d：%c--%c weight：%d\n", i, penode[i].v1, penode[i].v2, penode[i].weight);
	}
	printf("\n");
}


void matrix_udg_prim(p_matrix_udg pmudg, int key) {
	//找到key在图中的索引
	int startindex = matrix_udg_vnode_index(pmudg, key);
	//分配临时使用的内存空间
	p_vnode_prim vnodeprim = (p_vnode_prim)malloc(sizeof(vnode_prim) * pmudg->v_num);
	p_enode enodeprim = (p_enode)malloc(sizeof(enode) * pmudg->v_num);
	//初始化
	for (int i = 0; i < pmudg->v_num; i++) {
		vnodeprim[i].dist = INF_DIST;
		vnodeprim[i].preindex = startindex;
	}
	//将startindex顶点加入已选顶点集合中，并初始化剩余顶点到startindex顶点的距离
	vnodeprim[startindex].dist = 0;
	for (int i = 0; i < pmudg->v_num; i++) {
		if (pmudg->edges[startindex][i] < vnodeprim[i].dist) {
			vnodeprim[i].dist = pmudg->edges[startindex][i];
		}
	}

	int min = INF_DIST;
	int curindex = -1;
	enode selectedge;
	int selectedge_index = 0;
	for (int i = 0; i < pmudg->v_num; i++) {
		min = INF_DIST;
		curindex = -1;
		if (i == startindex) {
			continue;
		}
		//找到下一个需要加入已选顶点集合的顶点,curindex即为找到的顶点
		for (int j = 0; j < pmudg->v_num; j++) {
			if (vnodeprim[j].dist != 0 && vnodeprim[j].dist < min) {
				curindex = j;
				min = vnodeprim[j].dist;
			}
		}
		//保存找到的边
		selectedge.v1 = pmudg->vnodes[vnodeprim[curindex].preindex].key;
		selectedge.v2 = pmudg->vnodes[curindex].key;
		selectedge.weight = vnodeprim[curindex].dist;
		enodeprim[selectedge_index] = selectedge;
		selectedge_index++;

		//将该顶点加入到已选集合中
		vnodeprim[curindex].dist = 0;

		//更新记录
		for (int j = 0; j < pmudg->v_num; j++) {
			if (vnodeprim[j].dist != 0 && pmudg->edges[curindex][j] < vnodeprim[j].dist) {
				vnodeprim[j].dist = pmudg->edges[curindex][j];
				vnodeprim[j].preindex = curindex;
			}
		}
	}
	printf("prim：边数：%d\n", selectedge_index);
	matrix_udg_prim_print(enodeprim, selectedge_index);

	free(vnodeprim);
	free(enodeprim);
}




int main()
{
	p_matrix_udg pmudg = matrix_udg_example();
	matrix_udg_print(pmudg);

	matrix_udg_prim(pmudg, 'A');


	matrix_udg_uninit(pmudg);
	printf("Hello World!\n");
}



