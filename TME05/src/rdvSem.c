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

#define NB_PROCS 5

typedef struct _semaph{
  sem_t sem_pere,sem_fils;
}sema;
sema *semtab;

void wait_barrier(/*int arg*/)
{
  sem_post(&(semtab->sem_pere));
  sem_wait(&(semtab->sem_fils));
}

void process (int NB_PCS) 
{
  printf ("avant barrière\n");
  wait_barrier(NB_PCS);
  printf ("après barrière\n");
  exit (0);
}

/*
   Pas besoin de compteur partage
   Le pere attends NB_PROCS fois que tous ses fils envoient un V() sur son semaphore 
   (Le pere balance NB_PROCS fois le P() sur son semaphore). 
   Dans ce cas là, on sait que tous les fils ont dejà affiche "avant barrière".
   Par la suite, les fils se mettent en attente sur leur semaphore à eux (sem_fils), 
   et le père envoie NB_PROCS V() sur le sem_fils, debloquant ainsi tous les fils.
   Les fils demappent la zone de partage et se terminent.
   Le pere attend la fin des fils, demonte tout, et se termine.
*/

int main()
{
  int fd,i;
  pid_t pid;

  #ifdef DEBUG
  printf("+[CREATE SHM]\n");
  #endif
  
  if((fd=shm_open("myshmsem",O_RDWR | O_CREAT,0600))==-1)
    {
      perror("shm_open");
      exit(-1);
    }
  
  #ifdef DEBUG
  printf("[SUCCESS]\n+[ALLOCATE MEMORY]\n");
  #endif

  if(ftruncate(fd,sizeof(sema))==-1)
    {
      perror("ftruncate");
      exit(-2);
    }

  #ifdef DEBUG
  printf("[SUCCESS]\n+[MEMORY ATTACHEMENT]\n");
  #endif

  if((semtab=mmap(NULL,sizeof(sema),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)
    {
      perror("mmap");
      exit(-3);
    }
  
  #ifdef DEBUG
  printf("[SUCCESS]\n++++++++++++++++++++\n");
  #endif

  sem_init(&(semtab->sem_pere),1,0);
  sem_init(&(semtab->sem_fils), 1, 0);

  for(i=0;i<NB_PROCS;i++)
    {
      switch(pid=fork())
	{
	case -1:
	  perror("Create son FAILED");
	  exit(-1);
	case 0:
	process(NB_PROCS);
	if(munmap(semtab, sizeof(sema))==-1)
	  {
	    perror("Unmap sons semtab FAILED\n");
	    exit(1);
	  }
	return 0;
	default:
	  break;
	}
    }
  
  for(i=0;i<NB_PROCS;i++) sem_wait(&(semtab->sem_pere));
  for(i=0;i<NB_PROCS;i++) sem_post(&(semtab->sem_fils));
  for(i=0;i<NB_PROCS;i++) wait(NULL);

  /*Destruction semaphores + demappage + destruction de l'espace partage */
  puts("END OF SONS => main");
  
  #ifdef DEBUG
  printf("++++++++++++++++++++\n+[SEM DESTROY FATHER]\n");
  #endif
  
  if(sem_destroy(&(semtab->sem_pere))==-1)
    {
      perror("sem_destroy : father\n");
      exit(1);
    }
  
  #ifdef DEBUG
  printf("[SUCCESS]\n+[SEM DESTROY SONS]\n");
  #endif
  
  if(sem_destroy(&(semtab->sem_fils))==-1)
    {
      perror("sem_destroy : sons\n");
      exit(1);
    }
  
  #ifdef DEBUG
  printf("[SUCCESS]\n+[LIBERATE MEMORY]\n");
  #endif
  
  if(munmap(semtab, sizeof(sema))==-1)
    {
      perror("munmap\n");
      exit(1);
    }

  #ifdef DEBUG
  printf("[SUCCESS]\n+[UNLINK SHM]\n");
  #endif

  if(shm_unlink("myshmsem")==-1)
    {
      perror("shm_unlink\n");
      exit(1);
    }
  
  #ifdef DEBUG
  printf("[SUCCESS]\n++++++++++++++++++++\n");
  #endif
  
  return EXIT_SUCCESS;
}
