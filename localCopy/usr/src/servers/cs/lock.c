#include <stdio.h>

#include "inc.h"

Queue lockQueues[MAX_MUTEXES];
int mutexIds[MAX_MUTEXES];
int mutexHolders[MAX_MUTEXES];
int firstFreeMutexIndex;

void initLocks()
{
  firstFreeMutexIndex = 0;
}

void cleanLocks()
{
  for ( int i = 0; i < firstFreeMutexIndex; i++ )
  {
    clearQueue( lockQueues + i );
  }

  firstFreeMutexIndex = 0;
}

int hasLock( int callerId, int mutexId )
{
  for ( int i = 0; i < firstFreeMutexIndex; i++ )
  {
    if ( mutexIds[i] == mutexId )
    {
      return mutexHolders[i] == callerId;
    }
  }

  return 0;
}

void swapLocks( int a, int b )
{
  if ( a == b ) return;

  Queue tmpQ = lockQueues[a];
  lockQueues[a] = lockQueues[b];
  lockQueues[b] = tmpQ;

  int tmp = mutexIds[a];
  mutexIds[a] = mutexIds[b];
  mutexIds[b] = tmp;

  tmp = mutexHolders[a];
  mutexHolders[a] = mutexHolders[b];
  mutexHolders[b] = tmp;
}

void lock( int callerId, int mutexId )
{
  if ( verbose ) printf( "Locking %d for %d\n", mutexId, callerId );

  for ( int i = 0; i < firstFreeMutexIndex; i++ )
  {
    if ( mutexIds[i] == mutexId )
    {
      enqueue( lockQueues + i, callerId );

      if ( verbose ) printf( "Enqueued for lock %d, callerId = %d\n", mutexId, callerId );

      return;
    }
  }

  mutexIds[firstFreeMutexIndex] = mutexId;
  mutexHolders[firstFreeMutexIndex] = callerId;
  initQueue( lockQueues + firstFreeMutexIndex );

  firstFreeMutexIndex++;

  sendResponse( callerId, CS_ANS_OK );

  if ( verbose ) printf( "Got lock %d! callerId = %d\n", mutexId, callerId );
}

void unlock( int callerId, int mutexId, int notifyCaller )
{
  if ( verbose ) printf( "Unlocking %d...\n", callerId );

  if ( !hasLock( callerId, mutexId ) )
  {
    if ( verbose ) printf( "Trying to unlock wrong mutex %d\n", mutexId );

    if ( notifyCaller ) sendResponse( callerId, CS_ANS_WRONG );

    return;
  }

  for ( int i = 0; i < firstFreeMutexIndex; i++ )
  {
    if ( mutexIds[i] == mutexId )
    {
      if ( isEmpty( lockQueues + i ) )
      {
        swapLocks( i, firstFreeMutexIndex - 1 );
        firstFreeMutexIndex--;
      }
      else
      {
        int toWake = top( lockQueues + i );
        pop( lockQueues + i );
        mutexHolders[i] = toWake;

        sendResponse( toWake, CS_ANS_OK );
      }

      if ( notifyCaller ) sendResponse( callerId, CS_ANS_OK );

      return;
    }
  }
}
