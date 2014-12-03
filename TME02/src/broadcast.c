#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#include "broadcast.h"

#define NB_THREADS 4

pthread_barrier_t* the_barr;

void wait_barrier(int n)
{
  int res = pthread_barrier_wait(the_barr);
  if ((res != PTHREAD_BARRIER_SERIAL_THREAD) && (res != 0))
    {
      perror("pthread_barrier_wait failed");
      exit(errno);
    }
  return;
}

void* thread_func(void* arg)
{
  printf("avant la barrière\n");
  wait_barrier(NB_THREADS);
  printf("après barrière\n");
  pthread_exit( (void*)0 );
}

int main(int args, char* argv[])
{
  srand(time(NULL));
  the_barr = malloc(sizeof(pthread_barrier_t));
  if ( 0 != pthread_barrier_init(the_barr, NULL, NB_THREADS) ) {perror("pthread_barrier_init failed");exit(errno);}
  int res;
  int i;
  pthread_t my_threads[NB_THREADS];
  for (i=0; i < NB_THREADS; i++)
    {
      thread_args* his_args = malloc(sizeof(thread_args));
      his_args->nb = i;
      his_args->attr = malloc(sizeof(pthread_attr_t));
      pthread_attr_init(his_args->attr);
      /*pthread_attr_setdetachstate(his_args->attr, PTHREAD_CREATE_DETACHED);*/
      if ( 0 == (res = pthread_create(&(my_threads[i]), his_args->attr, thread_func, his_args)) ) {}
      else
	{
	  perror("pthread_create failed");
	  exit(res);
	}
    }

  for (i=0; i < NB_THREADS; i++)
    {
      if (0 != pthread_join(my_threads[i], NULL)) {perror("pthread_join failed");exit(errno);}
      printf("I joined with some1 i='%d'\n", i);
    }

  pthread_barrier_destroy(the_barr);
  printf("all my thread are finish\n");

  return EXIT_SUCCESS;
}
