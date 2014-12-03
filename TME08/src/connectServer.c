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
#include <sys/select.h>
#include <arpa/inet.h>

#include "color.h"

int main(int argc, char *argv[])
{
  struct sockaddr_in sin;
  struct sockaddr_in exp;
  int fd;
  int tab_ports[argc-1], i , tab_sock[argc-1];
  int scom;
  int fromlen = sizeof(exp);

  if (argc < 2)
    {
      printf("USE : %s port1 <port2 ...>\n",argv[0]);
      return EXIT_FAILURE;
    }

  #ifdef DEBUG
  printf("\n+[SERVER OPEN]     => ");
  #endif
  if((fd = open("cxlog" , O_CREAT | O_TRUNC | O_RDWR , 0666))==-1) {printf(FAILED);perror("open file");exit(1);}
  #ifdef DEBUG
  printf(OK);
  printf("\n---------------------------------\n");
  #endif
  
  for(i=1 ; i < argc; i++)
    tab_ports[i-1] = atoi(argv[i]);

  for(i=0 ; i < argc-1; i++)
    {
      #ifdef DEBUG
      printf("\n[PORT] : %d\n+[SERVER SOCKET]   => ",tab_ports[i]);
      #endif
      if ((tab_sock[i] = socket(AF_INET,SOCK_STREAM,0)) < 0) {printf(FAILED);perror("socket"); exit(1);}
      #ifdef DEBUG
      printf(OK);
      #endif
      
      memset((char *)&sin,0, sizeof(sin));
      sin.sin_addr.s_addr = htonl(INADDR_ANY);
      sin.sin_port = htons(tab_ports[i]);
      sin.sin_family = AF_INET;
      #ifdef DEBUG
      printf("\n+[SERVER BIND]     => ");
      #endif
      if (bind(tab_sock[i],(struct sockaddr *)&sin,sizeof(sin)) < 0) {printf(FAILED);perror("bind");exit(1);}
      #ifdef DEBUG
      printf(OK);
      printf("\n---------------------------------\n");
      #endif
      listen(tab_sock[i], 1);
    }
  
  for(;;)
    {
      fd_set mselect;
      FD_ZERO(&mselect);
      
      for(i = 0 ; i < argc-1 ; i++)
	FD_SET(tab_sock[i],&mselect); 
      
      #ifdef DEBUG
      printf("\n+[SERVER SELECT]     => ");
      #endif
      if (select(tab_sock[argc-2] + 1, &mselect, NULL, NULL, NULL) == -1) {printf(FAILED);perror("[CONNECT SERVER] select");exit(3);}
      #ifdef DEBUG
      printf(OK);
      #endif
      
      for(i = 0 ; i < argc-1 ; i++)
	{
	  if (FD_ISSET(tab_sock[i],&mselect))
	    {
	      #ifdef DEBUG
	      printf("\n+[SERVER ACCEPT]     => ");
              #endif
	      if ( (scom = accept(tab_sock[i], (struct sockaddr *)&exp, (socklen_t *)&fromlen))== -1) {printf(FAILED);perror("[CONNECT SERVER] accept");exit(2);}
	      #ifdef DEBUG
	      printf(OK);
              #endif
	      printf("\n<IP = %s, PORT = %d>\n", inet_ntoa(exp.sin_addr), ntohs(exp.sin_port));
	      write(fd,inet_ntoa(exp.sin_addr),strlen(inet_ntoa(exp.sin_addr)));
	      write(fd,"\n",1);
	    }
	}
    }
  
  return EXIT_SUCCESS;
}
