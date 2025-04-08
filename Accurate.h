#ifndef ACCURATE_H
#define ACCURATE_H

#include <vector>
#include <string>
#include <iostream>
#include <type_traits>

using namespace std;
typedef unsigned long long ull;



class Interger {
private:
	vector<int>data;
public:
	Interger(const vector<int>& data) :data(data) {};
	Interger() :data({ '0' }) {};
	Interger(const ull& num);

	friend ostream& operator<<(ostream& out, const Interger& num);
	bool operator==(const Interger& n)const;
	bool operator>(const Interger& n)const;
	bool operator<(const Interger& n)const;
	bool operator>=(const Interger& n)const;
	bool operator<=(const Interger& n)const;
	bool operator!=(const Interger& n)const;

	Interger operator+(const Interger& n)const;
	Interger operator-(const Interger& n)const;
	Interger operator*(const Interger& n)const;
	Interger operator/(const Interger& n)const;
};

#endif