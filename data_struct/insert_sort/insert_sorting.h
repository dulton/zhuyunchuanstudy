#ifndef INSERTSORTING_H
#define INSERTSORITNG_H

#include <stdio.h>
#include <stdlib.h>
#include "dlist.h"
#include "typedef.h"
 
DECLS_BEGIN

struct _SqList;
typedef _SqList SqList;
/*
 * arg: array and the length of array
 * return: none
 */
Ret insertsort(SqList* thiz);

DECLS_BEGIN

