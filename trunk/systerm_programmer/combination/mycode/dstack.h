#include <stdlib.h>

#ifndef DSTACK_H
#define DSTACK_H

#ifdef _cpp
extern "C"
{
#endif

struct _dstack;
typedef struct _dstack dstack;

typedef enum _dstack_ret
{
	DSTACK_RET_OK,
	DSTACK_RET_OOM,
	DSTACK_RET_STOP,
	DSTACK_RET_PARAMS,
	DSTACK_RET_FAIL,
}DStackRet;

dstack* queue_create(DataDestroyFunc data_destroy, void* ctx);
DStackRet dstack_destroy(dstack* head);

DStackRet dstack_push(dstack* head, void* data);
DStackRet dstack_pop(dstack* head);
DStackRet dstack_gettop(dstack* head, void** data);
DStackRet dstack_forsearch(dstack* head, ,void* ctx);
size_t dstack_length(dstack* head);


#ifdef _cpp
}
#endif /*_cpp*/

#ednif/*DSTACK_H*/
