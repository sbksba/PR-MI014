#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
  struct stat statnews;
  int fd1,fd2,i;
  char tmp[128];
  if (argc != 3)
    {
      printf("USE : %s <fic1> <fic2>\n",argv[0]);
      return EXIT_FAILURE;
    }

  if (stat(argv[1], &statnews) == -1)
    {
      perror("stat\n");
      return EXIT_FAILURE;
    }
  
  if ((fd1=open(argv[1],O_RDONLY)) == -1)
    {
      perror("ERROR => fic1 doesn't exist\n");
      return EXIT_FAILURE;
    }

  if (!access(argv[2], F_OK))
    {
      perror("ERROR => fic2 exist\n");
      return EXIT_FAILURE;
    }
  
  if ((fd2=open(argv[2],O_WRONLY|O_CREAT,0600)) == -1)
    {
      perror("ERROR => we can't open fic2\n");
      return EXIT_FAILURE;
    }

  if (statnews.st_mode & !S_IFREG)
    {
      perror("ERROR => fic1 isn't a regular file\n");
      return EXIT_FAILURE;
    }

  while ((i=read(fd1,tmp,128*sizeof(char)))>0)
    {
      if (write(fd2,tmp,i)==-1)
	{
	  perror("write\n");
	  return EXIT_FAILURE;
        }
    }
  
  close(fd1);
  close(fd2);
  
  return EXIT_SUCCESS;
}
