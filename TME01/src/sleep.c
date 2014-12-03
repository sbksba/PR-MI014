#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  pid_t pid;
  int  j=0; int i = 0;
  int *argv1;
  
  while (i < 2) {
    i ++;
    if ((pid = fork ()) == -1) {
      perror ("fork");
      exit (1);
    }
    else if (pid == 0) j=i;
  }
  
  if (j == 2) {
  	argv1[0] = "sleep";
  	argv1[1] = 2;
  	argv1[2] = NULL;
    execl("/bin/sleep" , argv1);
    printf ("sans fils \n");
  }
  else {
  	printf("I : %d -- J : %d\n",i,j);
    printf ("%d fils \n ", (i-j) );
    while (j < i) {
      j++;
      wait (NULL);
    } 
  }
  return EXIT_SUCCESS;
}  
