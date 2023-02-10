! DESCRIPTION:  Parallel code for simulating a cellular automaton running
!                Conway's Game of Life.
! AUTHOR:       Aaron Weeden, Shodor Education Foundation, Inc. 
! DATE:         January 2012
PROGRAM life
IMPLICIT NONE

!!!!!!!!!!!!!!!!!!!!!!!
! Libraries to import !
!!!!!!!!!!!!!!!!!!!!!!!
#ifdef MPI
    INCLUDE "mpif.h"
#endif

#define ALIVE 1
#define DEAD 0

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! Need at least this many rows and columns !
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
INTEGER, PARAMETER :: MINIMUM_ROWS = 1, MINIMUM_COLUMNS = 1, &
    MINIMUM_TIME_STEPS = 1

!!!!!!!!!!!!!!!!
! Main program !
!!!!!!!!!!!!!!!!
INTEGER :: NUMBER_OF_ROWS = 5, NUMBER_OF_COLUMNS = 5, NUMBER_OF_TIME_STEPS = 5,&
    OUR_NUMBER_OF_ROWS = 5, OUR_RANK = 0, NUMBER_OF_PROCESSES = 1, &
    our_current_row, my_current_column, my_neighbor_row, my_neighbor_column, &
    current_time_step, my_number_of_alive_neighbors, i, &
    print_usage = 0, ret_value = 0, ierror = 0
REAL :: random_value
CHARACTER(len=32) :: arg
INTEGER next_lowest_rank, next_highest_rank
INTEGER, ALLOCATABLE, DIMENSION(:,:) :: our_current_grid, our_next_grid
#ifdef SHOW_RESULTS
    INTEGER current_rank
#endif

! I.  Initialize the distributed memory environment
#ifdef MPI
    CALL MPI_INIT(ierror)
    IF(ierror .NE. MPI_SUCCESS) THEN
        WRITE(*,*) "ERROR in MPI_INIT"
        CALL EXIT(-1)
    END IF
    CALL MPI_COMM_RANK(MPI_COMM_WORLD, OUR_RANK, ierror)
    IF(ierror .NE. MPI_SUCCESS) THEN
        WRITE(*,*) "ERROR in MPI_COMM_RANK"
        CALL EXIT(-1)
    END IF
    CALL MPI_COMM_SIZE(MPI_COMM_WORLD, NUMBER_OF_PROCESSES, ierror)
    IF(ierror .NE. MPI_SUCCESS) THEN
        WRITE(*,*) "ERROR in MPI_COMM_SIZE"
        CALL EXIT(-1)
    END IF
#endif

! II.  Parse command line arguments
i = 1
DO WHILE(i <= COMMAND_ARGUMENT_COUNT())
    CALL GET_COMMAND_ARGUMENT(i, arg)
    SELECT CASE (arg)
    CASE ('-r')
        IF(i == COMMAND_ARGUMENT_COUNT()) THEN
            print_usage = 1
        END IF
        CALL GET_COMMAND_ARGUMENT(i+1, arg)
        READ(arg, *) NUMBER_OF_ROWS
        i = i + 1
    CASE ('-c')
        IF(i == COMMAND_ARGUMENT_COUNT()) THEN
            print_usage = 1
        END IF
        CALL GET_COMMAND_ARGUMENT(i+1, arg)
        READ(arg, *) NUMBER_OF_COLUMNS
        i = i + 1
    CASE ('-t')
        IF(i == COMMAND_ARGUMENT_COUNT()) THEN
            print_usage = 1
        END IF
        CALL GET_COMMAND_ARGUMENT(i+1, arg)
        READ(arg, *) NUMBER_OF_TIME_STEPS
        i = i + 1
    CASE DEFAULT
        print_usage = 1
    END SELECT
    i = i + 1
END DO
IF(print_usage == 1) THEN
    WRITE(*,*) "Available command line arguments:"
    WRITE(*,*) "\t-r NUMBER_OF_ROWS\n\t-c NUMBER_OF_COLUMNS\n\t-t NUMBER_OF_TIME_STEPS"
    STOP
END IF

