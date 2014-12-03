#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/mman.h>

int main(int argc,char**argv)
{
  struct stat info;
  int fd1,fd2,max,i;
  char inv[100],c;
  
  if (argc!=2)
    {
      printf("USE: %s <fic>",argv[0]);
      return EXIT_FAILURE;
    }
  
  strcpy(inv,argv[1]);
  strcat(inv,".inv");
  
  if ((fd1=open(argv[1],O_RDONLY))==-1)
    {
      perror("[OPEN fd1]");
      return EXIT_FAILURE;
    }
  
  if ((fd2=open(inv,O_WRONLY|O_TRUNC|O_CREAT,0600))==-1)
    {
      perror("[OPEN fd2]");
      return EXIT_FAILURE;
    }
  
  if(fstat(fd1,&info)==-1)
    {
      perror("fstat");
      return EXIT_FAILURE;
    }
  
  max=info.st_size;
  
  for (i=max-1; i>=0; i--)
    {
      pread(fd1,&c,sizeof(char),i);
      if(c!='\n'&&c!='\0')
	write(fd2,&c,sizeof(char));
      #ifdef DEBUG
      printf("Char => [%c]\n",(char)c);
      #endif
    }
    
  close(fd1);
  close(fd2);
  return EXIT_SUCCESS;
}
