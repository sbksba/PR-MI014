#ifndef DAEMON_H
#define DAEMON_H

typedef struct {
  int nb;
  int the_returned_value;
  pthread_attr_t* attr;
} thread_args;

typedef struct {
  pthread_t t;
  thread_args his_args;
} thread_t;

#endif
