#ifndef SERVERBIS_H
#define SERVERBIS_H

#define NB_PROCS 5

struct message {
  long type;
  struct message2{
    int mess; 
    int num_file;
  }corps;
}msgUniq[NB_PROCS+1];

#endif
