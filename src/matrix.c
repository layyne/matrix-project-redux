#include "matrix.h"

int is_allocated(Matrix *a) { return a && a->m; }

Status mx_acreate(Matrix *res, int r, int c)
{
    int i, j;

    if (!res)
        return NULL_ARGUMENT;

    if (res->m)
        return ALREADY_ALLOCATED;

    if (r < 1 || c < 1)
        return INVALID_DIMENSIONS;

    if (!(res->m = calloc(r, sizeof(double *))))
        return ALLOCATION_FAIL;

    for (i = 0; i < r; i++)
    {
        if (!(res->m[i] = calloc(c, sizeof(double))))
        {
            for (j = 0; j < i; j++)
                free(res->m[j]);
            free(res->m);
            return ALLOCATION_FAIL;
        }
    }

    return SUCCESS;
}

Status mx_acpy(Matrix *res, double a[3][3], int r, int c)
{
    int i, j;
    Status status;

    if (!res || !a)
        return NULL_ARGUMENT;

    if ((status = mx_resize(res, r, c)) < SUCCESS)
        return status;

    for (i = 0; i < r; i++)
        for (j = 0; j < c; j++)
            res->m[i][j] = a[i][j];

    return SUCCESS;
}

Status mx_add(Matrix *res, Matrix *a, Matrix *b)
{
    int i, j;
    Status status;

    if (!a || !b)
        return NULL_ARGUMENT;

    if ((a->r != b->r) || (a->c != b->c))
        return MISMATCHED_DIMENSIONS;

    if ((status = mx_resize(res, a->r, a->c)) < SUCCESS)
        return status;

    for (i = 0; i < a->r; i++)
        for (j = 0; j < a->c; j++)
            res->m[i][j] = a->m[i][j] + b->m[i][j];

    return SUCCESS;
}

Status mx_adestroy(Matrix *res)
{
    int i;

    if (!res || !res->m)
        return ALREADY_FREED;

    for (i = 0; i < res->r; i++)
        if (res->m[i])
            free(res->m[i]);

    free(res->m);
    res->m = NULL;

    return SUCCESS;
}

Status mx_adj(Matrix *res, Matrix *a)
{
    int i, j;
    double d;
    Matrix *minor;
    Status status;

    if (!res || !is_allocated(a))
        return NULL_ARGUMENT;

    if (a->r != a->c)
        return INVALID_DIMENSIONS;

    if ((status = mx_resize(res, a->r, a->c)) < SUCCESS)
        return status;

    minor = NULL;

    if ((status = mx_create(&minor, a->r, a->c)) < SUCCESS)
        return status;

    for (i = 0; i < a->r; i++)
        for (j = 0; j < a->c; j++)
        {
            if ((status = mx_minor(minor, a, j, i)) < SUCCESS)
                return status;
            if ((status = mx_det(&d, minor)) < SUCCESS)
                return status;

            res->m[i][j] = neg1_pow(i + j) * d;
        }

    mx_destroy(&minor);
    return SUCCESS;
}

Status mx_cpy(Matrix *dest, Matrix *src)
{
    int i, j;
    Status status;

    if (!dest || !is_allocated(src))
        return NULL_ARGUMENT;

    if ((status = mx_resize(dest, src->r, src->c)) < SUCCESS)
        return status;

    for (i = 0; i < src->r; i++)
        for (j = 0; j < src->c; j++)
            dest->m[i][j] = src->m[i][j];

    return SUCCESS;
}

Status mx_create(Matrix **res, int r, int c)
{
    int i, j;
    Status status;

    if (*res)
        return ALREADY_ALLOCATED;

    if (r < 1 || c < 1)
        return INVALID_DIMENSIONS;

    if (!(*res = calloc(1, sizeof(Matrix))))
        return ALLOCATION_FAIL;

    if ((status = mx_acreate(*res, r, c)) < SUCCESS)
        return status;

    (*res)->r = r;
    (*res)->c = c;

    return SUCCESS;
}

Status mx_destroy(Matrix **res)
{
    int i;

    if (!*res)
        return ALREADY_FREED;

    mx_adestroy(*res);
    free(*res);
    *res = NULL;

    return SUCCESS;
}

Status mx_det(double *res, Matrix *a)
{
    if (!res || !is_allocated(a))
        return NULL_ARGUMENT;

    if (a->r != a->c)
        return INVALID_DIMENSIONS;

    return _mx_det(res, a);
}

Status _mx_det(double *res, Matrix *a)
{
    int i;
    double sum, cur;
    Matrix *minor;
    Status status;

    if (a->r == 2)
    {
        *res = a->m[0][0] * a->m[1][1] - a->m[0][1] * a->m[1][0];
        return SUCCESS;
    }

    minor = NULL;

    if ((status = mx_create(&minor, a->r - 1, a->c - 1)) < SUCCESS)
        return status;

    for (sum = i = 0; i < a->r; i++)
    {
        if ((status = mx_minor(minor, a, 0, i)) < SUCCESS)
            return status;

        if ((status = _mx_det(&cur, minor)) < SUCCESS)
            return status;

        sum += neg1_pow(i) * a->m[0][i] * cur;
    }

    mx_destroy(&minor);

    *res = sum;
    return SUCCESS;
}

