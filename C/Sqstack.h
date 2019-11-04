#include <stdio.h>
#include <stdlib.h>

#define MAX 100//创建栈时栈的规格

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

typedef int Status;

typedef int SElemType;//基础数据类型

typedef struct {
	SElemType *base;
	SElemType *top;
	int stacksize;
}SqStack;

Status InitStack(SqStack &S){
	S.base = (SElemType *)malloc(MAX * sizeof(SElemType));
	if(!S.base){
		printf("error0\n");
		return ERROR;
	}
	S.top = S.base;
	S.stacksize = MAX;
	return OK;
}

Status DestoryStack(SqStack &S){
	if(!S.base)	
		return ERROR;
	free(S.base);
	S.base = S.top = NULL;
	S.stacksize = 0;
	return OK; 
}

Status ClearStack(SqStack &S){
	if(!S.base){
		printf("error1\n");
		return ERROR;
	}
	S.top = S.base;
	return OK;
}

Status StackEmpty(SqStack S){
	if(!S.base)	return ERROR;
	else{
		if(S.top == S.base)	return TRUE;
		else				return FALSE;
	}
}

int StackLength(SqStack S){
	int length;
	length = S.top - S.base;
	return length;
}

Status GetTop(SqStack S, SElemType &e){
	if(!StackEmpty(S)){
		SElemType *p;
		p = S.top - 1;
		e = *p;
		return OK;
	}
	else return ERROR;	
}

Status Push(SqStack &S, SElemType e){
	if(!S.base)	return ERROR;
	else{
		SElemType *new_p;//防止内存泄露  realloc申请不到内存空间时会返回NULL 
		if(S.top - S.base >= S.stacksize){    //栈内元素个数比容量少一  因为TOP指向栈顶下一个元素； 
			//realloc这个函数坑很多，小心用
			new_p = (SElemType *)realloc(S.base, (S.stacksize + MAX) * sizeof(SElemType));
			if(!new_p){
				printf("error2\n");
				return ERROR;
			}
			S.base = new_p;
			S.top = S.base + S.stacksize;//这个非常关键  原来空间不够时，
			//系统会分配另外一块空间，并拷贝数据到另外一个空间
			//top指针也要相应的改变
			S.stacksize += MAX;
		}
		*S.top =  e;//广义赋值
		S.top++;
		return OK;
	}
}

Status Pop(SqStack &S, SElemType &e){
	if(!S.base)	return ERROR;
	else{
		if(!StackEmpty(S)){
			e = *(--S.top);
			return OK;
		}
		else return ERROR;		
	}	
}









