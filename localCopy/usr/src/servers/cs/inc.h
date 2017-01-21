#define _POSIX_SOURCE 1 /* tell headers to include POSIX stuff */
#define _MINIX 1        /* tell headers to include MINIX stuff */
#define _SYSTEM 1       /* get OK and negative error codes */

#include <minix/callnr.h>
#include <minix/com.h>
#include <minix/config.h>
#include <minix/const.h>
#include <minix/endpoint.h>
#include <minix/ipc.h>
#include <minix/syslib.h>
#include <minix/sysutil.h>
#include <minix/type.h>

#include <machine/vm.h>
#include <machine/vmparam.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/vm.h>

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

EXTERN int identifier;
EXTERN endpoint_t who_e;
EXTERN int call_type;
EXTERN int verbose;

typedef struct
{
  int *root, *head;
} Queue;

void initQueue( Queue *q );
