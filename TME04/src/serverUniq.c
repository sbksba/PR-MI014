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

#include "serverBIS.h"

int msg_id; 

void handler()
{
  msgctl(msg_id, IPC_RMID,NULL);
  puts("CLEAN EXIT");
  exit(-1);
}

/*
  Utiliser toujours la meme structure définie dans le main ou déclarer un 
  tableau de messages initialement, les modifier dans le fork (1 par fork), 
  puis le lire dans un message spécial-main a la fin
*/

int main()
{
  signal(SIGINT,handler);
  key_t cle;
  char path[14]= ".", code='e';
  int i,j,res=0,boucle,fil_envoie;
  pid_t pid;

  cle = ftok(path, code);
  msg_id = msgget (cle, 0666 | IPC_CREAT | IPC_EXCL);
  
  if(msg_id==-1)
    {
      perror("ERROR -> Create file queue\n");
      exit(-1);
    }
  
  printf("Cle[%d] -> msgid = %d, path = %s , code = %c\n",cle,msg_id,path,code);

  for(i=0;i<=NB_PROCS;i++)
    msgUniq[i].type = i+1;

  for(i=0;i<NB_PROCS;i++)
    {
      switch(pid=fork())
	{
	case -1:
	  perror("Create Sons FAILED");
	  exit(-1);
	case 0:
	  srand(time(NULL) ^ (getpid()<<16));
	  msgUniq[i].corps.mess=(int) (10*(float)rand()/ RAND_MAX);
	  msgUniq[i].corps.num_file=i+1;
	  msgUniq[i].type=NB_PROCS+1;
	  boucle=msgUniq[i].corps.mess;
	  printf("Number of val to read %d for pid[%d]\n",boucle,getpid());
	  msgsnd(msg_id, &(msgUniq[i]) , sizeof(msgUniq[i].corps), 0);
	  for(j=0;j<boucle;j++){
	    printf("Wait type : %d\n",i+1);
	    msgrcv(msg_id , &(msgUniq[i]) , sizeof(msgUniq[i].corps), i+1,0);
	    printf("msgUniq[i] => %d -> file[%d]\n",msgUniq[i].corps.mess,i+1 );
	    res+=msgUniq[i].corps.mess;
	  }
	  printf("Final Result = %d for pid[%d] and file[%d]\n",res,getpid(),i+1);
	  return 0;
	default:
	  break;
	}
    }
  
  for(i=0;i<NB_PROCS;i++)
    {
      msgrcv(msg_id , &(msgUniq[NB_PROCS]) , sizeof(msgUniq[i].corps), NB_PROCS+1,0);
      boucle=msgUniq[NB_PROCS].corps.mess;
      fil_envoie=msgUniq[NB_PROCS].corps.num_file;
      printf("Receive : %d -> file[%d]\n",boucle,fil_envoie);
      for(j=0;j<boucle;j++){
	msgUniq[NB_PROCS].corps.mess=(10*(float)rand()/ RAND_MAX);
	msgUniq[NB_PROCS].type=msgUniq[NB_PROCS].corps.num_file;
	printf("Val send : %d , send for type %ld\n",msgUniq[NB_PROCS].corps.mess,msgUniq[NB_PROCS].type);
	msgsnd(msg_id , &(msgUniq[NB_PROCS]) , sizeof(msgUniq[i].corps), 0);
      }
      
    }
  
  /*
    Fin de file de message, avec option IPC_RMID, et id de la file 
    en params. Buf ne joue pas son role, mais le pouvait si on change l'option.
  */
  
  for(i=0;i<NB_PROCS;i++)
    wait(NULL);

  msgctl(msg_id, IPC_RMID,NULL);

  puts("END OF SONS");

  return EXIT_SUCCESS;
}
