// stack-array.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//栈的顺序存储及操作
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


typedef struct _array_stack {
	int* stackdata;
	int top;
	int size;
}array_stack, * p_array_stack;

//初始化内存
void array_stack_init(p_array_stack arraystack, int memsize) {
	assert(arraystack);
	arraystack->stackdata = (int*)malloc(sizeof(int) * memsize);
	arraystack->size = memsize;
	arraystack->top = -1;
}

//释放内存
void array_stack_uninit(p_array_stack arraystack) {
	assert(arraystack);
	assert(arraystack->stackdata);
	free(arraystack->stackdata);
}

//清空栈
bool array_stack_clear(p_array_stack arraystack) {
	arraystack->top = -1;
	return true;
}

//栈判空
bool array_stack_is_empty(p_array_stack arraystack) {
	return arraystack->top < 0 ? true : false;
}

//栈判满
bool array_stack_is_full(p_array_stack arraystack) {
	return arraystack->top == arraystack->size - 1 ? true : false;
}

//出栈
int array_stack_pop(p_array_stack arraystack) {
	if (array_stack_is_empty(arraystack))
	{
		printf("栈为空\n");
		//栈为空
		return -1;
	}
	int ret = arraystack->stackdata[arraystack->top];
	arraystack->top--;
	return ret;
}

//入栈
bool array_stack_push(p_array_stack arraystack, int key) {
	if (array_stack_is_full(arraystack)) {
		//栈满
		printf("栈已满\n");
		return false;
	}

	arraystack->top++;
	arraystack->stackdata[arraystack->top] = key;
	return true;
}

//栈长度
int array_stack_lenth(p_array_stack arraystack) {
	return arraystack->top + 1;
}

void array_stack_display(p_array_stack arraystack) {
	for (int i = 0; i <= arraystack->top; i++)
	{
		printf("%d", arraystack->stackdata[i]);
		printf(" ");
	}
	printf("\n");
}



int main()
{
	array_stack arraystack;
	array_stack_init(&arraystack, 3);
	array_stack_push(&arraystack, 1);
	array_stack_push(&arraystack, 2);
	array_stack_push(&arraystack, 3);
	array_stack_push(&arraystack, 4);
	array_stack_push(&arraystack, 5);
	array_stack_push(&arraystack, 11);
	array_stack_push(&arraystack, 12);
	array_stack_push(&arraystack, 13);
	printf("初始栈内容为：");
	array_stack_display(&arraystack);

	printf("弹出栈顶后为：");
	array_stack_pop(&arraystack);
	array_stack_display(&arraystack);

	printf("栈内数量：");
	int lenth = array_stack_lenth(&arraystack);
	printf("%d\n", lenth);

	array_stack_uninit(&arraystack);


	printf("Hello World!\n");
}



