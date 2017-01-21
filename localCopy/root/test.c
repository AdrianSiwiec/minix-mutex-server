#include <lib.h>
#include <minix/type.h>
#include <stdio.h>

#include <unistd.h>

int main()
{
  endpoint_t e;
  int res = minix_rs_lookup( "cs", &e );

  if ( res == -1 )
  {
    printf( "Cannot lookup cs. Exiting...\n" );
    exit( 1 );
  }

  message m;

  m.m1_i1 = 42;
  m.m_type = 9;
  m.m_source = 8;

  //_syscall(e, 0, &m);
  _sendrec( e, &m );

  printf( "Got: source: %d, type=%d, m1_i1=%d\n", m.m_source, m.m_type, m.m1_i1 );
  
  printf("Trying to cs_lock\n");
  
  cs_lock(1);
  
  printf("Exiting...\n");
}
