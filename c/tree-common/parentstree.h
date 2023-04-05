// commontree.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include<stdlib.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//													这里只做普通树的常见表示法
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//1、双亲表示法
typedef struct _vnode {
	int key;//关键字
	int value;
	int parentindex;//父结点下标索引
}vnode, * p_vnode;

typedef struct _parents_tree {
	p_vnode vnode_list_head;//存储数据组的头指针
	int vnum;//树中结点数
	int rootindex;//树根结点下标索引
}parents_tree, * p_parents_tree;




int main()
{
	printf("Hello World!\n");
}

