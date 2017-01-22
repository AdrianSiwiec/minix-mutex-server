#include "inc.h"

int identifier = 0x1234;
endpoint_t who_e;
int call_type;
int verbose = 1;

static void sef_cb_signal_handler( int signo );
static int sef_cb_init_fresh( int UNUSED( type ), sef_init_info_t *UNUSED( info ) );

int main( int argc, char *argv[] )
{
  env_setargs( argc, argv );
  sef_setcb_init_fresh( sef_cb_init_fresh );
  sef_setcb_init_restart( sef_cb_init_fresh );
  sef_setcb_signal_handler( sef_cb_signal_handler );
  sef_startup();

  message m;

  if ( verbose ) printf( "Hello, world from cs\n" );

  while ( TRUE )
  {
    int r;

    if ( verbose ) printf( "\nWaiting for message...\n" );

    if ( ( r = sef_receive( ANY, &m ) ) != OK ) printf( "sef_receive failed %d.\n", r );

    who_e = m.m_source;
    call_type = m.m_type;

    // if ( verbose ) printf( "CS: get %d from %d m1_i1 = %d\n", call_type, who_e, m.m1_i1 );

    switch ( call_type )
    {
      case CS_LOCK:
        lock( who_e, m.m1_i1 );
        break;

      case CS_UNLOCK:
        unlock( who_e, m.m1_i1, 1 );
        break;

      case CS_WAIT:
        wait( who_e, m.m1_i1, m.m1_i2 );
        break;

      case CS_BROADCAST:
        broadcast( who_e, m.m1_i2 );
        break;

      default:
        if ( verbose ) printf( "Ignoring unknown call type\n" );
    }

    /* for reference
    m.m_type = 99;
    m.m1_i1 = 101;
    m.m_source = 102;

    if ( ( r = sendnb( who_e, &m ) ) != OK )
    {
      printf( "Couldn't respond!" );
    }

    if ( verbose ) printf( "Responded succesfully\n" );
    */
    //        if ( ( r = sendnb( who_e, &m ) ) != OK ) printf( "IPC send error %d.\n", r );
  }

  /* no way to get here */
  return -1;
}

static void sef_cb_signal_handler( int signo )
{
  if ( verbose ) printf( "Got signal %d\n", signo );

  /* Only check for termination signal, ignore anything else. */
  if ( signo != SIGTERM ) return;

  cleanLocks();
  cleanWaits();

  /* Checkout if there are still IPC keys. Inform the user in that case. */
  // if ( !is_sem_nil() || !is_shm_nil() ) printf( "IPC: exit with un-clean states.\n" );
}

static int sef_cb_init_fresh( int UNUSED( type ), sef_init_info_t *UNUSED( info ) )
{
  /* Initialize the ipc server. */

  if ( verbose ) printf( "STARTING!\n" );

  initLocks();
  initWaits();

  return ( OK );
}
