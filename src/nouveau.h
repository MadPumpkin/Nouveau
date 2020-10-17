#ifndef Nouveau_h
#define Nouveau_h

enum Nouveau_RankNames {
  NOUVEAU_SCALAR = 0,
  NOUVEAU_VECTOR = 1,
  NOUVEAU_MATRIX = 2,
  NOUVEAU_TENSOR = 3
};

enum Nouveau_ScalarTypes {
  NOUVEAU_TYPE_STRING,
  NOUVEAU_TYPE_INTEGER,
  NOUVEAU_TYPE_REAL,
  NOUVEAU_TYPE_
};

struct NouveauTensorProcess {
  void * region;
  size_t type_size;
  int rank;

};




#endif
