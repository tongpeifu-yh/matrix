#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include "fraction.h"

/*
 ************************************************
 *               矩阵计算工具                   *
 *                by 通配符                     *
 *              begin@2021.9.20                 *
 ************************************************
 */



int main(int argc, char* argv[])
{
	//m = 5, n = 3;
	PrintInfo();
	puts("请输入矩阵的行数m，列数n（仅可输入整数）：");
	scanf("%d%d", &m, &n);
	if (m <= 0 || n <= 0)
	{
		puts("数字不正确！按任意键并回车后退出...");
		getchar();
		getchar();
		return -1;
	}
	GetSpace();
	GetValue();
	SimplifyMatrix();
	PrintMatrix();
	putc('\n', stdout);
	puts("开始运算。");
	PrintHelp();

	while (1)
	{
		int opt;
		int r1, r2, c1, c2;
		char str[100] = { 0 };
		fraction num;
		puts("请输入选项（1，2，3，4，5，6，7，8）：");
		scanf("%d", &opt);

		switch (opt)
		{
		case 0:
			PrintHelp();
			break;
		case 1:
			puts("请输入要对换的两个行号：");
			scanf("%d%d", &r1, &r2);
			//检查错误
			if (CheckRowNum(r1)==WRONG||CheckRowNum(r2)==WRONG)
				break;
			if (r1 == r2)
			{
				puts("行号不能相同！\n");
				break;
			}
			exchange_row(r1, r2);
			putchar('\n');
			break;
		case 2:
			puts("请输入行号与倍数：");
			scanf("%d%s",&r1, str);
			//检查错误
			if (CheckRowNum(r1)==WRONG)
				break;
			if (StringConvert(str, &num) == WRONG)
			{
				puts("数字不正确！\n");
				break;
			}
			row_multiply_fraction(r1, &num);
			putchar('\n');
			break;
		case 3:
			puts("请输入目标行号，源行号，倍数：");
			scanf("%d%d%s", &r1, &r2, str);
			//检查错误
			if(CheckRowNum(r1)==WRONG||CheckRowNum(r2)==WRONG)
				break;
			if (r1 == r2)
			{
				puts("行号不能相同！\n");
				break;
			}
			if (StringConvert(str, &num) == WRONG)
			{
				puts("数字不正确！\n");
				break;
			}
			add_row_multiply_fraction(r1, r2, &num);
			putchar('\n');
			break;
		case 4:
			puts("请输入要对换的两个列号：");
			scanf("%d%d", &c1, &c2);
			if(CheckColNum(c1)==WRONG||CheckColNum(c2)==WRONG)
				break;
			if (c1 == c2)
			{
				puts("列号不能相同！\n");
				break;
			}
			exchange_col(c1, c2);
			putchar('\n');
			break;
		case 5:
			puts("请输入列号与倍数：");
			scanf("%d%s", &c1, str);
			if(CheckColNum(c1)==WRONG)
				break;
			if (StringConvert(str, &num) == WRONG)
			{
				puts("数字不正确！\n");
				break;
			}
			col_multiply_fraction(c1, &num);
			putchar('\n');
			break;
		case 6:
			puts("请输入目标列号，源列号，倍数：");
			scanf("%d%d%s", &c1, &c2, str);
			//检查错误
			if (CheckColNum(c1) == WRONG || CheckColNum(c2) == WRONG)
				break;
			if (c1 == c2)
			{
				puts("列号不能相同！\n");
				break;
			}
			if (StringConvert(str, &num) == WRONG)
			{
				puts("数字不正确！\n");
				break;
			}
			add_col_multiply_fraction(c1, c2, &num);
			putchar('\n');
			break;
		case 7:
			AutoElementaryTransformation();
			break;
		case 8:
			goto END;
			break;
		default:
			break;
		}
		PrintMatrix();
	}

END:
	FreeSpace();
	return 0;
}

void FreeSpace()
{
	for (int i = 0; i < m; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
}


void GetSpace()
{
	matrix = malloc(sizeof(fraction*)*m);
	if (!matrix)
	{
		fputs("内存错误！", stderr);
		exit(-1);
	}
	for (int i = 0; i < m; i++)
	{
		matrix[i] = malloc(sizeof(fraction)*n);
		if (!matrix[i])
		{
			fputs("内存错误！", stderr);
			exit(-1);
		}
	}
}

void GetValue()
{
	char str[100] = { 0 };

	printf("请输入%d*%d个数字（可以是分数）：\n", m,n);
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			scanf("%s", str);
			if (StringConvert(str, &matrix[i][j]) == WRONG)
				j--;
			memset(str, 0, 100);
		}
	}
}

int StringConvert(char *str, fraction *num_ptr)
{
	char *tmp = strchr(str, '/');
	int tmp_numerator = 0, tmp_denominator = 0;

	tmp_numerator = atoi(str);
	if (!tmp)//若tmp==Null,则分母为1
	{
		tmp_denominator = 1;
	}
	else
	{
		tmp_denominator = atoi(tmp + 1);
	}

	if (tmp_denominator == 0)
		return WRONG;
	if (tmp_numerator == 0)
	{
		num_ptr->numerator = 0;
		num_ptr->denominator = 1;
		return 0;
	}

	if (tmp_denominator < 0)
	{
		tmp_numerator = -tmp_numerator;
		tmp_denominator = -tmp_denominator;
	}

	num_ptr->numerator = tmp_numerator;
	num_ptr->denominator = tmp_denominator;
	return 0;
}

void PrintMatrix()
{
	puts("现在的矩阵为：");
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (matrix[i][j].denominator != 1)
				printf("%d/%d\t", matrix[i][j].numerator, matrix[i][j].denominator);
			else
				printf("%d\t", matrix[i][j].numerator);
		}
		putc('\n', stdout);
	}
}

void PrintHelp()
{
	puts("1.进行行对换\n"
		"2.进行行倍乘运算\n"
		"3.进行行倍加运算\n"
		"4.进行列对换\n"
		"5.进行列倍乘运算\n"
		"6.进行列倍加运算\n"
		"7.进行自动行变换阶梯形\n"
		"8.退出\n"
		"请勿输入小数！仅支持整数和分数（如1/2）。\n"
		);
}

int CheckRowNum(int row)
{
	if (row <= 0 || row > m)
	{
		puts("行号不正确！\n");
		return WRONG;
	}
	return RIGHT;
}

int CheckColNum(int col)
{
	if (col <= 0 || col > n)
	{
		puts("列号不正确！\n");
		return WRONG;
	}
	return RIGHT;
}

void PrintInfo()
{
	puts("\t矩阵初等变换工具\n"
		"\tby 通配符\n"
		"\te-mail:tongpeifu_yh@163.com\n"
		"\tbegin@2021.9.20@SEU");
}