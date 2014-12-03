#define SVID_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define STACK_SIZE 100

char pile[STACK_SIZE];
int stack_pointer=0;

int nb_endormis=0;
int *p_int;
int sem_id;

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short int *array;
  struct seminfo *__buf;
}arg;

unsigned short int sem_table [2]={0,1};

void initialisation()
{
  int shm_id; 
  key_t cle;
  int *adr_att;

  cle = ftok(".", 'M');
  shm_id = shmget(cle, STACK_SIZE, 0666 | IPC_CREAT);
  adr_att = shmat(shm_id, 0, 0600);
  p_int = (int *)adr_att;

  arg.array = sem_table;

  key_t sem_cle; 

  if((sem_cle = ftok(".",'a'))==-1) {perror("erreur creation cle");exit(-1);}
  if ((sem_id=semget(sem_cle,2,IPC_CREAT | IPC_EXCL | 0660))==-1) {perror("semget: error");exit (3);}
  if (semctl(sem_id, 0, SETALL,arg)==-1) {perror("semctl: SETALL"); exit (3);}
}


void P(int sem,struct sembuf op)
{
  op.sem_num=sem;
  op.sem_op=-1;
  op.sem_flg=0;
  semop(sem_id,&op,1);
}

void V(int sem,struct sembuf op)
{
  op.sem_num=sem;
  op.sem_op=1;
  op.sem_flg=0;
  semop(sem_id,&op,1);
}

char Pop()
{
  char c;
  struct sembuf operation;
  P(1,operation);
  if(stack_pointer<=0)
    {
      nb_endormis++;
      V(1,operation);
      P(0,operation);
      return 0;
    }
  stack_pointer--;
  c=*(p_int+stack_pointer);
  V(1,operation);
  return c;
}

void Push(char c)
{
  struct sembuf operation;
  int i;
  P(1,operation);
  if(stack_pointer>=STACK_SIZE)
    {
      V(1,operation);
      return;
    }
  if(stack_pointer<=0)
    {
      for(i=0;i<nb_endormis;i++)
	V(0,operation);
    }
  nb_endormis=0;
  *(p_int+stack_pointer)=c;
  stack_pointer++;
  V(1,operation);
}
