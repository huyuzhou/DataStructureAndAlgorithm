// heap-max.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
typedef struct _max_heap_node {
	int key;
}max_heap_node, * p_max_heap_node;

typedef struct _max_heap {
	int maxsize;
	int size;
	p_max_heap_node mem;
}max_heap, * p_max_heap;


//初始化大顶堆，分配内存
void max_heap_init(p_max_heap pheap, int maxsize) {
	pheap->maxsize = maxsize;
	pheap->size = 0;
	pheap->mem = (p_max_heap_node)malloc(sizeof(max_heap_node) * maxsize);
	return;
}

//释放内存
void max_heap_uninit(p_max_heap pheap) {
	assert(pheap);
	assert(pheap->mem);
	free(pheap->mem);
}

//交换堆中两个数据结点
bool max_heap_swap(p_max_heap_node a, p_max_heap_node b) {
	max_heap_node tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
	return true;
}

//index索引位置的结点向0方向进行移动，以重新调整为大顶堆
void max_heap_up(p_max_heap pheap, int index) {

	int parentindex = (index - 1) / 2;
	while (index >= 0)
	{
		if (pheap->mem[parentindex].key >= pheap->mem[index].key) {
			break;
		}
		max_heap_swap(&pheap->mem[parentindex], &pheap->mem[index]);
		index = parentindex;
		parentindex = (index - 1) / 2;
	}
}

//向大顶堆中插入一个元素，
bool max_heap_insert(p_max_heap pheap, int key) {
	if (pheap->size >= pheap->maxsize) {
		return false;
	}

	pheap->mem[pheap->size].key = key;
	max_heap_up(pheap, pheap->size);
	pheap->size++;
}

//根据元素key获取其在堆中的索引位置
int max_heap_get_index(p_max_heap pheap, int key) {
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

//调整index索引位置的结点向size方向进行移动，以重新调整为大顶堆
void max_heap_down(p_max_heap pheap, int index) {
	int leftindex, rightindex;
	leftindex = index * 2 + 1;
	rightindex = leftindex + 1;
	while (leftindex < pheap->size)
	{
		int maxindex = leftindex;
		if (rightindex < pheap->size && pheap->mem[leftindex].key < pheap->mem[rightindex].key) {
			maxindex = rightindex;
		}
		if (pheap->mem[index].key >= pheap->mem[maxindex].key) {
			break;
		}
		max_heap_swap(&pheap->mem[index], &pheap->mem[maxindex]);
		index = maxindex;
		leftindex = index * 2 + 1;
		rightindex = leftindex + 1;
	}
}

//删除堆中元素key
bool max_heap_delete(p_max_heap pheap, int key) {
	int index = max_heap_get_index(pheap, key);
	if (index < 0) {
		return false;
	}
	max_heap_swap(&pheap->mem[index], &pheap->mem[pheap->size - 1]);
	pheap->size--;
	max_heap_down(pheap, index);
	return true;
}


void max_heap_print(p_max_heap pheap) {
	for (int i = 0; i < pheap->size; i++)
	{
		printf("%d(index %d) ", pheap->mem[i].key, i);
	}
	printf("\n");
}

p_max_heap max_heap_example() {
	int a[] = { 1,4,2,11,5,7 };
	p_max_heap myheap = (p_max_heap)malloc(sizeof(max_heap));
	max_heap_init(myheap, 10);
	for (int i = 0; i < 6; i++)
	{
		max_heap_insert(myheap, a[i]);
	}
	return myheap;
}

int main()
{
	int a[] = { 1,4,2,11,5,7 };
	p_max_heap pheap = max_heap_example();
	max_heap_print(pheap);
	max_heap_insert(pheap, 20);
	max_heap_print(pheap);
	max_heap_delete(pheap, 20);
	max_heap_print(pheap);
	max_heap_delete(pheap, 5);
	max_heap_print(pheap);
	printf("Hello World!\n");
}



