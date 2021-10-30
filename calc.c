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

void exchange_row(int row1, int row2)//��row1��row2��1��ʼ
{
	//�������ǰӦ���Լ�
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

void row_multiply_fraction(int row, fraction *num_ptr)//�б���
{
	//�������ǰӦ���Լ�
	row--;
	for (int i = 0; i < n;i++)
	{
		fraction_multiply(&matrix[row][i], num_ptr);
	}
	//simplify_row(row);
	//�˴����軯����Ϊfraction_multiply�������Ѿ�����
}

void row_pointer_multiply_fraction(fraction *row, fraction *num_ptr)
{
	for (int i = 0; i < n; i++)
	{
		fraction_multiply(row + i, num_ptr);
	}
}

void col_multiply_fraction(int col, fraction *num_ptr)//�б���
{
	//�������ǰӦ���Լ�
	col--;
	for (int i = 0; i < m; i++)
	{
		fraction_multiply(&matrix[i][col], num_ptr);
	}

	//�˴����軯����Ϊfraction_multiply�������Ѿ�����
}


void add_fraction(fraction *dest, fraction *src)//�����������
{
	int common_muitiple_num = common_multiple(dest->denominator, src->denominator);
	fraction temp = { src->numerator,src->denominator };

	//ͨ��
	dest->numerator *= common_muitiple_num/dest->denominator;
	dest->denominator = common_muitiple_num;
	temp.numerator *= common_muitiple_num/temp.denominator;
	temp.denominator = common_muitiple_num;

	//���
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

void add_row_multiply_fraction(int dest, int row, fraction *num_ptr)//row�г���numptr����ӵ�dest��
{
	dest--;//Ŀ����
	row--;//Դ��
	fraction *temp = malloc(sizeof(fraction)*n);
	for (int i = 0; i < n;i++)
	{
		temp[i].numerator = matrix[row][i].numerator;
		temp[i].denominator = matrix[row][i].denominator;

		fraction_multiply(&temp[i], num_ptr);
		//ϣ�������º��һ��ܿ���������
		add_fraction(&matrix[dest][i], &temp[i]);
		//�군�����Ѿ���������
	}

	free(temp);
}

void add_col_multiply_fraction(int dest, int col, fraction *num_ptr)//col�г���numptr����ӵ�dest��
{
	dest--;//Ŀ����
	col--;//Դ��
	fraction *temp = malloc(sizeof(fraction)*m);
	for (int i = 0; i < m; i++)
	{
		temp[i].numerator = matrix[i][col].numerator;
		temp[i].denominator = matrix[i][col].denominator;

		fraction_multiply(&temp[i], num_ptr);
		//Դ�и��Ƶ�temp�к����num_ptr��
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
		//���ȴӵ�i�п�ʼ���ҵ�i��Ԫ�ز�Ϊ0����
		int mark = i;
		while (mark < m && matrix[mark][i].numerator == 0)
			mark++;
		if (i != mark)
		{
			if (mark == m)//����û�з���Ԫ�أ����ش���
			{
				continue;
			}
			else//�ҵ��˵�mark�еĵ�i��Ԫ�ز�Ϊ0����֮ǰ��ȫΪ0
			{
				exchange_row(i + 1, mark + 1);
				printf("����%d�����%d�н�����\n", i + 1, mark + 1);
				PrintMatrix();
				putchar('\n');
			}
		}
		//���ڵ�i�еĵ�i��Ԫ�ز�Ϊ0��������֮������и���Ԫ����Ϊ0
		for (int j = i + 1; j < m; j++)
		{
			if (matrix[j][i].numerator==0)//�Ѿ�Ϊ0�Ͳ�������
				continue;
			fraction *temp = malloc(sizeof(fraction)*n);
			fraction temp_fraction, num;
			copy_row(temp, matrix[i]);
			if(GetInverseNum(&temp_fraction, &matrix[i][i])==WRONG)
				puts("���ִ��󣡶�0���˵���");//ȡ��������Ϊ1
			num = temp_fraction;
			row_pointer_multiply_fraction(temp, &temp_fraction);

			temp_fraction.numerator = -matrix[j][i].numerator;
			temp_fraction.denominator = matrix[j][i].denominator;
			fraction_multiply(&num, &temp_fraction);
			row_pointer_multiply_fraction(temp, &temp_fraction);
			add_row_pointer(matrix[j], temp);//��Ϊ0
			//��ӡ˵��
			if (num.denominator==1)
			{
				printf("��%d�м��ϵ�%d�г���%d����\n",j+1,i+1,num.numerator);
			} 
			else
			{
				printf("��%d�м��ϵ�%d�г���%d/%d����\n", j + 1, i + 1, num.numerator,num.denominator);
			}
			PrintMatrix();
			putchar('\n');
			free(temp);
		}
	}
	puts("���������\n");
}