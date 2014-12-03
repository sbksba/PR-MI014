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
#include <semaphore.h> 
#include <signal.h> 
#include <time.h> 
#include <stdbool.h>

#define NB_PROCS 5

typedef struct{
  int baguettes[NB_PROCS];
  sem_t semtab[NB_PROCS] , regulator_terminator;
}esp_com;

esp_com *com;
pid_t pid;

void handler()
{
  #ifdef DEBUG
  printf("++++++++++++++++++++\n+[LIBERATE MEMORY]\n");
  #endif
  if(munmap(com, sizeof(esp_com))==-1)
    {
      perror("munmap : father\n");
      exit(1);
    }
  #ifdef DEBUG
  printf("[SUCCESS]\n");
  #endif

  if(getpid()==pid)
    {
      #ifdef DEBUG
      printf("+[UNLINK SHM]\n");
      #endif
      if(shm_unlink("myshmsem")==-1)
	{
	  perror("shm_unlink\n");
	  exit(1);
	}
      #ifdef DEBUG
      printf("[SUCCESS]\n");
      #endif
      puts("END of the Diner");
    }
  exit(0);
}

void manger(int i)
{
  printf("Nietzsche [%d] mange\n",i);
  sleep((int)(rand()%5));
}

void penser(int i)
{
  printf("Nietzsche [%d] pense\n",i);
  sleep((int)(rand()%5));
}

int main()
{
  sigset_t sig;
  struct sigaction sigact;
  
  sigfillset(&sig);
  sigdelset(&sig,SIGINT);
  sigprocmask(SIG_SETMASK,&sig,NULL);
  
  sigact.sa_mask=sig;
  sigact.sa_handler=handler;
  sigact.sa_flags=0;
  sigaction(SIGINT,&sigact,0);
  
  int i,fd;
  pid=getpid();
  bool gauche=false,droite=false;

  #ifdef DEBUG
  printf("+[CREATE SHM]\n");
  #endif
  
  if((fd=shm_open("myshmsem",O_RDWR | O_CREAT | O_TRUNC,0600))==-1)
    {
      perror("shm_open\n");
      exit(-1);
    }
  
  #ifdef DEBUG
  printf("[SUCCESS]\n+[ALLOCATE MEMORY]\n");
  #endif
  
  if(ftruncate(fd,sizeof(esp_com))==-1)
    {
      perror("ftruncate\n");
      exit(-2);
    }
  
  #ifdef DEBUG
  printf("[SUCCESS]\n+[MEMORY ATTACHEMENT]\n");
  #endif
  
  if((com=mmap(NULL,sizeof(esp_com),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)
    {
      perror("mmap\n");
      exit(-3);
    }
  
  #ifdef DEBUG
  printf("[SUCCESS]\n++++++++++++++++++++\n");
  #endif
  
  /*init structure*/
  for(i=0;i<NB_PROCS;i++)
    {
      com->baguettes[i]=1;
      sem_init(&(com->semtab[i]),1,1);
    }
  sem_init(&(com->regulator_terminator),1,1);
  
  for(i=0;i<NB_PROCS;i++)
    {
      switch(fork())
	{
	case -1:
	  perror("Create process FAILED\n");
	  exit(1);
	case 0:
	  srand(time(NULL) ^ (getpid()<<16));
	  while(1)
	    {
	      sem_wait(&(com->regulator_terminator));
	      if(com->baguettes[i]==0)
		gauche=true;
	      if(com->baguettes[(i+1)%NB_PROCS]==0)
		droite=true;
	      if(!droite&&!gauche)
		{
		  com->baguettes[i]=0;
		  com->baguettes[(i+1)%NB_PROCS]=0;
		  sem_post(&(com->regulator_terminator));
		  manger(i);
		  sem_wait(&(com->regulator_terminator));
		  com->baguettes[i]=1; com->baguettes[(i+1)%NB_PROCS]=1;
		  sem_post(&(com->regulator_terminator));
		  penser(i);
		}
	      else
		{
		  sem_post(&(com->regulator_terminator));
		}
	      if(gauche==true)
		{
		  sem_wait(&(com->semtab[i]));
		  sem_post(&(com->semtab[i]));
		}
	      if(droite==true)
		{
		  sem_wait(&(com->semtab[(i+1)%NB_PROCS]));
		  sem_post(&(com->semtab[(i+1)%NB_PROCS]));
		}
	      gauche=false; droite=false;
	    }
	  break;

	default:
      
	  break;
	}
    }
  
  for(i=0;i<NB_PROCS;i++)
    wait(NULL);

  return EXIT_SUCCESS;
}
