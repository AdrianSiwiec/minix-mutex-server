#include "inc.h"
#include <cv.h>

int do_store(message *m_in, message *m_out);
static void sef_local_startup();


int main(int argc, char *argv[])
{
  message m, m_out;
  

	/* SEF local startup. */
	env_setargs(argc, argv);
	sef_local_startup();

	while (TRUE) {
		int r;

		if ((r = sef_receive(ANY, &m)) != OK)
			printf("sef_receive failed %d.\n", r);
		int who_e = m.m_source;

    //int result = do_store(&m, &m_out);
    int result = 0;
    m_out.m1_i1=9;

    if( result == -1 )
    {
      m_out.m_type = -1;
    }
    else
    {
      m_out.m_type = 0;
    }

		if ((r = sendnb(who_e, &m_out)) != OK)
    {
      printf("Send error\n");
	    //return -1;
    }


  } 

  return 0;
}

int do_store(message *m_in, message *m_out)
{
  static int val;
  static int isSet = 0;
  if(m_in->m_type == STR_MSG ) //store
  {
    if( isSet )
      return -1;
    else
    {
      isSet = 1;
      val = m_in->m1_i1;
      m_out->m1_i1 = 42;
      return 0;
    }
  }
  else if(m_in->m_type == GET_MSG ) //receive
  {
    if ( !isSet )
      return -1;
    else
    {
      isSet = 0;
      m_out->m1_i1 = val;
      return 0;
    }
  }
  else
  {
    //unexpected;
    return -1;
  }
}	

/*===========================================================================*
 *			       sef_local_startup			     *
 *===========================================================================*/
static void sef_local_startup()
{
  /* Let SEF perform startup. */
  sef_startup();
}

/*===========================================================================*
 *		            sef_cb_init_fresh                                *
 *===========================================================================*/
