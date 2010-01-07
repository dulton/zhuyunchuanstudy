#include <stdio.h>
#include <stdlib.h>
#include "bubblesort.h"

void bubblesort(void* array[], int n)
{
	int i = 0;
	int j = 0;
	void* tmp = NULL;

	for(i = n-1, i >= 0, i--)
	{
		for(j = 1, j <= i, j++)
	        {
			if (array[j] <= array[j-1])
		        {
				tmp = array[j];
				array[j] = array[j-1];
				array[j-1] = tmp;
			}
		}
	}
}
