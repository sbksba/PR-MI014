#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <string.h>

#include "thread_stack.h"

#define NBPRODS 2
#define NBCONSOS 2

void* Producteur(void* arg)
{
  int c;
  #ifdef DEBUG
  printf("[%s]\n",__FUNCTION__);
  #endif

  while((c = getchar()) != EOF) {Push(c);}

  #ifdef DEBUG
  printf("[%s] -> EOF => completed\n",__FUNCTION__);
  #endif
  pthread_exit(NULL);
}

void* Consommateur(void* arg)
{
  #ifdef DEBUG
  printf("[%s]\n",__FUNCTION__);
  #endif
  for(;;)
    {
      putchar(Pop());
      fflush(stdout);
    }
  
  pthread_exit(NULL);
}

int main()
{
  int res, i;
  pthread_t prod_threads[NBPRODS];
  pthread_t conso_thread[NBCONSOS];

  for (i = 0; i < NBPRODS; i++)
    {
      res = pthread_create(&(prod_threads[i]), NULL, Producteur, NULL);
      if (0 != res)
	{
	  perror("pthread_create failed\n");
	  exit(res);
	}
    }

  for (i = 0; i < NBCONSOS; i++)
    {
      res = pthread_create(&(conso_thread[i]), NULL, Consommateur, NULL);
      if (0 != res)
	{
	  perror("pthread_create failed\n");
	  exit(res);
	}
    }
  
  for (i = 0; i < NBPRODS; i++)
    pthread_join(prod_threads[i], NULL);

  for (i = 0; i < NBCONSOS; i++)
    pthread_join(conso_thread[i], NULL);

  #ifdef DEBUG
  printf("[%s] all my thread are done\n",__FUNCTION__);
  #endif
  return EXIT_SUCCESS;
}
