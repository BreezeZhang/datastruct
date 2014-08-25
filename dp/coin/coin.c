/*************************************************************************
    > File Name: coin.c
    > Author: breeze
    > Mail: breeze0202@163.com 
    > Created Time: 2014年08月25日 13:53:15
 ************************************************************************/

/*---------------------------------------------/
 * 问题描述：
 * 如果我们有面值为1元、3元和5元的硬币若干枚，如何用最少的硬币凑够11元
 *
 * -----------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

//定义面值数组N, 面值总数M

#define		N		3
#define		M		15

int d[M];
int	v[N];

int get_coin_num(int d[], int v[])
{
	int i = 0, j = 0;
	int tmp = 0;
	for(i = 0; i < M; i++)
	{
		*(d+i) = i;
		for(j = 0; j < N; j++)
		{
			if(*(v+j) <= i)
			{
				tmp = 1 + *(d + i - *(v+j));	
				if(*(d+i) > tmp)
					*(d+i) = tmp;
			}
		}
	}
	return 0;
}


int main()
{
	int i = 0, j = 0;
	v[0] = 1;
	v[1] = 3;
	v[2] = 5;

	get_coin_num(d, v);

	for(j = 0; j < M; j++)
	{
		printf("面值: %d   coin NUM: %d\n", j, *(d + j));
	}
	return 0;
}
