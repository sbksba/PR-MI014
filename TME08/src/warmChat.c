#define _XOPEN_SOURCE 700
#define _GNU_SOURCE 

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include "color.h"

#define QUERY_SIZE 256
#define MESSAGE_SIZE 200
#define NAME_SIZE 56

int main(int argc, char *argv[])
{
  struct sockaddr_in dest;
  int sock;
  pid_t pid;
  char requete[QUERY_SIZE], message[MESSAGE_SIZE], name[NAME_SIZE];
  
  if (argc < 4 || argc > 4)
    {
      printf("USE : %s <addr> <port> <pseudo>\n",argv[0]);
      return EXIT_FAILURE;
    }
  
  strcat(argv[3]," ");
  strcpy(name,argv[3]);

  pid = fork();
  
  if(pid == -1) {perror("fork");exit(1);}

  if(pid > 0)
    {
      /*-- Client --*/
      #ifdef DEBUG
      printf("\n+[SOCKET CLIENT] => ");
      #endif
      if ((sock = socket(AF_INET,SOCK_DGRAM,0)) == -1) {printf(FAILED);printf("\n");perror("[CLIENT] socket"); exit(1);}
      #ifdef DEBUG
      printf(OK);
      #endif
      
      memset((char *)&dest, 0, sizeof(dest));
      dest.sin_addr.s_addr = inet_addr(argv[1]);
      dest.sin_family = AF_INET;
      dest.sin_port = htons(atoi(argv[2]));
      
      while(1)
	{
	  fgets(message, MESSAGE_SIZE, stdin);
	  strcpy(requete, name);
	  strcat(requete, message);
	  
	  #ifdef DEBUG
	  printf("\n+[SENDTO]        => ");
          #endif
	  if ( sendto(sock,requete,QUERY_SIZE,0,(struct sockaddr*)&dest, sizeof(dest)) == -1) {printf(FAILED);printf("\n");perror("[CLIENT] sendto"); exit(1);}
	  #ifdef DEBUG
	  printf(OK);
	  printf("\n");
          #endif
	}
    }
  else
    {
      /*Server, receive QUERY_SIZE + 1*/
      char pseudo[NAME_SIZE];
      char *receive;
      
      struct sockaddr_in sin;
      struct ip_mreq imr; 

      #ifdef DEBUG
      printf("\n+[SOCKET SERVER] => ");
      #endif

      if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {printf(FAILED);printf("\n");perror("[SERVER] socket");exit(1);}

      #ifdef DEBUG
      printf(OK);
      #endif

      imr.imr_multiaddr.s_addr = inet_addr(argv[1]);
      imr.imr_interface.s_addr = INADDR_ANY;
      
      #ifdef DEBUG
      printf("\n+[SETSOCKOPT]    => ");
      #endif
      
      if(setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&imr,sizeof(struct ip_mreq)) == -1) {printf(FAILED);printf("\n");perror("[SERVER] setsockopt");exit(2);}

      #ifdef DEBUG
      printf(OK);
      #endif

      memset((char *)&sin,0, sizeof(sin));
      sin.sin_addr.s_addr = htonl(INADDR_ANY);
      sin.sin_port = htons(atoi(argv[2]));
      sin.sin_family = AF_INET;
      
      #ifdef DEBUG
      printf("\n+[BIND]          => ");
      #endif
      
      if (bind(sock,(struct sockaddr *)&sin,sizeof(sin)) < 0) {printf(FAILED);printf("\n");perror("[SERVER] bind");exit(3);}

      #ifdef DEBUG
      printf(OK);
      #endif

      while(1)
	{
	  if (recvfrom(sock,requete,QUERY_SIZE,0,NULL,NULL) == -1) {printf(FAILED);printf("\n");perror("[SERVER] recvfrom");exit(4);}
	  sscanf(requete,"%s",pseudo);
	  receive = requete;
	  while(*receive != ' ')
	    receive++;
	  receive++;
	  printf("[%s] say : %s\n",pseudo ,receive);
	}
    }
  
  return EXIT_SUCCESS;
}
