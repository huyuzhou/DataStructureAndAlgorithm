// list-double.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//双向链表存储及增删改查
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



typedef struct _double_list_node {
	int key;
	int value;//未使用
	struct _double_list_node* pre;
	struct _double_list_node* next;
}double_list_node, * p_double_list_node;

typedef struct _double_list {
	p_double_list_node head;
	p_double_list_node tail;
	int lenth;
}double_list, * p_double_list;


void double_list_display(p_double_list dlist, int mode) {
	if (mode) {
		p_double_list_node tmp = dlist->head;
		while (tmp->next) {
			printf("%d", tmp->next->key);
			printf(" ");
			tmp = tmp->next;
		}
		printf("\n");
	}
	else {
		p_double_list_node tmp = dlist->tail;
		while (tmp) {
			printf("%d", tmp->key);
			printf(" ");
			tmp = tmp->pre;
		}
		printf("\n");
	}
}

void double_list_init(p_double_list dlist) {
	assert(dlist);
	//dlist->lenth = 0;
	//dlist->head = NULL;//无表头结点

	dlist->lenth = 0;
	dlist->head = (p_double_list_node)malloc(sizeof(double_list_node));//有表头结点
	dlist->head->key = 0xffff;
	dlist->head->pre = NULL;
	dlist->head->next = NULL;
	dlist->tail = dlist->head;
	dlist->tail->key = 0xffff;
	dlist->tail->pre = NULL;
	dlist->tail->next = NULL;

}

//根据元素key创建结点node
p_double_list_node double_list_node_create(int key, int value) {
	p_double_list_node newnode = (p_double_list_node)malloc(sizeof(double_list_node));
	newnode->next = NULL;
	newnode->pre = NULL;
	newnode->key = key;
	newnode->value = value;
	return newnode;
}

//删除node结点
bool double_list_node_delete(p_double_list_node pnode) {
	assert(pnode);
	free(pnode);
	return true;
}

//根据pos位置在链表中查找node结点
p_double_list_node double_list_searchnode(p_double_list dlist, int pos) {
	assert(pos >= -1 && pos < dlist->lenth);
	int i;
	p_double_list_node tmp;
	if (pos < dlist->lenth / 2) {
		//从前往后找（这里由于从头结点开始所以i初始为-1）
		i = -1;
		tmp = dlist->head;
		while (tmp)
		{
			if (i == pos) {
				break;
			}
			tmp = tmp->next;
			i++;
		}
	}
	else {
		//从后往前找
		i = dlist->lenth - 1;
		tmp = dlist->tail;
		while (tmp)
		{
			if (i == pos) {
				break;
			}
			tmp = tmp->pre;
			i--;
		}
	}
	return tmp;
}

//链表长度
int double_list_lenth(p_double_list dlist) {
	return dlist->lenth;
}


//根据元素key在链表中查找元素位置(首次出现位置)
int double_list_search(p_double_list dlist, int key) {
	int pos = 0;
	p_double_list_node tmp = dlist->head->next;
	while (tmp)
	{
		if (tmp->key == key) {
			return pos;
		}
		tmp = tmp->next;
		pos++;
	}
	return -1;
}

//在pos位置插入key元素
bool double_list_insert(p_double_list dlist, int pos, int key, int value) {
	assert(pos <= dlist->lenth && pos >= 0);

	p_double_list_node prenode = double_list_searchnode(dlist, pos - 1);
	p_double_list_node newnode = double_list_node_create(key, value);

	if (pos == dlist->lenth) {
		//尾插入结点
		prenode->next = newnode;
		newnode->pre = prenode;
		//更新链表尾指针
		dlist->tail = newnode;
	}
	else {
		//头插入和中间插入
		newnode->next = prenode->next;
		prenode->next->pre = newnode;
		newnode->pre = prenode;
		prenode->next = newnode;
	}

	dlist->lenth++;
	return true;
}

//头插入
bool double_list_inserthead(p_double_list dlist, int key, int value) {
	return double_list_insert(dlist, 0, key, value);
}

//尾插入 
bool double_list_inserttail(p_double_list dlist, int key, int value) {
	return double_list_insert(dlist, dlist->lenth, key, value);
}


//删除位置pos的元素
bool double_list_delete(p_double_list dlist, int pos) {
	assert(pos < dlist->lenth&& pos >= 0);
	p_double_list_node del = double_list_searchnode(dlist, pos);
	del->pre->next = del->next;
	if (pos == dlist->lenth - 1) {
		//删除最后元素
		//更新链表tail指针
		dlist->tail = del->pre;
	}
	else {
		del->next->pre = del->pre;
	}
	double_list_node_delete(del);
	dlist->lenth--;
	return true;

}
//将pos位置元素内容修改为newkey，返回旧key
int double_list_modify(p_double_list dlist, int pos, int newkey, int newvalue) {
	assert(pos < dlist->lenth&& pos >= 0);
	int oldkey;
	p_double_list_node curnode = double_list_searchnode(dlist, pos);
	oldkey = curnode->key;
	curnode->value = newvalue;
	curnode->key = newkey;
	return oldkey;
}



int main()
{
	double_list dlist;
	double_list_init(&dlist);
	double_list_inserttail(&dlist, 11, 0);
	double_list_inserttail(&dlist, 12, 0);
	double_list_inserttail(&dlist, 13, 0);
	double_list_inserttail(&dlist, 14, 0);
	double_list_inserttail(&dlist, 15, 0);
	double_list_inserthead(&dlist, 1, 0);
	double_list_inserthead(&dlist, 2, 0);
	double_list_inserthead(&dlist, 3, 0);
	double_list_inserthead(&dlist, 4, 0);
	double_list_inserthead(&dlist, 5, 0);
	double_list_insert(&dlist, 5, 22, 0);
	double_list_insert(&dlist, 5, 22, 0);
	double_list_insert(&dlist, 5, 22, 0);
	double_list_insert(&dlist, 5, 22, 0);
	double_list_insert(&dlist, 5, 22, 0);
	printf("初始双向链表内容：");
	double_list_display(&dlist, 1);

	printf("删除位置pos=13后双向链表内容：");
	double_list_delete(&dlist, 13);
	double_list_display(&dlist, 1);

	int oldkey = double_list_modify(&dlist, 12, 15, 0);
	printf("修改位置pos=12(旧值：%d)内容为15：", oldkey);
	double_list_display(&dlist, 1);

	printf("删除尾元素后：");
	double_list_delete(&dlist, dlist.lenth - 1);
	double_list_display(&dlist, 1);

	printf("查找key=22首次出现位置：");
	int pos = double_list_search(&dlist, 22);
	printf("%d\n", pos);

	printf("当前双向链表长度为：");
	int lenth = double_list_lenth(&dlist);
	printf("%d\n", lenth);

	printf("反向输出双向链表：");
	double_list_display(&dlist, 0);

	printf("Hello World!\n");
}



