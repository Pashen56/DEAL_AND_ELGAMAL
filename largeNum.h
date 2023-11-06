#ifndef _LARGENUM_H_
#define _LARGENUM_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE 1000000000

struct bigInt
{
    long int sizeNum; // размер
    long int sign; // знак
    unsigned int* digits; // цифры
};

typedef struct bigInt bigInt;//определение типа, чтобы не писать каждый раз struct bigInt
bigInt getFrombinarFile(const char* filename);
int saveTobinarFile(const char* filename, bigInt A);
bigInt ReadFromTextFile(const char* fileName);
int WriteToTFile(const char* fileName, bigInt A);
struct bigInt createFromString(const char* string);
bigInt createFromInt(long long int value);
char* getString(bigInt a);
bigInt copy(bigInt from);
bigInt DelZeros(bigInt a);
long long int compare(bigInt A, bigInt B);
bigInt sumAndSub(bigInt left, bigInt right);
bigInt sum(bigInt A, bigInt B);
bigInt sub(bigInt A, bigInt B);
bigInt minus(bigInt a);
bigInt mul(bigInt A, bigInt B);
bigInt dividing(bigInt A, bigInt B, bigInt* ost);
bigInt shiftLeft(bigInt a, int s);
bigInt myPow(bigInt base, bigInt exp);
int odd(bigInt a); // четно ли a
bigInt powMod(bigInt base, bigInt exp, bigInt modul);
bigInt GenRandomByLen(int bitlen);
bigInt GenRandomByRange(bigInt max);
bigInt GenRandomOddByLen(int bitlen);
#endif

/*
 // Шифрование: Выбирается сессионный ключ - случайное целое число k такое, что 1 < k < p - 1
 // Вычисляются числа a = g^k (mod p) и b = y^k * M (mod p)
 // Пара чисел (a, b) является шифротекстом.
 */
