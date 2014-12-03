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
#include <signal.h>

#define QUERY_SIZE 512
#define PORTSERV 9876
#define DEVIL 666

int sc;

void handler()
{
  close(sc);
  exit(EXIT_SUCCESS);
}

int main()
{
  sigset_t sig;
  struct sigaction sigact;
  sigfillset(&sig);
  sigdelset(&sig,SIGINT);
  sigact.sa_handler=handler;
  sigact.sa_flags=0;
  sigact.sa_mask=sig;
  sigprocmask(SIG_SETMASK,&sig,0);
  sigaction(SIGINT,&sigact,0);

  struct sockaddr_in sin;
  struct sockaddr_in exp;

  char message[QUERY_SIZE];
  char *answer;
  char arg1[64], arg2[64];
  int fromlen = sizeof(exp);

  if ((sc = socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
      perror("socket"); 
      exit(1);
    }
  
  memset((char *)&sin,0, sizeof(sin));
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_port = htons(PORTSERV);
  sin.sin_family = AF_INET;

  if (bind(sc,(struct sockaddr *)&sin,sizeof(sin)) < 0)
    {
      perror("bind"); 
      exit(2);
    }
  
  while(DEVIL)
    {
      if ( recvfrom(sc,message,sizeof(char)*QUERY_SIZE,0,(struct sockaddr *)&exp,(socklen_t *)&fromlen) == -1)
	{
	  perror("recvfrom"); 
	  exit(2);
	}

      /*Port Extraction*/
      if(message[0] == 'S')
	{
	  if(sscanf(message,"%*s %s %s\n",arg1, arg2)==-1)
	    {
	      perror("sscanf [FAILED 1]");
	      exit(1);
	    }
	  /*Overwrite = 0 here, we only create!*/
	  setenv(arg1,arg2,0);
	}
      else
	{
	  if(message[0] == 'G')
	    {
	      if(sscanf(message,"%*s %s",arg1)==-1)
		{
		  perror("sscanf [FAILED 2]");
		  exit(1);
		}
	      
	      answer=strdup(getenv(arg1));
	      printf("answer => [%s] \n",answer);
	      
	      if (sendto(sc,answer,QUERY_SIZE,0,(struct sockaddr *)&exp,fromlen) == -1) 
		{
		  perror("sendto"); 
		  exit(4);
		}
	    }
	  else
	    {
	      perror("Kernel panic!");
	      exit(1);
	    }
	}
    }

  return EXIT_SUCCESS;
}
