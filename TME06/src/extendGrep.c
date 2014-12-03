#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/mman.h>

#define NBMAX 100
#define NBPROC 5
#define dbg printf("LINE : %d\n",__LINE__)

int main(int argc,char**argv)
{
#ifdef DEBUG
  dbg;
#endif
  
  struct dirent *strdir;
  int nb=0;
  FILE* f;
  char s[100];

#ifdef DEBUG
  dbg;
#endif

  if (argc!=3)
    {
      printf("USE: %s <exp> <path>",argv[0]);
      return EXIT_FAILURE;
    }
  
  DIR *dir1=opendir(argv[2]);
  
#ifdef DEBUG
  dbg;
#endif
  
  if (!dir1)
    {
      perror("opendir");
      return EXIT_FAILURE;
    }
  
#ifdef DEBUG
  dbg;
#endif
  while ((strdir=readdir(dir1))!=NULL&&nb<NBMAX)
    {
#ifdef DEBUG
      dbg;
#endif
      if (strdir->d_name[0]!='.')
	{
	  strcpy(s,argv[2]);
	  strcat(s,"/");
	  strcat(s,strdir->d_name);
	  printf("+FILE [%s]\n",s);
	  if ((f=fopen(s,"r"))==NULL)
	    {
	      perror("fopen");
	      return EXIT_FAILURE;
	    }
#ifdef DEBUG
	  dbg;
#endif
	  while(!feof(f))
	    {
#ifdef DEBUG
	      dbg;
#endif
	      fscanf(f,"%s",s);
#ifdef DEBUG
	      dbg;
	      printf("S => %s\n",s);
#endif
	      if (strstr(s,argv[1]))
		{
		  printf("\tFIND key-word [%s] in [%s]\n",argv[1],strdir->d_name);
		  nb++;
		  break;
		}
	    }
#ifdef DEBUG
	  dbg;
#endif
	  fclose(f);
	}
    }
  
#ifdef DEBUG
  dbg;
#endif
  
  if (!nb)
    printf("no valide file\n");
  closedir(dir1);
      
  return EXIT_SUCCESS;
}
