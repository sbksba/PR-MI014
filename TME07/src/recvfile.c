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

#define FILENAME_SIZE 128
#define BUFFER_SIZE 1028

int main(int argc, char **argv)
{
  /*Connection socket name*/
  struct sockaddr_in sin;
  /*Communication socket*/
  struct sockaddr_in exp;
  int num_port,sc,pos=0,fic_pos=0,fd,i,recu,size;
  char filename[FILENAME_SIZE];
  char buffer[BUFFER_SIZE];
  char *name_file;
  /* Connection socket*/
  int scom;
  /*struct hostent *hp;*/
  int fromlen = sizeof(exp);

  if(argc!=2)
    {
      printf("USE : %s <port>",argv[0]);
      return EXIT_FAILURE;
    }

  num_port=atoi(argv[1]);

  if ((sc = socket(AF_INET,SOCK_STREAM,0)) < 0) {perror("socket"); exit(1);}

  memset((char *)&sin,0, sizeof(sin));
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_port = htons(num_port);
  sin.sin_family = AF_INET;

  if (bind(sc,(struct sockaddr *)&sin,sizeof(sin)) < 0) {perror("bind");exit(1);}

  listen(sc, 1);

  if ((scom = accept(sc, (struct sockaddr *)&exp, (socklen_t *)&fromlen))== -1) {perror("accept");exit(3);}

  if (read(scom,filename, sizeof(char)*FILENAME_SIZE) < 0) {perror("read");exit(4);}

  while(filename[pos]!='\0')
    {
      if(filename[pos]=='/')
	fic_pos=pos;
      pos++;
    }

  name_file=&filename[fic_pos+1];
  printf("FILENAME => [%s]\n",name_file);

  fd=open(name_file,O_CREAT | O_TRUNC | O_WRONLY,0666);
  if(fd==-1) {perror("ERROR open file");exit(1);}
  if (read(scom,&size, sizeof(int)) < 0) {perror("read");exit(4);}
  
  printf("SIZE => [%d] -- DIV => [%d]\n",size,size/BUFFER_SIZE);

  for(i=0;i<=size/BUFFER_SIZE;i++)
    {
      if((recu=read(scom,buffer,BUFFER_SIZE))==-1) {perror("read");exit(1);}
      buffer[recu-1]='\0';
      printf("RECEIVES => [%s] -- received => [%d]\n",buffer,recu);
      if(write(fd,buffer,recu)==-1) {perror("write");exit(1);}
    }

  shutdown(scom,2);
  
  if(close(scom)==-1) {perror("close socket com");exit(1);}
  if(close(fd)==-1)   {perror("close file");exit(1);}
  if(close(sc)==-1)   {perror("close socket");exit(1);}

  return EXIT_SUCCESS;
}
