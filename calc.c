#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include "fraction.h"

void fraction_multiply(fraction *dest_ptr, fraction *source_ptr)
{
	fraction temp;
	temp.numerator = source_ptr->numerator * dest_ptr->numerator;
	temp.denominator = source_ptr->denominator * dest_ptr->denominator;

	dest_ptr->numerator = temp.numerator;
	dest_ptr->denominator = temp.denominator;

	simplify_fraction(dest_ptr);
}

void exchange_row(int row1, int row2)//该row1和row2从1开始
{
	//因此运行前应先自减
	row1--;
	row2--;
	fraction temp;
	for (int i = 0; i < n; i++)
	{
		temp.numerator = matrix[row1][i].numerator;
		temp.denominator = matrix[row1][i].denominator;
		matrix[row1][i].numerator = matrix[row2][i].numerator;
		matrix[row1][i].denominator = matrix[row2][i].denominator;
		matrix[row2][i].numerator = temp.numerator;
		matrix[row2][i].denominator = temp.denominator;
	}
}

void exchange_col(int col1,int col2)
{
	col1--, col2--;
	fraction temp;
	for (int i = 0; i < m;i++)
	{
		temp.numerator = matrix[i][col1].numerator;
		temp.denominator = matrix[i][col1].denominator;
		matrix[i][col1].numerator = matrix[i][col2].numerator;
		matrix[i][col1].denominator = matrix[i][col2].denominator;
		matrix[i][col2].numerator = temp.numerator;
		matrix[i][col2].denominator = temp.denominator;
	}
}

void row_multiply_fraction(int row, fraction *num_ptr)//行倍乘
{
	//因此运行前应先自减
	row--;
	for (int i = 0; i < n;i++)
	{
		fraction_multiply(&matrix[row][i], num_ptr);
	}
	//simplify_row(row);
	//此处无需化简，因为fraction_multiply函数中已经化简
}

void row_pointer_multiply_fraction(fraction *row, fraction *num_ptr)
{
	for (int i = 0; i < n; i++)
	{
		fraction_multiply(row + i, num_ptr);
	}
}

void col_multiply_fraction(int col, fraction *num_ptr)//列倍乘
{
	//因此运行前应先自减
	col--;
	for (int i = 0; i < m; i++)
	{
		fraction_multiply(&matrix[i][col], num_ptr);
	}

	//此处无需化简，因为fraction_multiply函数中已经化简
}


void add_fraction(fraction *dest, fraction *src)//两个分数相加
{
	int common_muitiple_num = common_multiple(dest->denominator, src->denominator);
	fraction temp = { src->numerator,src->denominator };

	//通分
	dest->numerator *= common_muitiple_num/dest->denominator;
	dest->denominator = common_muitiple_num;
	temp.numerator *= common_muitiple_num/temp.denominator;
	temp.denominator = common_muitiple_num;

	//相加
	dest->numerator += temp.numerator;

	simplify_fraction(dest);
}

void add_row_pointer(fraction *dest, fraction *src)
{
	for (int i = 0; i < n; i++)
	{
		add_fraction(dest + i, src+i);
	}
}

void add_row_multiply_fraction(int dest, int row, fraction *num_ptr)//row行乘以numptr倍后加到dest行
{
	dest--;//目标行
	row--;//源行
	fraction *temp = malloc(sizeof(fraction)*n);
	for (int i = 0; i < n;i++)
	{
		temp[i].numerator = matrix[row][i].numerator;
		temp[i].denominator = matrix[row][i].denominator;

		fraction_multiply(&temp[i], num_ptr);
		//希望几个月后我还能看懂。。。
		add_fraction(&matrix[dest][i], &temp[i]);
		//完蛋，我已经看不懂了
	}

	free(temp);
}

void add_col_multiply_fraction(int dest, int col, fraction *num_ptr)//col列乘以numptr倍后加到dest列
{
	dest--;//目标列
	col--;//源列
	fraction *temp = malloc(sizeof(fraction)*m);
	for (int i = 0; i < m; i++)
	{
		temp[i].numerator = matrix[i][col].numerator;
		temp[i].denominator = matrix[i][col].denominator;

		fraction_multiply(&temp[i], num_ptr);
		//源列复制到temp中后乘上num_ptr倍
		add_fraction(&matrix[i][dest], &temp[i]);
	}

	free(temp);
}

void copy_row(fraction *dest, fraction *src)
{
	for (int i = 0; i < n; i++)
	{
		dest[i].numerator = src[i].numerator;
		dest[i].denominator = src[i].denominator;
	}
}

void AutoElementaryTransformation()
{
	int counter = m < n ? m : n;
	for (int i = 0; i < counter; i++)
	{
		//首先从第i行开始，找第i列元素不为0的行
		int mark = i;
		while (mark < m && matrix[mark][i].numerator == 0)
			mark++;
		if (i != mark)
		{
			if (mark == m)//该列没有非零元素，不必处理
			{
				continue;
			}
			else//找到了第mark行的第i列元素不为0，且之前的全为0
			{
				exchange_row(i + 1, mark + 1);
				printf("将第%d行与第%d行交换。\n", i + 1, mark + 1);
				PrintMatrix();
				putchar('\n');
			}
		}
		//现在第i行的第i列元素不为0，用它把之后的所有该列元素消为0
		for (int j = i + 1; j < m; j++)
		{
			if (matrix[j][i].numerator==0)//已经为0就不用消了
				continue;
			fraction *temp = malloc(sizeof(fraction)*n);
			fraction temp_fraction, num;
			copy_row(temp, matrix[i]);
			if(GetInverseNum(&temp_fraction, &matrix[i][i])==WRONG)
				puts("出现错误！对0求了倒数");//取倒数，化为1
			num = temp_fraction;
			row_pointer_multiply_fraction(temp, &temp_fraction);

			temp_fraction.numerator = -matrix[j][i].numerator;
			temp_fraction.denominator = matrix[j][i].denominator;
			fraction_multiply(&num, &temp_fraction);
			row_pointer_multiply_fraction(temp, &temp_fraction);
			add_row_pointer(matrix[j], temp);//消为0
			//打印说明
			if (num.denominator==1)
			{
				printf("第%d行加上第%d行乘以%d倍。\n",j+1,i+1,num.numerator);
			} 
			else
			{
				printf("第%d行加上第%d行乘以%d/%d倍。\n", j + 1, i + 1, num.numerator,num.denominator);
			}
			PrintMatrix();
			putchar('\n');
			free(temp);
		}
	}
	puts("运算结束。\n");
}