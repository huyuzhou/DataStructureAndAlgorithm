// tree-AVL.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

/********************************************************************************
* @文件名: AVLtree.c
* @作者: yuzhouhu
* @版本: 1.1
* @时间: 2022-3-19
* @描述: AVL平衡二叉搜索树的链式存储及增删改查。
********************************************************************************/

#include <stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//平衡二叉搜索树的链式存储及增删改查
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define HEGHT(p) ((p == NULL)?0:((p_AVL_tree_node)(p))->heght)
#define MAX(a, b)    ( (a) > (b) ? (a) : (b) )
#define ABS(a) ( (a) > 0 ? (a) :  (- (a)))

typedef struct _AVL_tree_node
{
	int key;//关键字
	int value;//树上value，
	int heght;
	struct _AVL_tree_node* left_child;
	struct _AVL_tree_node* right_child;
}AVL_tree_node, * p_AVL_tree_node;


typedef struct _AVL_tree {
	p_AVL_tree_node root;
	int nodenum;
}AVL_tree, * p_AVL_tree;


void visit_node(p_AVL_tree_node pnode) {
	assert(pnode);
	printf("%c", pnode->key);
}
//递归中序遍历
void mid_order(p_AVL_tree_node pnode) {
	if (!pnode) {
		return;
	}
	mid_order(pnode->left_child);
	visit_node(pnode);
	mid_order(pnode->right_child);
}

//递归先序遍历
void pre_order(p_AVL_tree_node pnode) {
	if (!pnode) {
		return;
	}
	visit_node(pnode);
	pre_order(pnode->left_child);
	pre_order(pnode->right_child);
}

//递归后序遍历
void last_order(p_AVL_tree_node pnode) {
	if (!pnode) {
		return;
	}
	last_order(pnode->left_child);
	last_order(pnode->right_child);
	visit_node(pnode);
}

//创建元素key的结点
p_AVL_tree_node AVL_tree_node_create(int key, int value) {
	p_AVL_tree_node pnode = (p_AVL_tree_node)malloc(sizeof(AVL_tree_node));
	pnode->key = key;
	pnode->value = value;
	pnode->left_child = NULL;
	pnode->right_child = NULL;
	pnode->heght = 0;
	return pnode;
}

//释放结点元素
bool AVL_tree_node_delete(p_AVL_tree_node pnode) {
	assert(pnode);
	free(pnode);
	return true;
}

//RR平衡操作（单左旋）
p_AVL_tree_node RR_balance(p_AVL_tree_node pnode) {

	p_AVL_tree_node R;
	R = pnode->right_child;
	pnode->right_child = R->left_child;
	R->left_child = pnode;

	pnode->heght = MAX(HEGHT(pnode->left_child), HEGHT(pnode->right_child)) + 1;
	R->heght = MAX(HEGHT(R->left_child), HEGHT(R->right_child)) + 1;

	return R;
}

//LL平衡操作(单右旋操作)
p_AVL_tree_node LL_balance(p_AVL_tree_node pnode) {
	p_AVL_tree_node L;

	L = pnode->left_child;
	pnode->left_child = L->right_child;
	L->right_child = pnode;

	pnode->heght = MAX(HEGHT(pnode->left_child), HEGHT(pnode->right_child)) + 1;
	L->heght = MAX(HEGHT(L->left_child), HEGHT(L->right_child)) + 1;
	return L;
}

//LR平衡操作（先对pnode的左子树进行左旋操作，再对pnode自身进行右旋操作）
p_AVL_tree_node LR_balance(p_AVL_tree_node pnode) {
	pnode->left_child = RR_balance(pnode->left_child);
	pnode = LL_balance(pnode);
	return pnode;
}

//RL平衡操作（先对pnode的右子树进行右旋操作，再对pnode自身进行左旋操作）
p_AVL_tree_node RL_balance(p_AVL_tree_node pnode) {
	pnode->right_child = LL_balance(pnode->right_child);
	pnode = RR_balance(pnode);
	return pnode;
}


