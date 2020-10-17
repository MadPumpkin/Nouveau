#ifndef Nouveau_Bitwise_h
#define Nouveau_Bitwise_h

#define BIT_SET(value,bit) ((value) |= (1ULL<<(bit)))
#define BIT_FLIP(value,bit) ((value) ^= (1ULL<<(bit)))
#define BIT_CHECK(value,bit) (!!((value) & (1ULL<<(bit))))
#define BIT_CLEAR(value,bit) ((value) &= ~(1ULL<<(bit)))

#define BIT_MASK_SET(value,mask) ((value) |= (mask))
#define BIT_MASK_FLIP(value,mask) ((value) ^= (mask))
#define BIT_MASK_CHECK(value,mask) ((value) & (mask))
#define BIT_MASK_CLEAR(value,mask) ((value) &= (~(mask)))
#define BIT_MASK_MATCH(value,mask) (((value) & (mask)) == (mask))

#endif
