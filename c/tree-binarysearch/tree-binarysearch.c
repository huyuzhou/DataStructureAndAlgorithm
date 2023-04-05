// tree-binarysearch.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//二叉搜索树的链式存储及增删改查
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _binary_search_tree_node
{
	int key;
	int value;//如果不用，可以忽略
	struct _binary_search_tree_node* left_child;
	struct _binary_search_tree_node* right_child;
}binary_search_tree_node, * p_binary_search_tree_node;


typedef struct _binary_search_tree {
	p_binary_search_tree_node root;
	int nodenum;
}binary_search_tree, * p_binary_search_tree;


void visit_node(p_binary_search_tree_node pnode) {
	assert(pnode);
	printf("%c", pnode->key);
}

//递归中序遍历
void mid_order(p_binary_search_tree_node pnode) {
	if (!pnode) {
		return;
	}
	mid_order(pnode->left_child);
	visit_node(pnode);
	mid_order(pnode->right_child);
}

//递归先序遍历
void pre_order(p_binary_search_tree_node pnode) {
	if (!pnode) {
		return;
	}
	visit_node(pnode);
	pre_order(pnode->left_child);
	pre_order(pnode->right_child);
}

//递归后序遍历
void last_order(p_binary_search_tree_node pnode) {
	if (!pnode) {
		return;
	}
	last_order(pnode->left_child);
	last_order(pnode->right_child);
	visit_node(pnode);
}



//创建树结点
p_binary_search_tree_node binary_search_tree_node_create(int key, int value) {
	p_binary_search_tree_node pnode = (p_binary_search_tree_node)malloc(sizeof(binary_search_tree_node));
	pnode->key = key;
	pnode->value = value;
	pnode->left_child = NULL;
	pnode->right_child = NULL;
	return pnode;
}

//删除树结点
bool binary_search_tree_node_delete(p_binary_search_tree_node pnode) {
	assert(pnode);
	free(pnode);
	return true;
}



//在树中插入元素key
bool binary_search_tree_insert(p_binary_search_tree_node* ppnode, int key, int value) {

	if (*ppnode == NULL) {
		*ppnode = binary_search_tree_node_create(key, value);
		return true;
	}
	if ((*ppnode)->key == key) {
		return false;
	}
	if (key < (*ppnode)->key) {
		return binary_search_tree_insert(&(*ppnode)->left_child, key, value);
	}
	else
	{
		return binary_search_tree_insert(&(*ppnode)->right_child, key, value);
	}
}



//查找元素key在树中的位置
p_binary_search_tree_node binary_search_tree_search(p_binary_search_tree_node pnode, int key) {
	if (pnode == NULL) {
		return NULL;
	}

	if (pnode->key == key) {
		return pnode;
	}

	if (key < pnode->key) {
		return binary_search_tree_search(pnode->left_child, key);
	}
	else
	{
		return binary_search_tree_search(pnode->right_child, key);
	}
}



p_binary_search_tree_node  binary_search_tree_maxnode(p_binary_search_tree_node pnode)
{
	if (pnode == NULL)
		return NULL;

	while (pnode->right_child != NULL)
	{
		pnode = pnode->right_child;
	}
	return pnode;
}
p_binary_search_tree_node  binary_search_tree_minnode(p_binary_search_tree_node pnode) {
	if (pnode == NULL) {
		return NULL;
	}
	while (pnode->left_child != NULL) {
		pnode = pnode->left_child;
	}
	return pnode;
}

//
bool binary_search_tree_delete_inner(p_binary_search_tree_node* ppnode) {

	//1、要删除的结点为叶子结点
	//2、要删除的结点只有一个孩子结点
	if ((*ppnode)->left_child == NULL) {
		p_binary_search_tree_node tmp = *ppnode;
		(*ppnode) = (*ppnode)->right_child;
		binary_search_tree_node_delete(tmp);
		return true;
	}
	else if ((*ppnode)->right_child == NULL) {
		p_binary_search_tree_node tmp = *ppnode;
		(*ppnode) = (*ppnode)->left_child;
		binary_search_tree_node_delete(tmp);
		return true;
	}
	else
	{
		//3、要删除的结点有两个孩子结点
		//这里可以从(*ppnode)的右子树找最小结点，
		//也可以从(*ppnode)的左子树找最大结点，正如以下代码实现
		// 
		//

		//这里说明下这个pre指针，其实主要就是用来记录(*ppnode也就是要删除的结点)的左子树中最大结点（cur）的父结点，好修改它结构体中的孩子指针
		p_binary_search_tree_node pre = *ppnode;
		p_binary_search_tree_node cur = (*ppnode)->left_child;
		while (cur->right_child)
		{
			pre = cur;
			cur = cur->right_child;
		}
		(*ppnode)->key = cur->key;//左子树中最大结点的数据域直接复制给要删除的结点
		//为啥要判断一下是否相等？主要是处理下如果(*ppnode)的左子树跟结点就是找到的结点的情况，这个时候pre和（*pnode）指向同一个结点
		if (pre == (*ppnode)) {
			pre->left_child = cur->left_child;
		}
		else
		{
			pre->right_child = cur->left_child;
		}
		binary_search_tree_node_delete(cur);
		return true;
	}
}

