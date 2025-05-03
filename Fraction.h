#ifndef FRACTION_H
#define FRACTION_H

#include "Integer.h"

class Fraction {
private:
	Integer num;
	Integer den;
public:
	Fraction() :num(0), den(1) {};
	Fraction(const Integer& n, const Integer& d);
	Fraction(const Integer& n) :num(n), den(1) {};
	Fraction(const Fraction& f) :num(f.num), den(f.den) {}
	Integer getNum() const { return num; }
	Integer getDen() const { return den; }
	void setNum(const Integer& n) { num = n; reduction(); }
	void setDen(const Integer& d) { den = d; reduction(); }
	void reduction();
	Fraction reciprocal()const;
	void print(const string& end = "\n") const;

	Fraction operator+(const Fraction& f) const;
	Fraction operator-(const Fraction& f) const;
	Fraction operator*(const Fraction& f) const;
	Fraction operator/(const Fraction& f) const;
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

	friend ostream& operator<<(ostream& out, const Fraction& f) {
		out << f.num << " " << f.den;
		return out;
	}
};


#endif // !FRACTION_H

