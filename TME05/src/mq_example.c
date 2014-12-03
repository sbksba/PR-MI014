#define _XOPEN_SOURCE 700

#include <mqueue.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <string.h>

#define MAX_MSG_LEN 70

int main(/*int argc, char** argv*/)
{
  mqd_t mqid;
  char msgcontent[MAX_MSG_LEN];
  unsigned int prio;
  int msgsz;
  time_t currtime;

  struct mq_attr attr;
  attr.mq_flags = 0;  
  attr.mq_maxmsg = 10;  
  attr.mq_msgsize = MAX_MSG_LEN*sizeof(char);  
  attr.mq_curmsgs = 0; 

  if ((mqid = mq_open("/file:4", O_RDWR | O_CREAT, 0600, &attr)) == -1)
    {
      perror("mq_open");
      exit(1);
    }
  
  if (fork() == 0)
    {
      prio = 12;
      currtime = time(NULL);
      snprintf(msgcontent, MAX_MSG_LEN, "Hello from process %u (at %s).", getpid(), ctime(&currtime));
      
      if (mq_send(mqid, msgcontent, strlen(msgcontent)+1, prio) == -1)
	{
	  perror("mq_send");
	  exit(1);
	}
      mq_close(mqid);
      exit(0);

    }
  else
    {
      msgsz = mq_receive(mqid, msgcontent, 1+(sizeof(char)*MAX_MSG_LEN), &prio);
      if (msgsz == -1)
	{
	  perror("mq_receive");
	  exit(1);
	}
      printf("Received message (%d bytes) with prio %d: %s\n", msgsz, prio, msgcontent);
      
    }
  
  mq_close(mqid);
  mq_unlink("/file:0");
  
  return EXIT_SUCCESS;
}
