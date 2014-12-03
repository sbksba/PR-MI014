#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

#define FILENAME_SIZE 130
#define BUFFER_SIZE 1024
#define QUERY_SIZE 512

int main(int argc, char **argv)
{
  /*Server Name*/
  struct sockaddr_in dest;
  struct stat stat_info;
  char buffer[BUFFER_SIZE];
  int num_port=atoi(argv[2]);
  int fd,sock,size,i,recu;
  
  if(argc!=4)
    {
      printf("USE : %s <addr> <port> <filename>",argv[0]);
      return EXIT_FAILURE;
    }
  
  printf("FILE [%s]\n",argv[3]);
  fd=open(argv[3], O_RDONLY,0666);
  
  if(fd==-1)
    {
      perror("ERROR open file");
      return EXIT_FAILURE;
    }
  
  dest.sin_addr.s_addr = htonl(atoi(argv[1]));
  dest.sin_port = htons(num_port);
  dest.sin_family = AF_INET;

  if ((sock = socket(PF_INET,SOCK_STREAM,0)) == -1)
    {
      perror("socket");
      return EXIT_FAILURE;
    }

  if (connect(sock, (struct sockaddr *) &dest, sizeof(dest)) == -1)
    {
      perror("connect"); 
      return EXIT_FAILURE;
    }

  if ( stat (argv[3], &stat_info) == -1)
    { 
      perror ("stat");
      return EXIT_FAILURE;
    }
  
  
  if(write(sock,argv[3],FILENAME_SIZE)==-1)
    {
      perror("first write");
      return EXIT_FAILURE;
    }

  size=stat_info.st_size;
  printf("SIZE => [%d]\n",size);

  if(write(sock,&size,sizeof(int))==-1)
    {
      perror("second write");
      exit(1);
    }

  for(i=0;i<=stat_info.st_size/BUFFER_SIZE;i++)
    {
      if((recu=read(fd,buffer,BUFFER_SIZE))==-1)
	{
	  perror("read");
	  return EXIT_FAILURE;
	}
      
      printf("SEND => [%s]\n",buffer);

    if(write(sock,buffer,sizeof(char)*recu)==-1)
      {
	perror("third write");
	exit(1);
      }
    }
  
  shutdown(sock,2);
  close(sock);
  close(fd);
  
  return EXIT_SUCCESS;
}
