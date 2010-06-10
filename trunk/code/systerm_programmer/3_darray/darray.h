#include <typedef.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef DARRAY_H
#define DARRAY_H

DECLS_BEGIN

struct _DArray;
typedef struct _DArray DArray;

DArray* darray_create(DataDestroyFunc data_destroy, void* ctx);

Ret	darray_insert(DArray* thiz, size_t index, void* data);
Ret darray_prepend(DArray* thiz, void* data);
Ret darray_append(DArray* thiz, void* data);
Ret darray_delete(DArray* thiz, size_t index);
Ret darray_get_by_index(DArray* thiz, size_t index, void** data);
Ret darray_set_by_index(DArray* thiz, size_t index, void* Data);
size_t darray_length(DArray* thiz);
Ret darray_find(DArray* thiz, DataCompareFunc cmp, void* ctx);
Ret darray_search(DArray* thiz, DataVistFunc visit, void* ctx);

void darray_destroy(DArray* thiz);

DECLS_END

#endif/*DARRAY_H*/
