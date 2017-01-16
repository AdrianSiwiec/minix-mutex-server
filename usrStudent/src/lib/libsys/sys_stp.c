#include "syslib.h"

int sys_stp(signr)
int signr;			/* signal number: 1 - 16 */
{
  message m;

  m.SIG_NUMBER = signr;
  return(_kernel_call(SYS_STP, &m));
}

