#include <stdio.h>
#include <assert.h>
#include "dlist_v2.h"



dlist_ret cmp(void* ctr, void* data)
{
	if (NULL == ctr)
	{
		return DLIST_RET_STOP;
	}
	*ctr = (*ctr > *data) ? *ctr : *data;
	return DLIST_RET_OK;
}

static dlist_ret print_int(void* data)
{
	printf("%d", (int)data);

	return DLIST_RET_OK;
}


int main(int argc, char* argv[])
{
	int i = 0;
	int num = 10;
	
	
	dlist* head = dlist_creat();
	
	for (i = 0; i < num; i++)
	{
		assert(dlist_append(head, (void*)i) == DLIST_RET_OK);
	}
	
	void* max = head->first->data;
	
	dlist_foreach(head, cmp, void* max);
	
	print_int(max);
	
	dlist_print(head, print_int);

	dlist_destroy(head);
		
}
