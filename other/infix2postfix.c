#include <stdio.h>
#include <string.h>

#define BUF_MAX	32	
#define ELEMTYPE	char	
#define END_MARK	'#'

/*---------------------------------------------------------------/
算法描述： 

1. 用两个栈，一个存放运算符stack_op，一个存放操作数stack_postfix

2. 扫描中缀表达式判断：

    @1 操作数  直接入栈stack_postfix，

    @2 运算符 与stack_op栈顶优先级比较优先级 

          #1, 当前优先级大，则直接压入stack_postfix

          #2,否则，把stack_op栈顶元素取出，压入stack_postfix,把当前元素压入stack_op

    @3  ‘（' 直接入栈stack_op 

    @4 ’ ）‘把stack_op的操作符弹出，压入stack_postfix,直到弹出’（‘

3. 把stack_postfix元素出栈放入postfix链表或数组中.
*--------------------------------------------------------------------*/
struct stack{
	ELEMTYPE elem[BUF_MAX];
	int top;
};


void init_stack(struct stack *st)
{
	memset(st, 0, sizeof(struct stack));
	st->top = -1;
}
ELEMTYPE peek(struct stack *st)
{
	return st->elem[st->top];
}
ELEMTYPE pop(struct stack *st)
{
	return st->elem[st->top--];
}

void push(struct stack *st, ELEMTYPE elem)
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

int cmp_op(ELEMTYPE op1, ELEMTYPE op2)
{
	if(op1 == '*' || op1 == '/')
	{
		if(op2 == '*' || op2 == '/')
			return 0;
		else
			return 1;
	}
	if(op1 == '+' || op1 == '-')
	{
		if(op2 == '*' || op2 == '/')
			return -1;
		else
			return 0;
	}
	return -2;
}

int check_op(ELEMTYPE *infix)
{
	char ch = *infix;
	if(ch == '*' || ch == '/' || ch == '+' || ch == '-')
		return 1;
	if(ch == '(')
		return 2;
	if(ch == ')')
		return 3;
	else
		return 0;
}
int is_end(ELEMTYPE *infix)
{
	if(*infix == END_MARK)
		return 1;
	else
		return 0;
}
void add_end(ELEMTYPE *postfix, struct stack *st_postfix)
{
	*(postfix+st_postfix->top+1) = END_MARK;
}
ELEMTYPE* infix2postfix(ELEMTYPE *infix, ELEMTYPE *postfix)
{
	int index;
	ELEMTYPE tmp;
	struct stack st_op;
	struct stack st_postfix;
	init_stack(&st_op);
	init_stack(&st_postfix);
	while(!is_end(infix))
	{
		switch (check_op(infix))
		{
		case 0:/* op is not operator */
			push(&st_postfix, *infix);
			break;
		case 1:/* op is operator */
			if(!is_empty(&st_op))
			{	
				if(cmp_op(*infix, peek(&st_op)) <= 0)/* if new op > old op direct push stack
													    else need pop then push*/
				{
					push(&st_postfix, pop(&st_op));
				}
			}
			push(&st_op, *infix);
			break;
		case 2:	/* op == '(' */
			push(&st_op, *infix);
			break;
		case 3:/* op == ')' */
			while(check_op(&(tmp=pop(&st_op))) != 2) /* op != '(' */
			{
				push(&st_postfix, tmp);
			}
			break;
		default:
			break;
		}
		infix++;
	}
	while(!is_empty(&st_op))
		push(&st_postfix, pop(&st_op));
	/* now st_postfix store the posfix */
	add_end(postfix, &st_postfix);
	while(st_postfix.top != -1)
	{
	//	*(postfix+st_postfix.top) = pop(&st_postfix);
		index = st_postfix.top;
		*(postfix+index) = pop(&st_postfix);
		
	}
	return postfix;
}

int main()
{
	char infix[BUF_MAX] = "6+3*4-(2/1)-5#";
	char postfix[BUF_MAX] = "0";
	printf("infix is: %s\n", infix);
	printf("postfix is: %s\n", infix2postfix(infix, postfix));
	return 0;
}