//平衡入口
p_AVL_tree_node AVL_tree_balance(p_AVL_tree_node pnode) {
	if (!pnode) {
		return pnode;
	}
	//无需平衡
	if (ABS(HEGHT((pnode)->left_child) - HEGHT((pnode)->right_child)) < 2) {
		return pnode;
	}

	//进行平衡
	if (HEGHT((pnode)->left_child) > HEGHT((pnode)->right_child)) {

		if (HEGHT((pnode)->left_child->left_child) > HEGHT((pnode)->right_child->right_child)) {
			return LL_balance(pnode);
		}
		else
		{
			return LR_balance(pnode);
		}
	}
	else
	{
		if (HEGHT((pnode)->right_child->right_child) > HEGHT((pnode)->right_child->left_child)) {
			return RR_balance(pnode);
		}
		else
		{
			return RL_balance(pnode);
		}
	}
}



//插入结点
bool AVL_tree_insert(p_AVL_tree_node* ppnode, int key, int value) {

	if (*ppnode == NULL) {
		*ppnode = AVL_tree_node_create(key, value);
	}
	else if (key < (*ppnode)->key) {
		//key小于当前结点
		bool b = AVL_tree_insert(&(*ppnode)->left_child, key, value);
		if (!b) {
			return false;
		}
		//进行平衡
		(*ppnode)->left_child = AVL_tree_balance((*ppnode)->left_child);
	}
	else if (key > (*ppnode)->key)
	{
		//key大于当前结点
		bool b = AVL_tree_insert(&(*ppnode)->right_child, key, value);
		if (!b) {
			return false;
		}
		//进行平衡
		(*ppnode)->right_child = AVL_tree_balance((*ppnode)->right_child);
	}
	else {
		//相等
		return false;
	}
	(*ppnode)->heght = MAX(HEGHT((*ppnode)->left_child), HEGHT((*ppnode)->right_child)) + 1;
	return true;
}

//查找元素key
p_AVL_tree_node AVL_tree_search(p_AVL_tree_node pnode, int key) {
	if (!pnode) {
		return NULL;
	}
	//找到
	if (pnode->key == key) {
		return pnode;
	}
	//小于当前结点，递归到左子树取
	if (key < pnode->key) {
		return AVL_tree_search(pnode->left_child, key);
	}
	else
	{
		//大于当前结点，递归的右子树取
		return AVL_tree_search(pnode->right_child, key);
	}
}




//删除结点pdel
p_AVL_tree_node AVL_tree_delete_inner(p_AVL_tree_node pnode, p_AVL_tree_node pdel) {
	//待删除结点等于当前结点，进行删除
	if ((pnode)->key == pdel->key) {

		if ((pnode)->left_child && (pnode)->right_child) {

			if (HEGHT((pnode)->left_child) > HEGHT((pnode)->right_child)) {

				p_AVL_tree_node lmax = (pnode)->left_child;
				while (lmax->right_child)
				{
					lmax = lmax->right_child;
				}
				(pnode)->key = lmax->key;
				(pnode)->value = lmax->value;
				pnode->left_child = AVL_tree_delete_inner(pnode->left_child, lmax);
			}
			else
			{
				p_AVL_tree_node rmin = pnode->right_child;
				while (rmin->left_child)
				{
					rmin = rmin->left_child;
				}
				pnode->key = rmin->key;
				(pnode)->value = rmin->value;
				pnode->right_child = AVL_tree_delete_inner(pnode->right_child, rmin);
			}
		}
		else
		{
			p_AVL_tree_node del = pnode;
			pnode = pnode->left_child ? pnode->left_child : pnode->right_child;
			AVL_tree_node_delete(del);
		}
	}
	else if (pdel->key < (pnode)->key)
	{
		//待删除结点小于当前结点，到左子树进行删除
		pnode->left_child = AVL_tree_delete_inner(pnode->left_child, pdel);
		//当前结点左子树有变动，进行平衡
		pnode = AVL_tree_balance(pnode);

	}
	else
	{
		//待删除元素大于当前结点，到右子树进行删除，
		pnode->right_child = AVL_tree_delete_inner(pnode->right_child, pdel);
		//当前结点右子树有变更，进行平衡
		pnode = AVL_tree_balance(pnode);
	}
	if (pnode) {
		(pnode)->heght = MAX(HEGHT((pnode)->left_child), HEGHT((pnode)->right_child)) + 1;
	}
	return pnode;
}

