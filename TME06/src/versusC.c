#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define NBPROC 3
#define NBLETTER 26

int main(int argc, char **argv)
{
  FILE *f;
  int i;
  char c='a';

  if (argc!=2)
    {
      printf("USE: %s fic\n",argv[0]);
      return EXIT_FAILURE;
    }

  if ((f=fopen(argv[1],"r"))==NULL)
    {
      perror("fopen\n");
      return EXIT_FAILURE;
    }

  for (i=0; i<NBPROC; i++)
    {
      if (fork()==0)
	{
	  while ((c=fgetc(f))!=EOF)
	    {
	      printf("PROC [%d] => read [%c]\n",i,c);
	      sleep(1);
	    }
	  break;
	}
    }
  fclose(f);
  while (wait(NULL)!=-1);

  return EXIT_SUCCESS;
}
