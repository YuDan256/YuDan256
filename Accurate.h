#ifndef ACCURATE_H
#define ACCURATE_H

#include <vector>
#include <string>
#include <iostream>
#include <type_traits>

using namespace std;
typedef unsigned long long ull;



class Integer {
private:
	vector<int>data;
public:
	Integer(const vector<int>& data) :data(data) {};
	Integer() :data({ '0' }) {};
	Integer(const char* num);
	Integer(const string& num);
	Integer(const ull& num);

	friend ostream& operator<<(ostream& out, const Integer& num);
	bool operator==(const Integer& n)const;
	bool operator>(const Integer& n)const;
	bool operator<(const Integer& n)const;
	bool operator>=(const Integer& n)const;
	bool operator<=(const Integer& n)const;
	bool operator!=(const Integer& n)const;

	Integer operator+(const Integer& n)const;
	Integer operator-(const Integer& n)const;
	Integer operator*(const Integer& n)const;
	Integer operator/(const Integer& n)const;
	Integer operator%(const Integer& n)const;
};

#endif