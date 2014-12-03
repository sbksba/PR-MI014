#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

/***********/
/*UNLIMITED*/
/***********/
void* unlimited(void* arg)
{
  char* filename = (char*) arg;
  FILE *fp1, *fp2;
  int c = 1;
  
  #ifdef DEBUG
  printf("[%s] started with filename=%s\n",__FUNCTION__ ,filename);
  #endif

  fp1 = fopen(filename, "r");
  fp2 = fopen(filename, "r+");
  
  if ((fp1 == NULL) || (fp2 == NULL))
    {
      perror("fopen");
      exit(1);
    }
  
  while (c != EOF)
    {
      c = fgetc(fp1);
      if (c != EOF)
	fputc(toupper(c), fp2);
    }
  
  fclose(fp1);
  fclose(fp2);
  
  pthread_exit(EXIT_SUCCESS);
}

/******/
/*POOL*/
/******/
void unlimitedPOOL(void* arg)
{
  char* filename = (char*) arg;
  FILE *fp1, *fp2;
  int c = 1;
  
  #ifdef DEBUG
  printf("[%s] started with filename=%s\n",__FUNCTION__ ,filename);
  #endif

  fp1 = fopen(filename, "r");
  fp2 = fopen(filename, "r+");
  
  if ((fp1 == NULL) || (fp2 == NULL))
    {
      perror("fopen");
      exit(1);
    }
  
  while (c != EOF)
    {
      c = fgetc(fp1);
      if (c != EOF)
	fputc(toupper(c), fp2);
    }
  
  fclose(fp1);
  fclose(fp2);
  
  return;
}
