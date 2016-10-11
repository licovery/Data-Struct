#include <stdio.h>
#include <stdlib.h>

#define MAX 100//����ջʱջ�Ĺ��

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
		SElemType *new_p;//��ֹ�ڴ�й¶  realloc���벻���ڴ�ռ�ʱ�᷵��NULL 
		if(S.top - S.base >= S.stacksize){    //ջ��Ԫ�ظ�����������һ  ��ΪTOPָ��ջ����һ��Ԫ�أ� 
			//realloc��������Ӻܶ࣬С����
			new_p = (SElemType *)realloc(S.base, (S.stacksize + MAX) * sizeof(SElemType));
			if(!new_p){
				printf("error2\n");
				return ERROR;
			}
			S.base = new_p;
			S.top = S.base + S.stacksize;//����ǳ��ؼ�  ԭ���ռ䲻��ʱ��
			//ϵͳ���������һ��ռ䣬���������ݵ�����һ���ռ�
			//topָ��ҲҪ��Ӧ�ĸı�
			S.stacksize += MAX;
		}
		*S.top =  e;//���帳ֵ
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









