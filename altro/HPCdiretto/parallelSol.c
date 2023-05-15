

               // Game of life  --  parallel solution    //

                                  // Michel Vallieres, Fall 2009 //

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>          /* MPI header file */
#include <time.h>

#define DEAD 0
#define ALIVE 1

#define generations 75

#define matrix_size 100


/* For each element of the matrix apply the */
/* life game rules                          */
/* store under temp                         */
void new_generation(  int ** matrix, int ** temp,
                      int la, int lb )
{
  int i, j, sum;

  /* For each element of the matrix apply the */
  /* life game rules                          */
  /* store under temp                         */
  for (i = la; i <= lb; i++) 
    {
      for (j = 1; j <= matrix_size; j++) 
	{

	  /* find out the neighbors of the current cell */
	  sum = matrix[i-1][j-1] + matrix[i-1][j] + matrix[i-1][j+1] 
	    + matrix[i][j-1] + matrix[i][j+1] 
            + matrix[i+1][j-1] + matrix[i+1][j] + matrix[i+1][j+1] ;
        
	  /* check if the cell dies or life is born */
 
          /* dies of isolation */
	  if (sum < 2 || sum > 3)
	    temp[i][j] = DEAD ;

	  /* born through neighbors */
	  else if (sum == 3)
	    temp[i][j] = ALIVE ;

	  /* life continues */
	  else
	    temp[i][j] = matrix[i][j] ;
	}
    }
}


void swap_matrices(  int ** matrix, int ** temp, 
                     int *changes, int la, int lb )
{
  int i, j, aux;

  /* Swap the matrices */
  *changes = 0;
  for (i = la; i <= lb; i++) 
    {
      for (j = 1; j <= matrix_size; j++) 
	{
	  aux = matrix[i][j];
          matrix[i][j] = temp[i][j];
          temp[i][j] = aux;
          if ( matrix[i][j] != temp[i][j] )
	    (*changes)++;
	}
    }
}


/* output life matrix, one number per line */
void output_life_matrix( int ** matrix,
                            int iter, int changes )
{
  int i, j;

  /* output new configuration */
  for (i = 1; i <= matrix_size; i++) 
    {
      for (j = 1; j <= matrix_size; j++) 
	printf("%d\n", matrix[i][j] );
    }
  fprintf( stderr, "Iteration: %d  -- No of changes: %d\n", 
	   iter, changes );
}



/* uniformly disdributed random # in {0,1} */
double rand01()
{
  return  (double)rand()/(double)RAND_MAX;
}


/* sets initial life matrix and boundary */
void initial( int ** matrix, int ** temp, int myrank )
{
  int i, j;

  /* Initialize the boundaries of the life matrix */
  for (j = 0; j < matrix_size+2; j++)
    {
      matrix[0][j] = DEAD;
      matrix[matrix_size+1][j] = DEAD;
      temp[0][j] = DEAD;
      temp[matrix_size+1][j] = DEAD ;
    }
  for (i = 0; i < matrix_size+2; i++)
    {
      matrix[i][0] = DEAD;
      matrix[i][matrix_size+1] = DEAD;
      temp[i][0] = DEAD;
      temp[i][matrix_size+1] = DEAD ;
    }
  if( myrank == 0 )
    fprintf( stderr, "Boundary set\n");

  /* Initialize the life matrix */
  for (i = 1; i <= matrix_size; i++)  
    {
      for (j = 1; j<= matrix_size; j++)
	if (rand01() > 0.5)  
	  matrix[i][j] = ALIVE ;
	else
	  matrix[i][j] = DEAD ;
    }
  if( myrank == 0 )
    fprintf( stderr, "Population initialized\n");

}


