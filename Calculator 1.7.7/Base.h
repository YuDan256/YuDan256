#ifndef BASE_H
#define BASE_H

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>

using namespace std;
typedef unsigned long long ull;

class Base {
private:
	ull base;
	ull data;
	static map<char, int>bop;
public:
	Base(const ull& _base, const ull& _data);
	Base(const ull& data = 0) :base(10), data(data) {};
	Base(const Base& b) :base(b.base), data(b.data) {};

	ull getBase()const { return base; }
	ull getData()const { return data; }
	ull length()const;
	void print()const;
	void setBase(const ull& b) { base = b; }
	void setData(const ull& d) { data = d; }

	//运算符重载
	Base operator+(const Base& b)const;
	Base operator-(const Base& b)const;
	Base operator*(const Base& b)const;
	Base operator/(const Base& b)const;
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

	//函数
	static Base powb(const Base& b1, const Base& b2);
	static void compareb(const Base& b1, const Base& b2);

	//输入进制数
	static Base enterBase(const ull& base, const string& num);
	static Base enterBase1(const ull& base, const string& num);
	static Base enterBase2(const ull& base, const string& num);
	static Base enterBase3(const ull& base, const string& num);

	//旧版操作系统
	static void doBase();
	static void doBase1(Base& b1, Base& b2);
	static void doBase2(const ull& base, Base& b1, Base& b2);

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