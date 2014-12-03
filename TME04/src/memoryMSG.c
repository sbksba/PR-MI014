#define SVID_SOURCE 1

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

#define NB_PROCS 5

int main()
{
  int i, res=0, shm_id, *p_int, *adr_att, taille = sizeof(int)*NB_PROCS; 
  key_t cle;
  pid_t pid;

  cle = ftok(".", 'M');
  shm_id = shmget(cle, taille, 0666 | IPC_CREAT);
  adr_att = shmat(shm_id, 0, 0600);
  printf("ID[cle] = %d\n",cle);
  p_int = (int *)adr_att;

  for(i=0;i<NB_PROCS;i++)
    {
      switch(pid=fork())
	{
	case -1:
	  perror("Create Sons FAILED");
	  exit(-1);
	case 0:
	  srand(time(NULL) ^ (getpid()<<16));
	  *(p_int+i*sizeof(int))=(int) (10*(float)rand()/ RAND_MAX);
	  printf("Val in memory = %d\n",*(p_int+i*sizeof(int)));
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
      printf("Val read by Main : %d\n",*(p_int + i*sizeof(int)));
      res+=(*(p_int + i*sizeof(int)));
    }
  
  printf("\n");  
  printf("Final Result = %d\n",res);
  /*
    Fin de file de message, avec option IPC_RMID, et id de la file en params. 
    Buf ne joue pas son role, mais le pouvait si on change l'option.
  */

  puts("End of Sons");
  
  /*Liberation de l'espace partage*/
  shmctl(shm_id, IPC_RMID, NULL);

  return EXIT_SUCCESS;
}
