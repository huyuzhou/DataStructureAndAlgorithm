// tree-AVL-net.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
// AVLtree_net.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AVLTree_net.h"


#define DEBUG_LOG 0

#define AVL_LOG_DEBUG(fmt, ...) \
    do{ \
        if(DEBUG_LOG) \
        {\
            printf("%s at %d " fmt "\r\n", __FILE__, __LINE__, ##__VA_ARGS__);\
        }\
    }while(0);


typedef struct _avl_node avl_node_t;
typedef struct _avl_tree_private avl_tree_private_t;

struct _avl_node
{
	avl_node_t* parent;      // 父节点
	avl_node_t* left_child;  // 左孩子
	avl_node_t* right_child; // 右孩子

	void* element;  // 节点保存的元素
	int depth;      // 当前节点的高度
	int key;        // 键值
};

struct _avl_tree_private
{
	avl_node_t* m_root;
	int m_element_size;
	int m_node_cnt;
	bool m_is_thread_safe;
	//pthread_mutex_t m_tree_mutex;
};

#define MAX(a, b) (int)((a) > (b) ? (a) : (b))

// 获取节点高度
#define HEIGHT(node) ((NULL == (node)) ? 0 : (MAX((NULL != node->left_child ? node->left_child->depth : 0), (NULL != node->right_child ? node->right_child->depth : 0) ) + 1))

#define ABS(a) ( (a) > 0 ? (a) :  (- (a)))

#define INIT_KEY (int)(-1)

/*
@func:
	获取私有成员变量

@para:
	tree ： 树指针

@return:
	avl_tree_private_t* : 私有成员变量结构体指针
*/
static avl_tree_private_t* get_private_member(avl_tree_t* tree)
{
	if (NULL == tree) return NULL;

	return (avl_tree_private_t*)tree->_private_;
}


/*
@func:
	线程锁， 上锁/解锁

@para:
	tree ： 树指针

@return:
	None

@note:
	若用户代码结构不会造成冲突可不使用
*/

static void avl_tree_lock(avl_tree_t* tree)
{
	//avl_tree_private_t* _this = get_private_member(tree);
	//if (_this->m_is_thread_safe)
		//pthread_mutex_lock(&_this->m_tree_mutex);
}

static void avl_tree_unlock(avl_tree_t* tree)
{
	//avl_tree_private_t* _this = get_private_member(tree);
	//if (_this->m_is_thread_safe)
		//pthread_mutex_unlock(&_this->m_tree_mutex);
}

/*
@func:
	添加到目标节点的 左/右 孩子

@para:
	node ： 要添加的节点
	p : 目标节点

@return:
	None
*/
static void add_to_left(avl_node_t* node, avl_node_t* p)
{
	p->left_child = node;
	node->parent = p;
	node->left_child = node->right_child = NULL;
	node->depth = 1;
}

static void add_to_right(avl_node_t* node, avl_node_t* p)
{
	p->right_child = node;
	node->parent = p;
	node->left_child = node->right_child = NULL;
	node->depth = 1;
}


/*
@func:
	LL / RR / LR / RL 型旋转

@para:
	node : 要调整的节点

@return:
	avl_node_t* ： 调整后的根节点（此根节点并非树的根节点，而是替换被调整那个节点的位置的节点）

@note:
	算法说明：左左旋转(LL)

					+---+							+---+
		 node --->  | 5 |		         temp --->  | 3	|
					+---+							+---+
					 / \							 / \
					/	\							/	\
				   /	 \						   /	 \
				+---+	+---+					+---+	+---+
	  temp ---> | 3 |	| 6 |		===>		| 2	|	| 5 | <--- node
				+---+	+---+					+---+	+---+
				 / \							 /  	 / \
				/	\							/	    /
			   /	 \						   /	   /	 \
			+---+	+---+					+---+	+---+	+---+
			| 2 |	| 4 |					| 1 |	| 4 |	| 6 |
			+---+	+---+					+---+	+---+   +---+
			 /
			/
		   /
		+---+
		| 1 |
		+---+
*/
static avl_node_t* LL(avl_node_t* node)
{
	//AVL_LOG_DEBUG("LL %d", *((char*)(node->element) + 4));
	avl_node_t* temp = node->left_child;

	node->left_child = temp->right_child;
	if (NULL != temp->right_child) node->left_child->parent = node;

	temp->parent = node->parent;

	temp->right_child = node;
	node->parent = temp;

	node->depth = HEIGHT(node);  // 顺序不能换
	temp->depth = HEIGHT(temp);

	return temp;
}

static avl_node_t* RR(avl_node_t* node)
{
	//AVL_LOG_DEBUG("RR %d", *((char*)(node->element) + 4));
	avl_node_t* temp = node->right_child;

	node->right_child = temp->left_child;
	if (NULL != temp->left_child) node->right_child->parent = node;

	temp->parent = node->parent;

	temp->left_child = node;
	node->parent = temp;

	node->depth = HEIGHT(node);
	temp->depth = HEIGHT(temp);

	return temp;
}

static avl_node_t* RL(avl_node_t* node)
{
	node->right_child = LL(node->right_child);
	return RR(node);
}

static avl_node_t* LR(avl_node_t* node)
{
	node->left_child = RR(node->left_child);
	return LL(node);
}


/*
@func:
	调整树

@para:
	node : 要调整的节点

@return:
	avl_node_t* ： 调整后的根节点（此根节点并非树的根节点，而是替换被调整那个节点的位置的节点）

@note:
	None.
*/
static avl_node_t* avl_tree_adjust(avl_node_t* node)
{
	if (ABS(HEIGHT(node->left_child) - HEIGHT(node->right_child)) < 2)
		return node;

	if (HEIGHT(node->left_child) > HEIGHT(node->right_child))
	{
		if (HEIGHT(node->left_child->left_child) > HEIGHT(node->left_child->right_child))
		{
			return LL(node);
		}
		else
		{
			return LR(node);
		}
	}
	else
	{
		if (HEIGHT(node->right_child->left_child) > HEIGHT(node->right_child->right_child))
		{
			return RL(node);
		}
		else
		{
			return RR(node);
		}
	}
}

/*
@func:
	释放节点内存

@para:
	tree : 树指针
	node : 要释放内存的节点

@return:
	int ： 0 成功， -1 失败

@note:
	None.
*/
static int avl_tree_free_node(avl_tree_t* tree, avl_node_t* node)
{
	if (NULL == node || NULL == tree) return -1;

	if (NULL != tree->pf_free_element)
		tree->pf_free_element(node->element);

	if (NULL != node->element)
	{
		free(node->element);
		node->element = NULL;
	}

	free(node);
	node = NULL;

	return 0;
}

/*
@func:
	创建一个节点

@para:
	tree : 树指针

@return:
	avl_node_t* ： 创建的节点

@note:
	None.
*/
static avl_node_t* avl_tree_create_node(avl_tree_t* tree)
{
	if (NULL == tree) return NULL;

	avl_tree_private_t* _this = get_private_member(tree);
	avl_node_t* node = (avl_node_t*)malloc(sizeof(avl_node_t));
	if (NULL == node)
	{
		AVL_LOG_DEBUG("[ERROR]:node malloc");
		return NULL;
	}
	void* tmp = (void*)malloc(_this->m_element_size);
	node->element = tmp;

	node->key = INIT_KEY;

	return node;
}

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
static int avl_tree_add(avl_tree_t* tree, void* ele)
{
	if (tree == NULL) return -1;

	avl_node_t* node = avl_tree_create_node(tree);

	if (NULL == node) return -2;

	avl_tree_private_t* _this = get_private_member(tree);

	AVL_LOG_DEBUG("Add key[%d]", tree->pf_hash(ele));

	memcpy(node->element, ele, _this->m_element_size);

	int key = tree->pf_hash(node->element);

	if (INIT_KEY == node->key)
		node->key = key;

	avl_tree_lock(tree);

	if (NULL == _this->m_root) // 添加第一个节点
	{
		node->depth = 1;
		node->left_child = node->right_child = node->parent = NULL;
		_this->m_root = node;
		_this->m_node_cnt = 1;
		avl_tree_unlock(tree);
		return 0;
	}
	avl_node_t* p = _this->m_root;

	while (NULL != p)
	{
		if (key < p->key) // 添加到左子树
		{
			if (NULL == p->left_child)
			{
				add_to_left(node, p);
				break;
			}
			else
			{
				p = p->left_child;
			}

		}
		else if (key > p->key) // 添加到右子树
		{
			if (NULL == p->right_child)
			{
				add_to_right(node, p);
				break;
			}
			else
			{
				p = p->right_child;
			}

		}

		else
		{
			AVL_LOG_DEBUG("Element repetition");
			avl_tree_free_node(tree, node);
			avl_tree_unlock(tree);
			return -3; // 重复
		}
	}

	while (NULL != p)
	{
		p->depth = HEIGHT(p);
		if (NULL == p->parent) // 调整到根节点
		{
			_this->m_root = avl_tree_adjust(p);
			break;
		}
		else
		{
			if (p == p->parent->left_child)
			{
				p = p->parent;
				p->left_child = avl_tree_adjust(p->left_child);
			}
			else
			{
				p = p->parent;
				p->right_child = avl_tree_adjust(p->right_child);
			}
		}

	}

	_this->m_node_cnt++;
	avl_tree_unlock(tree);
	return 0;
}

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
static avl_node_t* query_by_key(avl_tree_t* tree, int key)
{
	if (NULL == tree) return NULL;
	AVL_LOG_DEBUG("Query key %d", key);
	avl_tree_private_t* _this = get_private_member(tree);

	avl_node_t* p = _this->m_root;
	while (NULL != p)
	{
		if (key > p->key)
		{
			p = p->right_child;
		}
		else if (key < p->key)
		{
			p = p->left_child;
		}
		else break;
	}
	return p;
}

static void* avl_tree_query_by_key(avl_tree_t* tree, int key)
{
	avl_node_t* node = query_by_key(tree, key);

	if (NULL == node) return NULL;

	return node->element;
}

/*
@func:
	通过元素查找节点

@para:
	tree : 树指针
	ele : 要查找的元素

@return:
	avl_node_t* ： 查找到的节点

@note:
	None.
*/
static avl_node_t* avl_tree_query_by_element(avl_tree_t* tree, void* ele)
{
	if (NULL == tree || NULL == ele) return NULL;

	avl_tree_private_t* _this = get_private_member(tree);

	int key = tree->pf_hash(ele);

	avl_node_t* p = _this->m_root;
	while (NULL != p)
	{
		if (key > p->key)
		{
			p = p->right_child;
		}
		else if (key < p->key)
		{
			p = p->left_child;
		}
		else break;
	}
	return p;
}

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
static int avl_tree_del_by_key(avl_tree_t* tree, int key)
{
	if (NULL == tree) return -1;
	avl_tree_private_t* _this = get_private_member(tree);
	avl_node_t* node = query_by_key(tree, key);

	if (NULL == node) return -1;

	avl_tree_lock(tree);

	_this->m_node_cnt--;

	avl_node_t* p = node->parent;  // 先保存删除节点的父节点，方便后面调整树
	avl_node_t* temp = NULL;       // 替换 node 节点的节点


	/*
		当该节点存在左子树或者右子树的时候，比较两边的高度；
		若左子树高度大于右子树，则取 node 节点左子树中最大的那个节点来替换 node 节点
		否则，取 node 节点右子树中最小的那个节点来替换 node 节点
	*/
	if (NULL != node->left_child || NULL != node->right_child)
	{
		if (HEIGHT(node->left_child) > HEIGHT(node->right_child))
		{
			temp = node->left_child;

			while (NULL != temp->right_child) // 找到 node 左子树中最大的节点
			{
				temp = temp->right_child;
			}

			if (temp != node->left_child)
			{
				p = temp->parent;

				temp->parent->right_child = temp->left_child;
				if (NULL != temp->left_child)
					temp->left_child->parent = temp->parent;

				temp->left_child = node->left_child;
				temp->left_child->parent = temp;
			}

			temp->right_child = node->right_child;
			if (NULL != temp->right_child)
				temp->right_child->parent = temp;
		}
		else
		{
			temp = node->right_child;
			while (NULL != temp->left_child)
			{
				temp = temp->left_child;
			}

			if (temp != node->right_child)
			{
				p = temp->parent;

				temp->parent->left_child = temp->right_child;
				if (NULL != temp->right_child)
					temp->right_child->parent = temp->parent;

				temp->right_child = node->right_child;
				temp->right_child->parent = temp;
			}

			temp->parent = node->parent;

			temp->left_child = node->left_child;
			if (NULL != temp->left_child)
				temp->left_child->parent = temp;
		}

		temp->parent = node->parent;
		temp->depth = HEIGHT(temp);
	}

	if (NULL != node->parent)
	{
		if (node == node->parent->left_child)
			node->parent->left_child = temp;
		else if (node == node->parent->right_child)
			node->parent->right_child = temp;
	}
	// 如果是删除的根节点，需要更新下根节点，否则会导致根节点为NULL
	if (NULL == p)
		_this->m_root = temp;

	while (NULL != p)
	{
		p->depth = HEIGHT(p);
		if (NULL == p->parent)
		{
			// 找到根节点
			_this->m_root = avl_tree_adjust(p);
			break;
		}
		else
		{
			if (p == p->parent->left_child)
			{
				p = p->parent;
				p->left_child = avl_tree_adjust(p->left_child);
			}
			else
			{
				p = p->parent;
				p->right_child = avl_tree_adjust(p->right_child);
			}
		}

	}
	avl_tree_free_node(tree, node);
	avl_tree_unlock(tree);
	return 0;
}

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
static int avl_tree_del_by_element(avl_tree_t* tree, void* ele)
{
	avl_node_t* node = avl_tree_query_by_element(tree, ele);

	if (NULL == node) return -1;

	return avl_tree_del_by_key(tree, node->key);
}

/*
@func:
	前序遍历

@para:
	tree ： 树指针
	visit : 遍历时对每个元素执行的操作

@return:
	None
*/
static void preorder(avl_node_t* node, void(*visit)(void* e))
{
	if (NULL == node) return;

	visit(node->element);                 // 访问结点
	preorder(node->left_child, visit); // 遍历左子树
	preorder(node->right_child, visit);// 遍历右子树
}

static void avl_tree_preorder(avl_tree_t* tree, void(*visit)(void* e))
{
	avl_tree_private_t* _this = get_private_member(tree);
	preorder(_this->m_root, visit);
}

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
static int avl_tree_size(avl_tree_t* tree)
{
	avl_tree_private_t* _this = get_private_member(tree);
	return _this->m_node_cnt;
}

/*
@func:
	清除目标节点以及其全部子树包含的节点

@para:
	tree : 树指针
	node : 节点的元素

@return:
	int ： 0 成功  -1 失败

@note:
	None.
*/
static void avl_tree_node_clear(avl_tree_t* tree, avl_node_t* node)
{
	if (NULL == node) return;

	avl_tree_node_clear(tree, node->left_child);
	avl_tree_node_clear(tree, node->right_child);

	avl_tree_free_node(tree, node);
}

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
static void avl_tree_clear(avl_tree_t* tree)
{
	avl_tree_private_t* _this = get_private_member(tree);
	avl_tree_node_clear(tree, _this->m_root);
	_this->m_node_cnt = 0;
}

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
static void avl_tree_destory(avl_tree_t** tree)
{
	avl_tree_t* _this = *tree;

	if (NULL == _this) return;

	if (_this->size(_this) > 0)
	{
		_this->clear_node(_this);
	}

	if (_this->_private_)
	{
		free(_this->_private_);
		_this->_private_ = NULL;
	}

	if (_this)
	{
		free(_this);
		_this = NULL;
	}
}

/*
@func:
	创建一颗平衡二叉树

@para:
	element_size : 节点保存元素的大小，单位字节
	pf_hash_func ； 从节点元素获得键值 key 的方法，由用户提供
	pf_free_element_func ： 若节点元素不包含额外的动态内存， 此参数可传 NULL；
							若节点包含的元素中还包含额外的动态内存，用户需传入此函数以正确释放内存
	thread_safe ： 是否启用线程安全，若用户保证不会涉及冲突，可关闭线程安全功能

@return:
	avl_tree_t* : 创建平衡二叉树的指针
*/
avl_tree_t* avl_tree_create(int element_size, int (*pf_hash_func)(void*), int (*pf_free_element_func)(void*), bool thread_safe)
{

	if (NULL == pf_hash_func)
		return NULL;
	avl_tree_t* tree = (avl_tree_t*)malloc(sizeof(avl_tree_t));
	memset(tree, 0, sizeof(avl_tree_t));

	avl_tree_private_t* private_member = (avl_tree_private_t*)malloc(sizeof(avl_tree_private_t));

	private_member->m_root = NULL;
	private_member->m_element_size = element_size;
	private_member->m_is_thread_safe = thread_safe;

	//pthread_mutex_init(&private_member->m_tree_mutex, NULL);

	tree->_this = tree;
	tree->_private_ = (void*)private_member;

	tree->pf_hash = pf_hash_func;
	tree->pf_free_element = pf_free_element_func;
	tree->add = avl_tree_add;
	tree->query_by_key = avl_tree_query_by_key;
	tree->preorder = avl_tree_preorder;
	tree->size = avl_tree_size;
	tree->del_node_by_key = avl_tree_del_by_key;
	tree->del_node_by_element = avl_tree_del_by_element;
	tree->clear_node = avl_tree_clear;
	tree->destory = avl_tree_destory;

	return tree;
}

