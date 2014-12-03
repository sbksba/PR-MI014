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

#include "ftpServerThread.h"

#define FILENAME_SIZE 128
#define BUFFER_SIZE 1024
#define TAILLE_REQUETE 1024
#define NUM_PORT 9876

int verif_message(char *message)
{
  /*
    Regarder dans quel cas sommes nous: UPLOAD,LIST ou DOWNLOAD.
    Pour cela, nous regardons la valeur du 1er caractère, et on le place dans switch
  */
  char *tmp=malloc(10);

  switch(message[0])
    {
      /* D */
    case 68:
      tmp=strncpy(tmp , message , 9);
      if(strcmp(tmp , "DOWNLOAD "))
	return -1;
      
      free(tmp);
      return 0;
      /* L */
    case 76:
      tmp=strncpy(tmp , message , 5);
      if(strcmp(tmp , "LIST "))
	return -1;
      
      free(tmp);
      return 0;
      /* U */
    case 85:
      tmp=strncpy(tmp , message , 7);
      if(strcmp(tmp , "UPLOAD "))
	return -1;
      
      free(tmp);
      return 0;
    default:
      free(tmp);
      return -1;
    }
}



/*
  On supposera ici pour faciliter que le stockage des fichiers se fait dans un 
  répértoire "connu et fixe" , nommé "ftp_database"
*/

int main(/*int argc, char **argv*/)
{
  puts("\nATTENTION : toute commande doit être suivie par un espace, puis son argument ou l'absence de son argument ,l'espace est conservé\n");
  struct stat buf;
  char buffer[BUFFER_SIZE];
  char message[TAILLE_REQUETE];
  char filename[FILENAME_SIZE];

  char data_dir[FILENAME_SIZE] = "ftp_database_client/";
  char *fusion;
  int size , size_tmp;



  struct sockaddr_in dest; /* Nom du serveur */
  dest.sin_addr.s_addr = htonl(0);
  dest.sin_port = htons(NUM_PORT);
  dest.sin_family = AF_INET;

  int sock;

  if ((sock = socket(AF_INET,SOCK_STREAM,0)) == -1) {perror("socket");exit(1);}
  if (connect(sock, (struct sockaddr *) &dest, sizeof(dest)) == -1) {perror("connect");exit(1);}

  int tmp_fd = -1;
  int lu;

  while(1)
    {
      puts("DEBUT CLIENT");
      fgets(message, TAILLE_REQUETE, stdin);
      printf("TMP = %s\n",message);
      if(verif_message(message) == -1)
	{
	  puts("requete incorrecte");
	  continue;
	}
      puts("envoie");
      
      if(write(sock,message,sizeof(char)*strlen(message))==-1) {perror("erreur lecture");exit(1);}

      /*Attente en fonction de la 1ere lettre*/
      switch(message[0])
	{
	  /* D */
	case 68:
	  /*Lecture filename*/
	  if(read(sock,buffer,sizeof(char))==-1) {perror("erreur lecture");exit(1);}
	  if(read(sock,&size,sizeof(int))==-1) {perror("erreur lecture");exit(1);}
      
	  sscanf(message,"%*s %s\n",filename);
      
	  if(buffer[0] == '0')
	    {
	      puts("le serveur n'a jamais entendu parler de votre fichier");
	      continue;
	    }
	  else
	    {
	      printf("fichier existe chez server %s\n",buffer);
	    }
	  fusion = strdup(strcat(data_dir , filename));
	  /*O_TRUNC, violence au maximum*/
	  tmp_fd = open( fusion , O_CREAT | O_RDWR | O_TRUNC , 0666 );
	  size_tmp = 0;
	  while(size_tmp < size/BUFFER_SIZE +1)
	    {
	      if((lu=read(sock,buffer,sizeof(char)*BUFFER_SIZE)) == -1) {perror("erreur lecture");exit(1);}
	      if(write(tmp_fd,buffer,sizeof(char)*lu)==-1) {perror("erreur lecture");exit(1);}
	      size_tmp++;
	      puts("boucle 1");
	    }
	  strcpy(data_dir , "ftp_database_client/");
	  close(tmp_fd);
	  break;
	  /* L */
	case 76:
	  puts("affichage de la liste : \n");
	  if(read(sock,&size,sizeof(int))==-1) {perror("erreur lecture");exit(1);}
	  while(size-- >= 0)
	    {
	      if(read(sock , buffer, sizeof(char)*FILENAME_SIZE) == -1) {	perror("erreur lecture");exit(1);}
	      printf("%s\n",buffer);
	    }
	  puts("\n");
	  break;
	  /* U */
	case 85:
	  sscanf(message,"%*s %s\n",filename);
	  fusion = strdup(strcat(data_dir , filename));
	  tmp_fd = open( fusion , O_RDWR  , 0666 );
	  printf("tmp_fd = %d , fusion = %s\n",tmp_fd , fusion);
	
	  /*Taille fic*/
	  if(fstat(tmp_fd, &buf)==-1) {perror("probleme utilisation fstat");exit(1);}
	  if(write(sock,&buf.st_size,sizeof(int))==-1) {perror("erreur lecture");exit(1);}
	
	  while((lu=read(tmp_fd,buffer,sizeof(char)*BUFFER_SIZE)) > 0)
	    {
	      if(write(sock,buffer,sizeof(char)*lu)==-1) {perror("erreur lecture");exit(1);}
	    }
	  strcpy(data_dir , "ftp_database_client/");
	  close(tmp_fd);
	  break;
	default:
	  exit(0);
	}
      puts("fin");
      fflush(stdin);
    }
  
  return EXIT_SUCCESS;
}
