/*
	计算器
	v1.0.0
	支持运算符：+ - * / ^ ! % sin cos tan log
	Copyright Dart 2020 - 2021
*/
#define _CRT_SECURE_NO_WARNINGS

#define LENGTH 1024

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>
#include<string.h>

char* calculate(char* e);
double handleNumber(char* e);
char* handleOperator(char* e);
void replace(char* e, char* oldChars, char* newChars, bool replaceAll, int replaceIndex);
bool isNumber(char c);
bool isExist(char* e, char c);
void test();

int places = 2;
char expression[LENGTH];

int main()
{
	test();
	while (1)
	{
		printf("\nPlease input the expression:");
		scanf("%s", expression);
		printf("Result:%s\n", calculate(expression));
	}
}

char* calculate(char* expression)
{
	//prepare for input
	char* e = (char*)malloc(sizeof(char) * LENGTH);
	strcpy(e, expression);
	if (isExist(e, 's') || isExist(e, 't') || isExist(e, 'l'))
	{
		for (int i = 0; i < strlen(e) - 2; i++)
		{
			if (e[i] == 's' && e[i + 1] == 'i' && e[i + 2] == 'n')
			{
				replace(e, "sin", "S", false, i);
			}
			if (e[i] == 'c' && e[i + 1] == 'o' && e[i + 2] == 's')
			{
				replace(e, "cos", "C", false, i);
			}
			if (e[i] == 't' && e[i + 1] == 'a' && e[i + 2] == 'n')
			{
				replace(e, "tan", "T", false, i);
			}
			if (e[i] == 'l' && e[i + 1] == 'o' && e[i + 2] == 'g')
			{
				char num[LENGTH] = { '\0' };
				int count = 0;
				for (int j = i + 3; j < strlen(e); j++)
				{
					if (e[j] == '(')
					{
						break;
					}
					else
					{
						num[count] = e[j];
						count++;
					}
				}
				char* oldChars = (char*)malloc(sizeof(char) * (count + 3 + 1));
				oldChars[0] = 'l';
				oldChars[1] = 'o';
				oldChars[2] = 'g';
				oldChars[count + 3] = '\0';
				for (int j = 0; j < count; j++)
				{
					oldChars[j + 3] = num[j];
				}
				num[count] = 'L';
				replace(e, oldChars, num, false, i);
			}
		}
	}
	//handle negative number
	if (e[0] == '-')
	{
		replace(e, "-", "O", false, 0);
	}
	for (int i = 0; i < strlen(e) - 1; i++)
	{
		if (e[i] == '(' && e[i + 1] == '-')
		{
			replace(e, "-", "O", false, i);
		}
	}
	//handle brackets
	while (isExist(e, '('))
	{
		//Find the right bracket which firstly appear from the left to the right.
		int rightIndex = 0;
		for (int i = 0; i < strlen(e); i++)
		{
			if (e[i] == ')')
			{
				rightIndex = i;
				break;
			}
		}
		//Find the left bracket which firstly appear from the right to the left.
		int leftIndex = 0;
		for (int i = rightIndex - 1; i >= 0; i--)
		{
			if (e[i] == '(')
			{
				leftIndex = i;
				break;
			}
		}
		char* exp_with_bracket = (char*)malloc(sizeof(char) * LENGTH);		char* exp_without_bracket = (char*)malloc(sizeof(char) * LENGTH);
		memset(exp_with_bracket, '\0', sizeof(char) * LENGTH);
		memset(exp_without_bracket, '\0', sizeof(char) * LENGTH);
		int index = 0;
		for (int j = leftIndex; j <= rightIndex; j++)
		{
			exp_with_bracket[index] = e[j];
			index++;
		}
		index = 0;
		for (int j = leftIndex + 1; j < rightIndex; j++)
		{
			exp_without_bracket[index] = e[j];
			index++;
		}
		//printf("%s\n", exp_without_bracket);
		char* temp = handleOperator(exp_without_bracket);
		replace(e, exp_with_bracket, temp, false, leftIndex);
	}
	//calculate expression without brackets
	char* result = (char*)malloc(sizeof(char) * LENGTH);
	strcpy(result, handleOperator(e));
	memset(e, '\0', sizeof(char) * LENGTH);
	strcpy(e, result);
	//prepare for output
	int index = 0;
	for (int i = 0; i < strlen(e); i++)
	{
		if (e[i] == 'O')
		{
			e[i] = '-';
		}
		if (e[i] == '.')
		{
			index = i;
		}
	}
	//set精度
	for (int i = index + 1 + places; i < strlen(e); i++)
	{
		e[i] = '\0';
	}
	//return result
	return e;
}

double handleNumber(char* e)
{
	double num = 0;
	int power = 0;
	for (int i = 0; i < strlen(e); i++)
	{
		if (isNumber(e[i]))
		{
			if (e[i] == '.')
			{
				num *= pow(0.1, power);
				power = 0;
			}
			else
			{
				num += (e[i] - 48) * pow(10, power);
				power++;
			}
		}
		else if (e[i] == '-')
		{
			num = 0 - num;
			break;
		}
	}
	//printf("\n%lf\n", num);
	return num;
}

