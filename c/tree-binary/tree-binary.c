// tree-binary.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"liststack.h"
#include"listqueue.h"
#define is_letter(a)  ((((a)>='a')&&((a)<='z')) || (((a)>='A')&&((a)<='Z')) || (a)=='#')

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//二叉树的链式存储
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _binary_tree_node
{
	int key;
	int value;//如果不需要，可以忽略
	struct _binary_tree_node* left_child;
	struct _binary_tree_node* right_child;
}binary_tree_node, * p_binary_tree_node;


typedef struct _binary_tree {
	p_binary_tree_node root;
	int nodenum;
}binary_tree, * p_binary_tree;

static char read_char()
{
	char ch;
	do {
		ch = getchar();
	} while (!is_letter(ch));
	return ch;
}

//
p_binary_tree_node binary_tree_node_create_from_input() {
	char d;
	d = read_char();
	if (d == '#') {
		return NULL;
	}
	else {
		p_binary_tree_node pnode = (p_binary_tree_node)malloc(sizeof(binary_tree_node));
		pnode->key = d;
		pnode->value = d;
		pnode->left_child = binary_tree_node_create_from_input();
		pnode->right_child = binary_tree_node_create_from_input();
		return pnode;
	}
}
//手动输入先序序列生成树
p_binary_tree binary_tree_create() {
	p_binary_tree ptree = (p_binary_tree)malloc(sizeof(binary_tree));
	printf("请输入结点个数：\n");
	scanf("%d", &ptree->nodenum);
	printf("请按照先序输入节点数据，#为空数据。例如：ABD##E##C#F##：\n");
	ptree->root = binary_tree_node_create_from_input();
	return ptree;
}

//创建一个结点
p_binary_tree_node binary_tree_node_create(int key, int value) {
	p_binary_tree_node pnode = (p_binary_tree_node)malloc(sizeof(binary_tree_node));
	pnode->key = key;
	pnode->value = value;
	pnode->left_child = NULL;
	pnode->right_child = NULL;
	return pnode;
}

//例子树
/*
		  A
		/    \
	  B       C
	/   \        \
  D      E        F

*/
p_binary_tree binary_tree_example() {
	p_binary_tree ptree = (p_binary_tree)malloc(sizeof(binary_tree));
	ptree->nodenum = 6;
	p_binary_tree_node tmp = binary_tree_node_create('A', 'A');
	ptree->root = tmp;
	ptree->root->left_child = binary_tree_node_create('B', 'B');
	ptree->root->right_child = binary_tree_node_create('C', 'C');
	ptree->root->left_child->left_child = binary_tree_node_create('D', 'D');
	//ptree->root->left_child->right_child = NULL;
	ptree->root->left_child->right_child = binary_tree_node_create('E', 'E');
	ptree->root->right_child->left_child = NULL;
	ptree->root->right_child->right_child = binary_tree_node_create('F', 'F');
	return ptree;
}

void visit_node(p_binary_tree_node pnode) {
	assert(pnode);
	printf("%c", pnode->key);
}

//递归中序遍历
void mid_order(p_binary_tree_node pnode) {
	if (!pnode) {
		return;
	}
	mid_order(pnode->left_child);
	visit_node(pnode);
	mid_order(pnode->right_child);
}

//递归先序遍历
void pre_order(p_binary_tree_node pnode) {
	if (!pnode) {
		return;
	}
	visit_node(pnode);
	pre_order(pnode->left_child);
	pre_order(pnode->right_child);
}

//递归后序遍历
void last_order(p_binary_tree_node pnode) {
	if (!pnode) {
		return;
	}
	last_order(pnode->left_child);
	last_order(pnode->right_child);
	visit_node(pnode);
}

//非递归先序遍历
void pre_order2(p_binary_tree_node pnode) {
	if (pnode == NULL) {
		return;
	}

	list_stack mystack;
	list_stack_init(&mystack);

	//32位系统中才能使用这里实现的 存储int类型的栈，如果是64位的会存在int无法放下64位地址
	while (pnode)
	{
		list_stack_push(&mystack, pnode, 0);
		visit_node(pnode);
		pnode = pnode->left_child;
	}
	while (!list_stack_is_empty(&mystack))
	{
		p_binary_tree_node top = (p_binary_tree_node)list_stack_pop(&mystack);
		p_binary_tree_node right = top->right_child;
		while (right)
		{
			list_stack_push(&mystack, right, 0);
			visit_node(right);
			right = right->left_child;
		}
	}

	list_stack_uninit(&mystack);
}


