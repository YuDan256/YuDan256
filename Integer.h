#ifndef INTEGER_H
#define INTEGER_H

#include <vector>
#include <string>
#include <iostream>
#include <type_traits>

using namespace std;
typedef unsigned long long ull;

class Integer {
private:
	vector<int> data;
	bool sign;
public:
	Integer(const vector<int>& data, const bool& sign) : data(data), sign(sign) {};
	Integer() : data({ '0' }), sign(true) {};
	Integer(const char* num);
	Integer(const string& num);
	Integer(const ull& num);

	friend ostream& operator<<(ostream& out, const Integer& num);
	friend istream& operator>>(istream& in, Integer& num);

	bool operator==(const Integer& n) const;
	bool operator>(const Integer& n) const;
	bool operator<(const Integer& n) const;
	bool operator>=(const Integer& n) const;
	bool operator<=(const Integer& n) const;
	bool operator!=(const Integer& n) const;

	Integer operator+(const Integer& n) const;
	Integer operator-(const Integer& n) const;
	Integer operator*(const Integer& n) const;
	Integer operator/(const Integer& n) const;
	Integer operator%(const Integer& n) const;

	Integer& operator+=(const Integer& n);
	Integer& operator-=(const Integer& n);
	Integer& operator*=(const Integer& n);
	Integer& operator/=(const Integer& n);
	Integer& operator%=(const Integer& n);
	Integer& operator++();
	Integer& operator--();
	Integer operator++(int);
	Integer operator--(int);

	Integer operator-()const;
	Integer operator+()const;

	Integer fabs(const Integer& n)const;
};

#endif