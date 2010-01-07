#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "dlist.h"
#include "dqueue.h"

struct _dqueue
{
	DList* dlist_head;
};

dqueue* queue_create(DataDestroyFunc data_destroy, void* ctx)
{
	dqueue* head = (dqueue*)malloc(sizeof(dqueue));

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


DQueueRet dqueue_destroy(dqueue* head)
{
	dqueue* tmp = head;
	DListRet ret = 0;
	
	if (head->dlist_head != NULL)
	{
		ret = dlist_destroy(head->dlist_head);
		
		free(head);
		head = NULL;	
	}
	
	return ret;
}

DQueueRet dqueue_push(dqueue* head, void* data)
{
	dqueue* tmp  = head;
	
	dlist_append(tmp->dlist_head, data);
	
	return DQUEUE_RET_OK;
}

DQueueRet dqueue_pop(dqueue* head)
{
	dqueue* tmp = head;
	
	return dlist_delete(head->dlist_head, 0);
}

DQueueRet dqueue_gettop(dqueue* head,void** data)
{
	dqueue* tmp = head;
	
	return dlist_get_by_index(tmp->dlist_head, 0, data);
}

size_t dqueue_length(dqueue* head)
{
	dqueue* tmp = head;
	
	return dlist_length(tmp->dlist_head);
}
