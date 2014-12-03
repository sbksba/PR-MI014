#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>

int nfork(int nb_fils){
	pid_t pid_fils;
	int i=0;
	
	while (i< nb_fils){
		pid_fils = fork();

		switch(pid_fils)
		{
			case -1:
				if (i==0) return -1;
				else {printf("ERREUR\n");return i;}
				break;
			case 0:
				exit(1);
				break;
		}
		i++;
	}
	
	#ifdef DEBUG
	printf("I : %d\n",i);
	#endif
	
	return 0;
}

int main (int arg, char* argv []) {
    int p;
    int i=1; int N = 3;
    do {
        p = nfork (i) ;
        if (p != 0 )
               printf ("%d \n",p); 
    } while ((p ==0) && (++i<=N));
    printf ("FIN \n");      
   return EXIT_SUCCESS;
}
