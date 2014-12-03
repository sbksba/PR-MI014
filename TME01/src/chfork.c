#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char **argv){
  srand(time(NULL));
  int i=0, size, j,alea=0;
  size = atoi(argv[1]);
  pid_t tab[size],p;
  tab[i] = getpid();

  for (i=0;((i<size) && ((p=fork())==0));i++){tab[i]=getpid();}
  
  if(i!=size)
    {
      if(waitpid(p, &alea , 0) == -1)
	{
	  perror("waitpid");
	  return -1;
	}
      if(i==0)
	{
	  printf("FATHER of ALL. My pid is: %d and my son is %d\n", getpid(), p);
	  printf("ALEA( JACTA EST ) -- %d\n",WEXITSTATUS(alea));
	}
      else
	{
	  printf("My pid is: %d, my father is : %d and my son is: %d\n", getpid(), getppid(), p);
	  return WEXITSTATUS(alea);
	}
    }
  else if(i==size)
    {
      printf("%d (number %d) I am the Last ONE (there can be only one).\n\tList of pid : [ ",getpid(),i);
      for(j=0; j<size; j++)
	printf("%d ",tab[j]);
      alea = (int)(rand () /(((double) RAND_MAX +1) /100));
      printf("]\n\tALEA -- %d \n\n",alea);
      return alea;
    }

  return EXIT_SUCCESS;
}

