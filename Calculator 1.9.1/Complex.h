#ifndef COMPLEX_H
#define COMPLEX_H

#include<iostream>
#include<cmath>
#include<vector>
#include<stdexcept>
#include<map>
#include<string>

using namespace std;

class Complex {

	double real, image;
	double modulus = sqrt(real * real + image * image);

	//映射
	static map<char, int> zop;
	static map<string, Complex(*)(const Complex&)> functionc;

	static const double PI, E;
public:
	//构造函数
	Complex(const double& r = 0, const double& i = 0) :real(r), image(i) {};
	Complex(const Complex& other) :real(other.real), image(other.image) {};

	//式外函数
	void print()const;
	void triangle()const;
	void root(const int& n)const;
	double getReal() const { return real; }
	double getImage() const { return image; }
	double getModulus() const { return modulus; }
	Complex powc(const int& n)const;
	Complex firstRoot(const int& n)const;
	double argument()const;

	//运算符重载
	Complex operator+(const Complex& other)const;
	Complex operator+(const double& p)const;
	friend Complex operator+(const double& p, const Complex& other);
	Complex operator-(const Complex& other)const;
	Complex operator-(const double& p)const;
	Complex operator-()const;
	friend Complex operator-(const double& p, const Complex& other);
	Complex operator*(const Complex& other)const;
	Complex operator*(const double& p)const;
	friend Complex operator*(const double& p, const Complex& other);
	Complex operator/(const Complex& other)const;
	Complex operator/(const double& p)const;
	friend Complex operator/(const double& p, const Complex& other);
	Complex operator^(const Complex& other)const;
	Complex operator^(const double& p)const;
	friend Complex operator^(const double& p, const Complex& other);
	bool operator==(const Complex& other)const;
	friend ostream& operator<<(ostream& out, const Complex& c);

	Complex& operator=(const Complex& other);
	Complex& operator+=(const Complex& other);
	Complex& operator+=(const double& p);
	Complex& operator-=(const Complex& other);
	Complex& operator-=(const double& p);
	Complex& operator*=(const Complex& other);
	Complex& operator*=(const double& p);
	Complex& operator/=(const Complex& other);
	Complex& operator/=(const double& p);
	Complex& operator^=(const Complex& other);
	Complex& operator^=(const double& p);

	//式中函数
	static Complex conjugate(const Complex& z);
	static Complex Re(const Complex& z);
	static Complex Im(const Complex& z);
	static Complex R(const Complex& z);
	static Complex arg(const Complex& z);
	static Complex sqrtc(const Complex& z);
	static Complex expc(const Complex& z);
	static Complex lnc(const Complex& z);
	static Complex sinc(const Complex& z);
	static Complex cosc(const Complex& z);
	static Complex tanc(const Complex& z);
	static Complex sinhc(const Complex& z);
	static Complex coshc(const Complex& z);
	static Complex tanhc(const Complex& z);
	static Complex asinc(const Complex& z);
	static Complex acosc(const Complex& z);
	static Complex atanc(const Complex& z);
	static Complex deg(const Complex& rad);
	static Complex rad(const Complex& deg);

	//新版操作系统
	static void newInput(map<string, Complex>& numbers);
	static Complex parseFunctionc(const string& expr, const map<string, Complex>& numbers);
	static Complex parseExpressionc(const string& expr, size_t& currentPos, const map<string, Complex>& numbers);
	static Complex parseTermc(const string& expr, size_t& currentPos, const map<string, Complex>& numbers);
	static Complex parsePowerc(const string& expr, size_t& currentPos, const map<string, Complex>& numbers);
	static void newComplex();
	static void storec(const map<string, Complex>& complexes);
	static map<string, Complex> loadc();
	static void deletec(map<string, Complex>& variables);
	static void showc(const map<string, Complex>& variables);

	//多项式方程
	static void doPolynomial();
	static void doDegreeOne(const Complex& a, const Complex& b);
	static void doDegreeTwo(const Complex& a, const Complex& b, const Complex& c);
	static void doDegreeThree(const Complex& a, const Complex& b, const Complex& c, const Complex& d);
	static void doDegreeFour(const Complex& a, const Complex& b, const Complex& c, const Complex& d, const Complex& e);
};

#endif