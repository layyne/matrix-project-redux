#include "demo.h"

void demo(void)
{
    int choice;
    Matrix *a, *b, *r;
    Status status;

    puts("===========================================================\n"
         "This program is meant to be a crude shell-like demo of this \n"
         "library's capabilities. These functions are intended to be\n"
         "paired with much more rigorous input validation.\n"
         "===========================================================\n");

    a = NULL;
    b = NULL;
    r = NULL;

    if ((status = mx_create(&a, 3, 3)) < SUCCESS)
    {
        printf("%s\n", status_string(status));
        return;
    }

    if ((status = mx_create(&b, 3, 3)) < SUCCESS)
    {
        printf("%s\n", status_string(status));
        return;
    }

    if ((status = mx_create(&r, 3, 3)) < SUCCESS)
    {
        printf("%s\n", status_string(status));
        return;
    }

    choice = -1;

    while (choice)
    {
        print_menu();
        choice = get_int(0, 9);
        if (choice)
            process_choice(choice, a, b, r);
    }

    puts("===========================================================\n"
         "Later.\n"
         "===========================================================");
}

void print_menu(void)
{
    puts("1.\tAddition\n"
         "2.\tSubtraction\n"
         "3.\tMatrix multiplication\n"
         "4.\tScalar multiplication\n"
         "5.\tDeterminant\n"
         "6.\tInverse\n"
         "7.\tTranspose\n"
         "8.\tExponentiation\n"
         "9.\tSet matrices\n"
         "0.\tExit\n");
    printf("Enter choice: ");
}

void process_choice(int choice, Matrix *a, Matrix *b, Matrix *r)
{
    int p;
    double s, d;
    Status status;

    switch (choice)
    {
    case 1:
        if ((status = mx_add(r, a, b)) < SUCCESS)
            printf("%s\n", status_string(status));
        else
            mx_print(r);
        getchar();
        break;

    case 2:
        if ((status = mx_sub(r, a, b)) < SUCCESS)
            printf("%s\n", status_string(status));
        else
            mx_print(r);
        getchar();
        break;

    case 3:
        if ((status = mx_mul(r, a, b)) < SUCCESS)
            printf("%s\n", status_string(status));
        else
            mx_print(r);
        getchar();
        break;

    case 4:
        if ((status = mx_smul(r, a, s)) < SUCCESS)
            printf("%s\n", status_string(status));
        else
            mx_print(r);
        getchar();
        break;

    case 5:
        if ((status = mx_det(&d, a)) < SUCCESS)
            printf("%s\n", status_string(status));
        else
            printf("%lf\n", d);
        getchar();
        break;

    case 6:
        if ((status = mx_inv(r, a)) < SUCCESS)
            printf("%s\n", status_string(status));
        else
            mx_print(r);
        getchar();
        break;

    case 7:
        if ((status = mx_trans(r, a)) < SUCCESS)
            printf("%s\n", status_string(status));
        else
            mx_print(r);
        getchar();
        break;

    case 8:
        printf("Enter a power to which to raise matrix A: ");
        p = get_int(0, 15);

        if ((status = mx_pow(r, a, p)) < SUCCESS)
            printf("%s\n", status_string(status));
        else
            mx_print(r);
        getchar();
        break;

    case 9:
        puts("Matrix A:");
        if ((status = input_matrix(a)) < SUCCESS)
            printf("%s\n", status_string(status));

        puts("Matrix B:");
        if ((status = input_matrix(b)) < SUCCESS)
            printf("%s\n", status_string(status));
        break;

    default:
        puts("WTF");
    }
}

Status input_matrix(Matrix *res)
{
    int i, j, r, c;
    Status status;

    if (!res)
        return NULL_ARGUMENT;

    printf("Enter number of rows: ");
    r = get_int(1, 10);
    printf("Enter number of columns: ");
    c = get_int(1, 10);

    if ((status = mx_resize(res, r, c)) < SUCCESS)
        return status;

    puts("Enter array values:");

    for (i = 0; i < r; i++)
        for (j = 0; j < c; j++)
            scanf("%lf", &(res->m[i][j]));

    res->r = r;
    res->c = c;

    return SUCCESS;
}

int get_int(int low, int high)
{
    int value, check;

    while (1)
    {
        check = scanf("%d", &value);
        while (getchar() != '\n')
            ;

        if (check == 1 && (value >= low && value <= high))
            break;

        printf("Invalid input, enter an integer between %d and %d: ", low,
               high);
    }

    return value;
}
