#ifndef FILEMSG_H
#define FILEMSG_H

#define NB_PROCS 5

struct message {
    long type;
    int mess; 
}msg[NB_PROCS+1];

#endif
