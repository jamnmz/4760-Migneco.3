/*
 * Jared Migneco
 * Project 3
 * Hauschild - 4760
 * 10-25-22
 * Child Class
 *
 * takes poroject 2 and tries to make work and implement clock
 *
 */


#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>


#define SHMKEY  960158     /* Parent and child agree on common key.*/
#define BUFF_SZ sizeof ( int )

int main(int argc, char** argv)
{
        printf("Worker: We are in the child class\n");
        int shmid = shmget ( SHMKEY, BUFF_SZ, 9045);

        //intializing variables and setting value of m to passed parameter m from parent
        int m = atoi(argv[1]);
        int x;
        int i;
        printf("Worker: the value of m is: %d\n", m);
        //int for what m is after 1000000
        int mAdd = m*1000000;

        //shared memory integers
        char *secClock = (char*) (shmat(SHMKEY, NULL, 9045));
        char *nanoClock = (char*) (shmat(SHMKEY, NULL , 9045));

        int *sharedNanoClock = (int*)(secClock);
        int *sharedSecClock = (int*)(nanoClock);

        printf("Worker: about to enter the for loop\n");
        //for loop to reach m processes
        for(x = 0; x < m; x++)
        {
                printf("Worker: Entering the for loop\n");
                //adds the million value to nanoClock
                *nanoClock += mAdd;
                printf("Worker: added to clock loop\n");

                //worker prints its pid, iteration number, and increment amount
                printf("Worker: %d",getpid());
                printf(": Iteration %d:", m);
                printf("  incrementing by %d\n", mAdd);

                if ((*sharedNanoClock >= 1000000000) && (*sharedNanoClock < 2000000000))
                {
                        *sharedNanoClock -= 1000000000;
                        *sharedSecClock++;
                }
                //in case nanoClock has somehow gone over 2000000000
                else if (*sharedNanoClock >= 2000000000)
                {
                        //creates loop to increase second clock and decrese nanoClock
                        while(*sharedNanoClock >= 1000000000)
                        {
                                *sharedNanoClock -= 1000000000;
                                *sharedSecClock++;
                        }
                }

        }

        printf("\nWorker is now ending.\n");
        //freeing memory
        shmdt(secClock);
        shmdt(nanoClock);
        shmdt(sharedSecClock);
        shmdt(sharedNanoClock);
        sleep(3);
        return EXIT_SUCCESS;
}
