// tree-huffman.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include"minheap.h"


typedef struct _huffman_node {
	int key;
	struct _huffman_node* left_child;
	struct _huffman_node* right_child;
	struct _huffmna_node* parent;
}huffman_node, * p_huffman_node;

typedef struct _huffman_tree {
	int nodenum;
	p_huffman_node* proot;
}huffman_tree, * p_huffman_tree;


void visit_node(p_huffman_node pnode) {
	assert(pnode);
	printf("%d ", pnode->key);
}

//递归中序遍历
void mid_order(p_huffman_node pnode) {
	if (!pnode) {
		return;
	}
	mid_order(pnode->left_child);
	visit_node(pnode);
	mid_order(pnode->right_child);
}

//递归先序遍历
void pre_order(p_huffman_node pnode) {
	if (!pnode) {
		return;
	}
	visit_node(pnode);
	pre_order(pnode->left_child);
	pre_order(pnode->right_child);
}

//递归后序遍历
void last_order(p_huffman_node pnode) {
	if (!pnode) {
		return;
	}
	last_order(pnode->left_child);
	last_order(pnode->right_child);
	visit_node(pnode);
}





//创建huffman结点
p_huffman_node huffman_tree_node_create(int key, p_huffman_node left, p_huffman_node right) {
	p_huffman_node pnode = (p_huffman_node)malloc(sizeof(huffman_node));
	pnode->left_child = left;
	pnode->right_child = right;
	pnode->key = key;
	pnode->parent = NULL;
	return pnode;
}

//释放huffman结点占用的内存空间
void huffman_tree_node_delete(p_huffman_node pnode) {
	if (pnode)
	{
		free(pnode);
	}
	return true;
}


//销毁huffman树
void huffman_tree_destroy(p_huffman_node pnode) {
	if (pnode == NULL) {
		return;
	}
	huffman_tree_destroy(pnode->left_child);
	huffman_tree_destroy(pnode->right_child);
	huffman_tree_node_delete(pnode);
}

//创建huffman树
p_huffman_tree huffman_tree_create(int a[], int size) {
	p_huffman_tree ptree;
	ptree = (p_huffman_tree)malloc(sizeof(huffman_tree));
	ptree->nodenum = 2 * size - 1;
	ptree->proot = NULL;
	min_heap myheap;
	min_heap_init(&myheap, size * 2);
	for (int i = 0; i < size; i++)
	{
		p_huffman_node pnode = huffman_tree_node_create(a[i], NULL, NULL);
		min_heap_node tmp;
		tmp.key = a[i];
		tmp.value = pnode;
		min_heap_insert(&myheap, tmp);
	}
	printf("初始小顶堆为：");
	min_heap_print(&myheap);

	while (true)
	{
		int size = min_heap_size(&myheap);
		if (size == 1) {
			min_heap_node root = min_heap_gettop(&myheap);
			ptree->proot = root.value;
			break;
		}
		min_heap_node min1 = min_heap_gettop(&myheap);
		min_heap_node min2 = min_heap_gettop(&myheap);
		p_huffman_node pnode1 = (p_huffman_node)min1.value;
		p_huffman_node pnode2 = (p_huffman_node)min2.value;
		int key = pnode1->key + pnode2->key;
		p_huffman_node pparent = huffman_tree_node_create(key, pnode1, pnode2);
		pnode1->parent = pparent;
		pnode2->parent = pparent;
		min_heap_node tmp;
		tmp.key = key;
		tmp.value = pparent;
		min_heap_insert(&myheap, tmp);
	}
	printf("\n当前堆大小为：%d", min_heap_size(&myheap));
	min_heap_uninit(&myheap);
	return ptree;

}

//打印树结构
void huffman_tree_print(p_huffman_node  pnode, p_huffman_node pfarther, int flag) {

	if (pnode) {
		if (pfarther == NULL) {
			printf("\n%d is root\n", pnode->key);
		}
		else
		{
			printf("%d is %d %s child\n", pnode->key, pfarther->key, (flag == 1 ? "left" : "right"));
		}
		huffman_tree_print(pnode->left_child, pnode, 1);
		huffman_tree_print(pnode->right_child, pnode, 2);
	}
}


int main()
{
	int a[] = { 1,4,2,11,5,7 };
	p_huffman_tree ptree = huffman_tree_create(a, 6);
	printf("\n-----------------------------------------------------\n");
	printf("先序递归遍历：");
	pre_order(ptree->proot);
	printf("\n中序递归遍历：");
	mid_order(ptree->proot);
	printf("\n后序递归遍历：");
	last_order(ptree->proot);
	huffman_tree_print(ptree->proot, NULL, 0);
	huffman_tree_destroy(ptree->proot);

	printf("Hello World!\n");
}


