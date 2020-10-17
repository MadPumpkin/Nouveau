#ifndef Tensor_h
#define Tensor_h

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <tgmath.h>
#include <assert.h>

/* Tensor
** Despite this Matrix being a generic structure, it has 3 specializations which each skip 2
** function pointers within the structure from being called, and thus needing to be defined.
** When the Tensor_newInt, Tensor_newFloat, or Tensor_newDouble */

// Requires C11 for specializations
#ifndef Tensor_IGNORE_C11
    #if __STDC__ != 1
    _Static_assert(0, "(needs C11) Compiler must support _Generic for 'struct Tensor' type to work with generic calls. To ignore this define Tensor_IGNORE_C11";
    #endif
#endif

#define MATRIX_SPECIALIZED_TYPE(TYPE,OPERATION)  _Generic((TYPE), \
                                        int: Tensor_ ## OPERATION ## _int, \
                                        float: Tensor_ ## OPERATION ## _float, \
                                        double: Tensor_ ## OPERATION ## _double, \
                                        struct Tensor: Tensor_ ## OPERATION ## Tensor, \
                                        default: Tensor_ ## OPERATION ## Scalar)

#define MATRIX_SPECIALIZED_TYPE_ENUM(T)  _Generic((T), \
                                        int: 1, \
                                        float: 2, \
                                        double: 3, \
                                        default: 0)

#define Tensor_sum(lhs,rhs) (MATRIX_SPECIALIZED_TYPE(lhs,sum)(lhs, rhs))
#define Tensor_product(lhs,rhs) (MATRIX_SPECIALIZED_TYPE(lhs,product)(lhs, rhs))

/* Tensor_SpecializedType
** must remain in sync with MATRIX_SPECIALIZED_TYPE_ENUM for specializations
** to work */
enum Tensor_SpecializedType {
    NONE = 0,
    INT = 1,
    FLOAT = 2,
    DOUBLE = 3,
};


/* TensorOperation
** This is the type of the callbacks a matrix must implement to define
** the specialized scalar operations for the generic matrix */
typedef (void *) (TensorOperation)(void *, void *);

/* Tensor - Column major, column vector natural
**  - data: if owned by the matrix by creating the matrix with Tensor_new,
        then 'allocated' will be 'true', otherwise allocated will be 'false'
    - data_size: the size of 'data', or in Tensor_new, the size to allocate
    - columns, rows: used by Tensor_initialize and Tensor_new to allocate 'data'
    - allocated: 'true' if Tensor_new was used, 'false' otherwise
    Notes:
        - Transpose of a jagged array must be sparse
        - If a matrix is both jagged and sparse, in general
*/
struct Tensor {
    void * region;
    size_t region_size;
    size_t scalar_size;
    size_t columns;
    size_t rows;
    bool allocated;
    enum Tensor_SpecializedType specialized_type;
    TensorOperation product;
    TensorOperation sum;
};

/* TensorOperation specializations
** specializations are provided for:
**  - float, double, int
*/
void * Tensor_sum_int(void * lhs, void * rhs);
void * Tensor_product_int(void * lhs, void * rhs);

void * Tensor_sum_float(void * lhs, void * rhs);
void * Tensor_product_float(void * lhs, void * rhs);

void * Tensor_sum_double(void * lhs, void * rhs);
void * Tensor_product_double(void * lhs, void * rhs);

/* Tensor constructor and destructor methods */
struct Tensor * Tensor_construct();

struct Tensor * Tensor_initialize(struct Tensor * matrix,
                                  void * data,
                                  size_t data_size,
                                  size_t scalar_size,
                                  size_t columns,
                                  size_t rows,
                                  TensorOperation scalar_product,
                                  TensorOperation scalar_sum);

struct Tensor * Tensor_create(size_t data_size,
                              size_t scalar_size,
                              size_t columns,
                              size_t rows,
                              TensorOperation scalar_product,
                              TensorOperation scalar_sum);

struct Tensor * Tensor_new(size_t scalar_size,
                           size_t columns,
                           size_t rows,
                           TensorOperation scalar_product,
                           TensorOperation scalar_sum);

