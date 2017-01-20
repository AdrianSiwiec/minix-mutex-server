#include "inc.h"

int identifier = 0x1234;
endpoint_t who_e;
int call_type;
int verbose = 1;

static void sef_cb_signal_handler( int signo );
static int sef_cb_init_fresh(int UNUSED(type), sef_init_info_t *UNUSED(info));

int main( int argc, char *argv[] )
{
  message m;

  env_setargs( argc, argv );

  sef_setcb_init_fresh( sef_cb_init_fresh );
  sef_setcb_init_restart( sef_cb_init_fresh );
  
  sef_setcb_signal_handler(sef_cb_signal_handler);

  sef_startup();

  if ( verbose ) printf( "Hello, world from cs\n" );

  while ( TRUE )
  {
    int r;
    int ipc_number;

    if ( verbose ) printf( "Waiting for message...\n" );

    if ( ( r = sef_receive( ANY, &m ) ) != OK ) printf( "sef_receive failed %d.\n", r );

    who_e = m.m_source;
    call_type = m.m_type;

    if ( verbose ) printf( "CS: get %d from %d\n", call_type, who_e );


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

  /* Checkout if there are still IPC keys. Inform the user in that case. */
  //if ( !is_sem_nil() || !is_shm_nil() ) printf( "IPC: exit with un-clean states.\n" );
}

static int sef_cb_init_fresh( int UNUSED( type ), sef_init_info_t *UNUSED( info ) )
{
  /* Initialize the ipc server. */

  if ( verbose ) printf( "STARTING!\n" );

  return ( OK );
}
