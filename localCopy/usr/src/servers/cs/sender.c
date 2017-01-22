#include <stdio.h>

#include "inc.h"

void sendResponse( int procNr, int response )
{
  message m;
  m.m_type = response;

  sendnb( procNr, &m );
}
