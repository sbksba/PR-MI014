#define SVID_SOURCE 1

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#include "fileMSG.h"

/* 
   Utiliser toujour la meme structure définie dans le main 
   ou déclarer un tableau de messages initialement, les modifier 
   dans le fork (1 par fork), puis le lire dans un message 
   spécial-main a la fin?
*/

int main()
{
  int msg_id,res=0,i,pid;
  char path[14]= ".", code='Q';
  key_t cle;
  pid_t father = getpid();
  
  cle = ftok(path, code);
  msg_id = msgget (cle, 0666 | IPC_CREAT);

  for(i=0;i<=NB_PROCS;i++)
    msg[i].type = 1;

  for(i=0;i<NB_PROCS;i++)
    {
      switch(pid=fork())
	{
	case -1:
	  perror("Create Sons FAILED");
	  exit(-1);
	case 0:
	  srand(time(NULL) ^ (getpid()<<16));
	  msg[i].mess=(int) (10*(float)rand()/ RAND_MAX);
	  printf("ID[%d] => val = %d\n",getpid(),msg[i].mess);
	  msgsnd(msg_id , &(msg[i]) , sizeof(int), 0);
	  return 0;
	default:
	  break;
	}
    }
  
  for(i=0;i<NB_PROCS;i++)
    wait(NULL);
  
  printf("\n");

  for(i=0;i<NB_PROCS;i++)
    {
      msgrcv(msg_id , &(msg[NB_PROCS]) , sizeof(int), 1L,0);
      res+=msg[NB_PROCS].mess;
      if (getpid() == father)
	printf("ID[father] => Receive : %d\n",msg[NB_PROCS].mess);
    }
  printf("\n");

  /*Fin de file de message, avec option IPC_RMID, et id de la file 
    en params. Buf ne joue pas son role, mais le pouvait si on 
    change l'option.
  */
  msgctl(msg_id, IPC_RMID,NULL);
  
  printf("Final Result = %d\n",res);
  puts("End of Sons");
  
  return EXIT_SUCCESS;
}
