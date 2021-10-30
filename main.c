#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include "fraction.h"

/*
 ************************************************
 *               ������㹤��                   *
 *                by ͨ���                     *
 *              begin@2021.9.20                 *
 ************************************************
 */



int main(int argc, char* argv[])
{
	//m = 5, n = 3;
	PrintInfo();
	puts("��������������m������n������������������");
	scanf("%d%d", &m, &n);
	if (m <= 0 || n <= 0)
	{
		puts("���ֲ���ȷ������������س����˳�...");
		getchar();
		getchar();
		return -1;
	}
	GetSpace();
	GetValue();
	SimplifyMatrix();
	PrintMatrix();
	putc('\n', stdout);
	puts("��ʼ���㡣");
	PrintHelp();

	while (1)
	{
		int opt;
		int r1, r2, c1, c2;
		char str[100] = { 0 };
		fraction num;
		puts("������ѡ�1��2��3��4��5��6��7��8����");
		scanf("%d", &opt);

		switch (opt)
		{
		case 0:
			PrintHelp();
			break;
		case 1:
			puts("������Ҫ�Ի��������кţ�");
			scanf("%d%d", &r1, &r2);
			//������
			if (CheckRowNum(r1)==WRONG||CheckRowNum(r2)==WRONG)
				break;
			if (r1 == r2)
			{
				puts("�кŲ�����ͬ��\n");
				break;
			}
			exchange_row(r1, r2);
			putchar('\n');
			break;
		case 2:
			puts("�������к��뱶����");
			scanf("%d%s",&r1, str);
			//������
			if (CheckRowNum(r1)==WRONG)
				break;
			if (StringConvert(str, &num) == WRONG)
			{
				puts("���ֲ���ȷ��\n");
				break;
			}
			row_multiply_fraction(r1, &num);
			putchar('\n');
			break;
		case 3:
			puts("������Ŀ���кţ�Դ�кţ�������");
			scanf("%d%d%s", &r1, &r2, str);
			//������
			if(CheckRowNum(r1)==WRONG||CheckRowNum(r2)==WRONG)
				break;
			if (r1 == r2)
			{
				puts("�кŲ�����ͬ��\n");
				break;
			}
			if (StringConvert(str, &num) == WRONG)
			{
				puts("���ֲ���ȷ��\n");
				break;
			}
			add_row_multiply_fraction(r1, r2, &num);
			putchar('\n');
			break;
		case 4:
			puts("������Ҫ�Ի��������кţ�");
			scanf("%d%d", &c1, &c2);
			if(CheckColNum(c1)==WRONG||CheckColNum(c2)==WRONG)
				break;
			if (c1 == c2)
			{
				puts("�кŲ�����ͬ��\n");
				break;
			}
			exchange_col(c1, c2);
			putchar('\n');
			break;
		case 5:
			puts("�������к��뱶����");
			scanf("%d%s", &c1, str);
			if(CheckColNum(c1)==WRONG)
				break;
			if (StringConvert(str, &num) == WRONG)
			{
				puts("���ֲ���ȷ��\n");
				break;
			}
			col_multiply_fraction(c1, &num);
			putchar('\n');
			break;
		case 6:
			puts("������Ŀ���кţ�Դ�кţ�������");
			scanf("%d%d%s", &c1, &c2, str);
			//������
			if (CheckColNum(c1) == WRONG || CheckColNum(c2) == WRONG)
				break;
			if (c1 == c2)
			{
				puts("�кŲ�����ͬ��\n");
				break;
			}
			if (StringConvert(str, &num) == WRONG)
			{
				puts("���ֲ���ȷ��\n");
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
		fputs("�ڴ����", stderr);
		exit(-1);
	}
	for (int i = 0; i < m; i++)
	{
		matrix[i] = malloc(sizeof(fraction)*n);
		if (!matrix[i])
		{
			fputs("�ڴ����", stderr);
			exit(-1);
		}
	}
}

void GetValue()
{
	char str[100] = { 0 };

	printf("������%d*%d�����֣������Ƿ�������\n", m,n);
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
	if (!tmp)//��tmp==Null,���ĸΪ1
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
	puts("���ڵľ���Ϊ��");
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
	puts("1.�����жԻ�\n"
		"2.�����б�������\n"
		"3.�����б�������\n"
		"4.�����жԻ�\n"
		"5.�����б�������\n"
		"6.�����б�������\n"
		"7.�����Զ��б任������\n"
		"8.�˳�\n"
		"��������С������֧�������ͷ�������1/2����\n"
		);
}

int CheckRowNum(int row)
{
	if (row <= 0 || row > m)
	{
		puts("�кŲ���ȷ��\n");
		return WRONG;
	}
	return RIGHT;
}

int CheckColNum(int col)
{
	if (col <= 0 || col > n)
	{
		puts("�кŲ���ȷ��\n");
		return WRONG;
	}
	return RIGHT;
}

void PrintInfo()
{
	puts("\t������ȱ任����\n"
		"\tby ͨ���\n"
		"\te-mail:tongpeifu_yh@163.com\n"
		"\tbegin@2021.9.20@SEU");
}