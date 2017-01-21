#include <stdio.h>

#include "inc.h"

// Public
const int MAX_MUTEXES = 1030;

// Private
Queue lockQueues[MAX_MUTEXES];
int lockNumbers[MAX_MUTEXES];
int firstFreeMutexIndex;

int initLocks()
{
  for ( int i = 0; i < MAX_MUTEXES; i++ )
  {
    initQueue( lockQueues + i );
    lockNumbers[i] = -1;
  }

  firstFreeMutexIndex = 0;
}

int lock( int callerId, int mutexId )
{
  if ( verbose ) printf( "Locking %d...\n", mutexId );

  Queue *q = lockQueues;
  initQueue( q );

  for ( int i = 0; i < 10; i++ )
  {
    enqueue( q, i );
  }

  printQueue( q );

  if ( verbose ) printf( "Locking %d done\n", mutexId );
}

int unlock( int callerId, int mutexId )
{
}
