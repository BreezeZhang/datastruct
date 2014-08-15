/*************************************************************************
    > File Name: stacklist.c
    > Author: breeze
    > Mail: breeze0202@163.com 
    > Created Time: 2014年08月11日 14:29:49
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef	MAX_NUM
#define	MAX_NUM	1000
#endif

#define		MAX_NUM_OF_KEY	6
#define		RADIX			10

typedef	int KEYTYPE;

struct list_node{
	KEYTYPE		ln_key;
	void		*ln_pdata;
	int			ln_next;
};
/* -----------------------------------------------------------------/
 * 最后一个有效节点的next指向-1,最后一个无效可用的结点的next指向-1
 * 不用分配数据空间里的结点表示，多维护两个指针提高了删除节点的效率
 * ----------------------------------------------------------------*/
struct static_list{
	struct	list_node	*sl_head;
	struct	list_node	*sl_ffree;	//指向第一个空闲的结点空间
	struct	list_node	*sl_fvalid;			//指向第一个有效的空间	
	int		sl_cnt;		//当前链表中有效数据的个数
	int		sl_len;	//静态链表的大小
};

int ln_compare(struct list_node *ln1, struct list_node *ln2)
{
	return (ln1->ln_key - ln2->ln_key);
}

struct static_list* sl_create(int num)	//因为实现时会浪费1个节点空间，所以NUM大于2
{
	int i = 0;
	if( num < 2)
		return NULL;
	struct static_list	*sl;
	sl = malloc(sizeof(struct static_list));
	if(sl == NULL)
	{
		return NULL;
	}
	sl->sl_head = malloc(num * sizeof(struct list_node));
	if(NULL == sl)
	{
		free(sl);
		return NULL;
	}
	memset(sl->sl_head, 0, sizeof(struct list_node)*num);
	sl->sl_len = num;
	sl->sl_cnt = 0;
	sl->sl_ffree = sl->sl_head;       //零节点指向下个空闲可用的结点，初始化为0；
	sl->sl_fvalid = NULL; 
	for( i = 0; i < sl->sl_len; i++)
	{
		(sl->sl_head + i)->ln_next = i + 1;
	}
	(sl->sl_head + sl->sl_len-1)->ln_next = -1;
	return sl;
}

/*---------------------------------/
 * sl_add 函数功能： 在静态链表的最后一个有效元素处添加一个节点
 * ---------------------------------*/

int sl_add(struct static_list *sl, struct list_node *new_ln)
{
	int i = 0;
	int tmp_next;
	struct list_node *tmp;
	if(new_ln->ln_pdata == NULL || sl == NULL)
		return -1;
	if(sl->sl_ffree->ln_next == -1)                //只剩下一个空闲节点空间时，先分配新空间
	{
		sl->sl_len *= 2;
		tmp = realloc(sl->sl_head, sl->sl_len * sizeof(struct list_node));
		if(tmp == NULL)
		{
			sl->sl_len /= 2;
			return -1;
		}
		sl->sl_head = tmp;
		sl->sl_ffree->ln_next = sl->sl_len/2;
		for(i = sl->sl_len/2; i < sl->sl_len; i++)
		{
			(sl->sl_head + i)->ln_next = i + 1;	
		}	
		(sl->sl_head + sl->sl_len - 1)->ln_next = -1;
	}
	sl->sl_ffree->ln_pdata = new_ln->ln_pdata;
	sl->sl_ffree->ln_key = new_ln->ln_key;
	if(sl->sl_fvalid == NULL)
		sl->sl_fvalid = sl->sl_ffree;
	else
	{
		tmp = sl->sl_fvalid;
		while(tmp->ln_next != -1)
			tmp = sl->sl_head + tmp->ln_next;
	}
	tmp->ln_next = sl->sl_ffree - sl->sl_head;

	tmp_next = sl->sl_ffree->ln_next;
	sl->sl_ffree->ln_next = -1;             //在最后插入
	sl->sl_ffree = sl->sl_head + tmp_next;

	sl->sl_cnt++;
	
	return 0;
}

/*-------------------------------------------------------/
 * 删除节点，返回删除节点的下表
 * -----------------------------------------------------*/
int sl_del(struct static_list *sl, struct list_node *del_ln)
{
	if(sl->sl_fvalid == NULL)
		return -1;

	struct list_node *tmp, *prev;
	prev = NULL;
	tmp = sl->sl_fvalid;
	do{
		if(ln_compare(tmp, del_ln) == 0)
		{
			if(prev == NULL)		//如果删除的是第一个有效的节点
			{
				if(tmp->ln_next == -1)	//只有一个节点有效
				{
					sl->sl_fvalid = NULL;
				}
				else
				{
					sl->sl_fvalid = sl->sl_head + tmp->ln_next;
				}
			}
			else
			{
				prev->ln_next = tmp->ln_next;
			}
			tmp->ln_next = sl->sl_ffree - sl->sl_head;
			sl->sl_ffree = tmp;		

			free(tmp->ln_pdata);			//释放数据空间
			sl->sl_cnt--;
			return (tmp - sl->sl_head);
		}
		if(tmp->ln_next == -1)
			break;
		prev = tmp;
		tmp = sl->sl_head + tmp->ln_next;
	}while(1);

	return -1;
}

/*----------------------------------------------/
 * sl_radixsort 基数排序
 * 参数： sl 待排序的静态链表  radix  传入的基数
 * 思路： 先分配一个节点数组，数组大小为sl的有效节点个数
 *        然后将有效节点的key和data指针赋值到节点数组，对
 *        其进行LSD基数排序，然后将排序后的节点数组，传入
 *        sl中，再次进行sl的初始化，和节点next域的更新。
 *----------------------------------------------*/
