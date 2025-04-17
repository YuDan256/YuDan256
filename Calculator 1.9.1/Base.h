#ifndef BASE_H
#define BASE_H

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include "Integer.h"

using namespace std;

class Base {
private:
	Integer base;
	Integer data;
	static map<char, int>bop;
public:
	Base(const Integer& _base, const Integer& _data);
	Base(const Integer& data = 0) :base(10), data(data) {};
	Base(const Base& b) :base(b.base), data(b.data) {};

	Integer getBase()const { return base; }
	Integer getData()const { return data; }
	Integer length()const;
	void print()const;
	void setBase(const Integer& b) { base = b; }
	void setData(const Integer& d) { data = d; }

	//运算符重载
	Base operator+(const Base& b)const;
	Base operator-(const Base& b)const;
	Base operator*(const Base& b)const;
	Base operator/(const Base& b)const;
	Base operator%(const Base& b)const;
	bool operator>(const Base& b)const;
	bool operator<(const Base& b)const;
	bool operator==(const Base& b)const;
	bool operator!=(const Base& b)const;
	bool operator>=(const Base& b)const;
	bool operator<=(const Base& b)const;
	Base operator&(const Base& bin)const;
	Base operator|(const Base& bin)const;
	Base operator^(const Base& bin)const;
	Base operator~() const;

	friend ostream& operator<<(ostream& out, const Base& b);

	Base& operator=(const Base& b);
	Base& operator+=(const Base& b);
	Base& operator-=(const Base& b);
	Base& operator*=(const Base& b);
	Base& operator/=(const Base& b);
	Base& operator%=(const Base& b);
	Base& operator&=(const Base& bin);
	Base& operator|=(const Base& bin);
	Base& operator^=(const Base& bin);
	Base& operator++();
	Base& operator--();

	//函数
	static Base powb(const Base& b1, const Base& b2);
	static void compareb(const Base& b1, const Base& b2);
	static void factorization(const Base& b);

	//输入进制数
	static Base enterBase(const Integer& base, const string& num);
	static Base enterBase1(const Integer& base, const string& num);
	static Base enterBase2(const Integer& base, const string& num);
	static Base enterBase3(const Integer& base, const string& num);

	//新版操作系统
	static void newInput(map<string, Base>& baseNumbers);
	static void newBase();
	static Base parseFunctionb(const string& expr, const map<string, Base>& baseNumbers);
	static Base parseExpressionb(const string& expr, size_t& currentPos, const map<string, Base>& baseNumbers);
	static Base parseAdditionb(const string& expr, size_t& currentPos, const map<string, Base>& baseNumbers);
	static Base parseTermb(const string& expr, size_t& currentPos, const map<string, Base>& baseNumbers);
	static Base parsePowerb(const string& expr, size_t& currentPos, const map<string, Base>& baseNumbers);
	static void storeb(const map<string, Base>& baseNumbers);
	static map<string, Base> loadb();
	static void deleteb(map<string, Base>& variables);
	static void showb(const map<string, Base>& variables);
};

#endif 