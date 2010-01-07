/*
version1:实现数据的封装。在.h中声明节点的数据结构，在.c中实现具体的节点结构。要数据结构提供相应的操作函数。
version2:实现通用性。1.将节点数据结构中的数据域设计成void *data 2.通用的函数返回值？？  3.通用的函数，能满足对双链表的操作。
version3:
这个双链表维持一个头结点，头节点的数据域存放链表的结点个数，pre的指针为空，next指针指向链表的第一个真正节点.
*/


#include <stdio.h>
#include <stdlib.h>

#ifndef dlist.h
#define dlist.h

#ifdef __cplusplus                          //version2，是函数能在C++中被调用
extern "C"
{
#endif
/*双链表的结点结构*/
struct _Dlist;                         //version1：实现数据封装       
typedef struct _Dlist dlist_node;          //version1

typedef enum  _dlist_ret
{
	DLIST_RET_OK,
	DLIST_RET_OOM,
	DLIST_RET_STOP,
	DLIST_RET_PARAMS,
	DLIST_RET_FAIL,
}DlistRet;

typedef DlistRet (*dlist_data_print)(void *data);

/*双链表的操作*/
	dlist_node* dlist_creat(void);                   //version1
	void dlist_destroy(dlist* head);      //version1

	//DlistRet dlist_init(dlist_node* head, int length);
	DlistRet dlist_insert(dlist* head, int location, void *data);
	//DlistRet *dlist_delte(dlist_node* head, int location);
	//dlist_ret dlist_getnode(dlist_node* head, void *data);
	//dlist_ret dlist_setnode(dlist_node* head, int location, void *data);
	DlistRet dlist_visit(dlist* head, dlist_data_print);
	size_t dlist_length(dlist* head);

#ifdef __cplusplus                        //version2
}
#endif

