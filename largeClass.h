#pragma once
#ifndef _LARGECLASS_H_
#define _LARGECLASS_H_
#include "largeNum.h"


class largeClass
{
	bigInt bigEx;

public:
	largeClass();
	~largeClass();
	largeClass(const largeClass& tmp); // копирования
	largeClass(const char* string); // из строки
	largeClass(long long int v);

    // методы
	char* str();
	bool readFromTextFile(const char* filename);
	bool writeToTextFile(const char* filename);
	bool readFromBinFile(const char* filename);
	bool writeToBinFile(const char* filename);

	// операторы
	largeClass& operator=(const largeClass& rhv); // присваивания
	largeClass operator+(const largeClass& right) const;
	largeClass operator-() const; // унарный
	largeClass operator-(const largeClass& right) const;
	largeClass operator*(const largeClass& right) const;
	largeClass operator/(const largeClass& right) const;
	largeClass operator%(const largeClass& right) const;
	largeClass operator^(const largeClass& right) const;

	bool operator>(const largeClass& tmp) const;
	bool operator<(const largeClass& tmp) const;
	bool operator!=(const largeClass& tmp) const;
	bool operator<=(const largeClass& tmp) const;
	bool operator>=(const largeClass& tmp) const;
	bool operator==(const largeClass& tmp) const;

    // дружественная функция
	friend largeClass powModClass(largeClass& base, largeClass& exp, largeClass& mod);
	friend largeClass GenRandomByLenC(int bitlen);
	friend largeClass GenRandomByRangeC(largeClass& max);
	friend largeClass GenRandomOddByLenC(int bitlen);
};

largeClass powModClass(largeClass& base, largeClass& exp, largeClass& mod);
largeClass GenRandomByLenC(int bitlen);
largeClass GenRandomByRangeC(largeClass& max);
largeClass GenRandomOddByLenC(int bitlen);

largeClass GenPrime(int bitlen);
#endif
