#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#define SIZEPILE 100

char pile[SIZEPILE];
int stack_size = 0;

pthread_mutex_t mutex_pile = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_pile_est_non_vide = PTHREAD_COND_INITIALIZER;

int est_pleine_PRODUC() {return (stack_size >= SIZEPILE);}
int est_vide_PRODUC() {return (stack_size == 0);}

void Push(char c)
{
  pthread_mutex_lock(&mutex_pile);
  int etait_vide = est_vide_PRODUC();
  if (! est_pleine_PRODUC())
    {
      int place = stack_size;
      stack_size++;
      pile[place] = c;
    }
  else
    {
      #ifdef DEBUG
      printf("[%s] PILE PLEINE\n",__FUNCTION__);
      #endif
    }

  if (etait_vide)
    {
      #ifdef DEBUG
      printf("<<< BROADCAST >>>\n");
      #endif
      pthread_cond_broadcast(&cond_pile_est_non_vide);
    }

  pthread_mutex_unlock(&mutex_pile);
  return;
}

char Pop()
{
  pthread_mutex_lock(&mutex_pile);
  
  while( est_vide_PRODUC() )
    pthread_cond_wait(&cond_pile_est_non_vide, &mutex_pile);

  stack_size--;
  char result = pile[stack_size];
  
  pthread_mutex_unlock(&mutex_pile);
  return result;
}

