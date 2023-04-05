// queue-array.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//队列的顺序存储及操作
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


typedef struct _array_queue {
	int size;
	int lenth;
	int head;
	int tail;
	int* memory;
}array_queue, * p_array_queue;

//初始化分配内存
void array_queue_init(p_array_queue arrayqueue, int memsize) {
	assert(arrayqueue);
	arrayqueue->memory = (int*)malloc(sizeof(int) * memsize);
	arrayqueue->size = memsize;
	arrayqueue->lenth = 0;
	arrayqueue->head = 0;
	arrayqueue->tail = 0;
}

//释放内存
void array_queue_uninit(p_array_queue arrayqueue) {
	assert(arrayqueue);
	assert(arrayqueue->memory);
	free(arrayqueue->memory);
}

//队列是否满
bool array_queue_is_full(p_array_queue arrayqueue) {
	return arrayqueue->lenth == arrayqueue->size ? true : false;
}

//队列判空
bool array_queue_is_empty(p_array_queue arrayqueue) {
	return arrayqueue->lenth == 0 ? true : false;
}

//入队列
bool array_queue_in(p_array_queue arrayqueue, int key) {
	if (array_queue_is_full(arrayqueue)) {
		printf("队列已满\n");
		return false;
	}
	arrayqueue->memory[arrayqueue->tail % arrayqueue->size] = key;
	arrayqueue->tail = (arrayqueue->tail + 1) % arrayqueue->size;
	arrayqueue->lenth++;
	return true;
}

//出队列
int array_queue_out(p_array_queue arrayqueue) {
	if (array_queue_is_empty(arrayqueue)) {
		printf("队列为空\n");
		return -1;
	}
	int ret = arrayqueue->memory[arrayqueue->head % arrayqueue->size];
	arrayqueue->head = (arrayqueue->head + 1) % arrayqueue->size;
	arrayqueue->lenth--;
	return ret;
}

void array_queue_display(p_array_queue arrayqueue) {

	for (int i = arrayqueue->head; i < arrayqueue->lenth + arrayqueue->head; i++)
	{
		printf("索引%d==>%d", i % arrayqueue->size, arrayqueue->memory[i % arrayqueue->size]);
		printf(" ");
	}
	printf("\n");
}


int main()
{
	array_queue arrayqueue;
	array_queue_init(&arrayqueue, 5);
	array_queue_in(&arrayqueue, 1);
	array_queue_in(&arrayqueue, 2);
	array_queue_in(&arrayqueue, 3);
	array_queue_in(&arrayqueue, 4);
	array_queue_in(&arrayqueue, 5);
	printf("初始顺序队列内容为：");
	array_queue_display(&arrayqueue);
	printf("出队列3个元素后内容为：");
	array_queue_out(&arrayqueue);
	array_queue_out(&arrayqueue);
	array_queue_out(&arrayqueue);
	array_queue_display(&arrayqueue);
	printf("入队列2个元素后内容为：");
	array_queue_in(&arrayqueue, 11);
	array_queue_in(&arrayqueue, 12);
	array_queue_display(&arrayqueue);
	printf("再入队列2个元素后内容为：");
	array_queue_in(&arrayqueue, 21);
	array_queue_in(&arrayqueue, 22);
	array_queue_display(&arrayqueue);

	array_queue_uninit(&arrayqueue);

	printf("Hello World!\n");
}

