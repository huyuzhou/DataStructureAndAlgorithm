#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <unistd.h>
#include "AVLtree_net.h"
#include <windows.h>

#define AVL_TEST_DEBUG(fmt, ...) \
    do{ \
        if(1) \
        {\
            printf("%s at %d " fmt "\r\n", __FILE__, __LINE__, ##__VA_ARGS__);\
        }\
    }while(0);


typedef struct
{
	void* record_data;
	int key;
}record_data_t;

static int get_element_key(void* data)
{
	// 节点元素的释放在 avl 里执行
	if (NULL == data) return -1;
	return ((record_data_t*)data)->key;
}

static int free_element_data(void* data)
{
	if (NULL == data)
	{
		AVL_TEST_DEBUG("free element error data null");
		return -1;
	}
	record_data_t* ele = (record_data_t*)data;

	if (NULL != ele->record_data)
	{
		//AVL_TEST_DEBUG("free [%d]%s", ele->key, (char *)ele->record_data);
		free(ele->record_data);
		ele->record_data = NULL;
	}
	return 0;
}

void print_data(void* data)
{
	if (NULL == data)
	{
		AVL_TEST_DEBUG("data null");
		return;
	}
	record_data_t* record = (record_data_t*)data;
	int out;
	sscanf((char*)record->record_data, "node%d", &out);
	AVL_TEST_DEBUG("[%c]----------------->[%c]\n", record->key, *(char*)record->record_data);

}

static record_data_t create_record_data(char* data, int key, int malloc_size)
{
	record_data_t ele;
	memset(&ele, 0, sizeof(record_data_t));
	ele.record_data = malloc(malloc_size);
	memset(ele.record_data, 0, malloc_size);
	memcpy(ele.record_data, data, malloc_size);
	ele.key = key;

	return ele;
}

int process(void)
{
	avl_tree_t* tree = avl_tree_create(sizeof(record_data_t), get_element_key, free_element_data, 0);
	if (NULL == tree)
	{
		AVL_TEST_DEBUG("create tree error!");
		return -1;
	}

	srand(time(NULL));
	char a[] = { 'D','B','A','C','E','F' };
	for (int i = 0; i < 6; i++)
	{
		record_data_t ele = create_record_data(&a[i], a[i], sizeof(char));
		tree->add(tree->_this, &ele);
	}

	AVL_TEST_DEBUG("tree size[%d]", tree->size(tree->_this));
	tree->preorder(tree->_this, print_data);

	tree->del_node_by_key(tree->_this, 'D');
	AVL_TEST_DEBUG("tree size[%d]", tree->size(tree->_this));
	tree->preorder(tree->_this, print_data);

	tree->del_node_by_key(tree->_this, 'F');
	AVL_TEST_DEBUG("tree size[%d]", tree->size(tree->_this));
	tree->preorder(tree->_this, print_data);

	AVL_TEST_DEBUG("clear");
	tree->clear_node(tree->_this);
	tree->destory(&(tree->_this));

	return 0;
}

int main()
{
	process();

}
