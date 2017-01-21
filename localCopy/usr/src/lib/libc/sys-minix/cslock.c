#include <lib.h>
#include <stdio.h>

int verbose = 1;

int cs_lock(int mutex_id)
{
  if(verbose) printf("CS LOCK for: %d", mutex_id);
}
