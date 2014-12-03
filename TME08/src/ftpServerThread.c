#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pthread.h>

#include "ftpServerThread.h"

#define FILENAME_SIZE 128
#define BUFFER_SIZE 1024
#define TAILLE_REQUETE 1024
#define NUM_PORT 9876
#define NB_THREADS 5

list *creer_element()
{
  list *res = malloc(sizeof(list));
  if(!res)
    {
      perror("erreur alloc list");
      exit(1);
    }
  return res;
}

void *traitement_requete(void *arg)
{
  list *premier , *tmp_list;
  premier = creer_element();
  int scom = *(int *)arg;
  char tmp_char;
  int lu , rep;
  char data_dir[FILENAME_SIZE] = "ftp_database_server/";
  char *fusion;
  DIR *pt_Dir;
  struct dirent* dirEnt;
  struct stat buf;
  int size;
  int size_tmp;
  int tmp_fd;
  char filename[FILENAME_SIZE];
  char buffer[BUFFER_SIZE];
  char requete[BUFFER_SIZE];

  while(1)
    {
      puts("debut");
      if (read(scom,requete, sizeof(char)*TAILLE_REQUETE) < 0)
	{
	  perror("READ DU WHILE"); 
	  exit(4);
	}
      printf("requete = %s\n",requete);
      
      switch(requete[0])
	{
	  /* D */
	case 68:
	  sscanf(requete,"%*s %s\n",filename);
	  
	  /*O_TRUNC, violence au maximum*/
	  fusion = strcat(data_dir , filename);
	  tmp_fd = open( fusion ,  O_RDONLY , 0666 );
	  /*Dire au client s'il doit attendre qqch ou non*/
	  if(tmp_fd ==-1)
	    {
	      rep='0';
	      if(write(scom,&rep,sizeof(char))==-1) {perror("erreur lecture");exit(1);}
	      size = 0;
	      
	      if(write(scom,&size,sizeof(int))==-1) {perror("erreur lecture");exit(1);}
	      puts("erreur ouverture fichier");
	      continue;
	    }
	  else
	    {
	      rep='1';
	      if(write(scom,&rep,sizeof(char))==-1) {perror("erreur lecture");exit(1);}
	      if(fstat(tmp_fd, &buf)==-1) {perror("probleme utilisation fstat");exit(1);}
	      /*Taille fic*/
	      if(write(scom,&buf.st_size,sizeof(int))==-1) {perror("erreur lecture");exit(1);}
	    }
	  
	  while((lu=read(tmp_fd,buffer,sizeof(char)*BUFFER_SIZE)) > 0)
	    {
	      if(write((int)scom,buffer,sizeof(char)*lu)==-1) {perror("erreur lecture");exit(1);}
	    }
	  
	  strcpy(data_dir , "ftp_database_server/");
	  close(tmp_fd);
	  break;

	  /* L */
	case 76:
	  tmp_list = premier;
	  size_tmp = 0;
	  if ( ( pt_Dir = opendir ("ftp_database_server") ) == NULL) {perror("erreur opendir");exit(2);}
	  
	  while ((dirEnt= readdir (pt_Dir)) !=NULL)
	    {
	      tmp_list->name = dirEnt->d_name;
	      size_tmp++;
	      if(tmp_list->suivant == NULL)
		tmp_list->suivant = creer_element();
	      
	      tmp_list = tmp_list->suivant;
	    }
	  tmp_list->name = NULL;
	  tmp_list->suivant = NULL;
	  
	  if(write(scom,&size_tmp,sizeof(int))==-1) {perror("erreur lecture");exit(1);}
	  
	  tmp_list = premier;
	  while(tmp_list->name != NULL)
	    {
	      if(write(scom,tmp_list->name,FILENAME_SIZE)==-1) {perror("erreur lecture");exit(1);}
	      tmp_list = tmp_list->suivant;
	    }

	  closedir (pt_Dir);
	  tmp_char = EOF;

	  if(write(scom,&tmp_char,sizeof(char))==-1) {perror("erreur lecture");exit(1);}
	  break;
	  
	  /* U */
	case 85:
	  sscanf(requete,"%*s %s\n",filename);
	  
	  /*O_TRUNC, violence au maximum*/
	  fusion = strcat(data_dir , filename);
	  printf("fusion = %s\n",fusion);
	  
	  if(read(scom,&size,sizeof(int))==-1) {perror("erreur lecture");exit(1);}
	  
	  tmp_fd = open( fusion ,  O_WRONLY | O_CREAT | O_TRUNC , 0666 );
	  size_tmp = 0;
	  while(size_tmp++ < (size / BUFFER_SIZE) + 1)
	    {
	      if((lu=read(scom,buffer,sizeof(char)*BUFFER_SIZE)) < 0 ) {perror("erreur lecture");exit(1);}
	      if(write(tmp_fd,buffer,sizeof(char)*lu)==-1) {perror("erreur lecture");exit(1);}
	    }
	  strcpy(data_dir , "ftp_database_server/");
	  close(tmp_fd);
	  break;
	default:
	  exit(0);
	}
    }
  
  pthread_exit(0);
}

int main(/*int argc, char **argv*/)
{
  struct sockaddr_in sin; /* Nom de la socket de connexion */
  int sc ;
  /* Socket de connexion */
  int scom;
  /* Socket de communication */
  struct sockaddr_in exp;
  /*struct hostent *hp;*/
  int fromlen = sizeof(exp);

  if ((sc = socket(AF_INET,SOCK_STREAM,0)) < 0) {perror("socket"); exit(1);}
  
  memset((char *)&sin,0, sizeof(sin));
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_port = htons(NUM_PORT);
  sin.sin_family = AF_INET;

  if (bind(sc,(struct sockaddr *)&sin,sizeof(sin)) < 0) {perror("bind");exit(1);}
  
  listen(sc, NB_THREADS);

  int i,tid[NB_THREADS];
  int *ptr_a;

  for (i = 0 ; i < NB_THREADS ; i++)
    {
      if ( (scom = accept(sc, (struct sockaddr *)&exp, (socklen_t *)&fromlen))== -1) {perror("accept");exit(2);}
      ptr_a = &scom;
      /* Creation d'un thread qui traite la requete */
      pthread_create((pthread_t *)&tid[i], 0, traitement_requete, (void *)ptr_a);
    }
  
  for (i = 0;i < 4; i++)
    pthread_join(tid[i], 0);
  
  return EXIT_SUCCESS;
}
