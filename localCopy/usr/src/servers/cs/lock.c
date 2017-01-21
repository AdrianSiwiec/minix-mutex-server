#include <stdio.h>

#include "inc.h"

int lock( int callerId, int mutexId )
{
  if ( verbose ) printf( "Locking mutexId...\n" );
}

int unlock( int callerId, int mutexId )
{
}
