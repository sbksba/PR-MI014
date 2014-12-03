#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>

const int N = 4;
pid_t * result;
pid_t first_pid;

void handle_sigusr1(int sig)
{
  fprintf(stderr, "SIGUSR1 receive\n");
  if (getpid() == first_pid)
    exit(0);
  else
    {
      kill(getppid(), SIGUSR1);
      exit(0);
    }
}

void chaine(pid_t my_gfather, int nb)
{
  if (nb <= 0)
    {
      int i;
      printf("{printed by '%d'} chaine= ", getpid());
      for (i = 0; i < N; i++) {printf("%d;", result[N - 1 - i]);}
      printf(".\n");
      kill(getppid(), SIGUSR1);
      return;
    }

  signal(SIGUSR1, handle_sigusr1);
  
  pid_t pere_pid = getpid();
  result[nb - 1] = pere_pid;
  
  int pid_fils = fork();
  if (pid_fils == 0)
    {
      chaine(pere_pid, nb - 1);
      exit( 0 );
    } 
  else
    {
    sigset_t mask, previousmask;
    sigemptyset (&mask);
    sigaddset (&mask, SIGUSR1);
    sigprocmask (SIG_BLOCK, &mask, &previousmask);
    while ( 1 )
      {
	sigsuspend (&previousmask);
	sleep(0);
      }
    sigprocmask (SIG_UNBLOCK, &mask, NULL);
    return;
  }
}


int main(int argc, char **argv)
{
  first_pid = getpid();
  fprintf(stderr, "main; pid=%d; N=%d\n", getpid(), N);
  result = malloc(sizeof(pid_t) * N);
  chaine(-1, N);
  
  return EXIT_SUCCESS;
}
