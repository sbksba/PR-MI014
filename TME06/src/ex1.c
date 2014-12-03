#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main (void) {
  int fd1, fd2, fd3;
  int tmp;
  pid_t father = getpid();
  sigset_t sig;

  sigemptyset(&sig);
  sigaddset(&sig,SIGUSR1);

  #ifdef DEBUG
  printf("+FATHER [open fd1]\n");
  #endif
  
  if ((fd1 = open ("./fich1", O_RDWR| O_CREAT | O_TRUNC,0600)) == -1)
    return EXIT_FAILURE;
  
  #ifdef DEBUG
  printf("[SUCCESS]\n+FATHER [write fd1 abcde]\n");
  #endif

  if (write (fd1,"abcde", strlen ("abcde")) == -1) /* A */
    return EXIT_FAILURE;

  #ifdef DEBUG
  printf("[SUCCESS]\n+SON [open fd2]\n");
  #endif

  if (fork () == 0) {
    sleep(1);
    if ((fd2 = open ("./fich1", O_RDWR)) == -1)
      return EXIT_FAILURE;
    #ifdef DEBUG
    printf("[SUCCESS]\n+SON [write fd1 123]\n");
    #endif
    if (write (fd1,"123", strlen ("123")) == -1) /* B */
      return EXIT_FAILURE;
    #ifdef DEBUG
    printf("[SUCCESS]\n+SON [write fd2 45]\n");
    #endif
    kill(father,SIGUSR1);
    sleep(1);
    if (write (fd2,"45", strlen ("45")) == -1) /* C */
      return EXIT_FAILURE;
    #ifdef DEBUG
    printf("[SUCCESS]\n");
    #endif
  } else {
    sigwait(&sig,&tmp);
    fd3 = dup(fd1);
    if (lseek (fd3,0,SEEK_SET) == -1) /* D */
      return EXIT_FAILURE;
    #ifdef DEBUG
    printf("+FATHER [write fd3 fg]\n");
    #endif
    if (write (fd3,"fg", strlen ("fg")) == -1) /* E */
      return EXIT_FAILURE;
    #ifdef DEBUG
    printf("[SUCCESS]\n+FATHER [write fd1 hi]\n");
    #endif
    if (write (fd1,"hi", strlen ("hi")) == -1) /* F */
      return EXIT_FAILURE;
    #ifdef DEBUG
    printf("[SUCCESS]\n");
    #endif
    wait (NULL);
    close (fd1);
    close(fd2);
    close(fd3);
    
    return EXIT_SUCCESS;
  }
}
