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
#include <signal.h>

#include "color.h"

#define BROADCAST_IP "255.255.255.255"
#define BC_PORT 9999
#define MESSAGE_SIZE 128

int sock_bc;
struct sockaddr_in dest;

void prep_sockets()
{
  struct sockaddr_in servad;
  int on = 1;

  #ifdef DEBUG
  printf("\n+[PONGER SOCKET] => "); 
  #endif
  if ((sock_bc = socket(AF_INET,SOCK_DGRAM,0)) < 0) {printf(FAILED);perror("[PONGER PREP SOCKETS] socket");exit(1);}
  #ifdef DEBUG
  printf(OK);
  #endif

  setsockopt(sock_bc, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  setsockopt(sock_bc, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
  
  /* nommage */
  memset((void *)&servad,0, sizeof(servad));
  servad.sin_addr.s_addr = htonl(INADDR_ANY);
  servad.sin_family = AF_INET;
  servad.sin_port = htons(BC_PORT);
  
  #ifdef DEBUG
  printf("\n+[PONGER BIND]   => ");
  #endif
  if (bind(sock_bc,(struct sockaddr *)&servad,sizeof(servad)) < 0) {printf(FAILED);perror("[PONGER PREP SOCKETS] bind");exit(2);}
  #ifdef DEBUG
  printf(OK);
  printf("\n---------------------------------\n");
  #endif
}

int main(/*int argc,char **argv*/)
{
  struct sockaddr_in exp;
  int fromlen = sizeof(exp);
  char message[MESSAGE_SIZE];

  dest.sin_addr.s_addr = htonl(INADDR_BROADCAST);
  dest.sin_family = AF_INET;
  dest.sin_port = htons(BC_PORT);
  
  prep_sockets();

  while(1)
    {
      #ifdef DEBUG
      printf("\n+[PONGER RECVFROM] => ");
      #endif
      if (recvfrom(sock_bc, message, MESSAGE_SIZE, 0, (struct sockaddr*) &exp, (socklen_t*)&fromlen) == -1) {printf(FAILED);perror("[PONGER] recvfrom");exit(2);}
      #ifdef DEBUG
      printf(OK);
      #endif
      
      if(strcmp(message, "PING") == 0)
	{
	  strcpy(message , "PONG");
	  #ifdef DEBUG
	  printf("\n+[PONGER SENDTO]   => ");
	  #endif
	  if (sendto(sock_bc, message, MESSAGE_SIZE, 0, (struct sockaddr *)&dest, sizeof(dest)) == -1) {printf(FAILED);perror("[PONGER] sendto");exit(1);}
	  #ifdef DEBUG
	  printf(OK);
	  #endif
	  puts("\nPING");
	}
    }

  return EXIT_SUCCESS;
}
