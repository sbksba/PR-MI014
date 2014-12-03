#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>

int max_func(int *tab, int size){
	int i, max = 0;
	for(i=0;i<size;i++)
		if(max<tab[i]) max=tab[i];
	
	return max;
}
