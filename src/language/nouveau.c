#include <stdio.h>
#include <stddef.h>
#include <threads.h>

#include "nouveau.h"

#define BIT_SET(value,bit) ((value) |= (1ULL<<(bit)))
#define BIT_FLIP(value,bit) ((value) ^= (1ULL<<(bit)))
#define BIT_CHECK(value,bit) (!!((value) & (1ULL<<(bit))))
#define BIT_CLEAR(value,bit) ((value) &= ~(1ULL<<(bit)))

#define BIT_MASK_SET(value,mask) ((value) |= (mask))
#define BIT_MASK_FLIP(value,mask) ((value) ^= (mask))
#define BIT_MASK_CHECK(value,mask) ((value) & (mask))
#define BIT_MASK_CLEAR(value,mask) ((value) &= (~(mask)))
#define BIT_MASK_MATCH(value,mask) (((value) & (mask)) == (mask))

/* Channel */
struct Nouveau_Channel * nou_make_channel(
  struct Nouveau_Primitive * input,
  struct Nouveau_Primitive * output
  Nouveau_Transformation transform)
{
  struct Nouveau_Channel * channel = malloc(sizeof (struct Nouveau_Channel));
  if(NULL==channel) return NULL;
  
}


void nou_newInstance(int * argc, char ** argv);
void nou_destroyInstance(struct Nouveau_Engine * nou);

void nou_load(struct Nouveau_Engine * nou);
void nou_execute(struct Nouveau_Engine * nou);

void nou_nil(struct Nouveau_Engine * nou);
void nou_output(struct Nouveau_Engine * nou);
void nou_input(struct Nouveau_Engine * nou);
void nou_parallel(struct Nouveau_Engine * nou);
void nou_restrict(struct Nouveau_Engine * nou); // create channel in process->symbols, increase symbol_count
void nou_replicate(struct Nouveau_Engine * nou);

void nou_read(struct NouveauEngine * nou);
void nou_eval()
void nou_print(struct NouveauEngine * nou);

void nou_eval() {

  for (;;) {
    switch (instruction = (*iptr++)) {  
      case NOU_NIL:
        printf("nil\n");
        break;
      case NOU_OUTPUT:
        printf("output\n");
        break;
      case NOU_INPUT:
        printf("input\n");
        break;
      case NOU_PARALLEL:
        printf("parallel\n");
        break;
      case NOU_RESTRICT:

        break;
      case NOU_REPLICATE:
        printf("replicate\n");
        break;
      default:
      break;
      }
  }
}

void nou_repl(struct Nouveau_Process * nou) {
  do {

  } while ()
}