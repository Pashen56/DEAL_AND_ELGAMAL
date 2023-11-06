#include "largeClass.h"
#include <iostream>


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
        this->bigEx.digits = nullptr; // == было
		//free(this->bigEx.digits);
	}

}

largeClass& largeClass::operator=(const largeClass& tmp)
{
	if (this->bigEx.digits != NULL)
	{
        //this->bigEx.digits = nullptr;
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
    // генерация простого числа на основе теста Ферма
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
