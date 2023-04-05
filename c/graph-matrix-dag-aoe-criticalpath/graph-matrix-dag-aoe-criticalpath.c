// graph-matrix-dag-aoe-criticalpath.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//



#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<assert.h>
#include"listqueue.h"
#include"liststack.h"

// Activity On Edge (AOE图)

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



//计算有向图中顶点的etv
int matrix_dg_etv(p_matrix_dg pmdg, int etv[], p_list_stack mystack) {
	int ret = -1;
	int* indegree = (int*)malloc(sizeof(int) * pmdg->v_num);
	memset(indegree, 0, sizeof(int) * pmdg->v_num);

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
		list_stack_push(mystack, top, 0);
		for (int i = 0; i < pmdg->v_num; i++) {
			if (i == top) {
				continue;
			}
			if (pmdg->edges[top][i] > 0) {
				indegree[i]--;
				if (indegree[i] == 0) {
					list_queue_in(&myqueue, i, 0);
				}
				if (etv[top] + pmdg->edges[top][i] > etv[i]) {
					etv[i] = etv[top] + pmdg->edges[top][i];
				}
			}
		}
	}

	if (mystack->lenth == pmdg->v_num) {
		ret = 0;
	}

	list_queue_uninit(&myqueue);
	free(indegree);

	return ret;
}


//计算有向图中顶点的ltv
int matrix_dg_ltv(p_matrix_dg pmdg, int ltv[], p_list_stack mystack, int maxcost) {
	//初始化ltv全部为关键路径长度
	for (int i = 0; i < pmdg->v_num; i++) {
		ltv[i] = maxcost;
	}

	while (!list_stack_is_empty(mystack))
	{
		int top = list_stack_pop(mystack);
		for (int i = 0; i < pmdg->v_num; i++) {
			if (top == i) {
				continue;
			}
			if (pmdg->edges[i][top] > 0) {
				if (ltv[top] - pmdg->edges[i][top] < ltv[i]) {
					ltv[i] = ltv[top] - pmdg->edges[i][top];
				}
			}
		}
	}
	return 0;
}

//打印etv 和ltv
void matrix_dg_etv_ltv_print(int tv[], int n, int flag) {
	printf("%s：", flag == 0 ? "etv：" : "ltv：");
	for (int i = 0; i < n; i++) {
		printf("%d ", tv[i]);
	}
	printf("\n");
}

//打印入选的关键路径
void matrix_dg_critialpath_print(p_matrix_dg pmdg, p_list_queue myqueue) {
	printf("关键路径：\n");
	int i, j;
	while (!list_queue_is_empty(myqueue))
	{
		i = list_queue_out(myqueue);
		j = list_queue_out(myqueue);
		printf("%c--%c  weight : %d\n", pmdg->vnodes[i].key, pmdg->vnodes[j].key, pmdg->edges[i][j]);
	}
	printf("\n");
}

void matrix_dg_critialpath(p_matrix_dg pmdg) {
	//事件的最早发生时间
	int* etv = (int*)malloc(sizeof(int) * pmdg->v_num);
	memset(etv, 0, sizeof(int) * pmdg->v_num);
	//事件的最晚发生时间
	int* ltv = (int*)malloc(sizeof(int) * pmdg->v_num);

	//这里的队列主要用来保存选取的关键路径
	list_queue myqueue;
	list_queue_init(&myqueue);

	//这里的栈主要用来保存拓扑排序列表
	list_stack mystack;
	list_stack_init(&mystack);

	//获取etv，同时将拓扑排序列表保存在mystack中
	int r = matrix_dg_etv(pmdg, etv, &mystack);
	if (r == -1) {
		goto end;
	}
	matrix_dg_etv_ltv_print(etv, pmdg->v_num, 0);

	int des = list_stack_top(&mystack);//栈顶就是终点，通过终点获取关键路径的长度
	r = matrix_dg_ltv(pmdg, ltv, &mystack, etv[des]);
	if (r == -1) {
		goto end;
	}
	matrix_dg_etv_ltv_print(ltv, pmdg->v_num, 1);

	//遍历边计算边的ete和lte，如果相等则入选关键边
	int ete = 0;
	int lte = 0;
	for (int i = 0; i < pmdg->v_num; i++) {
		for (int j = 0; j < pmdg->v_num; j++) {
			if (pmdg->edges[i][j] > 0) {
				ete = etv[i];
				lte = ltv[j] - pmdg->edges[i][j];
				if (ete == lte) {
					list_queue_in(&myqueue, i, 0);//头顶点入队
					list_queue_in(&myqueue, j, 0);//尾顶点入队
				}
			}
		}
	}
	//打印最后入选的边
	matrix_dg_critialpath_print(pmdg, &myqueue);

end:
	if (etv) {
		free(etv);
	}
	if (ltv) {
		free(ltv);
	}
	list_stack_uninit(&mystack);
	list_queue_uninit(&myqueue);
}



int main()
{
	p_matrix_dg pmdg = matrix_dg_example();
	matrix_dg_print(pmdg);

	matrix_dg_critialpath(pmdg);


	matrix_dg_uninit(pmdg);
	printf("\nHello World!\n");
}




