#ifndef _FRACTION_H_
#define _FRACTION_H_

#define RIGHT 1
#define NORMAL 0;
#define WRONG -1
#define YES 1
#define NO -1

typedef struct _fraction
{
	int numerator;
	int denominator;
}fraction;

void GetSpace();
void FreeSpace();
void GetValue();
int StringConvert(char *str, fraction *num_ptr);
void PrintMatrix();
int divisor(int a, int b);
void simplify_fraction(fraction *num_ptr);
void SimplifyMatrix();
void simplify_row(int i);
void fraction_multiply(fraction *dest_ptr, fraction *source_ptr);
void exchange_row(int row1, int row2);
void row_multiply_fraction(int row, fraction *num_ptr);
void add_row_multiply_fraction(int dest, int row, fraction *num_ptr);
int common_multiple(int a, int b);
void PrintHelp();
int CheckRowNum(int row);
void PrintInfo();
void exchange_col(int col1, int col2);
int CheckColNum(int col);
void col_multiply_fraction(int col, fraction *num_ptr);
void add_col_multiply_fraction(int dest, int col, fraction *num_ptr);

int GetInverseNum(fraction *result, fraction *num_ptr);
void copy_row(fraction *dest, fraction *src);
void row_pointer_multiply_fraction(fraction *row, fraction *num_ptr);
void add_row_pointer(fraction *dest, fraction *src);
void AutoElementaryTransformation();

fraction ** matrix;
int m, n;
#endif // !_FRACTION_H_