#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include "fraction.h"

int divisor_subtract(int a, int b)//更相减损法
{
	//仅对正数有效！
	if (a <= 0 || b <= 0)return 0;

	int temp;
	while (a != b)
	{
		temp = abs(a - b);
		a = a < b ? a : b;
		b = temp;
	}
	return a;
}

int divisor_traverse(int a, int b)
{
	//仅对正数有效！
	for (int i = (a < b ? a : b); i >0; i--)
	{
		if (a%i == 0 && b%i == 0)
		{
			return i;
		}
	}
	return 1;
}

int divisor(int a, int b)
{
	a = a < 0 ? -a : a;
	b = b < 0 ? -b : b;
	if ((a < b ? a : b) < 50)
	{
		return divisor_traverse(a, b);
	}
	else
	{
		return divisor_subtract(a, b);
	}
}

void simplify_fraction(fraction *num_ptr)
{
	if (num_ptr->numerator== 0)
	{
		num_ptr->denominator = 1;
		return;
	}
	int divisor_num = divisor(num_ptr->numerator, num_ptr->denominator);
	num_ptr->numerator /= divisor_num;
	num_ptr->denominator /= divisor_num;
}

void simplify_row(int i)
{
	for (int j = 0; j < n;j++)
	{
		if (matrix[i][j].denominator != 1)
			simplify_fraction(&matrix[i][j]);
	}
}

void SimplifyMatrix()
{
	for (int i = 0; i < m;i++)
	{
		for (int j = 0; j < n;j++)
		{
			if (matrix[i][j].denominator != 1)
				simplify_fraction(&matrix[i][j]);
		}
	}
}

int common_multiple(int a, int b)
{
	return a*b / divisor(a, b);
}

int GetInverseNum(fraction *result, fraction *num_ptr)
{
	if (num_ptr->numerator == 0 || num_ptr->denominator == 0)
	{
		return WRONG;
	}
	result->numerator = num_ptr->denominator;
	result->denominator = num_ptr->numerator;
	if (result->denominator < 0)
	{
		result->numerator = -result->numerator;
		result->denominator = -result->denominator;
	}
	return NORMAL;
}