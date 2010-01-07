#include <stdio.h>
#include <stdlib.h>
//#include <alloc.h>
#include "dlist_v2.h"

/*定义节点结构体*/
typedef struct _dlistnode
{
	struct _dlistnode* pre;
	struct _dlistnode* next;
		
	void* data;
}dlistnode;


/*头节点*/
struct _dlist
{
	dlistnode* first;

};

static dlistnode* dlist_node_creat(void* data)
{
	dlistnode* new = malloc(sizeof(dlistnode));
	
	if (new != NULL)
	{
		new->next = NULL;
		new->pre = NULL;
		new->data = data;
		
		return new;
	}
	else
        {
		exit(1);
	}
}

static dlist_ret dlist_node_destroy(dlistnode* node)
{
	//dlistnode* tmp = node;
	
	if (node != NULL)
	{
		node->next = NULL;
		node->pre = NULL;

		free(node);		
		node = NULL;
	}
	
	return DLIST_RET_OK;
}

static dlistnode* dlist_get_node(dlist* head, size_t index, int fail_return_last)
{
	dlistnode* tmp = head->first;
	
	while (tmp != NULL && tmp->next != NULL && index > 0)
	{
		tmp = tmp->next;
		
		index--;
	}
	
	/*if (head->first == NULL)
	{
		return DLIST_RET_FAIL;
	}*/

	//if (index < dlist_length(head))
	//{
	//if ()
	//}

	if (!fail_return_last)
	{
		tmp = index > 0 ? NULL : tmp;
	}

	return tmp;
};

dlist* dlist_creat(void)
{
	dlist* head = malloc(sizeof(dlist));
       
	if (head != NULL)
	{
		head->first = NULL;
	}
	
	return head;
}

dlist_ret dlist_destroy(dlist* head)
{
	dlistnode* tmp = head->first;
	
	while (tmp != NULL)
	{
		dlistnode* pre = tmp;
		tmp = tmp->next;
		
		free(pre);
		pre = NULL;
	}
	
	head->first = NULL;
	free(head);
	head = NULL;
	
	return DLIST_RET_OK;
}

size_t dlist_length(dlist* head)
{
	dlistnode* tmp = head->first;
	size_t count = 0;
	
	while (tmp != NULL)
	{			
		tmp = tmp->next;
		count++;
        }
	
	return count;
	
}

dlist_ret dlist_print(dlist* head, dlist_data_print_fun print)
{
	dlist_ret ret = DLIST_RET_OK;
	dlistnode* tmp = head->first;
	
	while (tmp != NULL)
	{
		print(tmp->data);
		
		tmp = tmp->next;
	}
	
	return ret;
}

dlist_ret dlist_insert(dlist* head, size_t index, void* data)
{
	dlistnode* tmp = NULL;
	dlistnode* new = dlist_node_creat(data);

	if (NULL == new)
	{
		return DLIST_RET_FAIL;
	}
	
	if (NULL == head->first)
	{
		head->first = new;
		return DLIST_RET_OK;
		//return DLIST_RET_FAIL;
	}
	
	tmp = dlist_get_node(head, index, 1);

	if (index < dlist_length(head))          //如果位置在链表范围内
	{
		new->next = tmp;

		if (tmp == head->first)
		{		
			tmp->pre = new;
			head->first = tmp;
			/*return DLIST_RET_OK;*/
		}
		else 
		{
			new->pre = tmp->pre;
			tmp->pre->next = new;
			tmp->pre = new;
		}
		//if (tmp == head->first)        //如果是first节点
		//{
		//	head->first->pre = new;
		//	new->next = head->first;
		//	head->first = new;
		//	
		//}
		//else
		//{
		//	tmp->pre->next = new;
		//	new->pre = tmp->pre;
		//	tmp->pre = new;
		//	new->next = tmp;
		//}
	}
	
	else
	{
		tmp->next = new;
		new->pre = tmp;
	}

	return DLIST_RET_OK;      
}

dlist_ret dlist_delete(dlist* head, size_t index)
{
	dlistnode* tmp = dlist_get_node(head, index, 0);
	
	//我的实现
	//if (NULL == tmp)
	//{
	//	return DLIST_RET_FAIL;	
	//}
	//
	//if (tmp == head->first)
	//{
	//	dlist_node_destroy(tmp);
	//	
	//	head->first = NULL;
	//	
	//	return DLIST_RET_OK;
	//}
	//
	//if (index < length(head))
	//{
	//	tmp->next->pre = tmp->pre;
	//	tmp->pre->next = tmp->next;
	//	
	//	dlist_destroy(tmp);
	//	
	//	/*return DLIST_RET_OK; */
	//}

	//return DLIST_RET_OK;
	
	if (tmp != NULL)
	{
		if (tmp == head->first)
		{
			head->first = NULL;
		}
		
		if (tmp->next != NULL)
		{
			tmp->pre->next = tmp->next;
		}
		
		if (tmp->pre != NULL)
		{
			tmp->next->pre = tmp->pre;
		}
		
		dlist_node_destroy(tmp);
	}
	
	return DLIST_RET_OK;
}

dlist_ret dlist_append(dlist* head, void* data)
{
	return dlist_insert(head, -1, data);
}

dlist_ret dlist_prepend(dlist* head, void* data)
{
	return dlist_insert(head, 0, data);	
}

int dlist_foreach_find(dlist* head, dlist_data_compare_fun cmp, void* ctr)
{
	//dlsit_ret ret = DLIST_RET_OK;
	dlsit_node* tmp = head->first;
	int i = 0;

	while (tmp != NULL)
	{
		
	}
	
}
