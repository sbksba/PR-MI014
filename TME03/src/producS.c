#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <string.h>

#include "thread_stack.h"

#define NBPRODS 5
#define NBCONSOS 5

void* Producteur(void* arg)
{
  #ifdef DEBUG
  printf("[%s]\n",__FUNCTION__);
  #endif
  int c;
  while ((c = getchar()) != EOF) {Push(c);}
}


void* Consommmateur(void* arg)
{
  #ifdef DEBUG
  printf("[%s]\n",__FUNCTION__);
  #endif
  for (;;)
    {
      putchar(Pop());
      fflush(stdout);
    }
}


int main()
{
  int i,j,k;
  pthread_t tab_tid[NBCONSOS+NBPRODS];

  for(i=0;i<NBPRODS;i++)
    {
      if(pthread_create(&(tab_tid[i]),NULL,Producteur,NULL)!=0)
	{
	  perror("Creating Failure");
	  exit(-1);
	}
    }
  
  for(j=0;j<NBCONSOS;j++)
    {
      if(pthread_create(&(tab_tid[j+i]),NULL,Consommmateur,NULL)!=0)
	{
	  perror("Creating Failure");
	  exit(-1);
	}
    }
  
  for(k=0;k<NBPRODS+NBCONSOS;k++)
    {
      if(pthread_join(tab_tid[k],NULL)!=0)
	{
	  perror("Creating Failure");
	  exit(-1);
	}
    }

  return EXIT_SUCCESS;
}
