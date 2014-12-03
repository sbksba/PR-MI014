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

int main(int argc,char**argv)
{
  int fd,i;
  char c;

  if (argc!=2)
    {
      printf("USE: %s fic\n",argv[0]);
      return EXIT_FAILURE;
    }
  
  if ((fd=open(argv[1],O_WRONLY|O_CREAT|O_SYNC|O_TRUNC,0600))==-1)
    {
      perror("First open\n");
      return EXIT_FAILURE;
    }
  
  for (i=0; i<NBLETTER; i++)
    {
      c='a'+i;
      write(fd,&c,sizeof(c));
    }
  close(fd);
  
  if ((fd=open(argv[1],O_RDONLY,0600))==-1)
    {
      perror("Second open\n");
      return EXIT_FAILURE;
    }
  
  for (i=0; i<NBPROC; i++)
    {
      if (fork()==0)
	{
	  while (read(fd,&c,sizeof(char))>0) {
	    printf("PROC [%d] => read [%c]\n",i,c);
	    sleep(1);
	  }
	  break;
	}
    }
  close(fd);
  while (wait(NULL)!=-1);
  
  return EXIT_SUCCESS;
}
