/*************************************************************************
    > File Name: sltest.c
    > Author: breeze
    > Mail: breeze0202@163.com 
    > Created Time: 2014年08月12日 11:05:55
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "staticlist.h"

int main()
{
	int i = 0;
	char tmpdata[] = "afdafafa";
	struct static_list	*sl;
	sl = sl_create(10);
	if(sl == NULL)
	{
		printf("sl create failed\n");
		return 0;
	}
	else
	{
		printf("sl create sucess\n");
	}

	struct list_node ln;

	for(i = 0; i < 8; i++)
	{
		ln.ln_key = i*333%24;
		ln.ln_pdata = malloc(10);
		memcpy(ln.ln_pdata, tmpdata, strlen(tmpdata)+1);
		sl_add(sl, &ln);
	}
	ln.ln_key = 5;

//	sl_del(sl, &ln);
	struct list_node *tmp;

	sl_radixsort(sl);

	tmp = sl->sl_fvalid;
	do{
		printf("sl key: %d , sl data: %s; ln next is:%d\n", tmp->ln_key, tmp->ln_pdata, tmp->ln_next);
		if(tmp->ln_next == -1)
			break;
		tmp = sl->sl_head + tmp->ln_next;
	}while(1);

	return 0;
}
