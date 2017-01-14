#include "inc.h"	/* include master header file */
#include <minix/endpoint.h>

/* Allocate space for the global variables. */
static endpoint_t who_e;	/* caller's proc number */
static int callnr;		/* system call number */

/* Declare some local functions. */
static void get_work(message *m_ptr);
static void reply(endpoint_t whom, message *m_ptr);

/* SEF functions and variables. */
static void sef_local_startup(void);

/*===========================================================================*
 *				main                                         *
 *===========================================================================*/
int main(int argc, char **argv)
{
  message m;
  int result;                 

  while (TRUE) {              

      /* Wait for incoming message, sets 'callnr' and 'who'. */
      get_work(&m);

      if (is_notify(callnr)) {
          printf("DS: warning, got illegal notify from: %d\n", m.m_source);
          result = EINVAL;
          goto send_reply;
      }

      switch (callnr) {
      case DS_PUBLISH:
          result = do_publish(&m);
          break;
      case DS_RETRIEVE:
	  result = do_retrieve(&m);
	  break;
      case DS_RETRIEVE_LABEL:
	  result = do_retrieve_label(&m);
	  break;
      case DS_DELETE:
	  result = do_delete(&m);
	  break;
      case DS_SUBSCRIBE:
	  result = do_subscribe(&m);
	  break;
      case DS_CHECK:
	  result = do_check(&m);
	  break;
      case COMMON_GETSYSINFO:
	  result = do_getsysinfo(&m);
	  break;
      default: 
          printf("DS: warning, got illegal request from %d\n", m.m_source);
          result = EINVAL;
      }

send_reply:
      /* Finally send reply message, unless disabled. */
      if (result != EDONTREPLY) {
          m.m_type = result;  		/* build reply message */
	  reply(who_e, &m);		/* send it away */
      }
  }
  return(OK);				/* shouldn't come here */
}

/*===========================================================================*
 *			       sef_local_startup			     *
 *===========================================================================*/
static void sef_local_startup()
{
  /* Register init callbacks. */
  sef_setcb_init_fresh(sef_cb_init_fresh);
  sef_setcb_init_restart(sef_cb_init_fail);

  /* No live update support for now. */

  /* Let SEF perform startup. */
  sef_startup();
}

/*===========================================================================*
 *				get_work                                     *
 *===========================================================================*/
static void get_work(
  message *m_ptr			/* message buffer */
)
{
    int status = sef_receive(ANY, m_ptr);   /* blocks until message arrives */
    if (OK != status)
        panic("failed to receive message!: %d", status);
    who_e = m_ptr->m_source;        /* message arrived! set sender */
    callnr = m_ptr->m_type;       /* set function call number */
}

/*===========================================================================*
 *				reply					     *
 *===========================================================================*/
static void reply(
  endpoint_t who_e,			/* destination */
  message *m_ptr			/* message buffer */
)
{
    int s = send(who_e, m_ptr);    /* send the message */
    if (OK != s)
        printf("DS: unable to send reply to %d: %d\n", who_e, s);
}

