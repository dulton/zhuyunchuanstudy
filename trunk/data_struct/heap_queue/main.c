#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

static print_int(void* data, void* ctr)
{
	printf();
}

int main(int argc, char* argv[])
{
	stack_array* base = stack_array_creat();
	int i = 0;
	int num = 10;

	for (i = 0, i < num,i++)
	{
		assert(stack_array_push(base, (void*)i) == STACK_RET_OK);
	}
	
	stack_pop(base);
	
	stack_get_top(base, print_int);

	stack_destroy(base);
}
