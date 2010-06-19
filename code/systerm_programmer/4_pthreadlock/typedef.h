#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifndef TYPEDEF_H
#define TYPEDEF_H

typedef enum _Ret
{
	RET_OK,
	RET_OOM,
	RET_STOP,
	RET_INVALID_PARAMS,
	RET_FAIL,	
}Ret;

typedef void (*DataDestroyFunc)(void* ctx, void* data);
typedef int  (*DataCompareFunc)(void* ctx, void* data);
typedef Ret	 (*DataVisitFunc)(void* ctx, void* data); 

#ifdef _cpluscplus
#define DECLS_BEGIN extern "C" {
#define DECLS_END }
#else
#define DECLS_BEGIN
#define DECLS_END
#endif/*_cpluscplus*/

#define return_if_fail(p) if(!(p)) \
	{printf("%s,%d Warning: "#p" failed.\n",\
		__func__, __LINE__); return;}
#define return_val_if_fail(p) if(!(p)) \
	{printf("%s,%d Warning: "#p" failed. \n", \
		__func__, __LINE__); return(ret);}
#define SAFE_FREE(p) if((p) != NULL) {free(p); p = NULL;}

typedef Ret (*SortFunc)(void** array, size_t nr, DataCompareFunc cmp);

#endif/*TYPEDEF_H*/
