#include <stdio.h>
#include <stdlib.h>
#include "shellsort.h"

void shell_sort_impl(void* array[], int dlat, int length)
{
	void* tmp = NULL;

	for(int i = dlat+1; i <= length; i++)
        {
		if (array[i] )
	        {
			tmp = array[i];
			for(int j = i - dlat; j >= 0 && array[j] > array[j-dlat]; j += dlat)
			{

			}
		
		}
	}
}

void shell_sort(void* array[], int increasment[], int t)
{
	for(i = 1; i <= t; i++)
	{
		shell_sort_impl(array, increasment[i]);
	}
}