! III.  Make sure we have enough rows, columns, and time steps
ret_value = assert_minimum_value("             row", NUMBER_OF_ROWS, &
    MINIMUM_ROWS)
ret_value = ret_value + assert_minimum_value("          column", &
    NUMBER_OF_COLUMNS, MINIMUM_COLUMNS)
ret_value = ret_value + assert_minimum_value("       time step", &
    NUMBER_OF_TIME_STEPS, MINIMUM_TIME_STEPS)

! IV.  Exit if we don't
IF(ret_value .ne. 0) THEN
    CALL EXIT(-1)
END IF

! V.  Determine our number of rows
OUR_NUMBER_OF_ROWS = NUMBER_OF_ROWS / NUMBER_OF_PROCESSES
IF(OUR_RANK == NUMBER_OF_PROCESSES - 1) THEN
    OUR_NUMBER_OF_ROWS = OUR_NUMBER_OF_ROWS &
        + MODULO(NUMBER_OF_ROWS, NUMBER_OF_PROCESSES)
END IF

! VI.  Allocate enough space in our current grid and next grid for the number of!   rows and the number of columns, plus the ghost rows and columns
ALLOCATE(our_current_grid((OUR_NUMBER_OF_ROWS + 2), (NUMBER_OF_COLUMNS + 2)))
ALLOCATE(our_next_grid((OUR_NUMBER_OF_ROWS + 2), (NUMBER_OF_COLUMNS + 2)))

! VII.  Initialize the grid (each cell gets a random state)
DO our_current_row = 2, OUR_NUMBER_OF_ROWS + 1, 1
#ifdef OPENMP
    !$OMP parallel do private(my_current_column)
#endif
    DO my_current_column = 2, NUMBER_OF_COLUMNS + 1, 1
        CALL RANDOM_NUMBER(random_value)
        our_current_grid(our_current_row, my_current_column) &
            = MODULO(INT(random_value * 10), ALIVE + 1)
    END DO
END DO
    
! VIII.  Determine the process with the next-lowest rank
IF(OUR_RANK == 0) THEN
    next_lowest_rank = NUMBER_OF_PROCESSES - 1
ELSE
    next_lowest_rank = OUR_RANK - 1
END IF

! IX.  Determine the process with the next-highest rank
IF(OUR_RANK == NUMBER_OF_PROCESSES - 1) THEN
    next_highest_rank = 0
ELSE
    next_highest_rank = OUR_RANK + 1
END IF

! X.  Run the simulation for the specified number of time steps
DO current_time_step = 1, NUMBER_OF_TIME_STEPS, 1
    ! X.B.  Set up the ghost columns
    DO our_current_row = 1, OUR_NUMBER_OF_ROWS + 2, 1
        ! X.B.1.  The left ghost column is the same as the farthest-right,
        !  non-ghost column
        our_current_grid(our_current_row, 1) = &
            our_current_grid(our_current_row, NUMBER_OF_COLUMNS + 1)

        ! X.B.2.  The right ghost column is the same as the farthest-left,
        !  non-ghost column */
        our_current_grid(our_current_row, NUMBER_OF_COLUMNS + 2) = &
            our_current_grid(our_current_row, 2)
    END DO

