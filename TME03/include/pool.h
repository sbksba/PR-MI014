#ifndef POOL_H
#define POOL_H

typedef struct {
  int first_pos_undone;
  int size;
  char** values;
} files_jobs_t;

int est_vide();
char* pop();
void* thread_fun(void* arg);

#endif
