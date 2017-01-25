#include <lib.h>
#include <minix/type.h>
#include <stdio.h>

#include <unistd.h>

int main()
{
  // int s = fork();

  printf( "Trying for lock.. my pid = %d\n", getpid() );

  cs_lock( 1 );

  printf( "Got lock..\n" );
  
  cs_wait( 1, 1 );

  printf( "Exiting, my pid=%d\n", getpid() );
}
