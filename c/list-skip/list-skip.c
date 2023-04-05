// list-skip.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<math.h>
#include<stdbool.h>
#define SKIP_LIST_MAX_LEVEL 12 //索引层数
#define SKIP_LIST_SPAN 0.5  //上一层/下一层的比值
#define SKIP_LIST_HEAD -1 //定义每个结点最大内保存的关键字


typedef struct _skiplist_node {
	int key;
	int value;
	int node_max_level;
	struct _skiplist_node** node_level_next;
}skiplist_node, * p_skiplist_node;

typedef struct _skiplist {
	int nodenum;
	int skiplist_max_level;
	p_skiplist_node head;
}skiplist, * p_skiplist;

//结点创建
p_skiplist_node skiplist_node_create(int key, int value, int level) {
	p_skiplist_node pnode = (p_skiplist_node)malloc(sizeof(skiplist_node));
	assert(pnode);
	pnode->key = key;
	pnode->value = value;
	pnode->node_max_level = level;
	pnode->node_level_next = (p_skiplist_node*)malloc(sizeof(p_skiplist_node) * level);
	memset(pnode->node_level_next, 0, sizeof(p_skiplist_node) * level);
	assert(pnode->node_level_next);
	return pnode;
}

//结点删除
void skiplist_node_delete(p_skiplist_node pnode) {
	assert(pnode);
	free(pnode->node_level_next);
	free(pnode);
	return;
}

//跳表初始化，这里在每一层都加入了一个头结点，方便操作
void skiplist_init(p_skiplist pskiplist, int maxlevel) {
	pskiplist->nodenum = 0;
	pskiplist->skiplist_max_level = 1;
	pskiplist->head = skiplist_node_create(SKIP_LIST_HEAD, SKIP_LIST_HEAD, SKIP_LIST_MAX_LEVEL);
}

void skiplist_uninit(p_skiplist pskiplist) {
	assert(pskiplist);
	p_skiplist_node cur = pskiplist->head;
	p_skiplist_node del;
	while (cur)
	{
		del = cur;
		cur = cur->node_level_next[0];
		skiplist_node_delete(del);
	}
}

//查找
p_skiplist_node skiplist_search(p_skiplist pskiplist, int key) {
	p_skiplist_node pnode = pskiplist->head;
	p_skiplist_node cur = NULL;
	for (int i = pskiplist->skiplist_max_level - 1; i >= 0; i--)
	{
		cur = pnode->node_level_next[i];
		while (cur && cur->key < key)
		{
			pnode = cur;
			cur = cur->node_level_next[i];
		}
	}
	if (cur && cur->key == key) {
		return cur;
	}
	return NULL;
}

//随机一个插入层次
int skiplist_random_level() {
	int level = 1;
	float b = rand() / (RAND_MAX + 1.0);
	while (b < SKIP_LIST_SPAN && level < SKIP_LIST_MAX_LEVEL)
	{
		level++;
		b = rand() / (RAND_MAX + 1.0);
	}
	return level;
}

//在跳表中进行插入
bool skiplist_insert(p_skiplist pskiplist, int key, int value) {
	//找到每一层的前驱结点
	p_skiplist_node plevel_pre[SKIP_LIST_MAX_LEVEL];
	memset(plevel_pre, 0, sizeof(p_skiplist_node) * SKIP_LIST_MAX_LEVEL);

	p_skiplist_node pre = pskiplist->head;
	p_skiplist_node cur;

	for (int i = pskiplist->skiplist_max_level - 1; i >= 0; i--) {
		cur = pre->node_level_next[i];
		while (cur && cur->key < key)
		{
			pre = cur;
			cur = cur->node_level_next[i];
		}
		plevel_pre[i] = pre;
	}
	//随机一个层次
	int level = skiplist_random_level();
	p_skiplist_node pnode = skiplist_node_create(key, value, level);
	assert(pnode);
	//如果随机的层次索引大于了当前跳表的最大索引层，则把新增加的层数所在层次补足前驱结点
	if (level > pskiplist->skiplist_max_level) {
		int k = pskiplist->skiplist_max_level;
		for (int i = k; i < level; i++)
		{
			plevel_pre[i] = pskiplist->head;
		}
	}
	//进行每一层的插入操作
	for (int i = 0; i < level; i++)
	{
		pnode->node_level_next[i] = plevel_pre[i]->node_level_next[i];
		plevel_pre[i]->node_level_next[i] = pnode;
	}
	if (pnode->node_max_level > pskiplist->skiplist_max_level) {
		pskiplist->skiplist_max_level = pnode->node_max_level;
	}
	pskiplist->nodenum++;
	return true;
}