//删除元素key所在结点
bool binary_search_tree_delete(p_binary_search_tree_node* ppnode, int key) {
	if (!(*ppnode)) {
		return false;
	}

	if ((*ppnode)->key == key) {
		//删除
		return binary_search_tree_delete_inner(ppnode);
	}

	if (key < (*ppnode)->key) {
		return binary_search_tree_delete(&(*ppnode)->left_child, key);
	}
	else
	{
		return binary_search_tree_delete(&(*ppnode)->right_child, key);
	}
}

//修改树中元素oldkey元素为newkey

bool binary_search_tree_modify(p_binary_search_tree_node pnode, int oldkey, int oldvalue, int newkey, int newvalue) {
	int b = binary_search_tree_delete(&pnode, oldkey);
	if (!b) {
		return b;
	}
	return binary_search_tree_insert(&pnode, newkey, newvalue);
}



//例子树
/*
		  D
		/    \
	  B       E
	/   \        \
 A      C        F

*/
p_binary_search_tree binary_search_tree_example() {

	p_binary_search_tree pbst = (p_binary_search_tree)malloc(sizeof(binary_search_tree));
	pbst->nodenum = 6;
	pbst->root = NULL;
	int a[] = { 'D','B','A','C','E','F' };
	for (int i = 0; i < 6; i++) {
		binary_search_tree_insert(&(pbst->root), a[i], 0);
	}
	return pbst;
}

//打印树结构
void binary_search_tree_print(p_binary_search_tree_node  pnode, p_binary_search_tree_node pfarther, int flag) {

	if (pnode) {
		if (pfarther == NULL) {
			printf("\n%c is root\n", pnode->key);
		}
		else
		{
			printf("%c is %c %s child\n", pnode->key, pfarther->key, (flag == 1 ? "left" : "right"));
		}
		binary_search_tree_print(pnode->left_child, pnode, 1);
		binary_search_tree_print(pnode->right_child, pnode, 2);
	}
}

int main()
{
	p_binary_search_tree pbst = binary_search_tree_example();
	printf("\n-----------------------------------------------------\n");
	printf("先序递归遍历：");
	pre_order(pbst->root);
	printf("\n中序递归遍历：");
	mid_order(pbst->root);
	printf("\n后序递归遍历：");
	last_order(pbst->root);
	binary_search_tree_print(pbst->root, NULL, 0);

	printf("\n-----------------------------------------------------");
	printf("\n添加一个元素G后：");
	binary_search_tree_insert(&(pbst->root), 'G', 0);
	binary_search_tree_print(pbst->root, NULL, 0);

	printf("\n-----------------------------------------------------");
	printf("\n删除一个元素D后：");
	int b = binary_search_tree_delete(&(pbst->root), 'D');
	if (b) {
		printf("\n删除成功");
	}
	else
	{
		printf("\n删除失败");
	}
	binary_search_tree_print(pbst->root, NULL, 0);

	printf("\n-----------------------------------------------------");
	printf("\n再次删除一个元素D后：");
	b = binary_search_tree_delete(&(pbst->root), 'D');
	if (b) {
		printf("\n删除成功");
	}
	else {
		printf("\n删除失败");
	}
	binary_search_tree_print(pbst->root, NULL, 0);

	printf("\n-----------------------------------------------------");
	printf("\n查找一个元素B：");
	p_binary_search_tree_node pnode = binary_search_tree_search((pbst->root), 'B');
	if (pnode) {
		printf("\n查找成功");
	}
	else
	{
		printf("\n查找失败");
	}
	binary_search_tree_print(pbst->root, NULL, 0);


	printf("\n-----------------------------------------------------");
	printf("\n查找一个不存在元素O：");
	b = binary_search_tree_delete(&(pbst->root), 'O');
	if (b) {
		printf("\n查找成功");
	}
	else
	{
		printf("\n查找失败");
	}
	binary_search_tree_print(pbst->root, NULL, 0);


	printf("\n-----------------------------------------------------");
	printf("\n修改一个元素A-->K后：");
	b = binary_search_tree_modify((pbst->root), 'A', 0, 'K', 0);
	if (b) {
		printf("\n修改成功");
	}
	else
	{
		printf("\n修改失败");
	}
	binary_search_tree_print(pbst->root, NULL, 0);

	printf("\nHello World!\n");
}


