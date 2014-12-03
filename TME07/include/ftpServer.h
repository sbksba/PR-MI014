#ifndef FTP_SERVER_H
#define FTP_SERVER_H

#define FILENAME_SIZE 128
#define BUFFER_SIZE 1024
#define QUERY_SIZE 1024
#define NUM_PORT 9876

typedef struct _list
{
  char *name;
  struct _list *suivant;
}list;

#endif
