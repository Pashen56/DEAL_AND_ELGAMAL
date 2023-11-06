#include "largeNum.h"
#include <stdlib.h>
#include <stdio.h>


bigInt getFrombinarFile(const char* filename)
{
    // будем считать, что в бинарном файле записаны разряды числа по модулю 256.
    // таким образом, чтобы считать число из бинарного файла необходимо каждый байт
    // умножить на 256 ^ i, где i - позиция байта в файле и всё это сложить
    FILE* pfSource = fopen(filename, "r+b");
	if (!pfSource)
	{
		bigInt res;
		res.sizeNum = 0;
		res.sign = 0;
		res.digits = NULL;
		return res;
	}

	// размер файла узнаем
	fseek(pfSource, 0, SEEK_END);
	long int filesizeNum = ftell(pfSource);
	fseek(pfSource, 0, SEEK_SET);

	// считываем содержимое
	unsigned char* fileContent = (unsigned char*)malloc(filesizeNum);
	fread((char*)fileContent, sizeof(unsigned char), filesizeNum, pfSource);
	fclose(pfSource);

	bigInt pow256 = createFromInt(1); // здесь будет 256 ^ i
	bigInt res = createFromInt(0); // здесь сформируем результат
	bigInt b256 = createFromInt(256); // число 256

    // цикл для всех байтов в файле
	int i;
	for (i = 0; i < filesizeNum; i++)
	{
		bigInt fi = createFromInt(fileContent[i]); // получили очередной байт файла

		// res = res + pow256 * fi;

		// tmp = fi * 256 ^ i
		bigInt tmp = mul(fi, pow256);
		free(fi.digits);

		// tmp2 = res + tmp = res + fi * 256^i
		bigInt tmp2 = sumAndSub(res, tmp);
		free(tmp.digits);
		free(res.digits);
		res = tmp2; // res = res + pow256 * fi;

		// pow256 = pow256*256
		tmp = mul(pow256, b256);
		free(pow256.digits);
		pow256 = tmp;
	}

	free(fileContent);
	free(pow256.digits);
	free(b256.digits);

	return res;
}
int saveTobinarFile(const char* filename, bigInt A)
{
    // будем считать, что в бинарный файл необходимо записать число по основанию 256
    // то есть необходимо перейти от BASE к 256 (256 - потому что максимальное значение байта - 255)
    // для этого надо находить остатки от деления на 256
    // пока число не уменьшиться до 0
    // возвращаем 0 в случае успешной записи
    // не 0 - в случае ошибки
	FILE* pfDestination = fopen(filename, "w+b");
	if (!pfDestination)
		return 1;

	struct bigInt tmp = copy(A); // число, которое будем делить на 256
	tmp.sign = 0; // не будем обращать внимания на знак
	bigInt zero = createFromInt(0);
	bigInt b256 = createFromInt(256);

    // пока число не станет равным 0
	while (compare(tmp, zero) != 0)
	{
        bigInt ost; // куда будет записан остаток от деления на 256

		// tmp = tmp / 256
		bigInt tmp2 = dividing(tmp, b256, &ost);
		free(tmp.digits);
		tmp = tmp2;

        // теперь в ost очередной остаток от деления на 256
        // запишем его в файл

		fwrite((char*)&ost.digits[0], sizeof(unsigned char), 1, pfDestination);
		free(ost.digits);
	}

	free(tmp.digits);
	free(b256.digits);
	free(zero.digits);

	fclose(pfDestination);
	return 0;
}
bigInt ReadFromTextFile(const char* fileName)
{
	FILE* fp = fopen(fileName, "r");
	if (!fp)
	{
		bigInt res;
		res.sizeNum = 0;
		res.sign = 0;
		res.digits = NULL;
		return res;
	}

    // узнаем размер файла
	fseek(fp, 0, SEEK_END);
	long int filesizeNum = ftell(fp);
	fseek(fp, 0, SEEK_SET);

    // считываем содержимое файла
	char* fileContent = (char*)malloc(filesizeNum + 1);
	fscanf(fp, "%s", fileContent);
	fileContent[filesizeNum] = '\0';
	fclose(fp);
	bigInt res = createFromString(fileContent);
	free(fileContent);
	return res;
}
int WriteToTFile(const char* fileName, bigInt A)
{
	FILE* fp = fopen(fileName, "w");
	if (!fp)
		return 1;

	char* res = getString(A);

	fprintf(fp, "%s", res);
	fclose(fp);
	free(res);

	return 0;
}
struct bigInt createFromString(const char* string)
{
	long int strsizeNum = strlen(string);
	int strSign = 0;
	if (string[0] == '-')
	{
		strsizeNum--;
		strSign = 1;
	}
	char* pStr = (char*)string + strsizeNum + strSign;
    // создаем экземпляр структуры, в которую запишем результат перевода
	struct bigInt res;
	res.sizeNum = ((strsizeNum + strSign + 8) / 9);
	res.sign = strSign;
	res.digits = (unsigned int*)malloc((res.sizeNum) * sizeof(unsigned int));
    // разбиваем строку на части по 9 символов
	int i;
	for (i = 0; i < (strsizeNum + strSign) / 9; i++)
	{
		pStr -= 9;
		char splStr[10];
		memcpy(splStr, pStr, 9);
		splStr[9] = '\0'; // получили очередную строку из 9 символов
        unsigned long int digitI = atol(splStr);
		res.digits[i] = digitI;
	}
    // обрабатываем оставшиеся символы (если длина строки не кратна 9)
	char ost[10];
	memset(ost, 0, 10);
    memcpy(ost, string + strSign, pStr - string - strSign); // получили строку - необработанная часть
	if (strlen(ost) > 0)
	{
		unsigned int lastDigit = atol(ost);
		res.digits[res.sizeNum - 1] = lastDigit;
	}
	res = DelZeros(res);
	return res;
}
bigInt createFromInt(long long int value)
{
	bigInt res;
    // выделяем память для 3-х элементов
	res.digits = (unsigned int*)malloc((3) * sizeof(unsigned int));
	res.sizeNum = 0;
	res.sign = 0;
	if (value < 0)
	{
		res.sign = 1;
		value = -value;
	}
	do
	{
		res.sizeNum++;
		res.digits[res.sizeNum - 1] = value % BASE;
		value = value / BASE;
	} while (value);
	return res;
}

char* getString(bigInt a)
{
	char* strBuffer = (char*)malloc((a.sizeNum * 9 + 1 + a.sign) * sizeof(char));
	char* pString = strBuffer + a.sizeNum * 9 + a.sign;
	*pString = '\0';
	int i;
	for (i = 0; i < a.sizeNum; i++)
	{
        // получаем строковое представление очередного разряда
		char splStr[10];
		sprintf(splStr, "%09u", a.digits[i]);
		pString -= 9;
		memcpy(pString, splStr, 9);
	}
    // удаление ведущих нулей
	while (*pString == '0' && *(pString + 1))
		pString++;
	if (a.sign)
	{
		pString--;
		*pString = '-';
	}
	char* string = (char*)malloc((strlen(pString) + 1) * sizeof(char));
	long int len = strlen(pString);
	string = new char[len + 1];
	strcpy(string, pString);
	free(strBuffer);
	return string;
}

bigInt copy(bigInt from)
{
	bigInt cpy;
	cpy.sizeNum = from.sizeNum;
	cpy.digits = (unsigned int*)malloc((cpy.sizeNum) * sizeof(unsigned int));
	cpy.sign = from.sign;
	memcpy(cpy.digits, from.digits, cpy.sizeNum * sizeof(unsigned int));
	return cpy;
}

bigInt DelZeros(bigInt a)
{
	while ((a.sizeNum - 1) && a.digits && a.digits[a.sizeNum - 1] == 0)
		a.sizeNum--;
	if (a.sizeNum == 1 && a.digits[0] == 0)
		a.sign = 0;
	return a;
}

long long int compare(bigInt A, bigInt B)
{
	int aSign = 1;
	if (A.sign == 1)
		aSign = -1;
	if (A.sign != B.sign)
		return aSign;
	if (A.sizeNum > B.sizeNum)
		return aSign;
	if (A.sizeNum < B.sizeNum)
		return -aSign;
	int i = A.sizeNum - 1;
	while (A.digits[i] == B.digits[i] && i > 0)
	{
		i--;
	}
	return ((long long int) A.digits[i] - (long long int)B.digits[i]) * aSign;
}

bigInt sumAndSub(bigInt left, bigInt right)
{
    // чсило в А будет не меньше по модулю, чем в B
	bigInt A = left, B = right;
	A.sign = 0;
	B.sign = 0;
	if (compare(A, B) > 0)
	{
		A.sign = left.sign;
		B.sign = right.sign;
	}
	else
	{
		A = right;
		B = left;
	}
    // если числа одного знака - складываем их (с выставлением нужного знака)
	if (A.sign == B.sign)
		return sum(A, B);
    else // иначе вычитаем
		return sub(A, B);
}

bigInt sum(bigInt A, bigInt B)
{// сложение двух чисел, причём A - не короче B
	bigInt res;
	res.sign = 0;
	res.sizeNum = A.sizeNum + 1;
	res.digits = (unsigned int*)malloc((A.sizeNum + 1) * sizeof(unsigned int));

	unsigned int carry = 0;
	int i;
	for (i = 0; i < A.sizeNum; i++)
	{
		unsigned int tmp = A.digits[i] + carry;
		if (i < B.sizeNum)
			tmp += B.digits[i];

		res.digits[i] = tmp % BASE;
		carry = tmp / BASE;
	}
	res.digits[A.sizeNum] = carry;
	res.sign = A.sign;
	res = DelZeros(res);
	return res;
}

bigInt sub(bigInt A, bigInt B)
{// вычитание двух чисел, причём A больше B по модулю
	bigInt res;
	res.sign = 0;
	res.sizeNum = A.sizeNum;
	res.digits = (unsigned int*)malloc((A.sizeNum) * sizeof(unsigned int));
	unsigned int carry = 0;
	int i;
	for (i = 0; i < A.sizeNum; i++)
	{
		int tmp = A.digits[i] - carry;
		if (i < B.sizeNum)
			tmp -= B.digits[i];
		carry = 0;
		if (tmp < 0)
		{
			carry = 1;
			tmp += BASE;
		}
		res.digits[i] = tmp;
	}
	res.sign = A.sign;
	res = DelZeros(res);
	return res;
}

bigInt minus(bigInt a)
{// делает копию числа a, но с противоположным знаком
	bigInt res = copy(a);
	res.sign = !a.sign;
	return res;
}

bigInt mul(bigInt A, bigInt B)
{
	bigInt res;
	res.sizeNum = A.sizeNum + B.sizeNum;
	res.digits = (unsigned int*)malloc((res.sizeNum) * sizeof(unsigned int));
	memset(res.digits, 0, res.sizeNum * sizeof(unsigned int));
	unsigned long long int carry = 0;
	int i;
	for (i = 0; i < B.sizeNum; i++)
	{
		carry = 0;
		int j;
		for (j = 0; j < A.sizeNum; j++)
		{
			unsigned long long int tmp = (unsigned long long int) B.digits[i] * (unsigned long long int) A.digits[j]
				+ carry + (unsigned long long int) res.digits[i + j];
			carry = tmp / BASE;
			res.digits[i + j] = tmp % BASE;
		}
		res.digits[i + A.sizeNum] = carry;
	}
	res.sign = (A.sign != B.sign);
	res = DelZeros(res);
	return res;
}
bigInt dividing(bigInt A, bigInt B, bigInt* ost)
{
	*ost = copy(A);
	ost->sign = 0;
	bigInt divider = copy(B);
	divider.sign = 0;
	if (1 == divider.sizeNum)
	{
		if (divider.digits[0] == 0)
		{
			ost->sizeNum = 0;
			ost->sign = 0;
			ost->digits = NULL;
			bigInt a;
			a.sizeNum = 0;
			a.sign = 0;
			a.digits = NULL;
			free(divider.digits);
			return a;
		}
	}
	if (compare(*ost, divider) < 0)
	{
		ost->sign = A.sign;
		bigInt res;
		res.sign = 0;
		res.sizeNum = 1;
		res.digits = (unsigned int*)malloc((res.sizeNum) * sizeof(unsigned int));
		res.digits[0] = 0;
		free(divider.digits);
		return res;
	}
	if (divider.sizeNum == 1)
	{
		bigInt res;
		res.sizeNum = A.sizeNum;
		res.digits = (unsigned int*)malloc((res.sizeNum) * sizeof(unsigned int));
		unsigned long long int carry = 0;
		int i;
		for (i = A.sizeNum - 1; i >= 0; i--)
		{
			unsigned long long int temp = carry;
			temp *= BASE;
			temp += A.digits[i];
			res.digits[i] = temp / divider.digits[0];
			carry = (unsigned long long int)  temp - (unsigned long long int) res.digits[i] * (unsigned long long int) divider.digits[0];
		}
		ost->sign = (A.sign != B.sign);
		if (ost->sizeNum > 0)
			free(ost->digits);
		ost->sizeNum = 1;
		ost->digits = (unsigned int*)malloc((1) * sizeof(unsigned int));
		ost->digits[0] = carry;
		*ost = DelZeros(*ost);
		res.sign = (A.sign != B.sign);
		res = DelZeros(res);
		free(divider.digits);
		return res;
	}
	bigInt res;
	res.sign = 0;
	res.sizeNum = A.sizeNum - B.sizeNum + 1;
	res.digits = (unsigned int*)malloc((res.sizeNum) * sizeof(unsigned int));

	int i;
	for (i = A.sizeNum - B.sizeNum + 1; i != 0; i--)
	{
        long long int qGuessMax = BASE; // для того, чтобы qGuessMin могло быть равно BASE - 1
		long long int qGuessMin = 0;
		long long int qGuess = qGuessMax;
		while (qGuessMax - qGuessMin > 1)
		{
			qGuess = (qGuessMax + qGuessMin) / 2;
			bigInt qGuesslargeNum = createFromInt(qGuess);
			bigInt tmp = mul(divider, qGuesslargeNum);
			free(qGuesslargeNum.digits);
			tmp = shiftLeft(tmp, i - 1);
			if (compare(tmp, *ost) > 0)
				qGuessMax = qGuess;
			else
				qGuessMin = qGuess;
			free(tmp.digits);
		}
		bigInt qGuessMinlargeNum = createFromInt(qGuessMin);
		bigInt tmp = mul(divider, qGuessMinlargeNum);
		free(qGuessMinlargeNum.digits);
		tmp = shiftLeft(tmp, i - 1);
		bigInt tmpRem = copy(*ost);
		bigInt minusTmp = minus(tmp);
		free(ost->digits);
		*ost = sumAndSub(tmpRem, minusTmp);
		free(tmpRem.digits);
		free(minusTmp.digits);
		free(tmp.digits);
		res.digits[i - 1] = qGuessMin;
	}
	res.sign = (A.sign != B.sign);
	ost->sign = (A.sign != B.sign);
	*ost = DelZeros(*ost);
	res = DelZeros(res);
	free(divider.digits);
	return res;
}

bigInt shiftLeft(bigInt a, int s)
{// сдвигает число a на s разрядов вправо
	unsigned int* newDig = (unsigned int*)malloc((a.sizeNum + s) * sizeof(unsigned int));
    // заполняем младшие разряды нулями
	memset(newDig, 0, s * sizeof(unsigned int));
    // копируем старшие разряды
	memcpy(newDig + s, a.digits, a.sizeNum * sizeof(unsigned int));
	free(a.digits);
	a.digits = newDig;
	a.sizeNum += s;
	return a;
}

bigInt myPow(bigInt base, bigInt exp)
{
    // возведение base в степень exp
    // такое имя потому, что есть стандартная функция с таким же именем,
    // а в си плоховато с перегрузкой функций
    bigInt res = createFromInt(1); // сюда будет записан результат
    bigInt zero = createFromInt(0);
    bigInt i = copy(exp); // сколько раз результат необходимо умножить на exp
    bigInt minusOne = createFromInt(-1); // -1


    // пока количество необходимых умножений больше нуля
	while (compare(i, zero) > 0)
	{
		// res = res * base
		bigInt tmp = mul(res, base);
		free(res.digits);
		res = tmp;

        // смысл следующих трёх строчек: i = i -1
        tmp = sumAndSub(i, minusOne); // складываем i и (-1), т.е. tmp = i - 1
        free(i.digits); // очищаем память
		i = tmp;
	}

    // очищаем память
	free(i.digits);
	free(zero.digits);
	free(minusOne.digits);

	return res;
}

int odd(bigInt a)
{
	if (a.sizeNum == 0)
		return 0;

	return a.digits[0] & 1;
}

bigInt powMod(bigInt base, bigInt exp, bigInt modul)
{
    // возведение base в степень exp по модулю modul
    // используется алгоритм быстрого возведения в степень
    bigInt res = createFromInt(1); // сюда будет записан результат
	bigInt zero = createFromInt(0);
	bigInt two = createFromInt(2);
	bigInt b = copy(exp);
	bigInt a = copy(base);

	while (compare(b, zero) > 0)
	{
		if (odd(b))
		{
			// res = (res * a) % modul;
			// tmp = res * base
			bigInt tmp = mul(res, a);

			// res = tmp % modul
            bigInt modRem; // сюда запишем остаток от деления
			bigInt tmp2 = dividing(tmp, modul, &modRem);
			free(tmp2.digits);
			free(tmp.digits);
			free(res.digits);
			res = modRem;
		}
		// a = (a*a) % modul
		bigInt tmp = mul(a, a);
		free(a.digits);

        bigInt modRem; // сюда запишем остаток от деления
		bigInt tmp2 = dividing(tmp, modul, &modRem);
		free(tmp2.digits);
		free(tmp.digits);
		a = modRem;

		// b = b / 2
		tmp = dividing(b, two, &modRem);
		free(b.digits);
		free(modRem.digits);
		b = tmp;
	}
    // очищаем память
	free(b.digits);
	free(zero.digits);
	free(two.digits);
	return res;
}

bigInt GenRandomByLen(int bitlen)
{
	int len = bitlen / (4 * 8);
	bigInt res;
	res.digits = (unsigned int*)malloc((len) * sizeof(unsigned int));
	res.sizeNum = len;
	res.sign = 0;

	int i = 0;
	for (i = 0; i < res.sizeNum; i++)
	{
		res.digits[i] = rand() % BASE;
	}
	return res;
}

bigInt GenRandomByRange(bigInt max)
{
	bigInt res;
	res.digits = (unsigned int*)malloc((max.sizeNum) * sizeof(unsigned int));
	res.sizeNum = max.sizeNum;
	res.sign = 0;

	int i = 0;
	for (i = 0; i < res.sizeNum - 1; i++)
	{
		res.digits[i] = rand() % BASE;
	}
	res.digits[res.sizeNum - 1] = rand() % max.digits[max.sizeNum - 1];
	return res;
}

bigInt GenRandomOddByLen(int bitlen)
{
	bigInt res = GenRandomByLen(bitlen);
	res.digits[0] |= 1;
	return res;
}
