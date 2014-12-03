#define SVID_SOURCE 1

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#include "thread_stack.h"


#define NB_CONS 5
#define NB_PROD 5

void* Producteur(void* arg)
{
  int c;
  while ((c = getchar()) != EOF) {Push(c);}
  return 0;
}

void* Consommmateur(void* arg)
{
  for (;;)
    {
      putchar(Pop());
      fflush(stdout);
    }
}

int main()
{
  int i,j,k;
  pthread_t tab_tid[NB_CONS+NB_PROD];
  
  initialisation();
  
  for(i=0;i<NB_PROD;i++)
    {
      if(pthread_create(&(tab_tid[i]),NULL,Producteur,NULL)!=0)
	{
	  perror("Creating Failure");
	  exit(-1);
	}
    }
  
  for(j=0;j<NB_CONS;j++)
    {
      if(pthread_create(&(tab_tid[j+i]),NULL,Consommmateur,NULL)!=0)
	{
	  perror("Creating Failure");
	  exit(-1);
	}
    }
  
  for(k=0;k<NB_PROD+NB_CONS;k++)
    {
      if(pthread_join(tab_tid[k],NULL)!=0)
	{
	  perror("Creating Failure");
	  exit(-1);
	}
    }

  return EXIT_SUCCESS;
}
