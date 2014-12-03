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
#include <arpa/inet.h>

#include "color.h"

#define BROADCAST_IP "255.255.255.255"
#define BC_PORT 9999
#define MESSAGE_SIZE 128

int sock_bc;
struct sockaddr_in dest;
pid_t pid;

/*SIGINT*/
void handler()
{
  kill(SIGKILL,pid);
  exit(0);
}

void prep_sockets()
{
  struct sockaddr_in servad;
  int on = 1;
  
  #ifdef DEBUG
  printf("\n+[SONAR SOCKET] => "); 
  #endif
  if ((sock_bc = socket(AF_INET,SOCK_DGRAM,0)) < 0) {printf(FAILED);perror("[SONAR PREP SOCKETS] socket");exit(1);}
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
  printf("\n+[SONAR BIND]   => ");
  #endif
  if (bind(sock_bc,(struct sockaddr *)&servad,sizeof(servad)) < 0) {printf(FAILED);perror("[SONAR PREP SOCKETS] bind");exit(2);}
  #ifdef DEBUG
  printf(OK);
  printf("\n---------------------------------\n");
  #endif
}

int main(/*int argc, char *argv[]*/)
{
  struct sockaddr_in exp;
  int fromlen = sizeof(exp);
  char message[MESSAGE_SIZE];

  dest.sin_addr.s_addr = htonl(INADDR_BROADCAST);
  dest.sin_family = AF_INET;
  dest.sin_port = htons(BC_PORT);
  
  prep_sockets();

  if (fork())
    {
      pid = getpid();
      
      sigset_t sig;
      sigfillset(&sig);
      struct sigaction sigact;
      sigact.sa_handler = handler;
      sigact.sa_flags = 0;
      sigact.sa_mask = sig;
      
      
      sigdelset(&sig , SIGINT);
      sigprocmask(SIG_SETMASK , &sig, 0);
      sigaction(SIGINT , &sigact , 0);
      
      printf("sending BC\n");
      while(1)
	{
	  #ifdef DEBUG
	  printf("\n+[SONAR RECVFROM] => ");
	  #endif
	  if (recvfrom(sock_bc, message, MESSAGE_SIZE, 0, (struct sockaddr*) &exp, (socklen_t*)&fromlen) == -1) {printf(FAILED);perror("[SONAR] recvfrom");exit(2);}
	  #ifdef DEBUG
	  printf(OK);
	  #endif
	  if(strcmp(message , "PONG") == 0 )
	    {
	      printf("\n<IP = %s, PORT = %d>\n", inet_ntoa(exp.sin_addr), ntohs(exp.sin_port));
	      printf("%s\n",message);
	    }
	}
    } 
  else
    {
      while(1)
	{
	  strcpy(message , "PING");
	  #ifdef DEBUG
	  printf("\n+[SONAR SENDTO]   => ");
	  #endif
	  if (sendto(sock_bc, message, MESSAGE_SIZE, 0, (struct sockaddr *)&dest, sizeof(dest)) == -1) {printf(FAILED);perror("[SONAR] sendto");exit(1);}
	  #ifdef DEBUG
	  printf(OK);
	  #endif
	  sleep(3);
	}
    }

  return EXIT_SUCCESS;
}
