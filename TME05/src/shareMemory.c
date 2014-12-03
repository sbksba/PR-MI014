#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <time.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h> 

#define NB_PROCS 5

int *sp;

int main()
{
  int fd,i,res=0;;
  pid_t pid;
  
  #ifdef DEBUG
  printf("+[CREATE SHM]\n");
  #endif
  
  /*Creation de descripteur de memoire partagee dans la table*/
  if ((fd = shm_open("myshm", O_RDWR | O_CREAT,0600)) == -1)
    {
      perror("shm_open");
      exit(1);
    }
  
  #ifdef DEBUG
  printf("[SUCCESS]\n+[ALLOCATE MEMORY]\n");
  #endif

  /*Allocation de la memoire partagee*/
  if (ftruncate(fd, sizeof(int)*NB_PROCS) == -1)
    {
      perror("ftruncate");
      exit(1);
    }

  #ifdef DEBUG
  printf("[SUCCESS]\n+[MEMORY ATTACHEMENT]\n");
  #endif
  
  /*Attachement de la memoire partagee au processus courant*/
  if ((sp = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))== MAP_FAILED)
    {
      perror("mmap");
      exit(1);
  }
  
  #ifdef DEBUG
  printf("[SUCCESS]\n++++++++++++++++++++\n");
  #endif

  for(i=0;i<NB_PROCS;i++)
    {
      switch(pid=fork())
	{
	case -1:
	  perror("Create son FAILED");
	  exit(-1);
	case 0:
	  srand(time(NULL) ^ (getpid()<<16));
	  *(sp+i*sizeof(int))=(int) (10*(float)rand()/ RAND_MAX);
	  printf("Val in Memory => [%d]\n",*(sp + i*sizeof(int)));
	  if(munmap(sp, sizeof(int)*NB_PROCS)==-1)
	    {
	      perror("Unmap son process FAILED\n");
	      exit(1);
	    }
	  return 0;
	default:
	  break;
	}
    }

  for(i=0;i<NB_PROCS;i++)
    wait(NULL);

  for(i=0;i<NB_PROCS;i++)
    {
      printf("Val read by Main => [%d]\n",*(sp + i*sizeof(int)));
      res+=*(sp+sizeof(int)*i);
    }


  printf("Final Result => [%d]\n",res);
  puts("END OF SONS");

  #ifdef DEBUG
  printf("++++++++++++++++++++\n+[LIBERATE MEMORY]\n");
  #endif

  /*Liberation de l'espace partage*/
  if(munmap(sp, sizeof(int)*NB_PROCS)==-1)
    {
      perror("Unmap father process FAILED\n");
      exit(1);
    }
  
  #ifdef DEBUG
  printf("[SUCCESS]\n+[UNLINK SHM]\n");
  #endif

  if(shm_unlink("myshm")==-1)
    {
      perror("Erase share memory area FAILED\n");
      exit(1);
    }

  #ifdef DEBUG
  printf("[SUCCESS]\n++++++++++++++++++++\n");
  #endif

  return EXIT_SUCCESS;
}
