#include <lib.h>
#include <minix/type.h>
#include <signal.h>
#include <stdio.h>

#include <unistd.h>

int main()
{
  int s = fork();

  if ( s )
  {
    cs_lock( 1 );

    printf( "Got lock 1 pid=%d\n", getpid() );

    cs_wait( 1, 1 );

    printf( "out of wait\n" );
  }
  else
  {
    sleep( 1 );
    printf( "ppid = %d\n", getppid() );

    int res = kill( getppid(), SIGUSR1 );
    printf( "%d\n", res );
  }
}
