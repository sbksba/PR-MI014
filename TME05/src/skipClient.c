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

#include "skipClient.h"
#include "skipShare.h"

sigset_t sig;
struct sigaction sigact;

req_client_t *requete;
char *nom_client,*nom_server;
Stockage *stock;

void handler()
{
  puts("Ask for client ending");
  int taille=strlen(nom_client);
  sem_wait(&(stock->poster_message));
  strcpy(stock->req.content,nom_client);
  stock->req.content[taille]='\0';
  stock->req.type=2;
  sem_post(&(stock->prevenir_server));

  sem_wait(&(stock->deco_sem));
  
#ifdef DEBUG
  printf("++++++++++++++++++++\n+[LIBERATE MEMORY Requete]\n");
#endif
  
  if(munmap(requete, sizeof(req_client_t))==-1)
    {
      perror("munmap : skipClient requete\n");
      exit(1);
    }

#ifdef DEBUG
  printf("[SUCCESS]\n+[LIBERATE MEMORY Stock]\n");
#endif
  
  if(munmap(stock, sizeof(Stockage))==-1)
    {
      perror("munmap : skipClient stock\n");
      exit(1);
    }
  
#ifdef DEBUG
  printf("[SUCCESS]\n+[SHM UNLINK]\n");
#endif
  
  if(shm_unlink(nom_client)==-1)
    {
      perror("shm_unlink : skipClient\n");
      exit(1);
    }
  
#ifdef DEBUG
  printf("[SUCCESS]\n");
#endif
  exit(0);
}

void handler_fils()
{
  if(munmap(requete, sizeof(req_client_t))==-1)
    {
      perror("munmap : skipClient requete\n");
      exit(1);
    }
  
  if(munmap(stock, sizeof(Stockage))==-1)
    {
      perror("munmap : skipClient stock\n");
      exit(1);
    }
  
  puts("Exit Client");
  exit(0);
}

int main(int argc,char **argv)
{
  int fd,fd2;
  char *arg1=strdup(argv[1]),*arg2=strdup(argv[2]);
  
  if(argc!=3)
    {
      printf("INVALIDE number of arguments: %s <client_id> <server_id>",argv[0]);
      exit(1);
    }
  
  sigfillset(&sig);
  sigdelset(&sig,SIGINT);
  sigprocmask(SIG_SETMASK,&sig,NULL);

  sigact.sa_mask=sig;
  sigact.sa_handler=handler;
  sigact.sa_flags=0;
  sigaction(SIGINT,&sigact,0);

  nom_client=strdup(strcat(arg1,"_shm:0"));
  nom_server=strdup(strcat(arg2,"_shm:0"));
  
  
  printf("SERVER NAME => [%s]\n",nom_server);

#ifdef DEBUG
  printf("++++++++++++++++++++\n+[CREATE SHM Server]\n");
#endif

  if((fd2=shm_open(nom_server,O_RDWR,0600))==-1)
    {
      fprintf(stderr,"SkipShare doesn't exist yet\n");
      exit(-1);
    }
  
#ifdef DEBUG
  printf("[SUCCESS]\n+[MEMORY ATTACHEMENT Server]\n");
#endif
  
  if((stock=mmap(NULL,sizeof(Stockage),PROT_READ|PROT_WRITE,MAP_SHARED,fd2,0))==MAP_FAILED)
    {
      perror("mmap\n");
      exit(-3);
    }
  
#ifdef DEBUG
  printf("[SUCCESS]\n+[CREATE SHM Client]\n");
#endif

  /*Debut Verif Client*/
  if((fd=shm_open(nom_client,O_RDWR,0600))!=-1)
    {
      fprintf(stderr,"Client already exist\n");
      exit(-1);
    }
  
#ifdef DEBUG
  printf("[SUCCESS]\n+[Create SHM]\n");
#endif
  
  if((fd=shm_open(nom_client,O_RDWR | O_CREAT,0600))==-1)
    {
      perror("shm_open\n");
      exit(-1);
    }
  
#ifdef DEBUG
  printf("[SUCCESS]\n+[ALLOCATE MEMORY]\n");
#endif
  
  if(ftruncate(fd,sizeof(req_client_t))==-1)
    {
      perror("ftruncate\n");
      exit(-2);
    }

#ifdef DEBUG
  printf("[SUCCESS]\n+[MEMORY ATTACHEMENT]\n");
#endif
  
  if((requete=mmap(NULL,sizeof(req_client_t),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)
    {
      perror("mmap\n");
      exit(-3);
    }
  
#ifdef DEBUG
  printf("[SUCCESS]\n++++++++++++++++++++\n");
#endif
  
  /*Future syncro de diffusion*/
  sem_init(&(requete->autoris_diff),1,0);

  /*Envoyer une connexion au server*/
  sem_wait(&(stock->poster_message));
  stock->req.type=0;
  strcpy(stock->req.content,nom_client);
  stock->req.content[strlen(nom_client)]='\0';
  sem_post(&(stock->prevenir_server));

  char buffer[CONTENT_SIZE];
  int taille=0,j;
  pid_t pid;
  
  switch(pid=fork())
    {
    case -1:
      perror("Fork Client\n");
      handler();
    case 0:
      /*Stock message*/
      while(1)
	{
	  if ((taille=read(STDIN_FILENO, buffer, CONTENT_SIZE)) == -1)
	    perror("read");
	  fflush(stdin);
	  /*Equivalant au \n, donc l'auteur veut diffuser du vide. Belle tentative...*/
	  if(taille==1)
	    continue;
	  sem_wait(&(stock->poster_message));
	  strcpy(stock->req.content,buffer);
	  stock->req.content[taille]='\0';
	  stock->req.type=1;
	  sem_post(&(stock->prevenir_server));
	  buffer[0]='\0';
	}
      break;
    default :
      sigact.sa_mask=sig;
      sigact.sa_handler=handler_fils;
      sigact.sa_flags=0;
      sigaction(SIGINT,&sigact,0);
      while(1)
	{
	  /*Pere,gere la diffusion*/
	  sem_wait(&(requete->autoris_diff));
	  /*au lieu de se trimbaler avec la taille,chaque client gere sa table*/
	  /*Il s'agit de ne pas surcharger la diffusion du serveur ici*/
	  
	  j=0;
	  while(requete->req.content[j]!='\0')
	    j++;
	  
	  if ((write(STDOUT_FILENO, requete->req.content, j)) == -1)
	    perror("read");
	  fflush(stdout);
	  
	  sem_post(&(stock->fin_diffusion));
	}
      break;
    }
  
  return EXIT_SUCCESS;
}
