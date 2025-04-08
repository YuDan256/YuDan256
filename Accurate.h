#ifndef ACCURATE_H
#define ACCURATE_H

#include <vector>
#include <string>
#include <iostream>
#include <type_traits>

using namespace std;
typedef unsigned long long ull;



class Accurate {
private:
	vector<char>data;
public:
	Accurate(const vector<char>& data) :data(data) {};
	Accurate() :data({ '0' }) {};
	Accurate(const ull& num);

	friend ostream& operator<<(ostream& out, const Accurate& num);
	Accurate operator+(const Accurate& n)const;
};

#endif