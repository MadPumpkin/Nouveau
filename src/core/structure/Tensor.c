#include "DynamicMatrix.h"

/* DynamicMatrix specializations for int, double and float */

void * DynamicMatrix_sum_int(void * lhs, void * rhs) {
    return (void *)(*(int *)lhs + *(int *)rhs);
}

void * DynamicMatrix_product_int(void * lhs, void * rhs) {
    return (void *)(*(int *)lhs * *(int *)rhs);
}

void * DynamicMatrix_sum_float(void * lhs, void * rhs) {
    return (void *)(*(float *)lhs + *(float *)rhs);
}

void * DynamicMatrix_product_float(void * lhs, void * rhs) {
    return (void *)(*(float *)lhs * *(float *)rhs);
}

void * DynamicMatrix_sum_double(void * lhs, void * rhs) {
    return (void *)(*(double *)lhs + *(double *)rhs);
}

void * DynamicMatrix_product_double(void * lhs, void * rhs) {
    return (void *)(*(double *)lhs * *(double *)rhs);
}

/* DynamicMatrix constructor and destructor functions */

struct DynamicMatrix * DynamicMatrix_construct() {
    struct DynamicMatrix * matrix;
    matrix = malloc(sizeof(struct DynamicMatrix));
    if(NULL == matrix) return NULL;
    return matrix;
}

struct DynamicMatrix * DynamicMatrix_initialize(struct DynamicMatrix * matrix,
                                  void * data,
                                  size_t data_size,
                                  size_t scalar_size,
                                  size_t columns,
                                  size_t rows)
{
    matrix->data = data;
    matrix->data_size = data_size;
    matrix->scalar_size = scalar_size;
    matrix->columns = columns;
    matrix->rows = rows;
    matrix->allocated = false;
    return matrix;
}

struct DynamicMatrix * DynamicMatrix_create(size_t data_size,
                              size_t scalar_size,
                              size_t columns,
                              size_t rows,
                              DynamicMatrixOperation scalar_product,
                              DynamicMatrixOperation scalar_sum)
{
  struct DynamicMatrix * matrix;
  matrix = DynamicMatrix_construct();
  if(NULL == matrix) return NULL;
  return DynamicMatrix_initialize(matrix,
                           data,
                           data_size,
                           scalar_size,
                           columns,
                           rows);
}

struct DynamicMatrix * DynamicMatrix_new(void * data, size_t size) {
  struct DynamicMatrix * list;
  list = DynamicMatrix_create(data, size, NULL);
  if(NULL == list) return NULL;
  if(NULL != data) {
    list->data = calloc(1, size);
    list->size = size;
    if(NULL == list->data) {
      DynamicMatrix_destroy(list);
      return NULL;
    } else {
      if(NULL != data)
        memcpy(list->data, data, size);
      list->allocated = true;
    }
  }
  return list;
}

bool DynamicMatrix_selfAllocated(struct DynamicMatrix * list) {
  if(NULL != list)
    return (NULL != list->data && list->allocated == true);
  return false;
}

struct DynamicMatrix * DynamicMatrix_destroy(struct DynamicMatrix * node) {
  if(NULL != node) {
    if(DynamicMatrix_selfAllocated(node)) free(node->data);
    node->data = NULL;
    free(node);
    node = NULL;
  }
  return node;
}


// struct DynamicArray * DynamicMatrix_new(int value_size, ...) {
//   va_list args;
//   va_start(args, value_size);
//
//   va_end(args);
//   return DynamicArray_new(size*dimensions, value_size);
// }
//
// void * DynamicMatrix_at(struct DynamicArray * array, ...) {
//   va_list args;
//   size_t offset = 1;
//   va_start(args, array);
//   for( size_t d=0; d < array->dimensions; d++ )
//     offset *= va_arg(args, size_t) + d;
//   va_end(args);
//   return DynamicArray_at(array, offset);
// }

// int DynamicMatrix_set( struct DynamicArray * array, void * value, ... ) {
//   void * data = DynamicMatrix_At(array, ...);
//   data = value;
//   return 0;
// }
