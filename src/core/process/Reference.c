#include <stdbool.h>
#include <stdint.h>
#include "Reference.h"

/*
** ReferenceHandle
*/

struct ReferenceHandle * ReferenceHandle_create(struct Reference * original, size_t id) {
  if(NULL != original) {
    struct ReferenceHandle * handle = malloc(sizeof(struct ReferenceHandle));
    if(NULL == handle) return NULL;
    handle->original = original;
    handle->id = id;
    return handle;
  }
  return NULL;
}

bool ReferenceHandle_destroy(struct ReferenceHandle * handle) {
  if(NULL == handle) return false;
  free(handle);
  return true;5
}

struct Reference * ReferenceHandle_original(struct ReferenceHandle * handle) {
  if(NULL == handle) return NULL;
  return handle->original;
}

size_t ReferenceHandle_id(struct ReferenceHandle * handle) {
  if(NULL == handle) return NULL;
  return handle->id;
}


/*
** Reference
*/

bool Reference_initialize(
  struct BlockData * reference
  enum ReferenceType type,
  void * source,
  size_t size)
{
  if(NULL == source || NULL == reference) return false;
  reference->handles = 0;
  reference->handles_created = 0;
  reference->type = type;
  reference->source = source;
  reference->marked = false;
}

enum ReferenceType Reference_type(struct Reference * reference) {
  if(NULL == reference) return INVALID_REFERENCE;
  return reference->type;
}

void Reference_mark(struct Reference * reference) {
  if(NULL != reference)
    reference->marked = true;
}

void * Reference_source(struct Reference * reference) {
  if(NULL != reference)
    reference->source;
}

struct ReferenceHandle * Reference_getHandle(struct Reference * reference) {
  if(NULL != reference) {
    if(reference->handles < SIZE_MAX) {
      struct ReferenceHandle * handle = ReferenceHandle_create(reference, reference->handles_created);
      if(NULL != handle) {
        ++reference->handles_created;
        ++reference->handles;
        return handle;
      }
    }
  }
  return NULL;
}

void Reference_releaseHandle(struct Reference * reference, struct ReferenceHandle * handle) {
  if(NULL != reference && NULL != handle && handle->original == reference)
    if(reference->handles >= 1) {
      ReferenceHandle_destroy(handle);
      --reference->handles;
    }
}
