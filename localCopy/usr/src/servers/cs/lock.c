#include <stdio.h>

#include "inc.h"

// Public
const int MAX_MUTEXES = 1030;

// Private
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

void swapLocks( int a, int b )
{
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
  if ( verbose ) printf( "Locking %d...\n", mutexId );

  for ( int i = 0; i < firstFreeMutexIndex; i++ )
  {
    if ( mutexIds[i] == mutexId )
    {
      enqueue( lockQueues + i, callerId );

      if ( verbose ) printf( "Enqueued for lock %d\n", mutexId );

      return;
    }
  }

  mutexIds[firstFreeMutexIndex] = mutexId;
  mutexHolders[firstFreeMutexIndex] = callerId;
  initQueue( lockQueues + firstFreeMutexIndex );

  sendResponse( callerId, CS_ANS_OK );

  if ( verbose ) printf( "Locking %d done\n", mutexId );
}

void unlock( int callerId, int mutexId )
{
  if ( verbose ) printf( "Unlocking %d...\n" );

  for ( int i = 0; i < firstFreeMutexIndex; i++ )
  {
    if ( mutexIds[i] == mutexId )
    {
      if ( mutexHolders[i] != callerId )
      {
        sendResponse( callerId, CS_ANS_WRONG );
      }
      else
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

        sendResponse( callerId, CS_ANS_OK );
      }

      return;
    }
  }

  sendResponse( callerId, CS_ANS_WRONG );

  if ( verbose ) printf( "Trying to unlock wrong mutex %d\n", mutexId );
}
