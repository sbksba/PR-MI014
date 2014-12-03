#ifndef SERVER_H
#define SERVER_H

#define NB_PROCS 5

int msg_id[NB_PROCS+1]; 
struct message {
  long type;
  int mess; 
  int num_file;
}msg[NB_PROCS+1];

#endif
