#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct _stack_array_node
{
	void* data;
}stack_array_node;

struct _stack_array
{
	stack_array_node* base;
	stack_array_node* top;
	
	size_t stack_size;
};

stack_array* stack_array_creat(size_t stack_init_size)
{
	stack_array* head = malloc(sizeof(stack_array));
	head->stack_size = stack_init_size;

	if (head != NULL)
	{
		head->base = malloc(stack_size * sizeof(stack_array_node));
		
		head->top = head->base;
	}

	return head;
}

stack_ret stack_push(stack_array* head, void* data)
{
	stack_node* tmp = head->top;
	
	if (head->top - head->base >= stack_size)
	{
		
	}
}
