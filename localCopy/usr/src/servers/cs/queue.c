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

void clearQueue( Queue *q )
{
  while ( !isEmpty( q ) )
  {
    pop( q );
  }
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

void removeQueueNode( QueueNode *ptr, QueueNode *father )
{
  father->next = ptr->next;
  free( ptr );
}

int removeFromQueue( Queue *q, int a )
{
  if ( isEmpty( q ) ) return 0;

  if ( a == q->root->val )
  {
    pop( q );
    return 1;
  }
  else
  {
    QueueNode *father = q->root;
    QueueNode *ptr = father->next;

    while ( ptr != 0 )
    {
      if ( ptr->val == a )
      {
        removeQueueNode( ptr, father );
        return 1;
      }

      father = ptr;
      ptr = ptr->next;
    }

    return 0;
  }
}

int removeFromTwinQueues( Queue *q, Queue *twin, int a, int *container )
{
  if ( isEmpty( q ) ) return 0;

  if ( a == q->root->val )
  {
    pop( q );
    *container = top( twin );
    pop( twin );
    return 1;
  }
  else
  {
    QueueNode *father = q->root;
    QueueNode *ptr = father->next;
    QueueNode *twinFather = twin->root;
    QueueNode *twinPtr = twinFather->next;

    while ( ptr != 0 )
    {
      if ( ptr->val == a )
      {
        *container = twinPtr->val;
        removeQueueNode( ptr, father );
        removeQueueNode( twinPtr, twinFather );
        return 1;
      }

      father = ptr;
      ptr = ptr->next;

      twinFather = twinPtr;
      twinPtr = twinPtr->next;
    }

    return 0;
  }
}

void printQueue( Queue *q )
{
  QueueNode *tmp = q->root;

  while ( tmp != 0 )
  {
    printf( "%d", tmp->val );
    tmp = tmp->next;

    if ( tmp != 0 ) printf( ", " );
  }

  printf( ";\n" );
}
