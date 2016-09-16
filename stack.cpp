#include <stdio.h>
#include <stdlib.h>

#define MAX 100
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

typedef int Status;

typedef int SElemType;//������������ 


typedef struct {
	SElemType *base;
	SElemType *top;
	int stacksize;
}SqStack;

Status InitStack(SqStack &S){
	S.base = (SElemType *)malloc(MAX * sizeof(SElemType));
	S.top = S.base;
	S.stacksize = MAX;
	if(S.base)	return OK;
	else	return ERROR;
}

Status DestoryStack(SqStack &S){
	if(!S.base)	return ERROR;
	else{
		free(S.base);
		S.base = S.top = NULL;
		S.stacksize = 0;
		return OK; 
	}
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

Status Push(SqStack &S, SElemType &e){
	if(!S.base)	return ERROR;
	else{
		if(S.top - S.base < S.stacksize){    //ջ��Ԫ�ظ�����������һ  ��ΪTOPָ��ջ����һ��Ԫ�أ� 
			*S.top =  e;//���帳ֵ
			S.top++;
			return OK; 
		}
		else	return ERROR;
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

//ע��





