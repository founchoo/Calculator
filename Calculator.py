#	计算器
#	v1.0.0
#	支持运算符：+ - * / ^ ! % sin cos tan log
#	Copyright Dart 2020 - 2021
import math

places = 2
expression = ""
LENGTH = 1024

def calculate(expression):
	#prepare for input
	e = expression
	if e.find('s') != -1 or e.find('t') != -1 or e.find('l') != -1:
		i = 0
		while i < len(e):
			if e[i] == 's' and e[i + 1] == 'i' and e[i + 2] == 'n':
				e = e.replace("sin", "S", 1)
			if e[i] == 'c' and e[i + 1] == 'o' and e[i + 2] == 's':
				e = e.replace("cos", "C", 1)
			if e[i] == 't' and e[i + 1] == 'a' and e[i + 2] == 'n':
				e = e.replace("tan", "T", 1)
			if e[i] == 'l' and e[i + 1] == 'o' and e[i + 2] == 'g':
				num = e[i + 3 : e.find('(', i + 3)]
				e = e.replace("log" + num, num + "L", 1)
			i += 1
	#handle negative number
	if e[0] == '-':
		e = e.replace("-", "O", 1)
	e = e.replace("(-", "(O")
	#handle brackets
	while e.find('(') != -1:
		rightIndex = e.find(')')
		leftIndex = e.rfind('(', 0, rightIndex)
		exp_with_bracket = e[leftIndex : rightIndex + 1]
		exp_without_bracket = e[leftIndex + 1 : rightIndex]
		#print(exp_without_bracket)
		temp = handleOperator(exp_without_bracket)
		e = e.replace(exp_with_bracket, temp, 1)
	#calculate expression without brackets
	e = handleOperator(e)
	#prepare for output
	if e.find('O') != -1:
		e = e.replace("O", "-", 1)
	#set精度
	if e.find('.') != -1:
		e = e[0 : e.find('.') + 1 + places]
	#return result
	return e

def handleNumber(e):
	num = 0
	power = 0
	for i in range(0, len(e)):
		if isNumber(e[i]):
			if e[i] == '.':
				num *= pow(0.1, power)
				power = 0
			else:
				num += (e[i] - 48) * pow(10, power)
				power += 1
		elif e[i] == '-':
			num = 0 - num
	#print(num)
	return num

def handleOperator(expression):
	#print("HandleOperator:" + expression)
	e = expression
	c = [
		"SCT",
		"L",
		"!",
		"%",
		"^",
		"*/",
		"+-"
	]
	for k in range(0, len(c)):
		i = 0
		while i < len(e):
			if c[k].find(e[i]) != -1: #When meet specific chars.
				leftChars = ""
				for j in range(i - 1, -1, -1):
					if isNumber(e[j]):
						leftChars += e[j]
					elif e[j] == 'O':
						leftChars += '-'
					else:
						break
				leftChars = leftChars[::-1]
				leftNumber = -1 if leftChars == '' else float(leftChars)
				rightChars = ""
				for j in range(i + 1, len(e)):
					if isNumber(e[j]):
						rightChars += e[j]
					elif e[j] == 'O':
						rightChars += '-'
					else:
						break
				rightNumber = float(rightChars)
				resultNum = 0 #The result num.
				if e[i] == 'S':
					resultNum = math.sin(rightNumber)
				elif e[i] == 'C':
					resultNum = math.cos(rightNumber)
				elif e[i] == 'T':
					resultNum = math.tan(rightNumber)
				elif e[i] == 'L':
					resultNum = math.log10(rightNumber) / math.log10(leftNumber)
				elif e[i] == '%':
					resultNum = leftNumber * 0.01
				elif e[i] == '!':
					resultNum = 1
					while leftNumber > 0:
						resultNum *= leftNumber
						leftNumber -= 1
				elif e[i] == '^':
					resultNum = pow(leftNumber, rightNumber)
				elif e[i] == '*':
					resultNum = leftNumber * rightNumber
				elif e[i] == '×':
					resultNum = leftNumber * rightNumber
				elif e[i] == '/':
					resultNum = leftNumber / rightNumber
				elif e[i] == '÷':
					resultNum = leftNumber / rightNumber
				elif e[i] == '+':
					resultNum = leftNumber + rightNumber
				elif e[i] == '-':
					resultNum = leftNumber - rightNumber
				resultChars = str(resultNum)
				if resultNum < 0:
					resultChars = resultChars.replace("-", "O", 1)
				e = e.replace(leftChars.replace("-", "O", 1) + e[i] + rightChars.replace("-", "O", 1), resultChars, 1)
				i = i - len(leftChars) + len(resultChars) - 1
			i += 1
	#ptint("HandleOperator:" + expression)
	return e

def isNumber(c):
	if c == '0' or c == '1' or c == '2' or c == '3' or c == '4' or c == '5' or c == '6' or c == '7' or c == '8' or c == '9' or c == '.':
		return True
	else:
		return False

def test():
	expression1 = [
		"(-2)+2-(-52)*(-1)*5+87/(-3)*(-1)",
		"(2/3+1/2)/(-1/12)*(-12)"
	]
	for i in range(0, 2):
		print(expression1[i] + " = " + calculate(expression1[i]))

if __name__ == "__main__":
	test()
	while 1:
		expression = input("Please input the expression:")
		print("Result:" + calculate(expression) + "\n")