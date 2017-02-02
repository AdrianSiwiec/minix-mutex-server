#include "inc.h"

int identifier = 0x1234;
endpoint_t who_e;
int call_type;

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

    if ( ( r = sef_receive( ANY, &m ) ) != OK ) printf( "CS: Unexpected error, sef_receive failed %d.\n", r );

    who_e = m.m_source;
    call_type = m.m_type;

    if ( verbose ) printf( "CS: get %d from %d m1_i1 = %d\n", call_type, who_e, m.m1_i1 );

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

      case CS_SIGNAL_FROM_PM:
        parseNotifyLocks( m.m1_i1 );
        parseExitSignalBroadcasts( m.m1_i1, 1 );
        break;

      case CS_EXIT_FROM_PM:
        parseExitLocks( m.m1_i1 );
        parseExitSignalBroadcasts( m.m1_i1, 0 );
        break;

      case CS_DEBUG_PRINT:
        printLockQueues();
        sendResponse( who_e, CS_ANS_OK );
        break;

      default:
        if ( verbose ) printf( "Ignoring unknown call type: %d\n", call_type );
    }
  }

  /* no way to get here */
  return -1;
}

static void sef_cb_signal_handler( int signo )
{
  if ( verbose ) printf( "Got signal %d\n", signo );

  if ( signo != SIGTERM ) return;

  cleanLocks();
  cleanWaits();
}

static int sef_cb_init_fresh( int UNUSED( type ), sef_init_info_t *UNUSED( info ) )
{
  if ( verbose ) printf( "CS STARTING!\n" );

  initLocks();
  initWaits();

  return ( OK );
}
