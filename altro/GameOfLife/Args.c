#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>


#define INIT 1
#define RUN  2

#define K_DFLT 100

#define ORDERED 0
#define STATIC  1


char fname_deflt[] = "game_of_life.pgm";

int   action = 0;
int   k      = K_DFLT;
int   e      = ORDERED;
int   n      = 10000;
int   s      = 1;
char *fname  = NULL;


int doJob(){
    printf("faccio cose");
}

int main ( int argc, char *argv[] )

{
  int action = 0;
  char *optstring = "irk:e:f:n:s:";

  int c;
  while ((c = getopt(argc, argv, optstring)) != -1) {
    switch(c) {
      
    case 'i':
      // action = INIT; break;
      printf(" - %c",c);
       break;
    
    case 'k':
      // k = atoi(optarg); break;
     printf(" - %c",c);
       break;
    case 'e':
      // e = atoi(optarg); break;
     printf(" - %c",c);
       break;
    case 'f':
      // fname = (char*)malloc( sizeof(optarg)+1 );
      // sprintf(fname, "%s", optarg );
      // break;
     printf(" - %c",c);
       break;
    case 'n':
      // n = atoi(optarg); break;
    printf(" - %c",c);
       break;
    case 's':
      // s = atoi(optarg); break;
     printf(" - %c",c);
       break;
        case 'r':
    //   action = RUN; break;
     printf(" - %c",c);
     doJob();
       break;
    default :
      printf("argument -%c not known\n", c ); break;
    }
  }

    if ( fname != NULL )
      free ( fname );

  return 0;
}