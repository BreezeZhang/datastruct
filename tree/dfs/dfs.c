#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define ELEMTYPE char
#define MAX_TREE	64
#define	ELEMTYPE_STACK	struct st_tree*
/*-----------------------------------------------/
实现的时候是顺着根节点，左子树，左子树的顺序访问，
如果符合就返回，不符合就压栈.先访问左子树到叶子节点，
然后出栈继续如此.直至栈空。
*------------------------------------------------*/
/*---------------------------------------------------------/
先提供个先压栈的实现
//深度优先遍历
void depthFirstSearch(Tree root){
    stack<Node *> nodeStack;  //使用C++的STL标准模板库
    nodeStack.push(root);
    Node *node;
    while(!nodeStack.empty()){
        node = nodeStack.top();
        printf(format, node->data);  //遍历根结点
        nodeStack.pop();
        if(node->rchild){
            nodeStack.push(node->rchild);  //先将右子树压栈
        }
        if(node->lchild){
            nodeStack.push(node->lchild);  //再将左子树压栈
        }
    }
}
*---------------------------------------------------------*/
struct st_tree{
	ELEMTYPE	data;
	struct	st_tree	*lchild;
	struct 	st_tree	*rchild;
};

struct stack{
	ELEMTYPE_STACK elem[MAX_TREE];
	int top;
};


void init_stack(struct stack *st)
{
	memset(st, 0, sizeof(struct stack));
	st->top = -1;
}
ELEMTYPE_STACK peek(struct stack *st)
{
	return st->elem[st->top];
}
ELEMTYPE_STACK pop(struct stack *st)
{
	return st->elem[st->top--];
}

void push(struct stack *st, ELEMTYPE_STACK elem)
{
	st->elem[++st->top] = elem;
}
int is_empty(struct stack *st)
{
	if(st->top == -1)
		return 1;
	else
		return 0;
}

void construct_tree(struct st_tree *st)
{
	char check = 0;
	st->lchild = st->rchild = NULL;
	printf("\nplease scanf data: ");
	scanf("%c", &st->data);
	fflush(stdin);
	printf("\ndo it construct left child: ");
	scanf("%c", &check);
	fflush(stdin);
	if(check == 'y')
	{
		st->lchild = (struct st_tree*)malloc(sizeof(struct st_tree));
		construct_tree(st->lchild);
	}
	printf("\ndo it construct right child: ");
	scanf("%c", &check);
	fflush(stdin);
	if(check == 'y')
	{
		st->rchild = (struct st_tree*)malloc(sizeof(struct st_tree));
		construct_tree(st->rchild);
	}
}
void vist_tree(struct st_tree *st)
{
	printf("%c\t", st->data);
	if(st->lchild != NULL)
		vist_tree(st->lchild);
	if(st->rchild != NULL)
		vist_tree(st->rchild);
}

int cmp(struct st_tree *st, ELEMTYPE data)
{
	if(st->data > data)
		return 1;
	else
		return 0;
}

struct st_tree *dfs_recursion(struct st_tree *st, int (*cmp)(struct st_tree *, ELEMTYPE), ELEMTYPE data)
{
	struct st_tree *tmp;
	if( cmp(st, data) )
		return st;
	if(st->lchild != NULL)
	{
		tmp = dfs_recursion(st->lchild, cmp, data);
		if(tmp != NULL)
			return tmp;
	}
	if(st->rchild != NULL)
	{
		tmp = dfs_recursion(st->rchild, cmp, data);
		if(tmp != NULL)
			return tmp;
	}
	return NULL;
}

struct st_tree *dfs_stack(struct st_tree *st, int (*cmp)(struct st_tree *, ELEMTYPE), ELEMTYPE data)
{
	struct stack	stack_tree;
	init_stack(&stack_tree);
	if( cmp(st, data) )
		return st;
	else
		push(&stack_tree, st);
		
	while((st = st->lchild) != NULL)
	{
		if( cmp(st, data) )
			return st;
		else
		{
			push(&stack_tree, st);
		}
	}
	while(!is_empty(&stack_tree))
	{
		st = pop(&stack_tree);
		while((st = st->rchild) == NULL)
		{
			if(is_empty(&stack_tree))
				return NULL;
			st = pop(&stack_tree);
		}
		if( cmp(st, data) )
			return st;
		else
			push(&stack_tree, st);
		while((st = st->lchild) !=  NULL)
		{
			if( cmp(st, data) )
				return st;
			else
			{
				push(&stack_tree, st);
			}
		}
	}
	return NULL;
}

int main()
{
	struct st_tree root;
	struct st_tree *result1, *result2;
	construct_tree(&root);
	vist_tree(&root);
	result1 = dfs_recursion(&root, cmp, 'y');
	if(result1 == NULL)
		printf("dfs_recursion result is: NULL\n");
	printf("\n dfs_recursion result: %c\n",result1->data);
	result2 = dfs_stack(&root, cmp, 'y');
	if(result2 == NULL)
		printf("dfs_stack result is: NULL\n");
	printf("\n dfs_stack result: %c\n",result2->data);

	return 0;
}




