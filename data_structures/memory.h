#ifndef MEMORY_H
#define MEMORY_H

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

freeListNode *firstFit(freeListNode *freeList, int size);
freeListNode *bestFit(freeListNode *freeList, int size);
freeListNode *worstFit(freeListNode *freeList, int size);
void reserveMemoryBlock(freeListNode *freeList, int size);
void freeMemoryBlock(freeListNode *freeList, int start, int size);

#endif