#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

#include "create.h"

#define N 4

int somme;

int random_val() {return (int) (10*((double)rand())/ RAND_MAX);}

void* thread_rand(void* args)
{
  thread_args* my_args = (thread_args*) args;
  pthread_t me = pthread_self();
  int a_random = random_val();
  fprintf(stderr, "thread_rand for nb°%d; my tid='%lu'; i generated '%d'\n", my_args->nb, me, a_random);
  somme += a_random;
  int* res = malloc(sizeof(int));
  (*res) = my_args->nb * 2;

  return ((void*) res);
}

int main(int args, char* argv[])
{
  srand(time(NULL));
  somme = 0;
  int i;
  pthread_t my_threads[N];
  for (i=0; i < N; i++)
    {
      int res;
      thread_args* his_args = malloc(sizeof(thread_args));
      his_args->nb = i;
      if ( 0 == (res = pthread_create(&(my_threads[i]), NULL, thread_rand, his_args)) ) {}
      else
	{
	  perror("pthread_create failed");
	  exit(res);
	}
    }

  for (i=0; i < N; i++)
    {
      void** aresult;
      if (0 != pthread_join(my_threads[i], &aresult)) {
	perror("pthread_join failed");
	exit(errno);
      }
      int result = (int) (*aresult);
      printf("I joined with some1and i got '%d'\n", (result));
    }
  
  printf("all my thread are finish\n");
  printf("the final sum is '%d'\n", somme);

  return EXIT_SUCCESS;
}
