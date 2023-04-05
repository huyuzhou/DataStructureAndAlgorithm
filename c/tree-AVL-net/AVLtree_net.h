#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

#include <stdbool.h>
//#include <pthread.h>

typedef struct _avl_tree avl_tree_t;


struct _avl_tree
{
	avl_tree_t* _this;
	void* _private_;    // ˽�г�Ա

	int (*pf_hash)(void*); //�û��ṩ�� hash ����
	int (*pf_free_element)(void* ele); // �û��ṩ�Ľڵ�Ԫ���б���Ķ�̬�ڴ���ͷŷ���

	/*
	@func:
		���ӽڵ�

	@para:
		tree : ��ָ��
		node : Ҫ���ӵĽڵ�

	@return:
		int : -1 ��ָ��Ϊ�գ� -2 �����ڵ�ʧ�ܣ� -3 �ظ�����

	@note:
		None.
	*/
	int (*add)(avl_tree_t* tree, void* node);

	/*
	@func:
		ͨ����ֵɾ���ڵ�

	@para:
		tree : ��ָ��
		key : �ڵ�Ԫ�ض�Ӧ�ļ�ֵ

	@return:
		int �� 0 �ɹ�  -1 ʧ��

	@note:
		None.
	*/
	int (*del_node_by_key)(avl_tree_t* tree, int key);

	/*
	@func:
		ͨ��Ԫ��ɾ���ڵ�

	@para:
		tree : ��ָ��
		ele : �ڵ��Ԫ��

	@return:
		int �� 0 �ɹ�  -1 ʧ��

	@note:
		None.
	*/
	int (*del_node_by_element)(avl_tree_t* tree, void* element);

	/*
	@func:
		ͨ����ֵ���ҽڵ�

	@para:
		tree : ��ָ��
		key : �ڵ�Ԫ�ض�Ӧ�ļ�ֵ

	@return:
		avl_node_t* �� ���ҵ��Ľڵ�

	@note:
		None.
	*/
	void* (*query_by_key)(avl_tree_t* tree, int key);

	/*
	@func:
		ǰ�����

	@para:
		tree �� ��ָ��
		visit : ����ʱ��ÿ��Ԫ��ִ�еĲ���

	@return:
		None
	*/
	void (*preorder)(avl_tree_t* tree, void(*visit)(void* ele));

	/*
	@func:
		��ȡ���ڵ������

	@para:
		tree : ��ָ��

	@return:
		int : ���ڵ������

	@note:
		None.
	*/
	int (*size)(avl_tree_t* tree);

	/*
	@func:
		�������ȫ���ڵ�

	@para:
		tree : ��ָ��

	@return:
		None.

	@note:
		None.
	*/
	void (*clear_node)(avl_tree_t* tree);

	/*
	@func:
		������

	@para:
		tree : ��ָ��

	@return:
		None.

	@note:
		None.
	*/
	void (*destory)(avl_tree_t** tree);
};


/*
@func:
	����һ��ƽ�������

@para:
	element_size : �ڵ㱣��Ԫ�صĴ�С����λ�ֽ�
	pf_hash_func �� �ӽڵ�Ԫ�ػ�ü�ֵ key �ķ��������û��ṩ
	pf_free_element_func �� ���ڵ�Ԫ�ز���������Ķ�̬�ڴ棬 �˲����ɴ� NULL�����ڵ������Ԫ���л���������Ķ�̬�ڴ棬�û��贫��˺�������ȷ�ͷ��ڴ�
	thread_safe �� �Ƿ������̰߳�ȫ�����û���֤�����漰��ͻ���ɹر��̰߳�ȫ����

@return:
	avl_tree_t* : ����ƽ���������ָ��
*/
extern avl_tree_t* avl_tree_create(int element_size, int (*pf_hash_func)(void*), int (*pf_free_element_func)(void*), bool thread_safe);


#endif /* end #ifndef _AVL_TREE_H_ */

