#ifndef SKIPSHARE_H
#define SKIPSHARE_H

#define CONTENT_SIZE 1024
#define NB_CONNECT_MAX 1453

struct request {
  long type;
  char content[CONTENT_SIZE];
};

typedef struct _req_client_t{
  struct request req;
  sem_t autoris_diff;
}req_client_t;

typedef struct _req_t{
  struct request req;
}req_t;

typedef struct _global_stockage{
  sem_t fin_diffusion;
  sem_t poster_message,prevenir_server,deco_sem;
  struct request req;
}Stockage;

struct {
  char *tab_connect[NB_CONNECT_MAX];
  req_client_t *link_table[NB_CONNECT_MAX];
}co_deco_table;

#endif
