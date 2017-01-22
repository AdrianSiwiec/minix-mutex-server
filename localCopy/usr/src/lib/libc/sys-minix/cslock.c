#include <lib.h>
#include <minix/com.h>
#include <stdio.h>

int verbose = 1;
int isInitialized = 0;
endpoint_t csEndpoint;

int initialize();

int cs_lock( int mutex_id )
{
  if ( verbose ) printf( "CS LOCK for: %d\n", mutex_id );

  if ( !isInitialized ) initialize();

  message m;

  do
  {
    m.m_type = CS_LOCK;
    m.m1_i1 = mutex_id;

    _sendrec( csEndpoint, &m );

  } while ( m.m_type == CS_ANS_AGAIN );

  return 0;
}

int cs_unlock( int mutex_id )
{
  if ( verbose ) printf( "CS UNLOCK for: %d\n", mutex_id );

  if ( !isInitialized )
  {
    errno = EPERM;
    return -1;
  }

  message m;

  do
  {
    m.m_type = CS_UNLOCK;
    m.m1_i1 = mutex_id;

    _sendrec( csEndpoint, &m );
  } while ( m.m_type == CS_ANS_AGAIN );  // probably will never happen

  if ( m.m_type == CS_ANS_OK )
    return 0;
  else
  {
    errno = EPERM;
    return -1;
  }
}

int cs_wait( int cond_var_id, int mutex_id )
{
  if ( verbose ) printf( "CS WAIT for mutex: %d, on condition: %d\n", mutex_id, cond_var_id );

  if ( !isInitialized )
  {
    errno = EINVAL;
    return -1;
  }

  message m;

  do
  {
    m.m_type = CS_WAIT;
    m.m1_i1 = mutex_id;
    m.m1_i2 = cond_var_id;

    _sendrec( csEndpoint, &m );
  } while ( m.m_type == CS_ANS_AGAIN );

  if ( m.m_type == CS_ANS_OK )
    return 0;
  else
  {
    errno = EINVAL;
    return -1;
  }
}

int cs_broadcast( int cond_var_id )
{
  if ( verbose ) printf( "CS BROADCAST for condition: %d\n", cond_var_id );

  if ( !isInitialized ) return 0;

  message m;

  do
  {
    m.m_type = CS_BROADCAST;
    m.m1_i2 = cond_var_id;

    _sendrec( csEndpoint, &m );
  } while ( m.m_type == CS_ANS_AGAIN ); //probably should not happen

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
