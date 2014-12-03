#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

#include "create.h"

#define N 4


pthread_mutex_t mutex_somme = PTHREAD_MUTEX_INITIALIZER;
int somme;

int random_val() {return (int) (10*((double)rand())/ RAND_MAX);}

void* thread_rand(void* args)
{
  thread_args* my_args = (thread_args*) args;
  pthread_t me = pthread_self();
  int a_random = random_val();
  fprintf(stderr, "thread_rand for nb°%d; my tid='%lu'; i generated '%d'\n", my_args->nb, me, a_random);
  pthread_mutex_lock(&mutex_somme);
  somme += a_random;
  pthread_mutex_unlock(&mutex_somme);

  int* res = malloc(sizeof(int));
  (*res) = my_args->nb * 2;

  pthread_exit((void*) res);
}

void* print_thread(void* args)
{
  fprintf(stderr, "[in print_thread] --> print_thread started\n");
  int res, i;
  pthread_t my_threads[N];
  for (i=0; i < N; i++)
    {
      thread_args* his_args = malloc(sizeof(thread_args));
      his_args->nb = i;
      if ( 0 == (res = pthread_create(&(my_threads[i]), NULL, thread_rand, his_args)) ) {}
      else
	{
	  perror("pthread_create failed");
	  exit(res);
	}
    }

  void** aresult = malloc(sizeof(void**));
  for (i=0; i < N; i++)
    {
      fprintf(stderr, "[in print_thread] waiting for '%d'\n", i);
      if (0 != pthread_join(my_threads[i], &aresult))
	{
	  perror("pthread_join failed");
	  exit(errno);
	}

    int result = (int) (*aresult);
    printf("[in print_thread] i joined with some1 and i got '%d'\n", (result));
  }
  free(aresult);
  pthread_mutex_lock(&mutex_somme);
  printf("[in print_thread] the final sum is '%d'\n", somme);
  pthread_mutex_unlock(&mutex_somme);

  return NULL;
}

int main(int args, char* argv[])
{
  srand(time(NULL));
  somme = 0;
  int res;
  pthread_t the_print_thread;
  if ( 0 == (res = pthread_create(&(the_print_thread), NULL, print_thread, NULL)) ) {}
  else
    {
      perror("pthread_create for print_thread failed");
      exit(res);
    }
  
  if (0 != pthread_join(the_print_thread, NULL))
    {
      perror("pthread_join on the_print_thread failed");
      exit(errno);
    }

  printf("all my thread are finish\n");

  return EXIT_SUCCESS;
}
