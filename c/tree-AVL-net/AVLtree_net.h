#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

#include <stdbool.h>
//#include <pthread.h>

typedef struct _avl_tree avl_tree_t;


struct _avl_tree
{
	avl_tree_t* _this;
	void* _private_;    // 私有成员

	int (*pf_hash)(void*); //用户提供的 hash 函数
	int (*pf_free_element)(void* ele); // 用户提供的节点元素中保存的动态内存的释放方法

	/*
	@func:
		增加节点

	@para:
		tree : 树指针
		node : 要增加的节点

	@return:
		int : -1 树指针为空， -2 创建节点失败， -3 重复插入

	@note:
		None.
	*/
	int (*add)(avl_tree_t* tree, void* node);

	/*
	@func:
		通过键值删除节点

	@para:
		tree : 树指针
		key : 节点元素对应的键值

	@return:
		int ： 0 成功  -1 失败

	@note:
		None.
	*/
	int (*del_node_by_key)(avl_tree_t* tree, int key);

	/*
	@func:
		通过元素删除节点

	@para:
		tree : 树指针
		ele : 节点的元素

	@return:
		int ： 0 成功  -1 失败

	@note:
		None.
	*/
	int (*del_node_by_element)(avl_tree_t* tree, void* element);

	/*
	@func:
		通过键值查找节点

	@para:
		tree : 树指针
		key : 节点元素对应的键值

	@return:
		avl_node_t* ： 查找到的节点

	@note:
		None.
	*/
	void* (*query_by_key)(avl_tree_t* tree, int key);

	/*
	@func:
		前序遍历

	@para:
		tree ： 树指针
		visit : 遍历时对每个元素执行的操作

	@return:
		None
	*/
	void (*preorder)(avl_tree_t* tree, void(*visit)(void* ele));

	/*
	@func:
		获取树节点的数量

	@para:
		tree : 树指针

	@return:
		int : 树节点的数量

	@note:
		None.
	*/
	int (*size)(avl_tree_t* tree);

	/*
	@func:
		清除树的全部节点

	@para:
		tree : 树指针

	@return:
		None.

	@note:
		None.
	*/
	void (*clear_node)(avl_tree_t* tree);

	/*
	@func:
		销毁树

	@para:
		tree : 树指针

	@return:
		None.

	@note:
		None.
	*/
	void (*destory)(avl_tree_t** tree);
};


/*
@func:
	创建一颗平衡二叉树

@para:
	element_size : 节点保存元素的大小，单位字节
	pf_hash_func ； 从节点元素获得键值 key 的方法，由用户提供
	pf_free_element_func ： 若节点元素不包含额外的动态内存， 此参数可传 NULL；若节点包含的元素中还包含额外的动态内存，用户需传入此函数以正确释放内存
	thread_safe ： 是否启用线程安全，若用户保证不会涉及冲突，可关闭线程安全功能

@return:
	avl_tree_t* : 创建平衡二叉树的指针
*/
extern avl_tree_t* avl_tree_create(int element_size, int (*pf_hash_func)(void*), int (*pf_free_element_func)(void*), bool thread_safe);


#endif /* end #ifndef _AVL_TREE_H_ */

