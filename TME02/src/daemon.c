#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#include "daemon.h"

#define N 4

pthread_mutex_t mutex_alive_count = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_somme = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_somme_ready = PTHREAD_COND_INITIALIZER;
int somme, alive_count = N;

int random_val() {return (int) (10*((double)rand())/ RAND_MAX);}

char* string_of_detachedstate(int detachstate)
{
  switch(detachstate)
    {
    case PTHREAD_CREATE_JOINABLE : {return strdup("PTHREAD_CREATE_JOINABLE");}
    case PTHREAD_CREATE_DETACHED : {return strdup("PTHREAD_CREATE_DETACHED");}
    default : {perror("invalide switch");exit(2);}
    }
}

void* print_thread(void* args)
{
  pthread_mutex_lock(&mutex_somme);
  pthread_cond_wait(&cond_somme_ready, &mutex_somme);
  /*we got the lock*/
  fflush(NULL);
  fprintf(stderr, "[in print_thread] the final sum is '%d'\n", somme);
  pthread_mutex_unlock(&mutex_somme);
  
  return NULL;
}

void* thread_rand(void* args)
{
  thread_args* my_args = (thread_args*) args;
  pthread_t me = pthread_self();
  int a_random = random_val();
  int detachstate;
  pthread_attr_getdetachstate(my_args->attr, &detachstate);
  fprintf(stderr, "thread_rand for nb°%d; my tid='%lu'; i generated '%d'; my detach state is %s; and alive=%d\n", my_args->nb, me, a_random, string_of_detachedstate(detachstate), alive_count);
  pthread_mutex_lock(&mutex_somme);
  somme += a_random;
  pthread_mutex_unlock(&mutex_somme);

  my_args->the_returned_value = my_args->nb * 2;

  pthread_mutex_lock(&mutex_alive_count);
  if (alive_count <= 1)
    {
      /*we are the last one*/
      fprintf(stderr, "sending signal for cond\n");
      pthread_cond_signal(&cond_somme_ready);
    }
  alive_count--;
  pthread_mutex_unlock(&mutex_alive_count);
  
  return NULL;
}

/*thread_t my_threads[N];*/
int main(int args, char* argv[])
{
  srand(time(NULL));
  int res;
  somme = 0;
  pthread_t the_print_thread;
  
  if ( 0 == (res = pthread_create(&(the_print_thread), NULL, print_thread, NULL)) ) {}
  else
    {
      perror("pthread_create for print_thread failed");
      exit(res);
    }
  
  int i;
  thread_t* my_threads = malloc(sizeof(thread_t) * N);
  for (i=0; i < N; i++)
    {
      my_threads[i].his_args.nb = i;
      my_threads[i].his_args.attr = malloc(sizeof(pthread_attr_t));
      my_threads[i].his_args.the_returned_value = 0;
      pthread_attr_init(my_threads[i].his_args.attr);

      if ( 0 == (res = pthread_create(&(my_threads[i].t), my_threads[i].his_args.attr, thread_rand, &(my_threads[i].his_args))) )
	{
	  pthread_attr_setdetachstate(my_threads[i].his_args.attr, PTHREAD_CREATE_DETACHED);
	}
      else
	{
	  perror("pthread_create failed");
	  exit(res);
	}
    }

  if (0 != pthread_join(the_print_thread, NULL))
    {
      perror("pthread_join on the_print_thread failed");
      exit(errno);
    }
  
  for(i = 0; i < N; i++)
    printf("I retrieve the result of i=%d and i got '%d'\n", i, my_threads[i].his_args.the_returned_value);
  
  printf("all my thread are finish\n");

  return EXIT_SUCCESS;
}
