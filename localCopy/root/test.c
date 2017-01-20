#include <stdio.h>
#include <minix/type.h>
#include <lib.h>
//#include <minix/rs.h>

int main()
{
  endpoint_t e;
  minix_rs_lookup("cs", &e);
  
  message m;
  
  _syscall(e, 0, &m);
  
  printf("WUT?\n");
}
