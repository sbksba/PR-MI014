#define _XOPEN_SOURCE 500
#define POSIX_1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>


__sighandler_t previous_SIGUSR2_handler;

void handle_P3_ending(int sig) {
  fprintf(stdout, "Processus P3 terminé\n");
  return;
}

void handle_P3_creation(int sig) {
  fprintf(stdout, "Processus P3 créé\n");
  /* now we can put the handle of SIGUSR2 */
  /* we create the handling to SIGUSR2, for P3 ending */
  previous_SIGUSR2_handler = signal(SIGUSR2, handle_P3_ending);
  sigrelse(SIGUSR2);
  return;
}

int main(int argc, char **argv) {

  pid_t grandfather_pid = getpid();

  /* we create the handling to SIGUSR1, for P3 creation */
  sigset_t sig_proc_P3_creation;
  struct sigaction action_handle_P3_creation;
  sigemptyset(&sig_proc_P3_creation);
  action_handle_P3_creation.sa_mask = sig_proc_P3_creation;
  action_handle_P3_creation.sa_flags = 0;
  action_handle_P3_creation.sa_handler = handle_P3_creation;
  sigaction(SIGUSR1, &action_handle_P3_creation, 0);
  /* done */

  /* we mask this SIGUSR2 because we want to always get the nb 1 before */
  sighold(SIGUSR2);

  /* we are in P1*/
  if (fork() == 0) {
    if (fork() ==0) {
      /* we signal P3 to the grandfather P1*/
      kill(grandfather_pid, SIGUSR1);
      exit(0);
    } else {
      /* sub father, P2*/
      wait(NULL);
      kill(grandfather_pid, SIGUSR2);
      exit(0);
    };
    perror("impossible");
  } else {
    wait(NULL);
    fprintf(stdout, "Processus P2 terminé\n");
    exit(0);
  }
  perror("impossible");
  exit(-1);
}
