#include <lib.h>
#include <minix/type.h>
#include <stdio.h>

#include <unistd.h>

int main()
{
  fork();

  printf( "Trying for lock..\n" );

  cs_lock( 1 );

  printf( "Got lock..\n" );

  sleep( 1 );

  cs_unlock( 1 );

  printf( "Unlocked\n" );
}