/* Domain decomposition in strips based on rows */
void decompose_domain( int *start_strip, int *end_strip, 
		       int MPIsize, int myrank )
{
  int ipointer, pe, strip_size;
 
  if ( myrank == 0  )
    fprintf( stderr, "Strips: " );
  start_strip[1] = 1;
  strip_size = ( matrix_size ) / ( MPIsize - 1 ) + 1;
  for (  pe=1; pe<MPIsize ; pe++ )
    {
      end_strip[pe] = start_strip[pe] + strip_size - 1;
      if ( pe == MPIsize-1 )
	end_strip[pe] = matrix_size;
      else
	start_strip[pe+1] = end_strip[pe] + 1;
      if ( myrank == 0  )
	{
	  fprintf( stderr, " [ %d , %d ]", 
		   start_strip[pe], end_strip[pe] );
          if ( pe == MPIsize-1 )
	    fprintf( stderr, "\n" );
	}
    }
}


void strips_boundary_exchange( int **matrix, int *start_strip, 
			       int *end_strip, int myrank, 
			       int MPIsize )
{
  MPI_Status recv_status;
  int top_process, bot_process;

  top_process = myrank+1;
  bot_process = myrank-1;


  /* odd nodes */
  if ( myrank % 2 != 0 )
    {
      if ( myrank < MPIsize-1 )  /* send to right - top */
	MPI_Send( &matrix[end_strip[myrank]][0], matrix_size+2, 
		  MPI_INT, top_process, 102, MPI_COMM_WORLD );
      if ( myrank > 1 )          /* recv from left - bottom */
	MPI_Recv( &matrix[start_strip[myrank]-1][0], matrix_size+2, 
		  MPI_INT, bot_process, 101, MPI_COMM_WORLD,
		  &recv_status );
      if ( myrank > 1 )          /* send to left - bottom */
	MPI_Send( &matrix[start_strip[myrank]][0], matrix_size+2, 
		  MPI_INT, bot_process, 104, MPI_COMM_WORLD );
      if ( myrank < MPIsize-1 )  /* recv from right - top */
	MPI_Recv( &matrix[end_strip[myrank]+1][0], matrix_size+2, 
		  MPI_INT, top_process, 103, MPI_COMM_WORLD,
		  &recv_status );
    }
  /* even nodes */
  else
    {	      
       if ( myrank > 1 )           /* recv from left - bottom */
	MPI_Recv( &matrix[start_strip[myrank]-1][0], matrix_size+2, 
		  MPI_INT, bot_process, 102, MPI_COMM_WORLD,
		  &recv_status );
      if ( myrank < MPIsize-1 )   /* send to right - top */
	MPI_Send( &matrix[end_strip[myrank]][0], matrix_size+2, 
		  MPI_INT, top_process, 101, MPI_COMM_WORLD );
      if ( myrank < MPIsize-1 )   /* recv from right - top */
	MPI_Recv( &matrix[end_strip[myrank]+1][0], matrix_size+2, 
		  MPI_INT, top_process, 104, MPI_COMM_WORLD,
		  &recv_status );       
      if ( myrank > 1 )           /* send to left - bottom */
	MPI_Send( &matrix[start_strip[myrank]][0], matrix_size+2, 
		  MPI_INT, bot_process, 103, MPI_COMM_WORLD );
    }
}


