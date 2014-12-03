#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

pthread_mutex_t mutex_dernier_thread = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_dernier_thread = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex_libere = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_libere = PTHREAD_COND_INITIALIZER;

void* chaine(void* arg)
{
  int nb = (*((int *) arg));
  #ifdef DEBUG
  printf("[%s] -> nb°'%d'\n",__FUNCTION__,nb);
  #endif

  if (nb <= 1)
    {
      pthread_mutex_lock(&mutex_libere);
      pthread_cond_signal(&cond_dernier_thread);
      
      pthread_cond_wait(&cond_libere, &mutex_libere);
      pthread_mutex_unlock(&mutex_libere);
    }
  else
    {
      int res;
      pthread_t sub;
      int* sub_arg = malloc(sizeof(int));
      (*sub_arg) = nb - 1;
      res = pthread_create(&sub, NULL, chaine, sub_arg);
      if (0 != res)
	{
	  perror("pthread_create failed\n");
	  exit(res);
	}
      
      pthread_join(sub, NULL);
    }
  #ifdef DEBUG
  printf("[%s] -> nb°'%d' : EXIT\n",__FUNCTION__ ,nb);
  #endif
  pthread_exit(NULL);
}

void handler_sigint(int s) {return;}

int main(int args, char* argv[])
{
  int res;
  int N = 3;
  
  if (N <= 0) {exit(0);}

  sigset_t new_mask;
  sigfillset(&new_mask);
  sigprocmask(SIG_BLOCK, &new_mask, NULL);

  pthread_t first;
  int* first_arg = malloc(sizeof(int));
  (*first_arg) = N;
  res = pthread_create(&first, NULL, chaine, first_arg);
  if (0 != res)
    {
      perror("pthread_create failed\n");
      exit(res);
    }

  pthread_mutex_lock(&mutex_dernier_thread);
  pthread_cond_wait(&cond_dernier_thread, &mutex_dernier_thread);
  pthread_mutex_unlock(&mutex_dernier_thread);
  
  printf("[%s] Tous mes decendants sont crees\n",__FUNCTION__);
  
  signal(SIGINT, handler_sigint); /* SIG_INT */
  sigset_t but_int_mask;
  sigemptyset(&but_int_mask);
  sigaddset(&but_int_mask, SIGINT);
  sigprocmask(SIG_UNBLOCK, &but_int_mask, NULL);
  /* racourci, vous que tout les signaux sont bloque seul le sigint passera le 
     pause. Les signaux imblocbles termiront de tout facon le programme
  */
  pause();
  #ifdef DEBUG
  printf("[%s] sigint, ou un signal inblocable recu\n",__FUNCTION__);
  #endif

  pthread_mutex_lock(&mutex_libere);
  pthread_cond_signal(&cond_libere);
  pthread_mutex_unlock(&mutex_libere);

  pthread_join(first, NULL);
  printf("[%s] Tous mes descendants se sont terminés\n",__FUNCTION__);
  
  return EXIT_SUCCESS;
}
