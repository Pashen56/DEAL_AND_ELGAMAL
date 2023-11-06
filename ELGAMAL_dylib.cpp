#include <iostream>
#include "stdlib.h"
#include "time.h"
#include <string>
#include <fstream>
#include <stdio.h>
#include <string.h>

#define BASE 1000000000

extern "C" struct bigInt
{
	long int sizeNum;
	long int sign;
	unsigned int* digits;
};

extern "C" typedef struct bigInt bigInt;
extern "C" bigInt getFrombinarFile(const char* filename);
extern "C" int saveTobinarFile(const char* filename, bigInt A);
extern "C" bigInt ReadFromTextFile(const char* fileName);
extern "C" int WriteToTFile(const char* fileName, bigInt A);
extern "C" struct bigInt createFromString(const char* string);
extern "C" bigInt createFromInt(long long int value);
extern "C" char* getString(bigInt a);
extern "C" bigInt copy(bigInt from);
extern "C" bigInt DelZeros(bigInt a);
extern "C" long long int compare(bigInt A, bigInt B);
extern "C" bigInt sumAndSub(bigInt left, bigInt right);
extern "C" bigInt sum(bigInt A, bigInt B);
extern "C" bigInt sub(bigInt A, bigInt B);
extern "C" bigInt minus(bigInt a);
extern "C" bigInt mul(bigInt A, bigInt B);
extern "C" bigInt dividing(bigInt A, bigInt B, bigInt * ost);
extern "C" bigInt shiftLeft(bigInt a, int s);
extern "C" bigInt myPow(bigInt base, bigInt exp);
extern "C" int odd(bigInt a);
extern "C" bigInt powMod(bigInt base, bigInt exp, bigInt modul);
extern "C" bigInt GenRandomByLen(int bitlen);
extern "C" bigInt GenRandomByRange(bigInt max);
extern "C" bigInt GenRandomOddByLen(int bitlen);