int main( int argc, char * argv[] )
{
  int i, j, iter, changes, total_changes;
  int **matrix, **temp;

  int *transfer;
  int *start_strip, *end_strip;
  int myrank, MPIsize, pe;
  int ipointer, strip_size, mesg_size, from_strip;


  MPI_Status recv_status;


  fprintf( stderr, "Game starts\n");

                          /* init into MPI */
  MPI_Init(&argc, &argv);


  clock_t start, end;//, start_parallel, end_parallel;
  double exec_time_total;//, exec_time_parallel;
  start = clock();

                          /* my rank - my id */
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
                          /* how many processes in the virtual machine */
  MPI_Comm_size(MPI_COMM_WORLD, &MPIsize);
 
  fprintf( stderr, "MPI communicator has been attached\n");


  /* allocate memory for the matrices */
  matrix = (int **)malloc( sizeof(int *) * (matrix_size+2) );
  temp = (int **)malloc( sizeof(int *) * (matrix_size+2) );
  for (i = 0; i < matrix_size+2; i++) 
    {
      matrix[i] = (int *)malloc( sizeof(int) * (matrix_size+2) );
      temp[i] = (int *)malloc( sizeof(int) * (matrix_size+2) );
    }
  transfer = (int *)malloc( sizeof(int) * 
			    (matrix_size+2) * (matrix_size+2) );
  if( myrank == 0 )
    fprintf( stderr, "Memory for matrix set aside \n");


  /* set strip geometry & size*/
  start_strip = (int *)malloc( sizeof(int) * MPIsize );
  end_strip = (int *)malloc( sizeof(int) * MPIsize );
  decompose_domain( start_strip, end_strip, MPIsize, myrank );
 

  /* Initialize the boundaries of the life matrix */
  /* Initialize the life matrix itself            */
  /* node 0 is ONLY for output */
  initial( matrix, temp, myrank );


  /* Ouput initial matrix */
  if( myrank == 0 )
    //output_life_matrix( matrix, 0, 0 );


  /* iterate over generations */
  for( iter=1 ; iter<=generations ; iter++ )
    {
                               /* nodes non-zero */
      if ( myrank != 0 )
	{
	  /* generate a new generation in strip */
	  new_generation( matrix, temp,
			  start_strip[myrank], 
			  end_strip[myrank] );

	  /* swap old strip content for new generation */
	  swap_matrices( matrix, temp, &changes,
			 start_strip[myrank], 
			 end_strip[myrank] );

	  /* internal boundaries between strips */
	  /* real boundary is not affected */
	  strips_boundary_exchange( matrix, start_strip, end_strip,
				    myrank, MPIsize );

	  /* synchronize all nodes */
	  MPI_Barrier( MPI_COMM_WORLD );

          /* counter of changes */
	  MPI_Reduce( &changes, &total_changes, 1, MPI_INT, 
		      MPI_SUM, 0, MPI_COMM_WORLD );

	  /* send strip to node 0 */
	  ipointer = 0;
	  for( i=start_strip[myrank]; i<=end_strip[myrank]; i++ )
	      for( j=0 ; j<=matrix_size+1 ; j++ )
		{
		  transfer[ipointer] = matrix[i][j];
		  ipointer++;
		}
          mesg_size = ipointer++;
	  MPI_Send( &transfer[0], mesg_size, 
		  MPI_INT, 0, 121, MPI_COMM_WORLD );
	}

                                /* node 0 */
      else
	{
	  /* synchronize all nodes */
	  MPI_Barrier( MPI_COMM_WORLD );

          /* counter of changes */
	  MPI_Reduce( &changes, &total_changes, 1, MPI_INT, 
		      MPI_SUM, 0, MPI_COMM_WORLD );

	  /* receive content of the strips */
	  strip_size = ( matrix_size ) / ( MPIsize - 1 ) + 1;
	  mesg_size = ( matrix_size + 2 ) * strip_size;
	  for( pe=1; pe <= MPIsize-1 ; pe++)
	    {
 	      MPI_Recv( &transfer[0], mesg_size, 
		  MPI_INT, MPI_ANY_SOURCE, 121, MPI_COMM_WORLD,
		  &recv_status );
              from_strip = recv_status.MPI_SOURCE;
	      ipointer = 0;
	      for( i=start_strip[from_strip]; 
		   i<= end_strip[from_strip]; i++ )
		{
		  for( j=0; j<=matrix_size+1; j++ )
		    {
		      matrix[i][j] = transfer[ipointer];
		      ipointer++;
		    }
		}
	    }

	  /* display current generation */
	  //output_life_matrix( matrix, iter, total_changes );

	}
    }

                          /* out of the virtual machine */
  
  end = clock();
  exec_time_total = ((double)(end - start) / CLOCKS_PER_SEC) * 1000;
  printf("exec_time_total: %f ms\n", exec_time_total);
  MPI_Finalize();
  printf("end of the program");
  exit(0);
}