char* handleOperator(char* expression)
{
	//printf("\nHandleOperator:%s\n", expression);
	char* e = (char*)malloc(sizeof(char) * LENGTH);
	memset(e, '\0', sizeof(char) * LENGTH);
	strcpy(e, expression);
	char c[7][4] =
	{
		"SCT",
		"L",
		"!",
		"%",
		"^",
		"*/",
		"+-"
	};
	for (int k = 0; k < 7; k++)
	{
		for (int i = 0; i < strlen(e); i++)
		{
			if (isExist(c[k], e[i]))//When meet specific chars.
			{
				int leftCount = 0;//The index at the left of the current char.
				char leftChars[LENGTH] = { '\0' };
				for (int j = i - 1; j >= 0; j--)
				{
					if (isNumber(e[j]))
					{
						leftChars[leftCount] = e[j];
						leftCount++;
					}
					else if (e[j] == 'O')
					{
						leftChars[leftCount] = '-';
						leftCount++;
					}
					else
					{
						break;
					}
				}
				double leftNumber = handleNumber(leftChars);
				int rightCount = 0;//The index at the right of the current char.
				char rightChars[LENGTH] = { '\0' };
				for (int j = i + 1; j < strlen(e); j++)
				{
					if (isNumber(e[j]))
					{
						rightChars[rightCount] = e[j];
						rightCount++;
					}
					else if (e[j] == 'O')
					{
						rightChars[rightCount] = '-';
						rightCount++;
					}
					else
					{
						break;
					}
				}
				double rightNumber = handleNumber(_strrev(rightChars));
				int allCount = leftCount + 1 + rightCount;//The count which need to be replaced.
				char* allChars = (char*)malloc(sizeof(char) * (allCount + 1));
				for (int j = 0; j < leftCount; j++)
				{

					allChars[j] = e[i - leftCount + j];
				}
				allChars[leftCount] = e[i];
				for (int j = 0; j < rightCount; j++)
				{
					allChars[j + leftCount + 1] = e[i + j + 1];
				}
				//an important a line of code below
				allChars[allCount] = '\0';
				double resultNum = 0;//The result num.
				switch (e[i])
				{
				case 'S':
					resultNum = sin(rightNumber);
					break;
				case 'C':
					resultNum = cos(rightNumber);
					break;
				case 'T':
					resultNum = tan(rightNumber);
					break;
				case 'L':
					resultNum = log10(rightNumber) / log10(leftNumber);
					break;
				case '%':
					resultNum = leftNumber * 0.01;
					break;
				case '!':
					resultNum = 1;
					while (leftNumber > 0)
					{
						resultNum *= leftNumber;
						leftNumber--;
					}
					break;
				case '^':
					resultNum = pow(leftNumber, rightNumber);
					break;
				case '*':
					resultNum = leftNumber * rightNumber;
					break;
				case '×':
					resultNum = leftNumber * rightNumber;
					break;
				case '/':
					resultNum = leftNumber / rightNumber;
					break;
				case '÷':
					resultNum = leftNumber / rightNumber;
					break;
				case '+':
					resultNum = leftNumber + rightNumber;
					break;
				case '-':
					resultNum = leftNumber - rightNumber;
					break;
				default:
					break;
				}
				char resultChars[LENGTH] = { '\0' };
				sprintf(resultChars, "%lf", resultNum);
				if (resultNum < 0)
				{
					replace(resultChars, "-", "O", false, 0);
				}
				replace(e, allChars, resultChars, false, i - leftCount);
				i = i - leftCount + strlen(resultChars) - 1;
			}
		}
	}
	//printf("HandleOperator:%s\n", expression);
	return e;
}

void replace(char* e, char* oldChars, char* newChars, bool replaceAll, int replaceIndex)
{
	//printf("\n替换的对象：%s\n旧字符串：%s 新字符串：%s", e, oldChars, newChars);
	int oldlength = strlen(oldChars);
	int newlength = strlen(newChars);
	//printf("\n旧字符串长度：%d 新字符串长度：%d", oldlength, newlength);
	do
	{
		//将字符串切割成 3 部分：前部分parta，旧字符串部分oldChars，后部分partb
		char* a = strstr(e, oldChars);
		int partalength = strlen(e) - strlen(a);
		char* parta = (char*)malloc(sizeof(char) * (partalength + 1));
		memset(parta, '\0', sizeof(char) * (partalength + 1));
		if (!replaceAll)
		{
			if (partalength != replaceIndex)
			{
				break;
			}
		}
		int partblength = strlen(a) - oldlength;
		//printf("\n字符串A：%s 长度：%d", a, partalength);
		char* partb = (char*)malloc(sizeof(char) * (partblength + 1));
		memset(partb, '\0', sizeof(char) * (partblength + 1));
		//printf("\n字符串B：%s 长度：%d", partb, partblength);
		//填充parta
		for (int i = 0; i < partalength; i++)
		{
			parta[i] = e[i];
		}
		//填充partb
		for (int i = 0; i < partblength; i++)
		{
			partb[i] = e[partalength + oldlength + i];
		}
		//填充源字符串
		memset(e, '\0', sizeof(char) * LENGTH);
		for (int i = 0; i < partalength; i++)
		{
			e[i] = parta[i];
		}
		for (int i = 0; i < newlength; i++)
		{
			e[i + partalength] = newChars[i];
		}
		for (int i = 0; i < partblength; i++)
		{
			char temp = partb[i];
			e[i + partalength + newlength] = partb[i];
		}
	} while (replaceAll && strstr(e, oldChars) != NULL);
	//printf("\n替换结果：%s\n", e);
}

bool isNumber(char c)
{
	if (('0' <= c && c <= '9') || c == '.')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isExist(char* e, char c)
{
	if (strchr(e, c) == NULL)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void test()
{
	char expression1[2][LENGTH] =
	{
		"(-2)+2-(-52)*(-1)*5+87/(-3)*(-1)",
		"(2/3+1/2)/(-1/12)*(-12)"
	};
	for (int i = 0; i < 2; i++)
	{
		printf("%s = %s\n", expression1[i], calculate(expression1[i]));
	}
}