bigInt getFrombinarFile(const char* filename)
{
	FILE* pfSource = fopen(filename, "r+b");
	if (!pfSource)
	{
		bigInt res;
		res.sizeNum = 0;
		res.sign = 0;
		res.digits = NULL;
		return res;
	}

	fseek(pfSource, 0, SEEK_END);
	long int filesizeNum = ftell(pfSource);
	fseek(pfSource, 0, SEEK_SET);

	unsigned char* fileContent = (unsigned char*)malloc(filesizeNum);
	fread((char*)fileContent, sizeof(unsigned char), filesizeNum, pfSource);
	fclose(pfSource);

	bigInt pow256 = createFromInt(1);
	bigInt res = createFromInt(0);
	bigInt b256 = createFromInt(256);

	int i;
	for (i = 0; i < filesizeNum; i++)
	{
		bigInt fi = createFromInt(fileContent[i]);

		bigInt tmp = mul(fi, pow256);
		free(fi.digits);

		bigInt tmp2 = sumAndSub(res, tmp);
		free(tmp.digits);
		free(res.digits);
		res = tmp2;

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
	FILE* pfDestination = fopen(filename, "w+b");
	if (!pfDestination)
		return 1;

	struct bigInt tmp = copy(A);
	tmp.sign = 0;
	bigInt zero = createFromInt(0);
	bigInt b256 = createFromInt(256);

	while (compare(tmp, zero) != 0)
	{
		bigInt ost;

		bigInt tmp2 = dividing(tmp, b256, &ost);
		free(tmp.digits);
		tmp = tmp2;

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

	fseek(fp, 0, SEEK_END);
	long int filesizeNum = ftell(fp);
	fseek(fp, 0, SEEK_SET);

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
	struct bigInt res;
	res.sizeNum = ((strsizeNum + strSign + 8) / 9);
	res.sign = strSign;
	res.digits = (unsigned int*)malloc((res.sizeNum) * sizeof(unsigned int));
	int i;
	for (i = 0; i < (strsizeNum + strSign) / 9; i++)
	{
		pStr -= 9;
		char splStr[10];
		memcpy(splStr, pStr, 9);
		splStr[9] = '\0';
		unsigned long int digitI = atol(splStr);
		res.digits[i] = digitI;
	}
	char ost[10];
	memset(ost, 0, 10);
	memcpy(ost, string + strSign, pStr - string - strSign);
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
		char splStr[10];
		sprintf(splStr, "%09u", a.digits[i]);
		pString -= 9;
		memcpy(pString, splStr, 9);
	}
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
	if (A.sign == B.sign)
		return sum(A, B);
	else
		return sub(A, B);
}

bigInt sum(bigInt A, bigInt B)
{
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
{
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
{
	bigInt res = copy(a);
	res.sign = ~a.sign;
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
		long long int qGuessMax = BASE;
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
{
	unsigned int* newDig = (unsigned int*)malloc((a.sizeNum + s) * sizeof(unsigned int));
	memset(newDig, 0, s * sizeof(unsigned int));
	memcpy(newDig + s, a.digits, a.sizeNum * sizeof(unsigned int));
	free(a.digits);
	a.digits = newDig;
	a.sizeNum += s;
	return a;
}

bigInt myPow(bigInt base, bigInt exp)
{
	bigInt res = createFromInt(1);
	bigInt zero = createFromInt(0);
	bigInt i = copy(exp);
	bigInt minusOne = createFromInt(-1);


	while (compare(i, zero) > 0)
	{
		bigInt tmp = mul(res, base);
		free(res.digits);
		res = tmp;

		tmp = sumAndSub(i, minusOne);
		free(i.digits);
		i = tmp;
	}
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
	bigInt res = createFromInt(1);
	bigInt zero = createFromInt(0);
	bigInt two = createFromInt(2);
	bigInt b = copy(exp);
	bigInt a = copy(base);

	while (compare(b, zero) > 0)
	{
		if (odd(b))
		{
			bigInt tmp = mul(res, a);

			bigInt modRem;
			bigInt tmp2 = dividing(tmp, modul, &modRem);
			free(tmp2.digits);
			free(tmp.digits);
			free(res.digits);
			res = modRem;
		}
		bigInt tmp = mul(a, a);
		free(a.digits);

		bigInt modRem;
		bigInt tmp2 = dividing(tmp, modul, &modRem);
		free(tmp2.digits);
		free(tmp.digits);
		a = modRem;

		tmp = dividing(b, two, &modRem);
		free(b.digits);
		free(modRem.digits);
		b = tmp;
	}
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

extern "C" class largeClass
{
	bigInt bigEx;

public:
	largeClass();
	~largeClass();
	largeClass(const largeClass & tmp);
	largeClass(const char* string);
	largeClass(long long int v);

	char* str();
	bool readFromTextFile(const char* filename);
	bool writeToTextFile(const char* filename);
	bool readFromBinFile(const char* filename);
	bool writeToBinFile(const char* filename);

	largeClass & operator=(const largeClass & rhv);
	largeClass operator+(const largeClass & right) const;
	largeClass operator-() const;
	largeClass operator-(const largeClass & right) const;
	largeClass operator*(const largeClass & right) const;
	largeClass operator/(const largeClass & right) const;
	largeClass operator%(const largeClass & right) const;
	largeClass operator^(const largeClass & right) const;

	bool operator>(const largeClass & tmp) const;
	bool operator<(const largeClass & tmp) const;
	bool operator!=(const largeClass & tmp) const;
	bool operator<=(const largeClass & tmp) const;
	bool operator>=(const largeClass & tmp) const;
	bool operator==(const largeClass & tmp) const;

	friend largeClass powModClass(largeClass & base, largeClass & exp, largeClass & mod);
	friend largeClass GenRandomByLenC(int bitlen);
	friend largeClass GenRandomByRangeC(largeClass & max);
	friend largeClass GenRandomOddByLenC(int bitlen);
};

extern "C" largeClass powModClass(largeClass & base, largeClass & exp, largeClass & mod);
extern "C" largeClass GenRandomByLenC(int bitlen);
extern "C" largeClass GenRandomByRangeC(largeClass & max);
extern "C" largeClass GenRandomOddByLenC(int bitlen);

extern "C" largeClass GenPrime(int bitlen);

largeClass::largeClass()
{
	this->bigEx = createFromInt(0);
}

largeClass::largeClass(const largeClass& tmp)
{
	this->bigEx = copy(tmp.bigEx);
}

largeClass::largeClass(const char* string)
{
	this->bigEx = createFromString(string);
}

largeClass::largeClass(long long int v)
{
	this->bigEx = createFromInt(v);
}
largeClass::~largeClass()
{
	if (this->bigEx.digits != NULL)
	{
		this->bigEx.digits = nullptr;
	}

}

largeClass& largeClass::operator=(const largeClass& tmp)
{
	if (this->bigEx.digits != NULL)
	{
		free(this->bigEx.digits);
	}
	this->bigEx = copy(tmp.bigEx);
	return *this;
}

char* largeClass::str()
{
	return getString(this->bigEx);
}

largeClass largeClass::operator+(const largeClass& right) const
{
	largeClass res;
	res.bigEx = sumAndSub(this->bigEx, right.bigEx);
	return res;
}

largeClass largeClass::operator-() const
{
	largeClass res;
	res.bigEx = minus(this->bigEx);
	return res;
}

largeClass largeClass::operator-(const largeClass& right) const
{
	return *this + (-right);
}

largeClass largeClass::operator*(const largeClass& right) const
{
	largeClass res;
	res.bigEx = mul(this->bigEx, right.bigEx);
	return res;
}

largeClass largeClass::operator/(const largeClass& right) const
{
	largeClass ost;
	largeClass res;
	res.bigEx = dividing(this->bigEx, right.bigEx, &ost.bigEx);
	return res;
}

largeClass largeClass::operator%(const largeClass& right) const
{
	largeClass ost;
	largeClass res;
	res.bigEx = dividing(this->bigEx, right.bigEx, &ost.bigEx);
	return ost;
}

largeClass largeClass::operator^(const largeClass& right) const
{
	largeClass res;
	res.bigEx = myPow(this->bigEx, right.bigEx);
	return res;
}

bool largeClass::readFromTextFile(const char* filename)
{
	largeClass res;
	res.bigEx = ReadFromTextFile(filename);
	if (res.bigEx.digits == NULL)
	{
		return false;
	}
	else
	{
		*this = res;
		return true;
	}
}

bool largeClass::readFromBinFile(const char* filename)
{
	largeClass res;
	res.bigEx = getFrombinarFile(filename);
	if (res.bigEx.digits == NULL)
	{
		return false;
	}
	else
	{
		*this = res;
		return true;
	}
}

bool largeClass::writeToTextFile(const char* filename)
{
	int res;
	res = WriteToTFile(filename, this->bigEx);
	if (res != 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool largeClass::writeToBinFile(const char* filename)
{
	return saveTobinarFile(filename, this->bigEx) == 0;
}

bool largeClass::operator>(const largeClass& tmp) const
{
	return compare(this->bigEx, tmp.bigEx) > 0;
}

bool largeClass::operator<(const largeClass& tmp) const
{
	return compare(this->bigEx, tmp.bigEx) < 0;
}

bool largeClass::operator!=(const largeClass& tmp) const
{
	return compare(this->bigEx, tmp.bigEx) != 0;
}

bool largeClass::operator>=(const largeClass& tmp) const
{
	return compare(this->bigEx, tmp.bigEx) >= 0;
}

bool largeClass::operator<=(const largeClass& tmp) const
{
	return compare(this->bigEx, tmp.bigEx) <= 0;
}

bool largeClass::operator==(const largeClass& tmp) const
{
	return compare(this->bigEx, tmp.bigEx) == 0;
}

largeClass powModClass(largeClass& base, largeClass& exp, largeClass& mod)
{
	largeClass res;
	res.bigEx = powMod(base.bigEx, exp.bigEx, mod.bigEx);
	return res;
}

largeClass GenRandomByLenC(int bitlen)
{
	largeClass res;
	res.bigEx = GenRandomByLen(bitlen);
	return res;
}

largeClass GenRandomByRangeC(largeClass& max)
{
	largeClass res;
	res.bigEx = GenRandomByRange(max.bigEx);
	return res;
}

largeClass GenRandomOddByLenC(int bitlen)
{
	largeClass res;
	res.bigEx = GenRandomOddByLen(bitlen);
	return res;
}

largeClass GenPrime(int bitlen)
{
	largeClass p = GenRandomOddByLenC(bitlen);
	bool isPrime = true;
	do
	{
		isPrime = true;
		for (int i = 0; i < 15; i++)
		{
			largeClass a = GenRandomByRangeC(p);
			largeClass p_1 = p - 1;
			if (powModClass(a, p_1, p) != 1)
			{
				isPrime = false;
				break;
			}
		}

		if (!isPrime)
			p = p + 2;
	} while (!isPrime);
	return p;
}

extern "C" void encrypt(largeClass m, largeClass p, largeClass g, largeClass y, largeClass & a, largeClass & b)
{
	largeClass pp = p - 2;
	largeClass k = GenRandomByRangeC(pp) + 1;
	a = powModClass(g, k, p);
	b = powModClass(y, k, p);
	b = (b * m) % p;
}

extern "C" largeClass decrypt(largeClass p, largeClass x, largeClass a, largeClass b)
{
	largeClass tmp = p - 1 - x;
	return (b * powModClass(a, tmp, p)) % p;
}


extern "C" void Encryption(char* filename, char* directory, int bitlen)
{
	std::cout << "Filename: " << filename << std::endl;
	std::cout << "Directory: " << directory << std::endl;
	std::string f = filename;
	std::cout << "f = " << f << std::endl;
	std::string filename2 = f.substr(f.find_last_of('/') + 1, f.length() - f.find_last_of('/'));
	std::cout << "Filename2 = " << filename2 << std::endl;

	largeClass p = GenPrime(bitlen);
	largeClass g = GenRandomByRangeC(p);
	largeClass x = GenRandomByRangeC(p);
	largeClass y = powModClass(g, x, p);

	largeClass m, a, b;
	m.readFromBinFile(filename);
	encrypt(m, p, g, y, a, b);

	std::string encname = "/Encrypted_ElGamal_" + filename2;
	std::string decname = "/Decrypted_ElGamal_" + filename2;

	std::ofstream ofst(directory + encname);
	ofst << a.str() << std::endl << b.str();
	ofst.close();

	std::ifstream ifst(directory + encname);
	std::string a_str, b_str;
	getline(ifst, a_str);
	getline(ifst, b_str);

	a = largeClass(a_str.c_str());
	b = largeClass(b_str.c_str());
	m = decrypt(p, x, a, b);

	std::string decryptname = directory + decname;

	m.writeToBinFile(decryptname.c_str());

}
