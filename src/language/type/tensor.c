#include "Tensor.h"

/* Tensor specializations for int, double and float */

void * Tensor_sum_int(void * lhs, void * rhs) {
    return (void *)(*(int *)lhs + *(int *)rhs);
}

void * Tensor_product_int(void * lhs, void * rhs) {
    return (void *)(*(int *)lhs * *(int *)rhs);
}

void * Tensor_sum_float(void * lhs, void * rhs) {
    return (void *)(*(float *)lhs + *(float *)rhs);
}

void * Tensor_product_float(void * lhs, void * rhs) {
    return (void *)(*(float *)lhs * *(float *)rhs);
}

void * Tensor_sum_double(void * lhs, void * rhs) {
    return (void *)(*(double *)lhs + *(double *)rhs);
}

void * Tensor_product_double(void * lhs, void * rhs) {
    return (void *)(*(double *)lhs * *(double *)rhs);
}

/* Tensor constructor and destructor functions */

struct Tensor * Tensor_construct() {
    struct Tensor * matrix;
    matrix = malloc(sizeof(struct Tensor));
    if(NULL == matrix) return NULL;
    return matrix;
}

struct Tensor * Tensor_initialize(struct Tensor * matrix,
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

struct Tensor * Tensor_create(size_t data_size,
                              size_t scalar_size,
                              size_t columns,
                              size_t rows,
                              TensorOperation scalar_product,
                              TensorOperation scalar_sum)
{
  struct Tensor * matrix;
  matrix = Tensor_construct();
  if(NULL == matrix) return NULL;
  return Tensor_initialize(matrix,
                           data,
                           data_size,
                           scalar_size,
                           columns,
                           rows);
}

struct Tensor * Tensor_new(void * data, size_t size) {
  struct Tensor * list;
  list = Tensor_create(data, size, NULL);
  if(NULL == list) return NULL;
  if(NULL != data) {
    list->data = calloc(1, size);
    list->size = size;
    if(NULL == list->data) {
      Tensor_destroy(list);
      return NULL;
    } else {
      if(NULL != data)
        memcpy(list->data, data, size);
      list->allocated = true;
    }
  }
  return list;
}

bool Tensor_selfAllocated(struct Tensor * list) {
  if(NULL != list)
    return (NULL != list->data && list->allocated == true);
  return false;
}

struct Tensor * Tensor_destroy(struct Tensor * node) {
  if(NULL != node) {
    if(Tensor_selfAllocated(node)) free(node->data);
    node->data = NULL;
    free(node);
    node = NULL;
  }
  return node;
}


// struct DynamicArray * Tensor_new(int value_size, ...) {
//   va_list args;
//   va_start(args, value_size);
//
//   va_end(args);
//   return DynamicArray_new(size*dimensions, value_size);
// }
//
// void * Tensor_at(struct DynamicArray * array, ...) {
//   va_list args;
//   size_t offset = 1;
//   va_start(args, array);
//   for( size_t d=0; d < array->dimensions; d++ )
//     offset *= va_arg(args, size_t) + d;
//   va_end(args);
//   return DynamicArray_at(array, offset);
// }

// int Tensor_set( struct DynamicArray * array, void * value, ... ) {
//   void * data = Tensor_At(array, ...);
//   data = value;
//   return 0;
// }