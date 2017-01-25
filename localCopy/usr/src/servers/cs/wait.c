#include <stdio.h>

#include "inc.h"

int condIds[MAX_MUTEXES];
Queue waitingMutexIds[MAX_MUTEXES];
Queue waitingCallerIds[MAX_MUTEXES];
int firstFreeCondIndex;

void initWaits()
{
  firstFreeCondIndex = 0;
}
void cleanWaits()
{
  for ( int i = 0; i < firstFreeCondIndex; i++ )
  {
    clearQueue( waitingMutexIds + i );
    clearQueue( waitingCallerIds + i );
  }

  firstFreeCondIndex = 0;
}

void swapWaitingQueues( int a, int b )
{
  if ( a == b ) return;

  Queue tmpQ = waitingMutexIds[a];
  waitingMutexIds[a] = waitingMutexIds[b];
  waitingMutexIds[b] = tmpQ;

  tmpQ = waitingCallerIds[a];
  waitingCallerIds[a] = waitingCallerIds[b];
  waitingCallerIds[b] = tmpQ;

  int tmp = condIds[a];
  condIds[a] = condIds[b];
  condIds[b] = tmp;
}

void wait( int callerId, int mutexId, int condId )
{
  if ( verbose ) printf( "Wait with mutex: %d on cond: %d for caller %d\n", mutexId, condId, callerId );

  if ( !hasLock( callerId, mutexId ) )
  {
    sendResponse( callerId, CS_ANS_WRONG );
  }

  int foundCondition = 0;

  for ( int i = 0; i < firstFreeCondIndex; i++ )
  {
    if ( condIds[i] == condId )
    {
      enqueue( waitingMutexIds + i, mutexId );
      enqueue( waitingCallerIds + i, callerId );
      foundCondition = 1;
      break;
    }
  }

  if ( !foundCondition )
  {
    initQueue( waitingMutexIds + firstFreeCondIndex );
    enqueue( waitingMutexIds + firstFreeCondIndex, mutexId );
    initQueue( waitingCallerIds + firstFreeCondIndex );
    enqueue( waitingCallerIds + firstFreeCondIndex, callerId );
    condIds[firstFreeCondIndex] = condId;
    firstFreeCondIndex++;
  }

  unlock( callerId, mutexId, 0 );
}

void broadcast( int callerId, int condId )
{
  if ( verbose ) printf( "Broadcasting condition: %d\n", condId );

  for ( int i = 0; i < firstFreeCondIndex; i++ )
  {
    if ( condIds[i] == condId )
    {
      while ( !isEmpty( waitingMutexIds + i ) )
      {
        int newMutexId = top( waitingMutexIds + i );
        pop( waitingMutexIds + i );
        int newCallerId = top( waitingCallerIds + i );
        pop( waitingCallerIds + i );

        if ( verbose ) printf( "Broadcast brings %d back to life with mutex %d\n", newCallerId, newMutexId );

        lock( newCallerId, newMutexId );
      }

      swapWaitingQueues( i, firstFreeCondIndex - 1 );
      i--;
      firstFreeCondIndex--;

      break;
    }
  }

  sendResponse( callerId, CS_ANS_OK );
}

void parseExitSignalBroadcasts( int procId, int relock )
{
  for ( int i = 0; i < firstFreeCondIndex; i++ )
  {
    int mutexToLockOn;

    if ( removeFromTwinQueues( waitingMutexIds + i, waitingMutexIds + i, procId, &mutexToLockOn ) && relock )
    {
      lock( procId, mutexToLockOn );
      break;
    }
  }
}
