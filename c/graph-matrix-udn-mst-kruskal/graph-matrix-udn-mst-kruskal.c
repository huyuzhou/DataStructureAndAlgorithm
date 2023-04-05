// graph-matrix-udn-mst-kruskal.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//



#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<assert.h>


/*
undirected network 无向网    无向带权图（无向网）
*/

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

//增加一个边的结构体，用于排序
typedef struct _enode {
	int v1;
	int v2;
	int weight;
}enode, * p_enode;

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
			printf("%2d", pmudg->edges[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

//简单使用冒泡排序
void swap(p_enode a, p_enode b) {
	enode tmp = *a;
	*a = *b;
	*b = tmp;
}
void bubble_sort(enode s[], int n) {
	for (int i = 0; i < n - 1; i++) {//
		for (int j = 0; j < n - 1 - i; j++) {//从0~n-1-i为还没有排好序
			if (s[j].weight > s[j + 1].weight) {
				swap(&s[j], &s[j + 1]);//相邻两个元素比较
			}
		}
	}
}

//获取图中所有边
int matrix_udg_enodes(p_matrix_udg pmudg, p_enode penode) {
	assert(penode);
	int index = 0;
	for (int i = 0; i < pmudg->v_num; i++) {
		for (int j = i + 1; j < pmudg->v_num; j++) {
			if (pmudg->edges[i][j] > 0) {
				penode[index].v1 = pmudg->vnodes[i].key;
				penode[index].v2 = pmudg->vnodes[j].key;
				penode[index].weight = pmudg->edges[i][j];
				index++;
			}
		}
	}
	return index;
}

//kruskal算法选取的边，打印
void matrix_udg_kruskal_print(p_enode penode, int n) {
	for (int i = 0; i < n; i++) {
		printf("%d：%c--%c weight：%d\n", i, penode[i].v1, penode[i].v2, penode[i].weight);
	}
	printf("\n");
}

//判断是否出现回路
int matrix_udg_vend(int vends[], int v) {
	while (vends[v] != 0)
	{
		v = vends[v];
	}
	return v;
}

//无向图kruskal算法
void matrix_udg_kruskal(p_matrix_udg pmudg) {
	p_enode penode;
	p_enode penodekruskal;
	int penodenum;
	//vends存放选取的顶点，这些顶点有相同的终点，用来判断是否有回路
	int vends[MATRIX_VNODE_NUM];
	memset(vends, 0, sizeof(int) * MATRIX_VNODE_NUM);

	//从图中提取出所有的边信息
	penode = (p_enode)malloc(sizeof(enode) * pmudg->e_num);
	memset(penode, 0, sizeof(enode) * pmudg->e_num);

	//kruskal算法保存选取的边
	penodekruskal = (p_enode)malloc(sizeof(enode) * pmudg->e_num);
	memset(penodekruskal, 0, sizeof(enode) * pmudg->e_num);

	//提取边
	penodenum = matrix_udg_enodes(pmudg, penode);
	//对边进行排序
	bubble_sort(penode, penodenum);

	printf("所有边：边数：%d\n", penodenum);
	matrix_udg_kruskal_print(penode, penodenum);

	int v1index, v2index;
	int v1end, v2end;
	int selectedge = 0;
	for (int i = 0; i < penodenum; i++) {
		if (selectedge == pmudg->e_num - 1) {
			break;
		}
		v1index = matrix_udg_vnode_index(pmudg, penode[i].v1);
		v2index = matrix_udg_vnode_index(pmudg, penode[i].v2);

		v1end = matrix_udg_vend(vends, v1index);
		v2end = matrix_udg_vend(vends, v2index);
		if (v1end != v2end) {
			vends[v1end] = v2end;
			penodekruskal[selectedge] = penode[i];
			selectedge++;
		}
	}
	printf("kruskal：边数：%d\n", selectedge);
	matrix_udg_kruskal_print(penodekruskal, selectedge);
	free(penode);
	free(penodekruskal);
}





int main()
{
	p_matrix_udg pmudg = matrix_udg_example();
	matrix_udg_print(pmudg);

	matrix_udg_kruskal(pmudg);

	matrix_udg_uninit(pmudg);
	printf("Hello World!\n");
}