struct Tensor * Tensor_new_int(size_t columns, size_t rows);

struct Tensor * Tensor_new_float(size_t columns, size_t rows);

struct Tensor * Tensor_new_double(size_t columns, size_t rows);

struct Tensor * Tensor_selfAllocated(struct Tensor * matrix);

struct Tensor * Tensor_destroy(struct Tensor * matrix);

/* Basic matrix operations */

void Tensor_setElements(struct Tensor * matrix, ...);

struct Tensor * Tensor_fromRowVectors(size_t row_count, ...);

struct Tensor * Tensor_fromColumnVectors(size_t column_count, ...);

struct Tensor * Tensor_vectorFromRow(struct Tensor * matrix,
                                     size_t row_number);

struct Tensor * Tensor_vectorFromColumn(struct Tensor * matrix,
                                        size_t column_number);

bool Tensor_isSquare(struct Tensor * matrix);

bool Tensor_isVector(struct Tensor * matrix);

bool Tensor_isRowEchelon(struct Tensor * matrix);

bool Tensor_isColumnEchelon(struct Tensor * matrix); // if M^T is row echelon then M is col echelon

/* Tensor_isCompatible
** whether the two matrices are compatible for multiplication. They are if the
** multiplier matrix's columns equal the multiplicand matrix's rows */
bool Tensor_isCompatible(struct Tensor * matrix,
                         struct Tensor * multiplicand);

bool Tensor_isPivot(struct Tensor * matrix,
                   size_t row,
                   size_t column);

int Tensor_rank(struct Tensor * matrix);


void * Tensor_get(struct Tensor * matrix,
                  size_t row,
                  size_t column);

void * Tensor_set(struct Tensor * matrix,
                  size_t row,
                  size_t column,
                  void * scalar);


struct Tensor * Tensor_getRange(struct Tensor * matrix,
                                size_t row_begin,
                                size_t column_begin,
                                size_t row_end,
                                size_t column_end);

struct Tensor * Tensor_setRange(struct Tensor * matrix,
                                struct Tensor * range,
                                size_t row,
                                size_t column);

struct Tensor * Tensor_swapRow(struct Tensor * matrix,
                               size_t first_row,
                               size_t second_row);

struct Tensor * Tensor_swapColumn(struct Tensor * matrix,
                                  size_t first_row,
                                  size_t second_row);

struct Tensor * Tensor_submatrix(struct Tensor * matrix,
                                 size_t row_begin,
                                 size_t column_begin,
                                 size_t row_end,
                                 size_t column_end);

struct Tensor * Tensor_productTensor(struct Tensor * matrix,
                               struct Tensor * multiplicand);

void * Tensor_productScalar(struct Tensor * matrix,
                            void * multiplicand);

struct Tensor * Tensor_sumTensor(struct Tensor * matrix,
                                 struct Tensor * summand);

struct Tensor * Tensor_sumScalar(struct Tensor * matrix,
                                 void * summand);

struct Tensor * Tensor_productKronecker(struct Tensor * matrix,
                                        struct matrix * multiplicand);

// A (+) B = A (x) Im + In (x) B
//      where (x) is the Kronecker product
struct Tensor * Tensor_sumKronecker(struct Tensor * matrix,
                                        struct matrix * multiplicand);

/* Other useful vector and matrix functions
*/

// Only exists for squares
void * Tensor_trace(struct Tensor * matrix);

// Only exists for squares
void * Tensor_determinant(struct Tensor * matrix);

// Only exists for squares
void * Tensor_minor(struct Tensor * matrix,
                    size_t row,
                    size_t column);

// Only exists for squares
struct Tensor * Tensor_inverse(struct Tensor * matrix);

struct Tensor * Tensor_guassianElimination(struct Tensor * matrix);

struct Tensor * Tensor_transpose(struct Tensor * matrix);

struct Tensor * Tensor_normalize(struct Tensor * matrix);

struct Tensor * Tensor_dot3(struct Tensor * matrix,
                              struct Tensor * miltiplicand);

struct Tensor * Tensor_cross3(struct Tensor * matrix,
                              struct Tensor * miltiplicand);

#endif