//删除
bool skiplist_delete(p_skiplist pskiplist, int key) {
	//先查找该结点是否存在，不存在直接返回
	p_skiplist_node pnode = skiplist_search(pskiplist, key);
	if (!pnode) {
		return false;
	}
	//找到每一层的前驱结点
	p_skiplist_node plevel_pre[SKIP_LIST_MAX_LEVEL];
	memset(plevel_pre, 0, sizeof(p_skiplist_node) * SKIP_LIST_MAX_LEVEL);
	p_skiplist_node pre = pskiplist->head;
	p_skiplist_node cur;
	for (int i = pskiplist->skiplist_max_level - 1; i >= 0; i--) {
		cur = pre->node_level_next[i];
		while (cur && cur->key < key)
		{
			pre = cur;
			cur = cur->node_level_next[i];
		}
		plevel_pre[i] = pre;
	}
	//每一层进行删除
	for (int i = 0; i < pnode->node_max_level; i++)
	{
		if (plevel_pre[i]->node_level_next[i] == pnode) {
			plevel_pre[i]->node_level_next[i] = pnode->node_level_next[i];
		}
	}
	skiplist_node_delete(pnode);
	pskiplist->nodenum--;
	//更新跳表的最大层次计数
	p_skiplist_node levelhead;
	for (int i = pskiplist->skiplist_max_level - 1; i >= 0; i--)
	{
		levelhead = pskiplist->head->node_level_next[i];
		if (levelhead == NULL) {
			pskiplist->skiplist_max_level--;
		}
		else
		{
			break;
		}
	}
	return true;
}

//输出跳表
void skiplist_print(p_skiplist pskiplist) {
	printf("当前跳表总结点数为：%d\n", pskiplist->nodenum);
	printf("当前跳表索引层数为：%d\n", pskiplist->skiplist_max_level);
	p_skiplist_node pre = pskiplist->head;
	p_skiplist_node cur;
	for (int i = pskiplist->skiplist_max_level - 1; i >= 0; i--)
	{
		cur = pre->node_level_next[i];
		printf("---当前第%d层\n", i);
		while (cur)
		{
			printf("key[%d]-value[%d]", cur->key, cur->value);
			if (cur->node_level_next[i]) {
				printf("---");
			}
			cur = cur->node_level_next[i];
		}
		printf("\n");
	}
}

//例子
p_skiplist skiplist_example() {
	int keys[] = { 1,7,2,4,9,3,5,6 };
	int values[] = { 11,17,12,14,19,13,15,16 };
	p_skiplist pskiplist = (p_skiplist)malloc(sizeof(skiplist));
	skiplist_init(pskiplist, SKIP_LIST_MAX_LEVEL);
	for (int i = 0; i < 8; i++) {
		skiplist_insert(pskiplist, keys[i], values[i]);
	}
	return pskiplist;
}


int main()
{
	p_skiplist pskiplist = skiplist_example();
	skiplist_print(pskiplist);

	p_skiplist_node pnode = skiplist_search(pskiplist, 2);
	if (pnode) {
		printf("查找2成功\n");
		printf("key %d : value %d\n", pnode->key, pnode->value);
	}
	else
	{
		printf("查找2失败\n");
	}

	bool b = skiplist_delete(pskiplist, 4);
	if (b) {
		printf("删除4成功\n");
	}
	else
	{
		printf("删除4失败\n");
	}
	skiplist_print(pskiplist);
	skiplist_uninit(pskiplist);
	printf("\nHello World!\n");
}


