#include <lib.h>
#include <minix/com.h>
#include <stdio.h>

int verbose = 1;
int isInitialized = 0;
endpoint_t csEndpoint;

int initialize();

int cs_lock( int mutex_id )
{
  if ( !isInitialized ) initialize();

  if ( verbose ) printf( "CS LOCK for: %d\n", mutex_id );

  message m;

  do
  {
    m.m_type = CS_LOCK;
    m.m1_i1 = mutex_id;

    _sendrec( csEndpoint, &m );
    
  } while ( m.m_type != CS_ANS_OK );

  return 0;
}

int initialize()
{
  if ( verbose ) printf( "Initializing cs_lock...\n" );

  int res = minix_rs_lookup( "cs", &csEndpoint );

  if ( res == -1 )
  {
    printf( "Cannot lookup cs. Try 'service up /usr/sbin/cs'\n" );
  }

  isInitialized = 1;
}
