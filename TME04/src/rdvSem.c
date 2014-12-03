#define SVID_SOURCE 1

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define NB_PROCS 5

int FLAGS = 0666 | IPC_CREAT; 
key_t sem_cle; 
int sem_id;
struct sembuf operation[NB_PROCS+1];
char sem_code = 'a';

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short int *array;
  struct seminfo *__buf;
}arg;

unsigned short int sem_table [2]={0,0};

void P(int sem)
{
  operation[sem].sem_num=sem;
  operation[sem].sem_op=-1;
  operation[sem].sem_flg=0;
  semop(sem_id,&operation[sem],1);
}

void V(int sem)
{
  operation[sem].sem_num=sem;
  operation[sem].sem_op=1;
  operation[sem].sem_flg=0;
  semop(sem_id,&operation[sem],1);
}

/*
  V(1),P(0)
  sem(0)=>attente generale des NB_PROCS, sem(1)=>incrementation pour le main
  Ajouter un parametre a P et V functions
*/
void wait_barrier(int arg)
{
  V(1);
  P(0);
}

void process (int NB_PCS) 
{
  printf ("avant barrière\n");
  wait_barrier (NB_PCS);
  printf ("après barrière\n");
  exit (0);
}

int main()
{
  int i;
  pid_t pid;
  arg.array = sem_table;

  if((sem_cle = ftok(".",sem_code))==-1) {perror("ERROR Create Key");exit(-1);}

  if ((sem_id=semget(sem_cle,2,IPC_CREAT | IPC_EXCL | 0660))==-1)
    {
      perror("semget: error"); 
      printf("ERROR [%s]\n",strerror(errno));
      exit (3);
    }
  
  if (semctl(sem_id, 0, SETALL,arg)==-1) {perror("semctl: SETALL"); exit (3);}

  for(i=0;i<NB_PROCS;i++)
    {
      switch(pid=fork())
	{
	case -1:
	  perror("ERROR Create Process\n");
	  exit(-1);
	case 0:
	  process(i);
	  return 0;
	default:
	  break;
	}
    }

  for(i=0;i<NB_PROCS;i++) {P(1);}
  for(i=0;i<NB_PROCS;i++) {V(0);}
  for(i=0;i<NB_PROCS;i++) {wait(NULL);}

  if(semctl(sem_id,0,IPC_RMID,0)==-1) {perror("semctl: IPC_RMID probleme");exit(-1);}
 
  return EXIT_SUCCESS;
}
