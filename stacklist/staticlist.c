/*************************************************************************
    > File Name: stacklist.c
    > Author: breeze
    > Mail: breeze0202@163.com 
    > Created Time: 2014年08月11日 14:29:49
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef	MAX_NUM
#define	MAX_NUM	1000
#endif

static	max_num = MAX_NUM;

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

			return (tmp - sl->sl_head);
		}
		if(tmp->ln_next == -1)
			break;
		prev = tmp;
		tmp = sl->sl_head + tmp->ln_next;
	}while(1);

	return -1;
}


