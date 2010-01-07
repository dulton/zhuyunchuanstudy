#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include "dlist.h"

/*双链表结点结构*/
//version1:实现封装
typedef struct _Dlistnode
{
	_Dlistnode* next;
	_Dlistnode* pre;

	void *data;                //version2:实现通用性
}dlist_node;

struct dlist
{
	dlist_node* first,
	int count,
};

/*创建一个节点*/
static dlist_node* dlist_node_creat(void *data)
{
	dlist_node* newnode = malloc(sizeof(dlist_node));
	
	if (newnode != NULL)
	{
		newnode->pre = NULL;
		newnode->next = NULL;

		newnode->data = data;
	}
	
	return newnode;
}


/*销毁一个节点*/
static void dlist_node_destroy(dlist_node* node)
{
	if (node != NULL)
	{
		node->pre = NULL;
		node->next = NULL;
		
		free(node);
		
		node = NULL;
	}

	return; 
}

/**/

/*
funtion：建立一个空链表
in：
out：头指针
*/

dlist* dlist_creat(void)
{
	dlist* head = malloc(sizeof(dlist_node));

	//head->data = NULL;
	//head->next = NULL;
	//head->pre = NULL;

	if (head != NULL)
	{
		head->first = NULL;
		
		head->count = 0;
	}

	return head;
}

/*根据数据域搜索某个节点，返回这个节点的指针*/
static dlist_node* dlist_get_node(dlist* head, size_t index, int fail_return_last)
{
	dlist_node* tmp = head->first;

	while (tmp != NULL && tmp->next != NULL && index > 0)
	{
		tmp = tmp->next;
		index--;
	}

	if (!fail_return_last)
	{
		tmp = index > 0 ? NULL : tmp;
	}
	
	return tmp; 
}



void dlist_destroy(dlist* head)
{
	dlist_node* tmp = head->first;

	while(tmp != NULL)
	{
		dlist_node* current = tmp;
		tmp = tmp->next;
		
		dlist_node_destroy(current);
	
		head->count--;	
	}
	
	dlist_node_destroy(tmp);
	
	head->first = NULL;
	free(head);
	head = NULL;

	return;
}

/*dlist_ret dlist_insert(dlist_node* head, int location, void *insert_data)
{
	dlist_node* tmp = head->first;
	
	if (tmp != NULL)
	{
		for (int i = 0; tmp->next != NULL, i < location; i++)
		{
			tmp = tmp->next;
		}
		
		dlist_node* new = dlist_node_creat(insert_data);

		tmp->next->pre = new;
		tmp->next = new;
		new->pre = tmp;
		new->next = tmp->next;
	}//end of if(tmp->next != NULL) 
	
	
	return 0;

}*/

DlistRet dlist_insert(dlist* head, size_t index, void* data)
{
	dlist_node* tmp = NULL;
	dlist_node* new = NULL;

	if ((new = dlist_node_creat(data)) == NULL)
	{
		return DLIST_RET_OOM;
	}
	
	if (head->first == NULL )
	{
		head->first = new;
		new->next = NULL;

		return DLIST_RET_OK;
	}
	
	tmp = dlist_get_node(head, index, 1)
	
	if (index < dlist_length(nead))
      	{
		tmp->next->pre = new;
		new->next = tmp->next;
		new->pre = tmp;
		tmp->next = new;

		//if (tmp == head->first)
		//{
			
		//}
	}
        else
        {
	       tmp->next = node;
	       node->pre = tmp;
        }
	
	return DLIST_RET_OK;	
}


/*void* dlist_delete(dlist_node* head, int location)
{
	dlist_node* tmp = head;
	
	if (tmp->next != NULL)
	{
		for (int i = 0; i < location; i++)
		{
			tmp = tmp->next;
		}
		
		void *datacpy = tmp->next->data;
      
		free(tmp);
		tmp = NULL;
	}
	
	return *datacpy;
}

/*获取结点数据域为*data的结点位置*/
/*int dlist_getnode(dlist_node* head, void *data)  
{
	dlist_node* tmp = head;
	int count = 0;

        if (tmp->next != NULL)
        {
		while (tmp->data != data)
		{
			count++;
			tmp = tmp->next;
		}
	}
	
	return count;
}*/

/*链表的遍历*/
 /*void dlist_visit(dlist_node* head)
{
	dlist_node* tmp = head;
	
	for (; tmp->next != NULL; tmp = tmp-next)
	{
		printf("the data = %p", tmp->data);
	}
	
	return 0;
}
 */

 size_t dlist_length(dlist* head)
{
	size_t length = 0;
	
	dlist_node* tmp = head->first;
	
	while (tmp != NULL)
	{
		tmp = tmp->next;
		length++;
	}
	
	return length;
}

DlistRet dlist_visit(dlist* head, dlistdataprintfun print)
{
	DlistRet ret = DLIST_RET_OK;
	dlist_node* tmp = head->first;
	
	while (tmp != NULL)
	{
		print(tmp->data);
		tmp = tmp->next;
	}

	return ret;
}