#ifdef MPI

    ! X.A.1.  Send our second-from-the-top row to the process with the 
    !  next-lowest rank
    CALL MPI_SEND(our_current_grid(2,1), NUMBER_OF_COLUMNS, MPI_INTEGER, &
        next_lowest_rank, 0, MPI_COMM_WORLD, ierror)
    IF(ierror .NE. MPI_SUCCESS) THEN
        WRITE(*,*) "ERROR in MPI_SEND"
        CALL EXIT(-1)
    END IF

    ! X.A.2.  Send our second-from-the-bottom row to the process with the 
    !  next-highest rank
    CALL MPI_SEND(our_current_grid(OUR_NUMBER_OF_ROWS + 1,1), &
        NUMBER_OF_COLUMNS, MPI_INTEGER, next_highest_rank, 0, &
        MPI_COMM_WORLD, ierror)
    IF(ierror .NE. MPI_SUCCESS) THEN
        WRITE(*,*) "ERROR in MPI_SEND"
        CALL EXIT(-1)
    END IF

    ! X.A.3.  Receive our bottom row from the process with the next-highest rank
    CALL MPI_RECV(our_current_grid(OUR_NUMBER_OF_ROWS + 2,1), &
           NUMBER_OF_COLUMNS, MPI_INTEGER, next_highest_rank, 0, &
           MPI_COMM_WORLD, MPI_STATUS_IGNORE, ierror)
    IF(ierror .NE. MPI_SUCCESS) THEN
        WRITE(*,*) "ERROR in MPI_RECV"
        CALL EXIT(-1)
    END IF

    ! X.A.4.  Receive our top row from the process with the next-lowest rank
    CALL MPI_RECV(our_current_grid(1,1), NUMBER_OF_COLUMNS, MPI_INTEGER, &
           next_lowest_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE, ierror) 
    IF(ierror .NE. MPI_SUCCESS) THEN
        WRITE(*,*) "ERROR in MPI_RECV"
        CALL EXIT(-1)
    END IF
#else
#ifdef OPENMP
        !$OMP parallel do private(my_current_column)
#endif
        DO my_current_column = 1, NUMBER_OF_COLUMNS + 2, 1
            ! X.A.1.  Set our top row to be the same as our second-to-last row
            our_current_grid(1, my_current_column) = &
                our_current_grid(OUR_NUMBER_OF_ROWS + 1, my_current_column)

            ! X.A.2.  Set our bottom row to be the same as our second-to-top row
            our_current_grid(OUR_NUMBER_OF_ROWS + 2, my_current_column) = &
                our_current_grid(2, my_current_column)
        END DO
#endif

    ! X.C.  Display our current grid
#ifdef SHOW_RESULTS
    DO current_rank = 0, NUMBER_OF_PROCESSES - 1, 1
    WRITE(*,*) "Time Step ", current_time_step, " Rank ", OUR_RANK
    WRITE(*,*) ""
        DO our_current_row = 1, OUR_NUMBER_OF_ROWS + 2, 1
            IF(our_current_row == 2) THEN
                DO my_current_column = 1, NUMBER_OF_COLUMNS + 2 + 2, 1
                    WRITE(*,"(A)",ADVANCE="no") "- "
                END DO
                WRITE(*,*) ""
            END IF

            DO my_current_column = 1, NUMBER_OF_COLUMNS + 2, 1 
                IF(my_current_column == 2) THEN
                    WRITE(*,"(A)",ADVANCE="no") "| "
                END IF

                WRITE(*,"(I1,A)",ADVANCE="no") our_current_grid(our_current_row, my_current_column), " "
                    
                IF(my_current_column == NUMBER_OF_COLUMNS + 1) THEN
                    WRITE(*,"(A)",ADVANCE="no") "| "
                END IF
            END DO
            WRITE(*,*) ""

            IF(our_current_row == OUR_NUMBER_OF_ROWS + 1) THEN
                DO my_current_column = 1, NUMBER_OF_COLUMNS + 2 + 2, 1
                    WRITE(*,"(A)",ADVANCE="no") "- "
                END DO
                WRITE(*,*) ""
            END IF
        END DO
    END DO
#endif
        
    ! X.D.  Determine our next grid -- for each row, do the following:
    DO our_current_row = 2, OUR_NUMBER_OF_ROWS + 1, 1
        ! X.D.1.  For each column, spawn threads to do the following:
#ifdef OPENMP
        !$OMP parallel do private(my_current_column, my_number_of_alive_neighbors)
