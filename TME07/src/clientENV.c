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

#define QUERY_SIZE 512
#define PORTSERV 9876

/*
  [FR]
  Le but ici n'est pas de vérifier que cette requete est syntaxiquement valide,
  on ne va pas faire 50 tests, juste les 2 premières lettres et un test minimum 
  sur la taille.

  [EN]
  The goal here is not to verify that this request is syntactically valid,
  we will not do 50 tests, just the first two letters and a minimum test.
*/

int requete_valide(char *sentence)
{
  int size=strlen(sentence);
  if(size < 5)
    return -1;
  if(sentence[0] != 'S' && sentence[0] != 'G')
    return -1;
  return 1;
}

/*EVERYTHING is in localhost*/

int main(int argc,char **argv)
{
  if(argc != 2)
    {
      printf("USE : %s <addr>\n",argv[0]);
      return EXIT_FAILURE;
    }

  struct sockaddr_in dest;
  int sock;
  int fromlen = sizeof(dest);
  struct addrinfo *result = NULL;

  struct addrinfo hints; /*= {};*/
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_ADDRCONFIG | AI_CANONNAME;
  hints.ai_protocol = 0;

  if (getaddrinfo(argv[1], 0, &hints, &result) != 0)
    {
      perror("getaddrinfo");
      exit(EXIT_FAILURE);
    }
  
  if ((sock = socket(AF_INET,SOCK_DGRAM,0)) == -1)
    {
      perror("socket"); 
      exit(1);
    }

  memset((char *)&dest,0, sizeof(dest));
  memcpy ((void*)&dest.sin_addr,(void*)&((struct sockaddr_in*)result->ai_addr)->sin_addr, fromlen);
  dest.sin_addr.s_addr = htonl(INADDR_ANY);
  dest.sin_family = AF_INET;
  dest.sin_port = htons(PORTSERV);

  char message[QUERY_SIZE], answer[QUERY_SIZE];
  int res = 0;

  while(1)
    {
      fgets(message, QUERY_SIZE, stdin);
      /*res=requete_valide(message);*/
      printf("TMP = %s\n",message);
      if(res == -1)
	{
	  puts("-- !! CAUTION !! --");
	  continue;
	}
      
      if (sendto(sock,message,QUERY_SIZE,0,(struct sockaddr *)&dest, fromlen) == -1)
	{
	  perror("sendto"); 
	  exit(1);
	}
      
      if(message[0] == 'G')
	{
	  if ( recvfrom(sock,answer,QUERY_SIZE,0,0,(socklen_t *)&fromlen) == -1)
	    {
	      perror("recvfrom"); 
	      exit(2);
	    }
	  printf("answer => [%s]\n",answer);
	}
      
      fflush(stdin);
    }
  
  shutdown(sock,2);

  return EXIT_SUCCESS;
}
