#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "dlist_v2.h"

#define TEST 

static dlist_ret print_int(void* data)
{
	printf("%d", (int)data);

	return DLIST_RET_OK;
}

int main(int argc, char* argv[])
{
	int i = 0;
	int num = 10;
	dlist* head = dlist_creat();
#ifdef TEST	

        //#ifdef DLIST_CREAT_ARRAY
	//dlist_creat_array();
        //#endif  /*DLIST_CREAT_ARRAY*/

	for (i = 0; i < num; i++)
	{
		/*dlist_ret ret = dlist_append(head, (void*)i);
		assert(ret == DLIST_RET_OK);*/
		assert(dlist_append(head, (void*)i) == DLIST_RET_OK);
		/*dlist_append(head, (void*)i);*/
	}
	
	/*for ()
	{
		assert(dlist_prepend() == DLIST_RET_OK);
	}
	*/
	
	//dlist_insert();
	
	//	dlist_delete();
	
	dlist_delete(head, 2);
	dlist_insert(head, 2 ,(void*)11);

	dlist_print(head, print_int);
	
	dlist_destroy(head);
	
	
#endif /*TEST*/
}
