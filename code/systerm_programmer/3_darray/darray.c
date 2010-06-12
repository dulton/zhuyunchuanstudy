#include <stdio.h>
#include <assert.h>
#include "darray.h"

struct _DArray
{
	void** data;
	size_t size;
	size_t alloc_size;
		
	void* data_destroy_ctx;
	DataDestroyFunc data_destroy;
};

static void darray_destroy(DArray* thiz, void* data)
{
	if(NULL != thiz->data_destroy_ctx)
	{
		thiz->data_destroy(thiz->data_destroy_ctx, data);
	}

	return;
}

DArray* darray_create(DataDestroyFunc darray_destroy, void* ctx)
{
		DArray* thiz = malloc(sizeof(struct DArray));	

		if(NULL != thiz)
		{
				thiz->data = NULL;
				thiz->size = 0;
				thiz->alloc_size = 0;
				thiz->data_destroy = darray_delete;
				thiz->data_destroy_ctx = ctx;
		}

		return thiz;
}

#define MINI_RELLOC_SIZE 10 
static Ret darray_expand(DArray* thiz, size_t need)
{
		return_val_if_fail(NULL != thiz, RET_INVALID_PARAMS);
		
		if((thiz->size) + nedd >= thiz->alloc_size)	
		{
				size_t alloc_size = (thiz->alloc_size>> 1) + thiz->alloc_size + MINI_RELLOC_SIZE;		

				void** data = (void**)realloc(sizeof(void*) * alloc_size);
				if(NULL != data)
				{
						thiz->data = data;
						thiz->alloc_size = alloc_size;
				}
		}
		
		return  ((thiz->size) + need <= thiz->alloc_size) ? RET_OK : RET_FAIL;
}
<<<<<<< .mine

Ret darray_insert(DArray* thiz, size_t index, void* data)
{
		return_val_if_fail(NULL != thiz, RET_INVALID_PARAMS);

		Ret ret = RET_OOM;
		size_t current = current < thiz->size ? current: thiz->size;

		if(RET_OK == darray_expand(thiz, 1))
		{
				for(size_t i = thiz->size; i > current; i--)
				{
						thiz->data[i] = thiz->data[i-1];
				}

				thiz->data[current] = data;
				thiz->size++;

				ret = RET_OK;
		}

		return ret;
}

Ret darray_append(DArray* thiz, void* data)
{
		return_val_if_fail(NULL != thiz, RET_INVALID_PARAMS);
		
		RET ret = darray_insert(thiz, 0, data);

		return ret;
}

Ret darray_prepend(DArray* thiz, void* data)
{
		return_val_if_fail(NULL != thiz, RET_INVALID_PARAMS);
		
		RET ret = darray_insert(thiz, thiz->size, data);

		return ret;
}

void** darray_get_by_index(DArray* thiz, size_t index)
{
		void* val = NULL;

		return_val_if_fail(NULL != thiz && index < thiz->size, RET_INVALID_PARAMS);

		val = thiz->data[index];

		return val;
}

Ret darray_set_by_index(DArray* thiz, size_t index, void* data)
{
		RET ret = RET_OOM;

		return_val_if_fail(NULL != thiz && index < thiz->size, RET_INVALID_PARAMS);
	
		thiz->data[index] = data;

		return RET_OK;
}



=======

static Ret darray_shrink(DArray* thiz) 
{
		return_val_if_fail(NULL != thiz, RET_INVALID_PARAMS);

		if(thiz->alloc_size < (thiz->size >> 1) && thiz->size >= MINI_RELLOC_SIZE)
		{
				size_t size = thiz->alloc_size + alloc_size >> 1;

				void** data = (void**)realloc(sizeof(void*) * size);
				if(NULL != data)
				{
						thiz->size = size;
						thiz->alloc_size = alloc_size;
						thiz->data = data;
				}
		}

		return RET_OK;
}

Ret darray_append(DArray* thiz, void* data)
{
		return_val_if_fail(NULL != thiz, RET_INVAILD_PARAMS);

					
}
>>>>>>> .r21
