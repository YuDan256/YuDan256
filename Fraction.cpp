#include "Fraction.h"

Fraction::Fraction(const Integer& n, const Integer& d) {
	if (d == 0)throw invalid_argument("The denominator cannot be 0.");
	Integer c = Integer::gcd(n, d);
	Integer _num = n / c;
	Integer _den = d / c;
	if (_num.getSign() == _den.getSign()) {
		num = Integer::fabs(_num);
		den = Integer::fabs(_den);
	}
	else {
		num = -Integer::fabs(_num);
		den = Integer::fabs(_den);
	}
}



void Fraction::reduction() {
	if (den == 0)throw invalid_argument("The denominator cannot be 0.");
	Integer c = Integer::gcd(num, den);
	num /= c;
	den /= c;
	if (num.getSign() == den.getSign()) {
		num = Integer::fabs(num);
		den = Integer::fabs(den);
	}
	else {
		num = -Integer::fabs(num);
		den = Integer::fabs(den);
	}
}

Fraction Fraction::reciprocal() const {
	return Fraction(den, num);
}

void Fraction::print(const string& end) const {
	if (den == 1)cout << num << end;
	else cout << num << "/" << den << end;
}

double Fraction::value() const {
	if (den == 0)throw invalid_argument("The denominator cannot be 0.");
	if (den == 1)return static_cast<double>(num.value());
	if (num == 0)return 0.0;
	return static_cast<double>(num.value()) / static_cast<double>(den.value());
}

Fraction Fraction::operator+(const Fraction& f) const {
	Integer l = Integer::lcm(den, f.den);
	Integer n = num * l / den + f.num * l / f.den;
	return Fraction(n, l);
}

Fraction Fraction::operator-(const Fraction& f) const {
	Integer l = Integer::lcm(den, f.den);
	Integer n = num * l / den + f.num * l / f.den;
	return Fraction(n, l);
}

Fraction Fraction::operator*(const Fraction& f) const {
	Integer n1 = Integer::gcd(num, f.den);
	Integer n2 = Integer::gcd(f.num, den);
	Integer _num = (num / n1) * (f.num / n2);
	Integer _den = (den / n2) * (f.den / n1);
	return Fraction(_num, _den);
}

Fraction Fraction::operator/(const Fraction& f)const {
	return (*this) * f.reciprocal();
}

Fraction Fraction::operator+() const {
	return (*this);
}

Fraction Fraction::operator-() const {
	return Fraction(-num, den);
}

bool Fraction::operator==(const Fraction& f) const {
	return (num == f.num && den == f.den);
}

bool Fraction::operator!=(const Fraction& f) const {
	return !((*this) == f);
}

bool Fraction::operator<(const Fraction& f) const {
	return (((*this) - f).num < 0);
}

bool Fraction::operator<=(const Fraction& f) const {
	return (((*this) - f).num <= 0);
}

bool Fraction::operator>(const Fraction& f) const {
	return (((*this) - f).num > 0);
}

bool Fraction::operator>=(const Fraction& f) const {
	return (((*this) - f).num >= 0);
}

Fraction& Fraction::operator=(const Fraction& f) {
	num = f.num;
	den = f.den;
	return *this;
}

Fraction& Fraction::operator+=(const Fraction& f) {
	Integer l = Integer::lcm(den, f.den);
	Integer n = num * l / den + f.num * l / f.den;
	num = n;
	den = l;
	reduction();
	return *this;
}

Fraction& Fraction::operator-=(const Fraction& f) {
	Integer l = Integer::lcm(den, f.den);
	Integer n = num * l / den - f.num * l / f.den;
	num = n;
	den = l;
	reduction();
	return *this;
}

Fraction& Fraction::operator*=(const Fraction& f) {
	Integer n1 = Integer::gcd(num, f.den);
	Integer n2 = Integer::gcd(f.num, den);
	Integer _num = (num / n1) * (f.num / n2);
	Integer _den = (den / n2) * (f.den / n1);
	num = _num;
	den = _den;
	return *this;
}

Fraction& Fraction::operator/=(const Fraction& f) {
	(*this) *= f.reciprocal();
	return *this;
}

ostream& operator<<(ostream& out, const Fraction& f) {
	out << f.num << " " << f.den;
	return out;
}

istream& operator>>(istream& in, Fraction& f) {
	in >> f.num >> f.den;
	f.reduction();
	return in;
}
