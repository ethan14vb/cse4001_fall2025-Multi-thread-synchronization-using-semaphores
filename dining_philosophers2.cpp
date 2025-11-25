//
// Example from: http://www.amparo.net/ce155/sem-ex.c
//
// Adapted using some code from Downey's book on semaphores
//
// Compilation:
//
//       g++ dining_philosophers2.cpp -lpthread -o dining_philosophers2 -lm
// or 
//      make
//

#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <string.h>     /* String handling */
#include <semaphore.h>  /* Semaphore */
#include <iostream>
using namespace std;

/*
 This wrapper class for semaphore.h functions is from:
 http://stackoverflow.com/questions/2899604/using-sem-t-in-a-qt-project
 */
class Semaphore {
public:
    // Constructor
    Semaphore(int initialValue)
    {
        sem_init(&mSemaphore, 0, initialValue);
    }
    // Destructor
    ~Semaphore()
    {
        sem_destroy(&mSemaphore); /* destroy semaphore */
    }
    
    // wait
    void wait()
    {
        sem_wait(&mSemaphore);
    }
    // signal
    void signal()
    {
        sem_post(&mSemaphore);
    }
    
    
private:
    sem_t mSemaphore;
};

/* semaphores are declared global so they can be accessed
 in main() and in thread routine. */
Semaphore footman(4);

int philosopher_amount = 5;

Semaphore forks[5] = {
    Semaphore(1),
    Semaphore(1),
    Semaphore(1),
    Semaphore(1),
    Semaphore(1)
};

int left(int i) 
{
    return i;
}

int right(int i)
{
    return (i + 1) % philosopher_amount;
}

void get_forks(int i)
{
    footman.wait();

    if (i % 2) 
    {
        forks[left(i)].wait();
        forks[right(i)].wait();
    } 
    else
    {
        forks[right(i)].wait();
        forks[left(i)].wait();
    }
    
}

void put_forks(int i)
{
        if (i % 2) 
    {
        forks[left(i)].signal();
        forks[right(i)].signal();
    } 
    else
    {
        forks[right(i)].signal();
        forks[left(i)].signal();
    }
    footman.signal();
}

void *Philosopher ( void *threadID )
{
    // Thread number 
    int x = (long)threadID;

    while( 1 )
    {
        printf("Philosopher %d is thinking \n", x + 1);
        fflush(stdout);
        sleep(1);

        get_forks(x);
    
        printf("Philosopher %d is eating \n", x + 1);
        fflush(stdout);
        sleep(1);
        
        put_forks(x);
    }

}


int main(int argc, char **argv )
{
    pthread_t philosopherThread[ philosopher_amount ];

    // Create the readers 
    for( long p = 0; p < philosopher_amount; p++ )
    {
        int rc = pthread_create(&philosopherThread[p], NULL, Philosopher, (void*)p);
        if (rc) 
        {
            printf("ERROR creating producer thread # %ld; \
                    return code from pthread_create() is %d\n", p, rc);
            exit(-1);
        }
    }

    printf("Main: program completed. Exiting.\n");


    // To allow other threads to continue execution, the main thread 
    // should terminate by calling pthread_exit() rather than exit(3). 
    pthread_exit(NULL); 


} /* main() */






