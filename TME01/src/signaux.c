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
pid_t child_pid;

void handle_sigusr1(int sig)
{
  fprintf(stderr, "SIGUSR1 receive\n");
  fprintf(stdout, "Tous les descendants suspendus\n");
  kill(child_pid, SIGCONT);
  return;
}

void chaine(pid_t my_gfather, int nb)
{
  if (nb <= 0)
    {
      sleep(1);
      int i;
      printf("{printed by '%d'} chaine= ", getpid());
      for (i = 0; i < N; i++) {printf("%d;", result[N - 1 - i]);}
    printf(".\n");
    kill(first_pid, SIGUSR1);
    printf("sending usr1 to '%d'\n", first_pid);
    kill(getpid(), SIGSTOP);
    sleep(1);
    kill(first_pid, SIGUSR2);
    exit(0);
  };

  signal(SIGUSR1, handle_sigusr1);

  pid_t pere_pid = getpid();
  result[nb - 1] = pere_pid;

  int pid_fils = fork();
  if (pid_fils == 0) {chaine(pere_pid, nb - 1);}
  else
    {
    child_pid = pid_fils;

    if (getpid() != first_pid)
      {
	fprintf(stderr, "going to stop pid=%d\n", getpid());
	raise(SIGSTOP);
	sleep(0);
	
	fprintf(stderr, "i am resuming pid=%d\n", getpid());
	kill(child_pid, SIGCONT);
	exit(0);
      };
    
    return;
    }
}


int main(int argc, char **argv)
{
  first_pid = getpid();
  fprintf(stderr, "main; pid=%d; N=%d\n", getpid(), N);
  result = malloc(sizeof(pid_t) * N);

  signal(SIGUSR1, handle_sigusr1);

  sigset_t mask, previousmask;
  sigemptyset (&mask);
  sigaddset (&mask, SIGUSR2);
  sigprocmask (SIG_BLOCK, &mask, &previousmask);
  chaine(-1, N);
  while ( 1 )
    {
      fprintf(stderr, "suspend ZZzzz\n");
      sigsuspend (&previousmask);
      fprintf(stderr, "after suspend\n");
    }
  sigprocmask (SIG_UNBLOCK, &mask, NULL);

  return EXIT_SUCCESS;
}
