#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define int_maxValue 0x7FFFFFFF
#define int_minValue 0xFFFFFFFF

typedef struct freeListNode
{
  int start;
  int size;
  struct freeListNode *next;
} freeListNode;

freeListNode *firstFit(freeListNode *freeList, int size)
{
  freeListNode *current = freeList;

  if (current == NULL)
    return NULL;

  while (current != NULL && current->size < size)
    current = current->next;
  return current;
}

freeListNode *bestFit(freeListNode *freeList, int size)
{
  freeListNode *current = freeList;

  if (current == NULL)
    return current;

  freeListNode *bestFit = freeList;
  int minFit = int_maxValue;

  while (current != NULL)
  {
    int fitSpace = current->size - size;
    if (fitSpace < 0 || fitSpace >= minFit)
    {
      current = current->next;
      continue;
    }
    minFit = fitSpace;
    bestFit = current;
    current = current->next;
  }
  return bestFit;
}
freeListNode *worstFit(freeListNode *freeList, int size)
{
  freeListNode *current = freeList;
  if (current == NULL)
    return current;

  freeListNode *worstFit = freeList;
  int maxFit = int_minValue;

  while (current != NULL)
  {
    int fitSpace = current->size - size;
    if (fitSpace < 0 || fitSpace <= maxFit)
    {
      current = current->next;
      continue;
    }
    maxFit = fitSpace;
    worstFit = current;
    current = current->next;
  }
  return worstFit;
}

int reserveMemoryBlock(freeListNode **freeList, int size)
{
  freeListNode *freeMem = firstFit(*freeList, size);
  if (freeMem == NULL)
    return -1; // No memory available
  int start = freeMem->start;
  freeMem->start += size;
  freeMem->size -= size;

  if (freeMem->size > 0)
    return start;
  freeListNode *previous = NULL;
  freeListNode *current = *freeList;
  while (current != freeMem)
  {
    previous = current;
    current = current->next;
  }
  if (previous == NULL)
    *freeList = freeMem->next;
  else
    previous->next = freeMem->next;
  free(freeMem);
  return start;
}

void freeMemoryBlock(freeListNode **freeList, int start, int size)
{
  freeListNode *newNode = (freeListNode *)malloc(sizeof(freeListNode));
  newNode->start = start;
  newNode->size = size;
  newNode->next = NULL;

  if (*freeList == NULL)
  {
    *freeList = newNode;
    return;
  }

  freeListNode *current = *freeList;
  freeListNode *prev = NULL;

  while (current != NULL && current->start < start)
  {
    prev = current;
    current = current->next;
  }

  newNode->next = current;
  if (prev == NULL)
    *freeList = newNode;
  else
    prev->next = newNode;

  // Merge adjacent free blocks
  if (newNode->next != NULL && newNode->start + newNode->size == newNode->next->start)
  {
    newNode->size += newNode->next->size;
    freeListNode *temp = newNode->next;
    newNode->next = newNode->next->next;
    free(temp);
  }

  if (prev != NULL && prev->start + prev->size == newNode->start)
  {
    prev->size += newNode->size;
    prev->next = newNode->next;
    free(newNode);
  }
}