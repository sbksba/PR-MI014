#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define NBMAX 100
#define NB_PROCS 5

int main(int argc,char**argv){
  srand(time(NULL));
  pid_t id[NBMAX],father=getpid();
  int i,som=0,fdp,fdf;
  struct flock lock;
  struct stat info;
  
  if (argc!=2)
    {
      printf("USE: %s fic\n",argv[0]);
      return EXIT_FAILURE;
    }
  
  if ((fdp=open(argv[1],O_RDONLY|O_CREAT|O_TRUNC,0600))==-1)
    {
    perror("father open\n");
    return EXIT_FAILURE;
  }
	
  for(i=0;i<NB_PROCS;i++)
    if(!(id[i]=fork()))
      {
	if ((fdf=open(argv[1],O_RDWR|O_SYNC|O_APPEND))==-1)
	  {
	    perror("father open\n");
	    return EXIT_FAILURE;
	  }
	
	lock.l_start=0;
	lock.l_len=0;
	lock.l_pid=id[i];
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	fcntl(fdf,F_SETLKW,&lock);
	som = (int) (10*(float)rand()/ RAND_MAX)+i;
	fstat(fdf,&info);
	printf("PROC [%d] => %d, fic size -> %ld octet\n",getpid(),i,(long)info.st_size);
	
	if(write(fdf,&som,sizeof(int))==-1) perror("die\n");
	
	printf("PROC [%d] => %d, SOM      -> %d\n",getpid(),i,som);
	lock.l_type=F_UNLCK;
	while(fcntl(fdf,F_SETLK,&lock)==-1)
	  {
	    sleep(1);
	    printf("\tPROC [%d] => %d, ending\n",getpid(),i);
	  }
	close(fdf);
	break;
      }
  
  while(wait(NULL)!=-1);
  if(father==getpid())
    {
      for(i=0;i<NB_PROCS;i++)
	{
	  if (read(fdp,&fdf,sizeof(int))==-1)
	    {
	      perror("son read\n");
	      return EXIT_FAILURE;
	    }
	  printf("PROC [%d] => %d\n",fdf,i);
	  som+=fdf;
	}
      printf("\nSOM = %d\n",som);
      printf("FATHER end\n\n");
    }
  else
    printf("PROC [%d] end\n\n",getpid());

  close(fdp);
  
  return EXIT_SUCCESS;
}