//删除元素key
bool AVL_tree_delete(p_AVL_tree_node* ppnode, int key) {
	p_AVL_tree_node del;
	del = AVL_tree_search(*ppnode, key);
	if (del) {
		*ppnode = AVL_tree_delete_inner(*ppnode, del);
		return true;
	}
	return false;
}

//例子树
/*
		  D
		/    \
	  B       E
	/   \        \
 A      C        F

*/
p_AVL_tree AVL_tree_example() {

	p_AVL_tree pavl = (p_AVL_tree)malloc(sizeof(AVL_tree));
	pavl->nodenum = 6;
	pavl->root = NULL;
	int a[] = { 'D','B','A','C','E','F' };
	for (int i = 0; i < 6; i++) {
		AVL_tree_insert(&(pavl->root), a[i], 0);
	}
	return pavl;
}


//打印树结构
void AVL_tree_print(p_AVL_tree_node  pnode, p_AVL_tree_node pfarther, int flag) {

	if (pnode) {
		if (pfarther == NULL) {
			printf("\n%c(heght %d) is root\n", pnode->key, pnode->heght);
		}
		else
		{
			printf("%c(heght %d) is %c %s child\n", pnode->key, pnode->heght, pfarther->key, (flag == 1 ? "left" : "right"));
		}
		AVL_tree_print(pnode->left_child, pnode, 1);
		AVL_tree_print(pnode->right_child, pnode, 2);
	}
}

int main()
{
	p_AVL_tree pavl = AVL_tree_example();
	printf("\n-----------------------------------------------------");
	printf("\n先序递归遍历：");
	pre_order(pavl->root);
	printf("\n中序递归遍历：");
	mid_order(pavl->root);
	printf("\n后序递归遍历：");
	last_order(pavl->root);
	AVL_tree_print(pavl->root, NULL, 0);

	printf("\n-----------------------------------------------------");
	printf("\n添加一个元素O后：");
	AVL_tree_insert(&(pavl->root), 'O', 0);
	printf("\n先序递归遍历：");
	pre_order(pavl->root);
	printf("\n中序递归遍历：");
	mid_order(pavl->root);
	printf("\n后序递归遍历：");
	last_order(pavl->root);
	AVL_tree_print(pavl->root, NULL, 0);

	printf("\n-----------------------------------------------------");
	printf("\n再添加一个元素L后：");
	AVL_tree_insert(&(pavl->root), 'L', 0);
	printf("\n先序递归遍历：");
	pre_order(pavl->root);
	printf("\n中序递归遍历：");
	mid_order(pavl->root);
	printf("\n后序递归遍历：");
	last_order(pavl->root);
	AVL_tree_print(pavl->root, NULL, 0);

	printf("\n-----------------------------------------------------");
	printf("\n删除元素D后：");
	bool ret = AVL_tree_delete(&pavl->root, 'D');
	if (ret) {
		printf("\n删除成功");
	}
	else
	{
		printf("\n删除失败");
	}
	printf("\n先序递归遍历：");
	pre_order(pavl->root);
	printf("\n中序递归遍历：");
	mid_order(pavl->root);
	printf("\n后序递归遍历：");
	last_order(pavl->root);
	AVL_tree_print(pavl->root, NULL, 0);

	printf("Hello World!\n");
}


