#ifndef TREE_H
#define TREE_H

#include "dlist.h"
#include "stack.h"
#include "queue.h"
#include "typedef.h"
#include <stdio.h>

typedef struct _Tree;
typedef struct _Tree Tree; 

Ret tree_inorder(Tree* thiz);
Ret tree_preorder(Tree* thiz);
Ret tree_postorder(Tree* thiz);
Ret tree_iter_inorder(Tree* thiz);
Ret tree_iter_preorder(Tree* thiz);
Ret tree_iter_postorder(Tree* thiz);
Ret tree_level_traver(Tree* thiz);

#endif /*TREE_H*/