Status mx_inv(Matrix *res, Matrix *a)
{
    int i, j;
    double d;
    Status status;

    if (!res || !is_allocated(a))
        return NULL_ARGUMENT;

    if (a->r != a->c)
        return INVALID_DIMENSIONS;

    if ((status = mx_det(&d, a)) < SUCCESS)
        return status;

    if (d == 0)
        return MATRIX_NOT_INVERTIBLE;

    if ((status = mx_resize(res, a->r, a->c)) < SUCCESS)
        return status;

    if ((status = mx_adj(res, a)) < SUCCESS)
        return status;

    for (i = 0; i < a->r; i++)
        for (j = 0; j < a->c; j++)
            res->m[i][j] /= d;

    return SUCCESS;
}

Status mx_minor(Matrix *res, Matrix *a, int r, int c)
{
    int i, j;
    Status status;

    if (!res || !is_allocated(a))
        return NULL_ARGUMENT;

    if ((status = mx_resize(res, a->r - 1, a->c - 1)) < SUCCESS)
        return status;

    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
            res->m[i][j] = a->m[i][j];
        for (j = c + 1; j < a->c; j++)
            res->m[i][j - 1] = a->m[i][j];
    }

    for (i = r + 1; i < a->r; i++)
    {
        for (j = 0; j < c; j++)
            res->m[i - 1][j] = a->m[i][j];
        for (j = c + 1; j < a->c; j++)
            res->m[i - 1][j - 1] = a->m[i][j];
    }

    return SUCCESS;
}

Status mx_sub(Matrix *res, Matrix *a, Matrix *b)
{
    int i, j;
    Status status;

    if (!a || !b)
        return NULL_ARGUMENT;

    if ((a->r != b->r) || (a->c != b->c))
        return MISMATCHED_DIMENSIONS;

    if ((status = mx_resize(res, a->r, a->c)) < SUCCESS)
        return status;

    for (i = 0; i < a->r; i++)
        for (j = 0; j < a->c; j++)
            res->m[i][j] = a->m[i][j] - b->m[i][j];

    return SUCCESS;
}

Status mx_mul(Matrix *res, Matrix *a, Matrix *b)
{
    int i, j, k;
    double cell;
    Status status;

    if (!res || !is_allocated(a) || !is_allocated(b))
        return NULL_ARGUMENT;

    if (a->c != b->r)
        return INCOMPATIBLE_DIMENSIONS;

    if ((status = mx_resize(res, a->r, b->c)) < SUCCESS)
        return status;

    for (i = 0; i < a->r; i++)
        for (j = 0; j < b->c; j++)
        {
            for (cell = k = 0; k < a->c; k++)
                cell += a->m[i][k] * b->m[k][j];
            res->m[i][j] = cell;
        }

    return SUCCESS;
}

Status mx_pow(Matrix *res, Matrix *a, int pow)
{
    int i;
    Matrix *tmp;
    Status status;

    if (!res || !is_allocated(a))
        return NULL_ARGUMENT;

    if (a->r != a->c)
        return INVALID_DIMENSIONS;

    if (pow == 1)
        return mx_cpy(res, a);

    tmp = NULL;

    if ((status = mx_create(&tmp, a->r, a->c)) < SUCCESS)
        return status;

    if ((status = mx_cpy(tmp, a)) < SUCCESS)
        return status;

    for (i = 1; i < pow; i++)
    {
        if ((status = mx_mul(res, a, tmp)) < SUCCESS)
            return status;
        if ((status = mx_cpy(tmp, res)) < SUCCESS)
            return status;
    }

    mx_destroy(&tmp);
    return SUCCESS;
}

Status mx_print(Matrix *a)
{
    int i, j;

    if (!is_allocated(a))
        return NULL_ARGUMENT;

    for (i = 0; i < a->r; i++)
    {
        printf("%.4f", a->m[i][0]);
        for (j = 1; j < a->c; j++)
            printf(" %.4f", a->m[i][j]);
        printf("\n");
    }

    return SUCCESS;
}

Status mx_resize(Matrix *res, int r, int c)
{
    Status status;

    if (!res)
        return NULL_ARGUMENT;

    if (r < 1 || c < 1)
        return INVALID_DIMENSIONS;

    if (!res->m)
    {
        if ((status = mx_acreate(res, r, c)) < SUCCESS)
            return status;
        res->r = r;
        res->c = c;
    }
    else if (res->r != r || res->c != c)
    {
        mx_adestroy(res);
        if ((status = mx_acreate(res, r, c)) < SUCCESS)
            return status;
        res->r = r;
        res->c = c;
    }

    return SUCCESS;
}

Status mx_smul(Matrix *res, Matrix *a, double s)
{
    int i, j;
    Status status;

    if (!res || !is_allocated(a))
        return NULL_ARGUMENT;

    if ((status = mx_resize(res, a->r, a->c)) < SUCCESS)
        return status;

    for (i = 0; i < a->r; i++)
        for (j = 0; j < a->c; j++)
            res->m[i][j] = a->m[i][j] * s;

    return SUCCESS;
}

Status mx_trans(Matrix *res, Matrix *a)
{
    int i, j;
    Status status;

    if (!res || !is_allocated(a))
        return NULL_ARGUMENT;

    if ((status = mx_resize(res, a->r, a->c)) < SUCCESS)
        return status;

    for (i = 0; i < a->r; i++)
        for (j = 0; j < a->c; j++)
            res->m[i][j] = a->m[j][i];

    return SUCCESS;
}

int neg1_pow(int p) { return (p % 2) ? -1 : 1; }

const char *status_string(Status s)
{
    static char res[4] = "TBD";
    return res;
}
