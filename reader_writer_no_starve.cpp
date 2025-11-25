//
// Example from: http://www.amparo.net/ce155/sem-ex.c
//
// Adapted using some code from Downey's book on semaphores
//
// Compilation:
//
//       g++ reader_writer_no_starve.cpp -lpthread -o reader_writer_no_starve -lm
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




/* global vars */
const int bufferSize = 5;
const int numReaders = 5; 
const int numWriters = 5; 

class Lightswitch {
public:
    Lightswitch()
        : counter(0), mutex(1)   // mutex starts at 1, like the Python version
    {}

    void lock(Semaphore &semaphore) 
    {
        mutex.wait();
        counter += 1;

        if (counter == 1) 
        {
            semaphore.wait();
        }
        mutex.signal();
    }

    void unlock(Semaphore &semaphore) 
    {
        mutex.wait();
        counter -= 1;

        if (counter == 0) 
        {
            semaphore.signal();
        }
        mutex.signal();
    }

private:
    int counter;
    Semaphore mutex;
};

/* semaphores are declared global so they can be accessed
 in main() and in thread routine. */
Lightswitch readSwitch = Lightswitch();
Semaphore roomEmpty(1);  
Semaphore turnstile(1);      
Semaphore readSwitchSemaphore(1);

int readers = 0;

/*
    Writer function 
*/
void *Writer ( void *threadID )
{
    // Thread number 
    int x = (long)threadID;

    while( 1 )
    {
        sleep(3); // Slow the thread down a bit so we can see what is going on
        turnstile.wait();
        roomEmpty.wait();
        printf("Writer %d is writing \n", x);
        fflush(stdout);

        turnstile.signal();
        roomEmpty.signal();
    }

}

/*
    Reader function 
*/
void *Reader ( void *threadID )
{
    // Thread number 
    int x = (long)threadID;
    
    while( 1 )
    {
        turnstile.wait();
        turnstile.signal();

        readSwitch.lock(readSwitchSemaphore);

        printf("Reader %d is reading \n", x);
        fflush(stdout);
        
        readSwitch.unlock(readSwitchSemaphore);

        sleep(5);   // Slow the thread down a bit so we can see what is going on
    }

}


int main(int argc, char **argv )
{
    pthread_t writerThread[ numWriters ];
    pthread_t readerThread[ numReaders ];

    // Create the readers 
    for( long p = 0; p < numWriters; p++ )
    {
        int rc = pthread_create ( &writerThread[ p ], NULL, 
                                  Writer, (void *) (p+1) );
        if (rc) 
        {
            printf("ERROR creating producer thread # %ld; \
                    return code from pthread_create() is %d\n", p, rc);
            exit(-1);
        }
    }

    // Create the writers 
    for( long c = 0; c < numReaders; c++ )
    {
        int rc = pthread_create ( &readerThread[ c ], NULL, 
                                  Reader, (void *) (c+1) );
        if (rc) 
        {
            printf("ERROR creating consumer thread # %ld; \
                    return code from pthread_create() is %d\n", c, rc);
            exit(-1);
        }
    }

    printf("Main: program completed. Exiting.\n");


    // To allow other threads to continue execution, the main thread 
    // should terminate by calling pthread_exit() rather than exit(3). 
    pthread_exit(NULL); 


} /* main() */