//非递归中序遍历
void mid_order2(p_binary_tree_node pnode) {
	if (pnode == NULL) {
		return;
	}

	list_stack mystack;
	list_stack_init(&mystack);

	//32位系统中才能使用这里实现的 存储int类型的栈，如果是64位的会存在int无法放下64位地址
	while (pnode)
	{
		list_stack_push(&mystack, pnode, 0);
		pnode = pnode->left_child;
	}
	while (!list_stack_is_empty(&mystack))
	{
		p_binary_tree_node top = (p_binary_tree_node)list_stack_pop(&mystack);
		visit_node(top);
		p_binary_tree_node right = top->right_child;
		while (right)
		{
			list_stack_push(&mystack, right, 0);
			right = right->left_child;
		}
	}

	list_stack_uninit(&mystack);
}

//非递归后序遍历
void last_order2(p_binary_tree_node pnode) {
	if (pnode == NULL) {
		return;
	}

	list_stack mystack;
	list_stack_init(&mystack);

	//32位系统中才能使用这里实现的 存储int类型的栈，如果是64位的会存在int无法放下64位地址
	while (pnode)
	{
		list_stack_push(&mystack, pnode, 0);
		pnode = pnode->left_child;
	}
	p_binary_tree_node p_last_visit = NULL;
	while (!list_stack_is_empty(&mystack))
	{

		p_binary_tree_node top = (p_binary_tree_node)list_stack_top(&mystack);
		if ((top->left_child == NULL && top->right_child == NULL) ||
			top->right_child == p_last_visit ||
			(top->left_child == p_last_visit && top->right_child == NULL)) {
			list_stack_pop(&mystack);
			p_last_visit = top;
			visit_node(top);
		}
		else {
			p_binary_tree_node right = top->right_child;
			while (right)
			{
				list_stack_push(&mystack, right, 0);
				right = right->left_child;
			}
		}
	}
	list_stack_uninit(&mystack);
}

//层次遍历
void level_order(p_binary_tree_node pnode) {
	if (pnode == NULL) {
		return;
	}
	//32位系统中才能使用这里实现的 存储int类型的队列，如果是64位的会存在int无法放下64位地址
	list_queue myqueue;
	list_queue_init(&myqueue);

	list_queue_in(&myqueue, (int)pnode, 0);
	while (!list_queue_is_empty(&myqueue))
	{
		p_binary_tree_node tmp = (p_binary_tree_node)list_queue_out(&myqueue);
		visit_node(tmp);
		if (tmp->left_child) {
			list_queue_in(&myqueue, (int)tmp->left_child, 0);
		}
		if (tmp->right_child) {
			list_queue_in(&myqueue, (int)tmp->right_child, 0);
		}
	}

	list_queue_uninit(&myqueue);
}


//树的深度
int binary_tree_depth(p_binary_tree_node pnode) {
	if (pnode == NULL) {
		return 0;
	}
	int leftdepth = binary_tree_depth(pnode->left_child);
	int rightdepth = binary_tree_depth(pnode->right_child);
	int ret = leftdepth > rightdepth ? leftdepth + 1 : rightdepth + 1;
	return ret;
}

//这里如果传入的是p_binary_tree直接返回tree->nodenum即可，
//但这里使用遍历方式
int binary_tree_size(p_binary_tree_node pnode) {
	if (pnode == NULL) {
		return 0;
	}
	int leftsize = binary_tree_size(pnode->left_child);
	int rightsize = binary_tree_size(pnode->right_child);
	return leftsize + rightsize + 1;
}

//树的叶子结点个数
int binary_tree_leaf_size(p_binary_tree_node pnode) {
	if (pnode == NULL) {
		return 0;
	}
	if (pnode->left_child == NULL && pnode->right_child == NULL) {
		return 1;
	}
	int leftleafsize = binary_tree_leaf_size(pnode->left_child);
	int rightleafsize = binary_tree_leaf_size(pnode->right_child);
	return leftleafsize + rightleafsize;
}


int main()
{

	p_binary_tree ptree = binary_tree_example();
	//p_binary_tree ptree = binary_tree_create();
	printf("先序递归遍历：");
	pre_order(ptree->root);
	printf("\n先序非递归遍历：");
	pre_order2(ptree->root);
	printf("\n中序递归遍历：");
	mid_order(ptree->root);
	printf("\n中序非递归遍历：");
	mid_order2(ptree->root);
	printf("\n后序递归遍历：");
	last_order(ptree->root);
	printf("\n后序非递归遍历：");
	last_order2(ptree->root);
	printf("\n层次遍历：");
	level_order(ptree->root);


	int depth = binary_tree_depth(ptree->root);
	printf("\n当前树的深度为%d", depth);

	int num = binary_tree_size(ptree->root);
	printf("\n当前树的结点个数：%d", num);

	num = binary_tree_leaf_size(ptree->root);
	printf("\n当前树的叶子结点个数：%d", num);

	printf("\n");
	printf("Hello World!\n");
}


