#include <lib.h>
#include <minix/type.h>
#include <stdio.h>

#include <unistd.h>

int main()
{
  pause();
  printf("Unpaused\n");
  /*
  int s = fork();

  if ( s )
  {
    int s2 = fork();
    printf( "Trying for lock..\n" );

    cs_lock( s2 );

    printf( "Got lock..\n" );
    printf( "Waiting for 8\n" );

    int res = cs_wait( 8, s2 );

    printf( "Got back from wait with res = %d\n", res );

    cs_unlock( s2 );

    printf( "Unlocked\n" );
  }
  else
  {
    sleep( 2 );
    printf( "Broadcasting 8\n" );
    cs_broadcast( 8 );
    printf( "Came back from broadcast\n" );
  }
  */
}
