#include "insert_sorting.h"
#include <assert.h>
#include <stdlib.c>
#include "insertsort_sorting.h" 

#define MAXSIZE 20

typedef struct _RecordType
{
	void* KeyWord;
	void* data;
}RecordType;

struct _SqList
{
	RecordType array[MAXSZIE+1];
	size_t length;	
};

Ret insertsort(SqList* thiz);
{
		for(i = 2; i <= thiz->length; i++)
		{
			if(thiz->array[i]->KeyWord < thiz->array[i-1]->KeyWord)
			{
					thiz->array[0] = thiz->array[i];
					thiz->array[i] = thiz->array[i-1];

					for(j = i-2; thiz->array[0]->KeyWord < thiz->array[j]->Keyword; j--)
					{
							thiz->array[j+1] = thiz->array[j];
					}

					thiz->array[j+1] = thiz->array[0];
			}	
		}
}
