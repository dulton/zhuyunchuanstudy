#include <stdio.h>
#include <stdlib.h>
#include "dstack.h"
#include "dlsit.h"

struct dstack
{
	DList* dlist_head;
};

dstack* queue_create(DataDestroyFunc data_destroy, void* ctx)
{
	dstack* head = (dstack*)malloc(sizeof(dstack));

	if(head != NULL)
	{
		if((head->dlist_head = dlist_create(data_destroy, ctx)) == NULL)
		{
			free(head);
			head = NULL;
		}
	}

	return head;
}


DStackRet dstack_destroy(dstack* head)
{
	dstack* tmp = head;
	DListRet ret = 0;
	
	if (head->dlist_head != NULL)
	{
		ret = dlist_destroy(head->dlist_head);
		
		free(head);
		head = NULL;	
	}
	
	return ret;
}

DStackRet dstack_push(dstack* head, void* data)
{
	dstack* tmp  = head;
	
	dlist_append(tmp->dlist_head, data);
	
	return DQUEUE_RET_OK;
}

DStackRet dstack_pop(dstack* head)
{
	dstack* tmp = head;
	
	return dlist_delete(head->dlist_head, 0);
}

DStackRet dstack_gettop(dstack* head,void** data)
{
	dstack* tmp = head;
	
	return dlist_get_by_index(tmp->dlist_head, 0, data);
}

size_t dstack_length(dstack* head)
{
	dstack* tmp = head;
	
	return dlist_length(tmp->dlist_head);
}
