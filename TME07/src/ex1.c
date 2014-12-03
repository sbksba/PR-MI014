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

#define RANDOM_MAX 10
#define NBPROC 5
#define PORTSERV 4567

int main(/*int argc, char **argv*/)
{
  struct sockaddr_in sin;
  int fils=0,sc,fromlen,random_value,i,buf,total=0;
  pid_t pid,father=getpid();

  /* Name of the server's socket and client*/
  fromlen = sizeof(sin);

  if ((sc = socket(AF_INET,SOCK_DGRAM,0)) < 0) {perror("socket");exit(1);}
  
  memset((char *)&sin,0, sizeof(sin));
  sin.sin_addr.s_addr = htonl(INADDR_ANY);

  sin.sin_port = htons(PORTSERV);
  sin.sin_family = AF_INET;

  if (bind(sc,(struct sockaddr *)&sin,sizeof(sin)) < 0) {perror("bind"); exit(2);}
  
  for(fils=0;fils<NBPROC;fils++)
    {
      pid=fork();
      switch(pid)
	{
	case -1:
	  perror("ERROR processus creation");
	  exit(1);
	case 0:
	  srand(time(NULL) ^ (getpid()<<16));
	  random_value=(10*(unsigned)rand() % RANDOM_MAX);
	  printf("PROC [%d] => SEND -> [%d]\n",fils,random_value);
	  
	  if (sendto(sc,&random_value,sizeof(int),0,(struct sockaddr *)&sin, sizeof(sin)) == -1) {perror("sendto");exit(1);}
	  return 0;
	  break;
	default:
	  break;
	}
    }
  
  for(i=0;i<NBPROC;i++)
    {
      if ( recvfrom(sc,&buf,sizeof(int),0,(struct sockaddr *)&sin,(socklen_t *)&fromlen) == -1) {perror("recvfrom");exit(2);}
      total+=buf;
    }
  
  if (getpid() == father)
    printf("FATHER   => TOTAL Receive -> [%d]\n",total);
  
  close(sc);

  return EXIT_SUCCESS;

}
