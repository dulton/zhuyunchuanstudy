#ifndef DLIST_H
#define DLIST_H

#ifdef __CPP			//扩展可以直接给C++调用
extern "C"
{
#endif

struct _dlist;			//实现数据封装，在dlist.c中定义
typedef struct _dlist dlist; 

typedef enum dlist_ret		
{
	DLIST_RET_OK,
	DLIST_RET_OOM,
	DLIST_RET_STOP,
	DLIST_RET_PARAMS,
	DLIST_RET_FAIL,
}dlist_ret;

typedef dlist_ret (*dlist_data_print_fun)(void* data);   //通过使用回调函数，提高了dlist_print的通用性

	dlist* dlist_creat(void);
	dlist_ret dlist_destroy(dlist* head);       

	dlist_ret dlist_insert(dlist* head, size_t index, void* data);
	dlist_ret dlist_delete(dlist* head, size_t index);
	dlist_ret dlist_get_by_index(dlist* head, size_t index, void** data);
	dlist_ret dlist_set_by_index(dlist* head, size_t index, void* data);
	dlist_ret dlist_append(dlist* head, void* data);
	dlist_ret dlist_prepend(dlist* head, void* data);
	size_t dlist_length(dlist* head);
	dlist_ret dlist_print(dlist* head, dlist_data_print_fun print);
	
	
#ifdef __CPP
}
#endif

#endif /*DLIST_H*/
