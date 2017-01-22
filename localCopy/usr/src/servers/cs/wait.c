#include <stdio.h>

#include "inc.h"

void wait( int callerId, int mutexId, int condId )
{
  if ( verbose ) printf( "Wait with mutex: %d on cond: %d for caller %d\n", mutexId, condId, callerId );
}

void broadcast( int condId )
{
  if ( verbose ) printf( "Broadcasting condition: %d\n", condId );
}
