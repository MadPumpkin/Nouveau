#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "Region.h"

/*
** RegionBlock
*/

struct RegionBlock * RegionBlock_mergeSortMerge( struct RegionBlock * a, struct RegionBlock * b ) {
  struct RegionBlock * result = NULL;
  if(a == NULL) return b;
  if(b == NULL) return a;
  if(!( a->offset < b->offset )) {
    result = a;
    result->next = RegionBlock_mergeSortMerge(a->next, b);
  } else {
    result = b;
    result->next = RegionBlock_mergeSortMerge(a, b->next);
  }
  return result;
}

void RegionBlock_mergeSortSplit( struct RegionBlock * root, struct RegionBlock ** front, struct RegionBlock ** back)
{
  struct RegionBlock *fast, *slow;
  slow = root;
  fast = root->next;
  while(fast != NULL) {
    fast = fast->next;
    if( fast != NULL) {
      slow = slow->next;
      fast = fast->next;
    }
  }
  *front = root;
  *back = slow->next;
  slow->next = NULL;
}

void RegionBlock_mergeSort(struct RegionBlock ** region_head) {
  struct RegionBlock *a, *b, *head = *region_head;
  if ((head == NULL) || (head->next == NULL)) return;
  RegionBlock_mergeSortSplit(head, &a, &b);
  RegionBlock_mergeSort(&a);
  RegionBlock_mergeSort(&b);
  *region_head = RegionBlock_mergeSortMerge(a, b);
}


/*
** Region
*/

struct Region * Region_create(size_t size) {
  struct Region * region = malloc(sizeof(struct Region));
  if(!region) return NULL;
  region->data = calloc(size, sizeof(RegionByte));
  if(!region->data) return NULL;
  region->size = size;
  region->fragmented = 0;
  region->head = NULL;
  region->tail = NULL;
  return region;
}

struct Region * Region_new(size_t blocks, size_t block_size) {
  return Region_create(blocks * block_size);
}

void Region_destroy(struct Region * region) {
  if(!Region_isEmpty(region)) {
    // Beginning with current_block at head
    struct RegionBlock *next_block, *current_block = region->head;
    do {
      next_block = current_block->next;   // Determine next_block
      free(current_block);                // Destroy current_block
      current_block = next_block;         // Setup for next iteration
    } while(current_block != NULL);
  }
  free(region->data);
  free(region);
}

void Region_sortBlocks(struct Region * region) {
  if(!Region_isEmpty(region))
    RegionBlock_mergeSort(&region->head);
}

void Region_defragment(struct Region * region) {
  if(region->fragmented) {
    Region_sortBlocks(region);
    if(!Region_isEmpty(region)) {
      // Beginning with current_block at head
      struct RegionBlock *next_block, *current_block = region->head;
      size_t block_start_offset, block_end_offset;
      do {
        block_start_offset = current_block->offset;
        block_end_offset = block_start_offset + current_block->size;
        next_block = current_block->next;
        // If data is misaligned
        if( block_end_offset+1 != next_block->offset ) {
          // Move the next block to fill the gap
          next_block->offset = block_end_offset + 1;
          memmove((RegionByte*)region->data + next_block->offset, next_block->data, next_block->size);
        }
        current_block = next_block;
      } while(current_block != NULL);
    }
    region->fragmented = 0;
  }
}

size_t Region_remainingSpace(struct Region * region) {
  size_t remaining_space = region->size;
  if(!Region_isEmpty(region)) {
    // Beginning with current_block at head
    struct RegionBlock *next_block, *current_block = region->head;
    do {
      next_block = current_block->next;   // Determine next_block
      remaining_space = remaining_space - current_block->size;
      current_block = next_block;         // Setup for next iteration
    } while(current_block != NULL);
  }
  return remaining_space;
}

bool Region_isEmpty(struct Region * region) {
  return (NULL == region->head);
}

size_t Region_countBlocks(struct Region * region) {
  size_t count = 0;
  struct RegionBlock * counter = region->head;
  do {
    ++count;
    counter = counter->next;
  } while(counter != NULL);
  return count;
}

struct RegionBlock * Region_allocateBlock(struct Region * region, size_t size, bool defragment) {
  if(size < 1) return NULL;
  if( Region_remainingSpace(region) < size ) return NULL;
  if(defragment) Region_defragment(region);
  // Initialize block
  struct RegionBlock *block = malloc(sizeof(struct RegionBlock));
  size_t offset = 0;
  if(NULL != region->tail)
    offset = region->tail->offset + region->tail->size;
  block->size = size;
  block->offset = offset;
  block->next = NULL;
  block->data = (RegionByte*)region->data + offset;

  // Link block
  if(Region_isEmpty(region)) {
    region->head = block;
  } else {
    region->tail->next = block;
  }
  region->tail = block;
  return block;
}

struct RegionBlock * Region_allocate(struct Region * region, size_t size) {
  return Region_allocateBlock(region, size, true);
}

void Region_releaseBlock(struct Region * region, struct RegionBlock * block) {
  if( block != NULL ) {
    struct RegionBlock * predecessor = region->head;

    // If block is not head
    if(block != region->head) {
      // While block is not next
      while(block != predecessor->next) {
        // Move predecessor
        predecessor = predecessor->next;
      }
      // Close gap
      predecessor->next = block->next;
    } else {
      // If block is head, move head
      region->head = block->next;
    }

    // Determine fragmentation
    if( block != region->tail )
      region->fragmented = 1;

    free(block);
  }
}
