/*************************************************************************
    > File Name: lis.c
    > Author: breeze
    > Mail: breeze0202@163.com 
    > Created Time: 2014年08月25日 15:22:32
 ************************************************************************/
/*---------------------------------------------------------------------------/
 * 很多时候，自己都是在尝试理解算法的思想，而不是自己主动去探究如何去想，总是妄图
 * 抄近路，可在往往近路还不如自己想的快，理解的深，而总是尝试理解别人的想法很难，
 * 不如有自己的想法，顺着自己的道路
 * ------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lis(int a[], int n)
{
	int i, j;
	int len = 1;

	int *dp = (int*)malloc(sizeof(a) * n);
	memset(dp, 0, sizeof(a)* n);

	for(i = 0; i < n; i++)
	{
		dp[i] = 1;
		for(j = 0; j < i; j++)
		{
			if(*(a+j) <= *(a+i))
			{
				if(*(dp + i) <= *(dp + j))
				{
					*(dp + i) = *(dp + j) + 1;
				}
			}
		}
		if(len < dp[i])
		{
			len = dp[i];
		}
	}
	free(dp);

	return len;
}

int main()
{
	int a[] = {
		1, 2, 3, 2, 5, 8, 5, 9
	};
	printf("LIS is %d\n", lis(a, 8));

	return 0;
}
