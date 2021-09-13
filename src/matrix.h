#include <stdio.h>
#include <stdlib.h>

typedef struct Matrix
{
    double **m;
    int r, c;
} Matrix;

typedef enum Status
{
    MATRIX_NOT_INVERTIBLE = -33,
    INCOMPATIBLE_DIMENSIONS = -17,
    MISMATCHED_DIMENSIONS = -9,
    INVALID_DIMENSIONS = -5,
    ALREADY_ALLOCATED = -3,
    ALLOCATION_FAIL = -2,
    NULL_ARGUMENT = -1,
    SUCCESS = 0,
    ALREADY_FREED = 1,
} Status;

int is_allocated(Matrix *a);
Status mx_acreate(Matrix *res, int r, int c);
Status mx_acpy(Matrix *res, double a[3][3], int r, int c);
Status mx_add(Matrix *res, Matrix *a, Matrix *b);
Status mx_adestroy(Matrix *res);
Status mx_adj(Matrix *res, Matrix *a);
Status mx_cpy(Matrix *dest, Matrix *src);
Status mx_create(Matrix **res, int r, int c);
Status mx_destroy(Matrix **res);
Status mx_det(double *res, Matrix *a);
Status _mx_det(double *res, Matrix *a);
Status mx_inv(Matrix *res, Matrix *a);
Status mx_minor(Matrix *res, Matrix *a, int r, int c);
Status mx_mul(Matrix *res, Matrix *a, Matrix *b);
Status mx_pow(Matrix *res, Matrix *a, int pow);
Status mx_print(Matrix *a);
Status mx_resize(Matrix *res, int r, int c);
Status mx_smul(Matrix *res, Matrix *a, double s);
Status mx_sub(Matrix *res, Matrix *a, Matrix *b);
Status mx_trans(Matrix *res, Matrix *a);
int neg1_pow(int p);
const char *status_string(Status s);
