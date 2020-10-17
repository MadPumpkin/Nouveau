#ifndef Reference_h
#define Reference_h

#include <stdlib.h>
#include <stdbool.h>

/* ReferenceType
**  An enum stored in a Reference to identify it's type:
**    - REFERENCE_UNRETAINED - Stores address, invalid when freed
**    - REFERENCE_WEAK - Will not keep reference alive. Null when reference count is 0
**    - REFERENCE_STRONG - Keeps object alive until reference count is 0 */
enum ReferenceType {
  REFERENCE_UNRETAINED,
  REFERENCE_WEAK,
  REFERENCE_STRONG,
  INVALID_REFERENCE
};

/* ReferenceHandle
** */
struct ReferenceHandle {
  struct Reference * original;
  size_t id;
};

struct ReferenceHandle * ReferenceHandle_create(struct Reference * original, size_t id);

bool ReferenceHandle_destroy(struct ReferenceHandle * handle);

struct Reference * ReferenceHandle_original(struct ReferenceHandle * handle);

size_t ReferenceHandle_id(struct ReferenceHandle * handle);

/* Reference
** */
struct Reference {
  void * source;
  size_t handles;
  size_t handles_created;
  enum ReferenceType type;
  bool marked;
};

/* Reference_initialize
** Sets values in ReferenceData, and sets the ReferenceType */
bool Reference_initialize(void * reference,
                          enum ReferenceType type,
                          void * source,
                          size_t size);

/* Reference_type
** Returns the ReferenceType for the initialized source */
enum ReferenceType Reference_type(void * reference);

/* Reference_mark
** Marks reference when it's ReferenceType threshold is met */
void Reference_mark(struct Reference * reference);

/* Reference_source
** The raw pointer */
void * Reference_source(struct Reference * reference);

/* Reference_getHandle
** Increases the handles count and allocates a ReferenceHandle */
struct ReferenceHandle * Reference_getHandle(struct Reference * reference);

/* Reference_releaseHandle
** Decreases the handles count and deallocates the passed in handle returning false if handle->reference isn't
** pointing at the address of reference */
bool Reference_releaseHandle(struct Reference * reference, struct ReferenceHandle * handle)

#endif
