#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include "create.h"

#define N 4
#define dbg printf("FUNCTION : %s -- LINE : %d\n",__FUNCTION__,__LINE__)

void* thread_rand(void* args)
{
  thread_args* my_args = (thread_args*) args;
  pthread_t me = pthread_self();
  fprintf(stderr, "thread_rand for nb°%d; my tid='%lu'\n", my_args->nb, me);

  int* res = malloc(sizeof(int));
  (*res) = my_args->nb * 2;
  
  #ifdef DEBUG
  dbg;
  #endif

  return ((void*) res);
}

int main(int args, char* argv[])
{
  int i;
  pthread_t my_threads[N];
  for (i=0; i < N; i++)
    {
      int res;
      thread_args* his_args = malloc(sizeof(thread_args));
      his_args->nb = i;
      if ( 0 == (res = pthread_create(&(my_threads[i]), NULL, thread_rand, his_args)) )
	{
	  #ifdef DEBUG
	  dbg;
          #endif
	}
      else
	{
	  perror("pthread_create failed");
	  exit(res);
	}
    }

  #ifdef DEBUG
  dbg;
  #endif
  
  for (i=0; i < N; i++)
    {
      void** aresult;
      if (pthread_join(my_threads[i], &aresult) != 0)
	{
	  perror("pthread_join failed");
	  exit(errno);
	}
      int result = (int) (*aresult);
      printf("I joined with some1and i got '%d'\n", (result));
    }
  
  printf("all my thread are finish\n");
  
  return EXIT_SUCCESS;
}
