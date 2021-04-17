/*
	计算器
	v1.0.0
	支持运算符：+ - * / ^ ! % sin cos tan log
	Copyright Dart 2020 - 2021
*/
#include <iostream>
#include <sstream>

using namespace std;

string calculate(string e);
double handleNumber(string e);
string handleOperator(string e);
bool isNumber(char c);
void string2num(string str, double& num);
void test();

int places = 2;
string expression;

int main()
{
	test();
	while (1)
	{
		cout << "Please input the expression:";
		cin >> expression;
		cout << "Result:" << calculate(expression) << endl;
	}
}

string calculate(string expression)
{
	//prepare for input
	string e = expression;
	if (e.find_first_of('s') != string::npos || e.find_first_of('t') != string::npos || e.find_first_of('l') != string::npos)
	{
		for (int i = 0; i < e.length() - 2; i++)
		{
			if (e[i] == 's' && e[i + 1] == 'i' && e[i + 2] == 'n')
			{
				e.replace(i, 3, "S");
			}
			if (e[i] == 'c' && e[i + 1] == 'o' && e[i + 2] == 's')
			{
				e.replace(i, 3, "S");
			}
			if (e[i] == 't' && e[i + 1] == 'a' && e[i + 2] == 'n')
			{
				e.replace(i, 3, "T");
			}
			if (e[i] == 'l' && e[i + 1] == 'o' && e[i + 2] == 'g')
			{
				string num = e.substr(i + 3, e.find_first_of('(', i + 3) - (i + 2));
				e.replace(i, num.length() + 3, num + "L");
			}
		}
	}
	//handle negative number
	if (e[0] == '-')
	{
		e.replace(0, 1, "O");
	}
	for (int i = 0; i < e.length() - 1; i++)
	{
		if (e[i] == '(' && e[i + 1] == '-')
		{
			e.replace(i + 1, 1, "O");
		}
	}
	//handle brackets
	while (e.find_first_of('(') != string::npos)
	{
		int rightIndex = e.find_first_of(')');
		int leftIndex = e.find_last_of('(', rightIndex);
		string exp_with_bracket = e.substr(leftIndex, rightIndex - leftIndex + 1);
		string exp_without_bracket = e.substr(leftIndex + 1, rightIndex - leftIndex - 1);
		//cout << exp_without_bracket;
		string temp = handleOperator(exp_without_bracket);
		e.replace(leftIndex, exp_with_bracket.length(), temp);
	}
	//calculate expression without brackets
	e = handleOperator(e);
	//prepare for output
	if (e.find_first_of('O') != string::npos)
	{
		e.replace(e.find_first_of('O'), 1, "-");
	}
	//set精度
	if (e.find_first_of('.') != string::npos)
	{
		e = e.substr(0, e.find_first_of('.') + 1 + places);
	}
	//return result
	return e;
}

double handleNumber(string e)
{
	double num = 0;
	int power = 0;
	for (int i = 0; i < e.length(); i++)
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
	//cout << num;
	return num;
}

string handleOperator(string expression)
{
	//cout << "HandleOperator:" << expression;
	string e = expression;
	string c[] =
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
		for (int i = 0; i < e.length(); i++)
		{
			if (c[k].find_first_of(e[i]) != string::npos)//When meet specific chars.
			{
				string leftChars = "";
				for (int j = i - 1; j >= 0; j--)
				{
					if (isNumber(e[j]))
					{
						leftChars += e[j];
					}
					else if (e[j] == 'O')
					{
						leftChars += '-';
					}
					else
					{
						break;
					}
				}
				reverse(leftChars.begin(), leftChars.end());
				double leftNumber = 0;
				string2num(leftChars, leftNumber);
				string rightChars = "";
				for (int j = i + 1; j < e.length(); j++)
				{
					if (isNumber(e[j]))
					{
						rightChars += e[j];
					}
					else if (e[j] == 'O')
					{
						rightChars += '-';
					}
					else
					{
						break;
					}
				}
				double rightNumber = 0;
				string2num(rightChars, rightNumber);
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
				string resultChars = to_string(resultNum);
				if (resultNum < 0)
				{
					resultChars.replace(0, 1, "O");
				}
				e.replace(i - leftChars.length(), leftChars.length() + 1 + rightChars.length(), resultChars);
				i = i - leftChars.length() + resultChars.length() - 1;
			}
		}
	}
	//cout << "HandleOperator:" << expression;
	return e;
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

void string2num(string str, double& num)
{
	stringstream ss;
	ss << str;
	ss >> num;
}

void test()
{
	string expression1[] =
	{
		"(-2)+2-(-52)*(-1)*5+87/(-3)*(-1)",
		"(2/3+1/2)/(-1/12)*(-12)"
	};
	for (int i = 0; i < 2; i++)
	{
		cout << expression1[i] << " = " << calculate(expression1[i]) << endl;
	}
}