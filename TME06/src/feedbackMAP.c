#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

#define FILE_MODE 0600

int err_sys(const char* x) 
{ 
  perror(x); 
  return EXIT_FAILURE; 
}

int main(int argc, char **argv)
{
  int fdin, fdout;
  char *src, *dst;
  struct stat statbuf;

  if (argc != 3)
    err_sys("USE: ./feedBACKMAP <fromfile> <tofile>");

  /* open the input file */
  if ((fdin = open (argv[1], O_RDONLY)) < 0)
      err_sys("can't open fdin for reading");

  /* open/create the output file */
  if ((fdout = open (argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)
    err_sys("can't create fdout for writing");
    

  /* find size of input file */
  if (fstat (fdin,&statbuf) < 0)
    err_sys("fstat error");

  /* go to the location corresponding to the last byte */
  if (lseek (fdout, statbuf.st_size - 1, SEEK_SET) == -1)
      err_sys("lseek error");
 
  /* write a dummy byte at the last location */
  if (write (fdout, "", 1) != 1)
    err_sys("write error");
    
  /* mmap the input file */
  if ((src = mmap (0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == -1)
    err_sys ("mmap error for input");
  
  /* mmap the output file */
  if ((dst = mmap (0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0)) == -1)
    err_sys ("mmap error for output");

  /* this copies the input file to the output file */
  memcpy (dst, src, statbuf.st_size);
  
  return EXIT_SUCCESS;
}
