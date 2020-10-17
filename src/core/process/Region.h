#ifndef Region_h
#define Region_h

#include <stdlib.h>
#include <stdbool.h>

/* Region
** The Region structure is designed to be an efficient container for allocation
** and deallocation. A Region is like a set of book-ends on a shelf, with a list
** of books from left to right. Each block of memory is a book, and removing one
** from the shelf leaves a gap. When this type of fragmentation occurs it can be
** defragmented with Region_defragment. If a Region is defragmented by way of
** Region_releaseBlock then the fragmentation will be caught and easily solved.
*/


/* RegionByte
** Subject to change size, and for this reason should be used instead of types
** such as char or unsigned char. */
typedef unsigned char RegionByte;

/* struct RegionBlock
** A linked list structure for holding metadata for Region segments */
struct RegionBlock {
  size_t size;
  size_t offset;
  struct RegionBlock *next;
  void * data;
};

/* RegionBlock_mergeSortMerge
** Used by the RegionBlock_mergeSort algorithm for merging two lists of RegionBlocks */
struct RegionBlock * RegionBlock_mergeSortMerge( struct RegionBlock * a, struct RegionBlock * b );

/* RegionBlock_mergeSortSplit
** Used by the RegionBlock_mergeSort algorithm for splitting a list of RegionBlocks */
void RegionBlock_mergeSortSplit( struct RegionBlock * root, struct RegionBlock ** front, struct RegionBlock ** back);

/* RegionBlock_mergeSort
** A destructive algorithm which reorders a list of RegionBlocks by */
void RegionBlock_mergeSort( struct RegionBlock ** region_head );

/* ********** */

/* struct Region
** A structure for holding a large allocate once memory area
*/
struct Region {
  size_t size;
  struct RegionBlock *head, *tail;
  void * data;
  int fragmented;
};

/* Region_create
** Creates a new memory region, allocating 'size' uninitialized bytes (unsigned chars) */
struct Region * Region_create(size_t size);

/* Region_new
** Calls Region_create with a block_size and block_count instead of bytes */
struct Region * Region_new(size_t blocks, size_t block_size);

/* Region_destroy
** Frees all blocks, then the region itself */
void Region_destroy(struct Region * region);

/* Region_sortBlocks
** Reorders blocks to be ordered by pointed to memory */
void Region_sortBlocks(struct Region * region);

/* Region_defragment
** Removes 'data' fragmentation by moving existing blocks */
void Region_defragment(struct Region * region);

/* Region_remainingSpace
** Calculates the total amount of space remaining, disregarding fragmentation*/
size_t Region_remainingSpace(struct Region * region);

/* Region_isEmpty
** Returns true if *tail points to *head, false otherwise */
bool Region_isEmpty(struct Region * region);

/* Region_countBlocks
** Returns the number of blocks in the list */
size_t Region_countBlocks(struct Region * region);


/*
** RegionBlock
*/


/* Region_allocateBlock
** If 'defragment' = 1 then 'region' will be sorted and defragmented before allocation */
struct RegionBlock * Region_allocateBlock(struct Region * region, size_t size, bool defragment);

/* Region_allocate
** Calls Region_allocateBlock with 'defragment' = true, and returns the result */
struct RegionBlock * Region_allocate(struct Region * region, size_t size);

/* Region_releaseBlock
** Releases a block, removing it from the block space
** A block, released in this way, fragments it's region.
** Will set region.fragmented = 1 */
void Region_releaseBlock(struct Region * region, struct RegionBlock * block);

#endif
