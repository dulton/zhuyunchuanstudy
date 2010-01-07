#ifndef STACK_H
#define STACK_H

#include <stdlib.h>


#ifdef __CPP
extern "C"
{
#endif

typedef enum _StackRet
{
	STACK_RET_OK,
	STACK_RET_FAIL,
}stackret;

#ifdef DLIST_IMPLEMENT

#endif

#ifdef ARRAY_IMPLEMENT

	typedef struct _stack_array stack_array;
	typedef stackret (stack_data_print_fun)(void* data, void* ctr);

	stackret stack_array_creat(size_t stack_init_size);
	stackret stack_array_destroy(stack_array* base);
	stackret stack_array_push(stack_array* base,void* data);
	stackret stack_array_pop(stack_array* base);
	size_t stack_length(stack_array* base);
	stackret stack_get_top(stack_array* base, stack_data_print_fun print);
	stackret stack_print(stack_array* base, stack_data_print_fun print, void* ctr);
#endif


#ifdef __CPP
}
#endif
