#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include "max_func.h"

int main(int argc, char **argv){
	int taille = argc-1;
	int tab [taille], i, max;
	
	if (argc < 2)
		printf("ERREUR : Usage ./%s number(s)\n",argv[0]);
	
	for(i=0;i<taille;i++)
		tab[i] = atoi(argv[i+1]);	
	max = max_func(tab,taille);
	printf("\tLe maximum est : %d\n", max);
	
	return EXIT_SUCCESS;
}

