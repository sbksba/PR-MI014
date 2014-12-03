#define SVID_SOURCE 1

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include "server.h"

/*
  Utiliser tjs la meme structure définie dans le main ou déclarer un tableau 
  de messages initialement, les modifier dans le fork (1 par fork), puis le lire
  dans un message spécial-main a la fin
*/

void handler()
{
  int i=0;
  for(i=0;i<=NB_PROCS;i++)
    msgctl(msg_id[i], IPC_RMID,NULL);
  puts("CLEAN EXIT");
  exit(0);
}

int main()
{
  signal(SIGINT,handler);
  key_t cle;
  char path[14]= ".", code='e';
  int i,pid,j,res=0,boucle,fil_envoie;

  for(i=0;i<=NB_PROCS;i++)
    {
      cle = ftok(path, code);
      msg_id[i] = msgget (cle, 0666 | IPC_CREAT | IPC_EXCL);
      if(msg_id[i]==-1)
	{
	  perror("ERROR -> Create MSG QUEUE\n");
	  exit(-1);
	}
      
    printf("CLE[%d] -> msgid = %d, path = %s, code = %c\n",cle,msg_id[i],path,code);
    path[6]=path[6]+1;
    code++;
  }

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
	  msg[i].num_file=i;
	  boucle=msg[i].mess;
	  printf("Number of val to read %d for queue[%d], send on queue[%d]\n",boucle,msg[i].num_file,i);
	  msgsnd(msg_id[NB_PROCS] , &(msg[i]) , sizeof(msg), 0);
	  for(j=0;j<boucle;j++)
	    {
	      msgrcv(msg_id[i] , &(msg[i]) , sizeof(msg), 1L,0);
	      res+=msg[i].mess;
	    }
	  printf("Final Result = %d for queue[%d]\n",res,i);
	  msgctl(msg_id[i], IPC_RMID,NULL);
	  return 0;
	default:
	  break;
	}
    }
  
  for(i=0;i<NB_PROCS;i++)
    {
      msgrcv(msg_id[NB_PROCS] , &(msg[NB_PROCS]) , sizeof(msg), 1L,0);
      boucle=msg[NB_PROCS].mess;
      fil_envoie=msg[NB_PROCS].num_file;
      for(j=0;j<boucle;j++)
	{
	  msg[NB_PROCS].mess=(10*(float)rand()/ RAND_MAX);
	  msgsnd(msg_id[fil_envoie] , &(msg[NB_PROCS]) , sizeof(msg), 0);
	}
    }
  
  /*
    Fin de file de message, avec option IPC_RMID, et id de la file en params. 
    Buf ne joue pas son role, mais le pouvait si on change l'option.
  */

  for(i=0;i<NB_PROCS;i++)
    wait(NULL);

  for(i=0;i<=NB_PROCS;i++)
    msgctl(msg_id[i], IPC_RMID,NULL);
  
  puts("END OF SONS");

  return EXIT_SUCCESS;
}
