#ifndef FRACTION_H
#define FRACTION_H

#include "Integer.h"

class Fraction {
private:
	Integer num;
	Integer den;
public:
	Fraction() :num(0), den(1) {}
	Fraction(const Integer& n, const Integer& d);
	Fraction(const Integer& n) :num(n), den(1) {}
	INT_T Fraction(const T& n) : num(Integer(n)), den(1) {}
	Fraction(const Fraction& f) :num(f.num), den(f.den) {}
	Integer getNum() const { return num; }
	Integer getDen() const { return den; }
	void setNum(const Integer& n) { num = n; reduction(); }
	void setDen(const Integer& d) { den = d; reduction(); }
	void reduction();
	Fraction reciprocal()const;
	void print(const string& end = "\n") const;
	double value() const;
	Fraction fabs() const;
	Fraction pow(const Integer& n) const;
	Fraction floor() const;
	Fraction ceil() const;
	Fraction round() const;

	static Fraction stof(const string& expr);
	static string to_string(const Fraction& f);
	static Fraction random(const Fraction& f1, const Fraction& f2);

	Fraction operator+(const Fraction& f) const;
	Fraction operator-(const Fraction& f) const;
	Fraction operator*(const Fraction& f) const;
	Fraction operator/(const Fraction& f) const;
	Fraction operator^(const Fraction& f) const;
	Fraction operator+()const;
	Fraction operator-()const;

	bool operator==(const Fraction& f) const;
	bool operator!=(const Fraction& f) const;
	bool operator<(const Fraction& f) const;
	bool operator<=(const Fraction& f) const;
	bool operator>(const Fraction& f) const;
	bool operator>=(const Fraction& f) const;

	Fraction& operator=(const Fraction& f);
	Fraction& operator+=(const Fraction& f);
	Fraction& operator-=(const Fraction& f);
	Fraction& operator*=(const Fraction& f);
	Fraction& operator/=(const Fraction& f);
	Fraction& operator^=(const Fraction& f);

	friend ostream& operator<<(ostream& out, const Fraction& f);
	friend istream& operator>>(istream& in, Fraction& f);

	friend Fraction operator+(const Integer& n, const Fraction& f) { return Fraction(n) + f; }
	friend Fraction operator-(const Integer& n, const Fraction& f) { return Fraction(n) - f; }
	friend Fraction operator*(const Integer& n, const Fraction& f) { return Fraction(n) * f; }
	friend Fraction operator/(const Integer& n, const Fraction& f) { return Fraction(n) / f; }
	INT_T friend Fraction operator+(const T& n, const Fraction& f) { return f + n; }
	INT_T friend Fraction operator-(const T& n, const Fraction& f) { return -f + n; }
	INT_T friend Fraction operator*(const T& n, const Fraction& f) { return f * n; }
	INT_T friend Fraction operator/(const T& n, const Fraction& f) { return f.reciprocal() * n; }
};

#endif