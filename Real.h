#ifndef REAL_H
#define REAL_H

#include "Normal.h"
#include <string>
using namespace std;

class Real {
private:
	double value;
	static map<char, int>rop;
	static map<string, Real(*)(const Real&)>functionr1;
public:
	Real(const double& value = 0) :value(value) {}
	Real(const Real& r) :value(r.value) {}

	//‘ÀÀ„∑˚÷ÿ‘ÿ
	Real operator+(const Real& r);
	Real operator-(const Real& r);
	Real operator*(const Real& r);
	Real operator/(const Real& r);
	Real operator^(const Real& r);

	friend Real operator+(const double& p, const Real& r);
	friend Real operator-(const double& p, const Real& r);
	friend Real operator*(const double& p, const Real& r);
	friend Real operator/(const double& p, const Real& r);
	friend Real operator^(const double& p, const Real& r);

	Real& operator=(const Real& r);
	Real& operator+=(const Real& r);
	Real& operator-=(const Real& r);
	Real& operator*=(const Real& r);
	Real& operator/=(const Real& r);
	Real& operator^=(const Real& r);
	Real& operator++();
	Real& operator--();
	Real operator++(int);
	Real operator--(int);

	bool operator==(const Real& r) const;
	bool operator!=(const Real& r) const;
	bool operator<(const Real& r) const;
	bool operator<=(const Real& r) const;
	bool operator>(const Real& r) const;
	bool operator>=(const Real& r) const;

	friend ostream& operator<<(ostream& out, const Real& r);
	friend istream& operator>>(istream& in, Real& r);

	double getValue() const;
	void print(const string& end = "\n") const;

	static Real parseFunctionr(const string& expr, const map<string, Real>& variables);
	static Real parseExpressionr(const string& expr, size_t& currentPos, const map<string, Real>& variables);
	static Real parseTermr(const string& expr, size_t& currentPos, const map<string, Real>& variables);
	static Real parsePowerr(const string& expr, size_t& currentPos, const map<string, Real>& variables);

	static void input(map<string, Real>& variables);
	static Real stor(const string& expr);
	static bool isValidName(const string& name);
	static map<string, Real> loadr();
	static void storer(const map<string, Real>& variables);
	static void deleter(map<string, Real>& variables);
	static void showr(const map<string, Real>& variables);
	static void processr(const string& expression, map<string, Real>& variables);
	static void doReal();
};

#endif
