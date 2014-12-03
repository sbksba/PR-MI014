#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <string.h>

#include "common.h"
#include "pool.h"

#define NB_THREAD 2

pthread_mutex_t mutex_files_jobs = PTHREAD_MUTEX_INITIALIZER;
files_jobs_t* files_jobs;

int est_vide()
{
  int tmp;
  tmp = (files_jobs->first_pos_undone < files_jobs->size)? 0 : 1;
  #ifdef DEBUG
  printf("[%s] -> (%d/%d) => %d\n",__FUNCTION__,files_jobs->first_pos_undone,files_jobs->size,tmp);
  #endif
  return tmp;
}

char* pop()
{
  char* tmp = files_jobs->values[files_jobs->first_pos_undone];
  files_jobs->first_pos_undone++;
  #ifdef DEBUG
  printf("[%s] -> %d\n",__FUNCTION__ ,files_jobs->first_pos_undone);
  #endif
  return tmp;
}

void* thread_fun(void* arg)
{    
  #ifdef DEBUG
  int id = (*((int *) arg));
  printf("[%s nb°'%d'] >>>>> started\n",__FUNCTION__ ,id);
  #endif

  while(1)
    {
      pthread_mutex_lock(&mutex_files_jobs);
      if (est_vide())
	{
	  pthread_mutex_unlock(&mutex_files_jobs);
	  break;
	} 
      else
	{
	  char* target = pop();
	  pthread_mutex_unlock(&mutex_files_jobs);
	  #ifdef DEBUG
	  printf("[%s nb°'%d'] got a new job to do !\n",__FUNCTION__,id);
	  #endif
	  unlimitedPOOL(target);
	}
    }
  pthread_exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
  int i, res;
  pthread_t my_threads[NB_THREAD];

  files_jobs = malloc(sizeof(files_jobs_t));
  files_jobs->first_pos_undone = 0;
  files_jobs->size = argc - 1;
  files_jobs->values = &(argv[1]);
  
  for (i = 0; i < NB_THREAD; i++)
    {
      int* arg = malloc(sizeof(int));
      (*arg) = i;
      res = pthread_create(&(my_threads[i]), NULL, thread_fun, arg);
      if (0 != res)
	{
	  perror("pthread_create failed\n");
	  exit(res);
	}
    }
  
  for (i = 0; i < NB_THREAD; i++)
    pthread_join(my_threads[i], NULL);

  #ifdef DEBUG
  printf("[%s] all my thread are done\n",__FUNCTION__);
  #endif
  
  return EXIT_SUCCESS;
}