#endif
        DO my_current_column = 2, NUMBER_OF_COLUMNS + 1, 1
            ! X.D.1.a.  Initialize the count of ALIVE neighbors to 0
            my_number_of_alive_neighbors = 0

            ! X.D.1.b.  For each row of the cell's neighbors, do the following:
            DO my_neighbor_row = our_current_row - 1, our_current_row + 1, 1
                ! X.D.1.b.i.  For each column of the cell's neighbors, do the 
                !  following:
                DO my_neighbor_column = my_current_column - 1, &
                    my_current_column + 1, 1
                    ! X.D.1.b.i.I.  If the neighbor is not the cell itself,
                    !  and the neighbor is ALIVE, do the following:
                    IF(our_current_grid(my_neighbor_row, my_neighbor_column) &
                        == ALIVE) THEN
                        ! X.D.1.b.i.I.  If the neighbor is not the cell itself,
                        !  and the neighbor is ALIVE, do the following:
                        IF((my_neighbor_row /= our_current_row) &
                            .OR. (my_neighbor_column /= my_current_column)) THEN
                            ! X.D.1.b.i.I.A.  Add 1 to the count of the number 
                            !  of ALIVE neighbors
                            my_number_of_alive_neighbors &
                                = my_number_of_alive_neighbors + 1
                        END IF
                    END IF
                END DO
            END DO 

            ! X.D.1.c.  Apply Rule 1 of Conway's Game of Life
            IF(my_number_of_alive_neighbors < 2) THEN
                our_next_grid(our_current_row, my_current_column) = DEAD
            END IF

            ! X.D.1.d.  Apply Rule 2 of Conway's Game of Life
            IF(our_current_grid(our_current_row, my_current_column) == ALIVE &
                .AND. (my_number_of_alive_neighbors == 2 .OR. &
                my_number_of_alive_neighbors == 3)) THEN
                our_next_grid(our_current_row, my_current_column) = ALIVE
            END IF

            ! X.D.1.e.  Apply Rule 3 of Conway's Game of Life
            IF(my_number_of_alive_neighbors > 3) THEN
                our_next_grid(our_current_row, my_current_column) = DEAD
            END IF

            ! X.D.1.f.  Apply Rule 4 of Conway's Game of Life
            IF(our_current_grid(our_current_row, my_current_column) == DEAD &
                .AND. my_number_of_alive_neighbors == 3) THEN
                our_next_grid(our_current_row, my_current_column) = ALIVE
            END IF
        END DO
    END DO

    ! X.E.  Spawn threads to copy the next grid into the current grid
    DO our_current_row = 2, OUR_NUMBER_OF_ROWS + 1, 1
#ifdef OPENMP
        !$OMP parallel do private(my_current_column)
#endif
        DO my_current_column = 2, NUMBER_OF_COLUMNS + 1, 1
            our_current_grid(our_current_row, my_current_column) = &
                our_next_grid(our_current_row, my_current_column)
        END DO
    END DO
END DO

! XI.  Deallocate data structures
DEALLOCATE(our_next_grid)
DEALLOCATE(our_current_grid)

! XII.  Finalize the distributed memory environment
#ifdef MPI
    CALL MPI_FINALIZE(ierror)
    IF(ierror .NE. MPI_SUCCESS) THEN
        WRITE(*,*) "ERROR in MPI_FINALIZE"
        CALL EXIT(-1)
    END IF
#endif

CONTAINS

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! Add an "s" to the end of a value's name if needed !
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
SUBROUTINE pluralize_value_if_needed(value)
    IMPLICIT NONE
    
    INTEGER, INTENT(IN) :: value
    IF(value /= 1) THEN
        WRITE (*,"(A)", advance="no") "s"
    END IF
END SUBROUTINE pluralize_value_if_needed

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! Make sure a value is >= another value, print error and return -1 if it isn't !
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
INTEGER FUNCTION assert_minimum_value(which_value, actual_value, expected_value)
    IMPLICIT NONE

    CHARACTER(len=16), INTENT(IN) :: which_value
    INTEGER, INTENT(IN) :: actual_value, expected_value

    IF(actual_value < expected_value) THEN
        WRITE (*, "(A, I16, A, A)", advance="no") "ERROR: ", actual_value, " ",&
            which_value
        CALL pluralize_value_if_needed(actual_value)
        WRITE (*, "(A, I16, A, A)", advance="no") "; need at least ", &
            expected_value, " ", which_value
        CALL pluralize_value_if_needed(expected_value)
        WRITE (*,*) ""
        assert_minimum_value = -1
    ELSE
        assert_minimum_value = 0
    END IF
END FUNCTION assert_minimum_value

END PROGRAM life
