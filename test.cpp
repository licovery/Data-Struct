#include <stdio.h>
#include <stdlib.h>

typedef struct LongNode
{
	int int_4;
	struct LongNode *next;
	struct LongNode *prior;
}LongNode, *Longinteger;


Longinteger InitLonginteger();
Longinteger CreateLonginteger();
void PrintLonginterger(Longinteger);
int LongintegerLength(Longinteger);
int abscompare(Longinteger, Longinteger);
void HeadIntsert(int, Longinteger &);
Longinteger abs_add(Longinteger , Longinteger);
Longinteger abs_sub(Longinteger , Longinteger);
void DestoryLonginteger(Longinteger &);
Longinteger Plus(Longinteger, Longinteger);
Longinteger Minus(Longinteger, Longinteger);

int main(int argc, char const *argv[])
{
	Longinteger a, b, c, d;
	while(1){
		a = CreateLonginteger();
		b = CreateLonginteger();
		getchar();
		PrintLonginterger(a);
		PrintLonginterger(b);
		c = Plus(a, b);
		d = Minus(a, b);
		PrintLonginterger(c);
		PrintLonginterger(d);
		DestoryLonginteger(a);
		DestoryLonginteger(b);
		DestoryLonginteger(c);
		DestoryLonginteger(d);
	}
	return 0;
}



Longinteger InitLonginteger(){
	Longinteger l;
	l = (Longinteger)malloc(sizeof(LongNode));
	if(!l)
		printf("error\n");
	l->next = l->prior = l;
	return l;
}


Longinteger CreateLonginteger(){
	int sum = 0;
	char c;
	Longinteger l;
	LongNode *p, *q;
	l = InitLonginteger();
	c = getchar();
	if(c == '-'){
		l->int_4 = 1;
	} 	
	else {
		l->int_4 = 0;
		sum = sum * 10 + c - '0';
	}
	q = l;
	while(1){
		c = getchar();
		if(c == ',' || c == ';'){
			p = (LongNode *)malloc(sizeof(LongNode));
			p->int_4 = sum;
			sum = 0;
			p->next = q->next;
			p->prior = q;
			q ->next->prior = p;
			q->next = p;
			q = p;
			if(c == ';')
				break;
		}
		else
			sum = sum * 10 + c - '0';
	}
	return l;
}

void PrintLonginterger(Longinteger l){
	if(l){
		Longinteger p;
		p = l;
		if(p->int_4)
			printf("-");
		p = p->next;
		while(p != l){
			if(p != l->next){
					if(p->int_4 >= 1000);
				else if(p->int_4 >= 100)
					printf("0");
				else if(p->int_4 >= 10)
					printf("00");
				else
					printf("000");
			}
			printf("%d", p->int_4);
			if(p->next != l)
				printf(",");
			p = p->next;
		}
		printf("\n");
	}
}

int LongintegerLength(Longinteger l){
	int i = 0;
	Longinteger p;
	p = l->next;
	while(p != l){
		i++;
		p = p->next;
	}
	return i;
}


int abscompare(Longinteger a, Longinteger b){
	int a1, b1;
	Longinteger p, q;
	p = a->next;
	q = b->next;
	a1 = LongintegerLength(a);
	b1 = LongintegerLength(b);
	if(a1 > b1)
		return 1;
	else if(a1 < b1)
		return -1;
	else{
		while(p->int_4 == q->int_4){
			p = p->next;
			q = q->next;
			if(p == a && q == b){
				return 0;
			}
		}
		if(p->int_4 > q->int_4)
			return 1;
		else
			return -1;
	}
}


void HeadIntsert(int s, Longinteger &c){
	Longinteger r;
	r = (Longinteger)malloc(sizeof(LongNode));
	r->int_4 = s;
	r->next = c->next;
	c->next->prior = r;
	r->prior = c;
	c->next = r;

	// r->next = c->next;
	// r->prior = c;
	// c->next->prior = r;
	// c->next = r;
	
}



//  |a| + |b|
Longinteger abs_add(Longinteger a, Longinteger b){
	int s = 0, o = 0, a1, b1;
	Longinteger p, q, c, temp_node, temp_head;//为了节省代码量
	a1 = LongintegerLength(a);
	b1 = LongintegerLength(b);
	c = InitLonginteger();
	p = a->prior;
	q = b->prior;
	while(p != a && q != b){
		s = p->int_4 + q->int_4 + o;
		if(s > 9999){
			s = s - 10000;
			o = 1;
		}
		else
			o = 0;
		HeadIntsert(s, c);
		p = p->prior;
		q = q->prior;
	}
	if(a1 != b1){
		if(a1 > b1){
			temp_node = p;
			temp_head = a;
		}
		else{
			temp_node = q;
			temp_head = b;
		}
		while(temp_node != temp_head){
			s = temp_node->int_4  + o;
			if(s > 9999){
				s = s - 10000;
				o = 1;
			}
			else
				o = 0;
			HeadIntsert(s, c);
			temp_node = temp_node->prior;
		}
	}
	if(o){
		HeadIntsert(o, c);
	}		
	return c;
}


//  已知|a| >= |b|,求 |a| - |b| 
Longinteger abs_sub(Longinteger a, Longinteger b){
	int s = 0, o = 0;
	Longinteger p, q, c, r;
	c = InitLonginteger();//没有确定c的符号，因为只关心数值
	p = a->prior;
	q = b->prior;
	while(p != a && q != b){
		s = p->int_4 - q->int_4 + o;
		if(s < 0){
			o = -1;
			s =  s + 10000;
		}
		else{
			o = 0;
		}
		HeadIntsert(s, c);
		p = p->prior;
		q = q->prior;
	}
	while(p != a){
		s = p->int_4 + o;
		if(s < 0){
			o = -1;
			s =  s + 10000;
		}
		else{
			o = 0;
		}
		HeadIntsert(s, c);
		p = p->prior;
	}
	
	p = c->next;
	while(p != c->prior){
		if(p->int_4 == 0){
			p->prior->next = p->next;
			p->next->prior = p->prior;
			r = p;
			p = p->next;
			free(r);
			r = NULL;
		}
		else{
			break;
		}
	}
	return c;
}

void DestoryLonginteger(Longinteger &l){
	Longinteger r, p;
	r = l->next;
	p = l->next->next;
	while(p != l){
		free(r);
		r = p;
		p = p->next;
	}
	free(r);
	free(p);
	l = NULL;
}

Longinteger Plus(Longinteger a, Longinteger b){
	Longinteger c;
	if(a->int_4 == b->int_4){
		c = abs_add(a, b);
		c->int_4 = a->int_4;
	}
	else{
		if(abscompare(a, b) == 1){
			c = abs_sub(a, b);
			c->int_4 = a->int_4;
		}
		else if(abscompare(a, b) == -1){
			c = abs_sub(b, a);
			c->int_4 = b->int_4;
		}
		else{
			c = InitLonginteger();
			c->int_4 = 0;
			HeadIntsert(0, c);
		}

	}
	return c;
}

Longinteger Minus(Longinteger a, Longinteger b){
	if(b->int_4 == 1){
		b->int_4 = 0;
	}
	else{
		b->int_4 = 1;
	}
	return Plus(a, b);
}


