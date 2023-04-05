// graph-list-dg.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

// graph-list-dg.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

/*
undirected graph 无向图 dg
undirected network 无向网 udn
directed graph 有向图 dg
directed network 有向网 dn
*/


#include <stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<assert.h>
#include<string.h>

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

typedef struct _list_dg {
	int v_num;
	int e_num;
	p_vnode vnodes;
}list_dg, * p_list_dg;

//创建边
p_enode list_dg_enode_create(int v1, int v2) {
	p_enode pe = (p_enode)malloc(sizeof(enode));
	pe->key = v1;
	pe->value = v2;
	pe->next = NULL;
	return pe;
}

//删除边
void list_dg_enode_delete(p_enode pe) {
	assert(pe);
	free(pe);
}

//初始化分配内存
void list_dg_init(p_list_dg pldg, int v_num, int e_num) {
	assert(pldg);
	pldg->v_num = v_num;
	pldg->e_num = e_num;
	pldg->vnodes = (p_vnode)malloc(sizeof(vnode) * v_num);
	memset(pldg->vnodes, 0, sizeof(vnode) * v_num);
}

//释放内存
void list_dg_uninit(p_list_dg pldg) {
	assert(pldg);
	for (int i = 0; i < pldg->v_num; i++) {
		p_enode pe = pldg->vnodes[i].next;
		p_enode del = NULL;
		while (pe)
		{
			del = pe;
			pe = pe->next;
			list_dg_enode_delete(del);
		}
	}
	free(pldg->vnodes);
}


//返回结点在数组中的索引下标
int matrix_dg_vnode_index(p_list_dg pldg, int v) {
	int ret = -1;
	for (int i = 0; i < pldg->v_num; i++) {
		if (pldg->vnodes[i].key == v) {
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

p_list_dg list_dg_example() {
	p_list_dg pldg = (p_list_dg)malloc(sizeof(list_dg));
	int vnodes[] = { 'A','B','C','D','E' };
	int edges[][2] = { {'A','B'},{'A','C'},{'A','D'},{'C','D'},{'D','E'} };
	list_dg_init(pldg, 5, 5);
	for (int i = 0; i < pldg->v_num; i++) {
		pldg->vnodes[i].key = vnodes[i];
	}
	int m, n;
	for (int i = 0; i < pldg->e_num; i++) {
		//头顶点索引
		m = matrix_dg_vnode_index(pldg, edges[i][0]);
		//尾顶点索引
		n = matrix_dg_vnode_index(pldg, edges[i][1]);
		p_enode pe1 = list_dg_enode_create(m, n);
		pe1->next = pldg->vnodes[m].next;
		pldg->vnodes[m].next = pe1;
	}
}

//打印
void list_dg_print(p_list_dg pldg) {
	printf("有向图顶点数：%d 边数：%d\n", pldg->v_num, pldg->e_num);
	for (int i = 0; i < pldg->v_num; i++) {
		printf("%2c", pldg->vnodes[i].key);
		p_enode pe = pldg->vnodes[i].next;
		while (pe)
		{
			printf("  %c--%c  ", pldg->vnodes[i].key, pldg->vnodes[pe->value].key);
			pe = pe->next;
		}
		printf("\n");
	}
	printf("\n");
}

int main()
{
	p_list_dg pldg = list_dg_example();
	list_dg_print(pldg);
	list_dg_uninit(pldg);
	printf("Hello World!\n");
}


