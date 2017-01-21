#include "inc.h"


void initQueueNode( QueueNode *qn, int val )
{
  qn->val = val;
  qn->next = 0;
}


void initQueue( Queue *q )
{
  q->root = q->head = 0;
}

int isEmpty( Queue *q )
{
  return q->root == 0;
}

void enqueue( Queue *q, int a )
{
  if ( isEmpty( q ) )
  {
    q->root = malloc( sizeof( QueueNode ) );
    initQueueNode( q->root, a );
    q->head = q->root;
  }
  else
  {
    q->head->next = malloc( sizeof( QueueNode ) );
    initQueueNode( q->head->next, a );
    q->head = q->head->next;
  }
}

int top( Queue *q )
{
  if ( !isEmpty( q ) )
  {
    return q->root->val;
  }
  else
  {
    if ( verbose ) printf( "ERROR: trying to top from empty queue\n" );

    return 0;  // not -1 to prevent assuming -1 is bad value
  }
}

void pop( Queue *q )
{
  if ( !isEmpty( q ) )
  {
    QueueNode *tmp = q->root;
    q->root = q->root->next;
    free( tmp );
  }
  else
  {
    if ( verbose ) printf( "WARN: trying to pop from empty queue\n" );
  }
}

void printQueue( Queue *q )
{
  printf( "Printing queue: " );
  QueueNode *tmp = q->root;

  while ( tmp != 0 )
  {
    printf( "%d", tmp->val );
    tmp = tmp->next;

    if ( tmp != 0 ) printf( ", " );
  }

  printf( ";\n" );
}