//   思路错误，静态链表不应赋值，这样会影响效率:
int sl_radixsort_bad(struct static_list	*sl, int radix)
{
	assert(sl != NULL);

	if(sl->sl_cnt < 2)
		return 1;

	struct list_node *arrln = (struct list_node *)malloc(sl->sl_cnt * sizeof(struct list_node));
	if(arrln== NULL)
	{
		printf("malloc failed \n");
		return 0;
	}
		
	struct list_node *tmpln = sl->sl_fvalid;
	struct list_node *tmparrln = arrln;
	while(1)
	{
		arrln->ln_key = tmpln->ln_key;
		arrln->ln_pdata = tmpln->ln_pdata;
		if(tmpln->ln_next == -1)
			break;
		tmpln = sl->sl_head + tmpln->ln_next;
		arrln++;
	}
	arrln = tmparrln;
	
	/*开始基数排序*/


	return 0;
}


int key2int(KEYTYPE key, int i)
{
	int j = 0;
	int k = 1;
	if(i == 0)
		return key%10;
	else
	{

		for(j = 0; j < i; j++)
		{
			k *= 10;
		}
		return key/i%10;
	}
}
/*-----------------------------------------------/
 *函数描述：对静态链表sl进行基数排序
 *思路：  选用LSD进行排序，即分发、收集	
 *		  分发排序时，需要保存最后一个节点的next和第一个节点
 *		  因为静态链表节点的地址是相对的，所以可以保存在一个
 *		  数组里，这是需要两个整形数组即可f 和 l
 * ----------------------------------------------*/
int sl_radixsort(struct static_list	*sl)
{
	assert(sl != NULL);

	if(sl->sl_cnt < 2)
		return 1;
	
	int j, k;
	int i = 0;
	int keynum = MAX_NUM_OF_KEY;
	int arrf[RADIX];
	int arrl[RADIX];
	struct list_node *tmpln;

	
	for(i = 0; i < keynum; i++)
	{
		memset(arrf, -1, sizeof(int) * RADIX);
		memset(arrl, -1, sizeof(int) * RADIX);

		//分配
		tmpln = sl->sl_fvalid;
		while(1)
		{
			k = key2int(tmpln->ln_key, i);
			if(*(arrf+k) == -1)
			{
				*(arrf + k) = tmpln - sl->sl_head;
			}
			else
			{
				//在遍历到第i个数据时，前i个的next都已经用过了，可以修改
				(sl->sl_head + *(arrl + k))->ln_next = tmpln - sl->sl_head;	
			}
			*(arrl + k) = tmpln - sl->sl_head;

			if(tmpln->ln_next != -1)
			{
				tmpln = sl->sl_head + tmpln->ln_next;
			}
			else
			{
				break;
			}
		}

		//收集
		for(j = 0; j < RADIX; j++)
		{
			if(*(arrf + j) != -1)
				break;
		}
		if(j == RADIX)
			break;

		sl->sl_fvalid = sl->sl_head + *(arrf + j);
		tmpln = sl->sl_head + *(arrl + j);

		for(j = j+1; j < RADIX; j++)
		{
			if(*(arrf + j) != -1)
			{
				tmpln->ln_next = *(arrf + j);
				tmpln = sl->sl_head + *(arrl + j);
			}
		}
		tmpln->ln_next = -1;
	}

	return 1;
}


/*----------------------------------------------------------------/
 * 函数描述： 对有序的静态链表进行插入操作
 *			  当满足节点compare函数时,在其后插入新节点newln
 * 思路：     可先把链表数组的下表存入一个数组中，然后进行二分查找，
 *			  进行插入，或者直接遍历链表进行插入操作
 *			  此函数是直接遍历插入
 * ---------------------------------------------------------------*/
int sl_insert(struct static_list *sl, struct list_node *new_ln, int (*compare(struct list_node *ln1, struct list_node *ln2)))
{
	int i = 0;
	int tmp_next;
	struct list_node *tmp;
	if(new_ln->ln_pdata == NULL || sl == NULL)
		return -1;
	if(sl->sl_ffree->ln_next == -1)                //只剩下一个空闲节点空间时，先分配新空间
	{
		sl->sl_len *= 2;
		tmp = realloc(sl->sl_head, sl->sl_len * sizeof(struct list_node));
		if(tmp == NULL)
		{
			sl->sl_len /= 2;
			return -1;
		}
		sl->sl_head = tmp;
		sl->sl_ffree->ln_next = sl->sl_len/2;
		for(i = sl->sl_len/2; i < sl->sl_len; i++)
		{
			(sl->sl_head + i)->ln_next = i + 1;	
		}	
		(sl->sl_head + sl->sl_len - 1)->ln_next = -1;
	}
	sl->sl_ffree->ln_pdata = new_ln->ln_pdata;
	sl->sl_ffree->ln_key = new_ln->ln_key;
	if(sl->sl_fvalid == NULL)
		sl->sl_fvalid = sl->sl_ffree;
	else
	{
		tmp = sl->sl_fvalid;
		while(compare(new_ln, tmp))
		{
			tmp = sl->sl_head + tmp->ln_next;
		}
	}
	tmp->ln_next = sl->sl_ffree - sl->sl_head;

	tmp_next = sl->sl_ffree->ln_next;
	sl->sl_ffree->ln_next = -1;             //在最后插入
	sl->sl_ffree = sl->sl_head + tmp_next;

	sl->sl_cnt++;
	
	return 0;
}
