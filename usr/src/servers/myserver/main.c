#include <minix/endpoint.h>
#include "inc.h" /* include master header file */

/* Allocate space for the global variables. */
static endpoint_t who_e; /* caller's proc number */
static int callnr;       /* system call number */

/* Declare some local functions. */
static void get_work( message *m_ptr );
static void reply( endpoint_t whom, message *m_ptr );

/*===========================================================================*
 *				main                                         *
 *===========================================================================*/
int main( int argc, char **argv )
{
  message m;
  int result;

  while ( TRUE )
  {
    /* Wait for incoming message, sets 'callnr' and 'who'. */
    get_work( &m );

    switch ( callnr )
    {
      default:
        printf( "DS: warning, got illegal request from %d\n", m.m_source );
        result = EINVAL;
    }


    /* Finally send reply message, unless disabled. */
    if ( result != EDONTREPLY )
    {
      m.m_type = result;  /* build reply message */
      reply( who_e, &m ); /* send it away */
    }
  }

  return ( OK ); /* shouldn't come here */
}

/*===========================================================================*
 *				get_work                                     *
 *===========================================================================*/
static void get_work( message *m_ptr /* message buffer */
                      )
{
  int status;
  sef_receive_status( ANY, m_ptr, &status ); /* blocks until message arrives */

  if ( OK != status ) panic( "failed to receive message!: %d", status );

  who_e = m_ptr->m_source; /* message arrived! set sender */
  callnr = m_ptr->m_type;  /* set function call number */
}

/*===========================================================================*
 *				reply					     *
 *===========================================================================*/
static void reply( endpoint_t who_e, /* destination */
                   message *m_ptr    /* message buffer */
                   )
{
  int s = send( who_e, m_ptr ); /* send the message */

  if ( OK != s ) printf( "DS: unable to send reply to %d: %d\n", who_e, s );
}
