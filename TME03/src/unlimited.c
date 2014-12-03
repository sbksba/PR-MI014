#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "common.h"

int main (int argc, char ** argv)
{
  int i;
  int NBTHREADS = argc - 1;
  pthread_t my_threads[NBTHREADS];

  for (i = 1; i < argc; i++)
    {
      int res;
      /* /!\ decalage des cases /!\ */
      res = pthread_create(&(my_threads[i - 1]), NULL, unlimited, argv[i]);
      if (0 != res)
	{
	  perror("pthread_create failed\n");
	  exit(res);
	}
    }
  
  for (i = 0; i < NBTHREADS; i++)
    pthread_join(my_threads[i], NULL);
 
  #ifdef DEBUG
  printf("[%s] all my thread are done\n",__FUNCTION__);
  #endif

  return EXIT_SUCCESS;
}
