#include "stdio.h"
#include "stdlib.h"
#include "Sqstack.h"
#include <time.h> //获取时间生成随机数

//0代表空白  1代表障碍
typedef struct 
{
	int x;
	int y;
}coordinate;

//{0, 0}代表不动，把初始位置也入栈
coordinate dir[5] = {{0, 0}, {0, 1}, {1, 0}, {0, -1}, {-1, 0}};

//1-4分别指示东南西北
enum direction
{
	east = 1, 
	south,
	west,
	north
};

//当前位置可通  按照东南西北顺序返回可通的方向
//这是因为起点在左上角  终点的右下角  东南优先
//如果起点终点位置不确定，可以做一些优化
int Accessible(coordinate c, int **m){
	coordinate d;
	int i;
	for(i = east; i <= north; i++){
		d.x = c.x + dir[i].x;
		d.y = c.y + dir[i].y;
		if(!m[d.x][d.y]){//只有方块是0才认为通
			return i;
		}
	}
	return 0;
}

//向某个方向移动
int move(coordinate &c, int d){
	c.x += dir[d].x;
	c.y += dir[d].y;
}

//从起点开始 判断是否可通 可通意味下一个点没有障碍并且没有走过
//可通就继续走 入栈  直到走到终点   不可通就回退  出栈
//如果最后回到原点且周围不可通（包括障碍与走过的） 即无解
//如果走过终点即为一个解
Status FindPath(coordinate start, coordinate end, int **m, SqStack &s){
	int d;
	coordinate c;
	c = start;
	InitStack(s);
	Push(s, 0);
	m[start.x][start.y] = 2;//走过的点设为2 与 1（原本的障碍区分）
	while(1){
		d = Accessible(c, m);
		if(c.x == end.x && c.y == end.y){//到终点了
			break;
		}
		if(!d && !StackLength(s)){//该点不通且不可往回走
			printf("impossible\n");
			return 0;
		}
		if(d){//可通
			move(c, d);
			m[c.x][c.y] = 2;
			Push(s, d);//栈储存每次走的方向
		}
		else{//不通，往回走
			Pop(s, d);
			c.x -= dir[d].x;
			c.y -= dir[d].y;
		}
	}
	return 1;
}

int** CreateMaze(int length, int width, coordinate start, coordinate end){
	int **m;
	m = (int **)malloc(sizeof(int *) * width);
	int i, j;
	for(i = 0; i < width; i++){
		m[i] = (int *)malloc(sizeof(int) * length);
	}
	//边界填充
	for(i = 0; i < length; i++){
		m[0][i] = 1;
		m[width-1][i] = 1;
	}
	for (i = 0; i < width ; i++){
		m[i][0] = 1;
		m[i][length-1] = 1;
	}
	//随机数填充
	for(i = 1; i < width-1; i++){
		for(j = 1; j < length-1; j++){
			int a;
			if(rand()%3)
				a = 0;
			else
				a = 1;
			m[i][j] = a;
		}
	}
	//起点和终点必定没有障碍
	m[start.y][start.x] = 0;
	m[end.y][end.y] = 0;
	return m;
}

void PrintMaze(int **m, int length, int width){
	int i, j;
	printf(" ");
	for(j = 0; j < length; j++)
		printf("%2d", j);
	printf("\n");
	for (i = 0; i < width; ++i){
		printf("%2d", i);
		for (j = 0; j < length; ++j){
			if(m[i][j])
				printf("# ");
			else
				printf("  ");
		}
		printf("\n");
	}
}

void PrintPath(coordinate start, SqStack s){
	if(s.base){
		int *p;
		for(p = s.base; p < s.top; p++){
			start.x += dir[*p].x;  
			start.y += dir[*p].y;
			printf("(%d, %d)\n", start.x, start.y);
		}
	}
}

//根据栈中数据已经起点，确定算法正确性
int TruePath(coordinate start, coordinate end, SqStack s, int **m){
	int *p;
	for(p = s.base; p < s.top; p++){
		start.x += dir[*p].x;  
		start.y += dir[*p].y;
		if(m[start.x][start.y] == 1)
			return 0;
	}
	if(start.x == end.x && start.y == end.y)
		return 1;
	return 0;
}

void DestoryMap(int **m, int length, int width){
	int i;
	for(i = 0; i < width; i++){
		free(m[i]);
		m[i] = NULL;
	}
	free(m);
	m = NULL;
}

int main(int argc, char const *argv[])
{
	srand((unsigned)time(NULL));  
	int length, width;
	int **map;
	coordinate start, end;
	SqStack s; 
	scanf("%d%d", &length, &width);
	scanf("%d%d", &start.x, &start.y);
	scanf("%d%d", &end.x, &end.y);
	map = CreateMaze(length, width, start, end);
	PrintMaze(map, length, width);
	FindPath(start, end, map, s);
	PrintPath(start, s);
	//printf("%d\n", TruePath(start, end, s, map));
	DestoryMap(map, length, width);
	return 0;
}

