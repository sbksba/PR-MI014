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
#include <string.h>
#include <signal.h>

#include "skipShare.h"

sigset_t sig;
struct sigaction sigact;

char buf[CONTENT_SIZE];
Stockage *stock;
int nb_connect=0;
char *nom_server=NULL;

void handler()
{
  puts("Ask for server ending");
  
  #ifdef DEBUG
  printf("++++++++++++++++++++\n+[LIBERATE MEMORY]\n");
  #endif
  
  if(munmap(stock, sizeof(Stockage))==-1)
    {
      perror("munmap : skipShare\n");
      exit(1);
    }
  
  #ifdef DEBUG
  printf("[SUCCESS]\n+[SHM UNLINK]\n");
  #endif
  
  if(shm_unlink(nom_server)==-1)
    {
      perror("shm_unlink : skipShare\n");
      exit(1);
    }

  #ifdef DEBUG
  printf("[SUCCESS]\n");
  #endif

  exit(0);
}

int connexion()
{
  int i,fd;
  for(i=0;i<NB_CONNECT_MAX;i++)
    {
      if(co_deco_table.tab_connect[i]==NULL)
	{
	  /*Alloc memory stock*/
	  co_deco_table.tab_connect[i]=malloc(sizeof(char)*1024);
	  strcpy(co_deco_table.tab_connect[i],stock->req.content);
	  printf("arg a shm_open = %s\n",stock->req.content);
	  	  
	  if ((fd = shm_open(stock->req.content, O_RDWR,0600)) == -1)
	    {
	      perror("shm_open : connexion");
	      return -1;
	    }
	  
	  if(((co_deco_table.link_table[i])=mmap(NULL, sizeof(req_client_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))== MAP_FAILED)
	    {
	      perror("mmap : connexion\n");
	      return -1;
	    }
	  break;
	}
    }
  
  if(i==NB_CONNECT_MAX) return -1;
  nb_connect++;
  return 0;
}

int deconnexion()
{
  int i;
  for(i=0;i<NB_CONNECT_MAX;i++)
    {
      if(co_deco_table.tab_connect[i]!=NULL)
	{
	  if(strcmp(co_deco_table.tab_connect[i],stock->req.content)==0)
	    {
	      if(munmap(co_deco_table.link_table[i], sizeof(req_client_t))==-1)
		perror("munmap : deconnexion\n");
	      
	      sem_post(&(stock->deco_sem));
	      /*On simule une place disponible au chat*/
	      co_deco_table.tab_connect[i]=NULL;
	      co_deco_table.link_table[i]=NULL;
	      nb_connect--;
	      return 0;
	    }
	}
    }
  
  if(i==NB_CONNECT_MAX)
    return -1;
  return 0;
}

void diffusion()
{
  int i;
  
  for(i=0;i<NB_CONNECT_MAX;i++)
    {
      if(co_deco_table.tab_connect[i]!=NULL)
	{
	  strcpy(co_deco_table.link_table[i]->req.content,stock->req.content);
	  stock->req.type=1;
	  sem_post(&(co_deco_table.link_table[i]->autoris_diff));
	}
    }
  
  for(i=0;i<nb_connect;i++)
    sem_wait(&(stock->fin_diffusion));
}

int main(int argc,char **argv)
{
  int fd,i;
  
  if(argc!=2)
    {
      printf("INVALIDE number of arguments : USE => %s <server_id>",argv[0]);
      return EXIT_FAILURE;
    }
  
  sigfillset(&sig);
  sigdelset(&sig,SIGINT);
  sigprocmask(SIG_SETMASK,&sig,NULL);
  
  sigact.sa_mask=sig;
  sigact.sa_handler=handler;
  sigact.sa_flags=0;
  sigaction(SIGINT,&sigact,0);

  nom_server=strdup(strcat(argv[1],"_shm:0"));
  
  #ifdef DEBUG
  printf("+[CREATE SHM]\n");
  #endif

  if((fd=shm_open(nom_server,O_RDWR | O_CREAT,0600))==-1)
    {
      perror("shm_open : skipShare\n");
      exit(-1);
    }
  
  #ifdef DEBUG
  printf("[SUCCESS]\n+[ALLOCATE MEMORY]\n");
  #endif
  
  if(ftruncate(fd,sizeof(Stockage))==-1)
    {
      perror("ftruncate : skipShare\n");
      exit(-2);
    }

  #ifdef DEBUG
  printf("[SUCCESS]\n+[MEMORY ATTACHEMENT]\n");
  #endif

  if((stock=mmap(NULL,sizeof(Stockage),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)
    {
      perror("mmap : skipShare\n");
      exit(-3);
    }

  #ifdef DEBUG
  printf("[SUCCESS]\n++++++++++++++++++++\n");
  #endif

  printf("SERVER NAME => [%s]\n",nom_server);
  
  /*Initialisation du tableau des connectes, tous sont NULLS!*/
  for(i=0;i<NB_CONNECT_MAX;i++)
    {
      co_deco_table.tab_connect[i]=NULL;
      co_deco_table.link_table[i]=NULL;
    }
  
  /*Initialisation semaphores*/
  sem_init(&(stock->fin_diffusion),1,0);
  sem_init(&(stock->poster_message),1,1);
  sem_init(&(stock->prevenir_server),1,0);
  
  while(1)
    {
      sem_wait(&(stock->prevenir_server));
      switch(stock->req.type)
	{
	case 0:
	  if(connexion()==-1)
	    perror("Connexion FAILED [skipShare still running...]\n");
	  break;
	  
	case 1:
	  diffusion();
	  break;
	  
	case 2:
	  if(deconnexion()==-1)
	    perror("Deconnexion FAILED [skipShare still running...]\n");
	  break;
	  
	default:
	  puts("Unknow message");
	  continue;
	}
      
      sem_post(&(stock->poster_message));
    }
  return EXIT_SUCCESS;
}
