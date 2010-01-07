#include <asset.h>
#include <stdio.h>
#include "tree.h"

typedef struct _tree_node
{
	_tree_node* leftchild;
	_tree_node* rightchild;
	void* data;
}tree_node;

struct _Tree 
{
	tree_node* root;
};


tree_inorder(Tree* thiz, DataFunprint print, void* ctr)
{
	tree_node* tmp = thiz->root;

	if (tmp != NULL)
	{
		inorder(tmp->leftchild, print, );
		print();
		inorder(tmp->leftchild, print, );
	}
}

tree_iter_inorder(Tree* thiz, DataFunprint print, void* ctr)
{
	tree_node* tmp = thiz->root;
	stack_creat();
	stack_push(, tmp);

	while{!stack_empty}
	{
		for( ; tmp != NULL; tmp = tmp->leftchild)
		{
			stack_push(thiz, tmp->data);
		}
		if (!stack_empty)
		{
			pop();
			tmp = tmp->right;
		}
	}
}

tree_iter_preorder(Tree* thiz, ,void* ctr)
{
	tree_node* tmp = thiz->root;
	= stack_creat();

	if(tmp != NULL)
	{
		stack_push( ,tmp);
		while (!stack_empty())
		{
			p = stack_pop();
		}
		
		if (tmp->rightchild != NULL)
		{
			stack_push(tmp->rightchild);
		}

		if (tmp->leftchild != NULL)
		{
			stack_push(tmp->leftchild);
		}
	}
     
}

tree_iter_postorder()
{
	tree_node* tmp = thiz->root;
	= stack_creat();

	if 
}


tree_level_traver(Tree* thiz)
{
	tree_node* tmp = thiz->root;
	= queue_creat();
	queue_enqueue( ,tmp);
	
	while (!queue_empty)
	{
		 = queue_dequeue();
		visit();

		if (tmp->leftchild != NULL)
		{
			queue_enqueue(, tmp->rightchild);
		}
		if (tmp->rightchild != NULL)
		{
			queue_enqueue(, tmp->righrchild);
		}
	}
}
