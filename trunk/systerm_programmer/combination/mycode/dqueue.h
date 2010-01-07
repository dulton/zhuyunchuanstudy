#include <stdio.h>
#include <stdlib.h>
#include "dlist.h"

#ifndef DQUEUE_H
#define DQUEUE_H

#ifdef _cpp
extern "C"
{
#endif

struct _dqueue;
typedef struct _dqueue dqueue;


typedef enum _dqueue
{
	DQUEUE_RET_OK,
	DQUEUE_RET_OOM,
	DQUEUE_RET_STOP,
	DQUEUE_RET_PARAMS,
	DQUEUE_RET_FAIL,
}DQueueRet;

dqueue* queue_create(DataDestroyFunc data_destroy, void* ctx);
DQueueRet dqueue_destroy(dqueue* head);

DQueueRet dqueue_push(dqueue* head, void* data);
DQueueRet dqueue_pop(dqueue* head);
DQueueRet dqueue_gettop(dqueue* head, void** data);
DQueueRet dqueue_visit(dqueue* head, );
size_t dqueue_length(dqueue* head);



#ifdef _cpp
}
#endif  /*_cpp*/ 

#endif /* DQUEUE_H */
