#include "matrix.h"
#include <stdio.h>

void demo(void);
void print_menu(void);
void process_choice(int choice, Matrix *a, Matrix *b, Matrix *r);
void menu(void);
Status input_matrix(Matrix *res);
Status hardcode(Matrix *res);
int get_int(int low, int high);
int ask_hardcoded(void);
int destroy(int n, ...);
