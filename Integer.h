#ifndef INTEGER_H
#define INTEGER_H

#include <vector>
#include <string>
#include <iostream>

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

	template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
	Integer(const T& num) {
		ull n = 0;
		vector<int>result;
		if (num < 0) {
			sign = false;
			n = static_cast<ull>(-num);
		}
		else {
			sign = true;
			n = static_cast<ull>(num);
		}
		do {
			result.push_back(n % 10);
			n /= 10;
		} while (n > 0);
		data = result;
	};

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
