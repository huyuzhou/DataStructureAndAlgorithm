// heap-min.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//



#include <stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
typedef struct _min_heap_node {
	int key;
}min_heap_node, * p_min_heap_node;

typedef struct _min_heap {
	int minsize;
	int size;
	p_min_heap_node mem;
}min_heap, * p_min_heap;


//初始化小顶堆，分配内存
void min_heap_init(p_min_heap pheap, int minsize) {
	pheap->minsize = minsize;
	pheap->size = 0;
	pheap->mem = (p_min_heap_node)malloc(sizeof(min_heap_node) * minsize);
	return;
}

//释放内存
void min_heap_uninit(p_min_heap pheap) {
	assert(pheap);
	assert(pheap->mem);
	free(pheap->mem);
}

//交换堆中两个数据结点
bool min_heap_swap(p_min_heap_node a, p_min_heap_node b) {
	min_heap_node tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
	return true;
}

//index索引位置的结点向0方向进行移动，以重新调整为小顶堆
void min_heap_up(p_min_heap pheap, int index) {

	int parentindex = (index - 1) / 2;
	while (index >= 0)
	{
		if (pheap->mem[parentindex].key <= pheap->mem[index].key) {
			break;
		}
		min_heap_swap(&pheap->mem[parentindex], &pheap->mem[index]);
		index = parentindex;
		parentindex = (index - 1) / 2;
	}
}

//向小顶堆中插入一个元素，
bool min_heap_insert(p_min_heap pheap, int key) {
	if (pheap->size >= pheap->minsize) {
		return false;
	}

	pheap->mem[pheap->size].key = key;
	min_heap_up(pheap, pheap->size);
	pheap->size++;
}

//根据元素key获取其在堆中的索引位置
int min_heap_get_index(p_min_heap pheap, int key) {
	int i;
	for (i = 0; i < pheap->size; i++)
	{
		if (pheap->mem[i].key == key) {
			break;
		}
	}
	if (i == pheap->size) {
		return -1;
	}
	return i;
}

//调整index索引位置的结点向size方向进行移动，以重新调整为小顶堆
void min_heap_down(p_min_heap pheap, int index) {
	int leftindex, rightindex;
	leftindex = index * 2 + 1;
	rightindex = leftindex + 1;
	while (leftindex < pheap->size)
	{
		int minindex = leftindex;
		if (rightindex < pheap->size && pheap->mem[leftindex].key > pheap->mem[rightindex].key) {
			minindex = rightindex;
		}
		if (pheap->mem[index].key <= pheap->mem[minindex].key) {
			break;
		}
		min_heap_swap(&pheap->mem[index], &pheap->mem[minindex]);
		index = minindex;
		leftindex = index * 2 + 1;
		rightindex = leftindex + 1;
	}
}

//删除堆中元素key
bool min_heap_delete(p_min_heap pheap, int key) {
	int index = min_heap_get_index(pheap, key);
	if (index < 0) {
		return false;
	}
	min_heap_swap(&pheap->mem[index], &pheap->mem[pheap->size - 1]);
	pheap->size--;
	min_heap_down(pheap, index);
	return true;
}


void min_heap_print(p_min_heap pheap) {
	for (int i = 0; i < pheap->size; i++)
	{
		printf("%d(index %d) ", pheap->mem[i].key, i);
	}
	printf("\n");
}

p_min_heap min_heap_example() {
	int a[] = { 1,4,2,11,5,7 };
	p_min_heap myheap = (p_min_heap)malloc(sizeof(min_heap));
	min_heap_init(myheap, 10);
	for (int i = 0; i < 6; i++)
	{
		min_heap_insert(myheap, a[i]);
	}
	return myheap;
}

int main()
{
	int a[] = { 1,4,2,11,5,7 };
	p_min_heap pheap = min_heap_example();
	min_heap_print(pheap);
	min_heap_insert(pheap, 20);
	min_heap_print(pheap);
	min_heap_delete(pheap, 20);
	min_heap_print(pheap);
	min_heap_delete(pheap, 5);
	min_heap_print(pheap);
	printf("Hello World!\n");
}




