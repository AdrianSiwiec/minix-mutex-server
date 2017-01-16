#include "kernel/system.h"
#include <signal.h>

int do_stop(struct proc * caller, message * m_ptr)
{
  return(OK);

  /*
  if (!isokendpt(proc_nr_e, &proc_nr)) return(EINVAL);
  if (sig_nr >= _NSIG) return(EINVAL);
  if (iskerneln(proc_nr)) return(EPERM);

   //Set pending signal to be processed by the signal manager. 
  cause_sig(proc_nr, sig_nr);

  return(OK);
  */
}

