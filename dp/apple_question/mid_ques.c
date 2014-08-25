/*************************************************************************
    > File Name: mid_ques.c
    > Author: breeze
    > Mail: breeze0202@163.com 
    > Created Time: 2014年08月25日 16:49:23
 ************************************************************************/

/*---------------------------------------------------------/
 *平面上有N*M个格子，每个格子中放着一定数量的苹果。你从左上角的格子开始， 
 *每一步只能向下走或是向右走，每次走到一个格子上就把格子里的苹果收集起来，
 *这样下去，你最多能收集到多少个苹果。
 * -----------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define	N	5
#define	M	5
#define	MAX	10

void initapple(int *a, int m, int n)
{
	int i, j;
	srand((int)time(0));

	for(i = 0; i < m; i++)
	{
		for(j = 0; j < n; j++)
		{
			*(a + i*n + j) = rand()%(MAX+1);
		}
	}
}

// m行， n 列
int get_max_apple(int *a, int m, int n)
{
	int i, j;
	int tmp1, tmp2;

	int max;

	int *dp = (int *)malloc(sizeof(int)*m*n);
	if(dp == NULL)
		return -1;
	memset(dp, 0, sizeof(int)*m*n);

	*dp = *a;
	//先对边界值进行处理
	for(i = 1; i < m; i++)
	{
		*(dp + i*n) = *(dp + (i - 1)*n) + *(a + i*n);	
	}
	
	for(j = 1; j < n; j++)
	{
		*(dp + j) = *(dp + j - 1) + *(a + j);
	}

	for(i = 1; i < m; i++)
	{
		for(j = 1; j < n; j++)
		{
			tmp1 = *(dp + j-1 + i*n); 
			tmp2 = *(dp + (i-1)*n +j);
			*(dp + j + i*n) = *(a + j + i*n) + (tmp1>tmp2?tmp1:tmp2);
		}
	}

	max = *(dp + m*n - 1);

	// just for test
	printf("apple gahter print as follow:\n");
	for(i = 0; i < M; i++)
	{
		for(j = 0; j < N; j++)
		{
			printf("%5d", *(dp + i*N + j));
		}
		printf("\n");
	}

	free(dp);

	return max;
}

int main()
{
	int i, j;
	int *a;

	a = (int *)malloc(sizeof(int)*M*N);

	initapple(a, M, N);
	printf("apple print as follow:\n");
	for(i = 0; i < M; i++)
	{
		for(j = 0; j < N; j++)
		{
			printf("%5d", *(a + i*N + j));
		}
		printf("\n");
	}
	
	printf("\n\n the max apple can gather is : %d", get_max_apple(a, M, N));

	free(a);
	return 0;
}

