/*************************************************************************
    > File Name: staticlist.h
    > Author: breeze
    > Mail: breeze0202@163.com 
    > Created Time: 2014年08月12日 10:00:09
**************************/

#ifndef	__STATICLIST_H__
#define	__STATICLIST_H__

struct static_list{
	struct	list_node	*sl_head;
	struct	list_node	*sl_ffree;	//指向第一个空闲的结点空间
	struct	list_node	*sl_fvalid;			//指向第一个有效的空间	
	int		sl_cnt;		//当前链表中有效数据的个数
	int		sl_len;	//静态链表的大小
};

typedef int		KEYTYPE;
struct list_node{
	KEYTYPE		ln_key;
	void		*ln_pdata;
	int			ln_next;
};

struct static_list* sl_create(int num);	//因为实现时会浪费1个节点空间，所以NUM大于2
int sl_del(struct static_list *sl, struct list_node *del_ln);
int sl_add(struct static_list *sl, struct list_node *new_ln);
int sl_radixsort(struct static_list	*sl);

#endif
