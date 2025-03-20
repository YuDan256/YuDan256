#ifndef VECTOR_H
#define VECTOR_H

#include<iostream>
#include<vector>
#include<cmath>
#include<stdexcept>
#include<string>
#include<map>

using namespace std;

class Vector {
private:
	int n;
	vector<double>data;

	//映射
	static map<char, int>vop;
	static map<string, Vector(*)(const Vector&)>functionv1;
	static map<string, Vector(*)(const Vector&, const Vector&)>functionv2;

	static const double PI;
public:
	Vector(const int& n, const vector<double>& data) :n(n), data(data) {};
	Vector(const int& n) :n(n), data(n, 0.0) {};
	Vector() :n(1), data(1, 0.0) {};
	Vector(const Vector& v) :n(v.n), data(v.data) {};
	Vector(const double& value) :n(1), data(vector<double>(1, value)) {};

	void set(const int& place, const double& value) { data[place] = value; }
	double get(const int& place)const { return data[place]; }
	int getDimension()const { return n; }
	void print()const;

	//运算符重载
	Vector operator+(const Vector& other)const;
	Vector operator-(const Vector& other)const;
	Vector operator-()const;
	Vector operator*(const Vector& other)const;
	Vector operator*(const double& k)const;
	friend Vector operator* (const double& k, const Vector& other);
	Vector operator/(const double& k)const;
	Vector operator/(const Vector& other)const;
	Vector operator^(const Vector& other)const;
	bool operator==(const Vector& other)const;
	friend ostream& operator<<(ostream& out, const Vector& v);

	//向量运算
	double modulus()const;
	double dot(const Vector& other)const;
	double angle(const Vector& other)const;
	double mixedProduct(const Vector& v1, const Vector& v2)const;
	double scalarProjection(const Vector& other)const;
	Vector projectionVector(const Vector& other)const;
	bool vertical(const Vector& other)const;
	bool parallel(const Vector& other)const;

	//旧版操作系统
	static Vector input();
	static void doVector();

	//新版操作系统
	static void newInput(map<string, Vector>& vectors);
	static Vector parseFunctionv(const string& expr, const map<string, Vector>& vectors);
	static Vector parseExpressionv(const string& expr, size_t& currentPos, const map<string, Vector>& vectors);
	static Vector parseTermv(const string& expr, size_t& currentPos, const map<string, Vector>& vectors);
	static Vector parsePowerv(const string& expr, size_t& currentPos, const map<string, Vector>& vectors);
	static void newVector();
	static void storev(const map<string, Vector>& vectors);
	static map<string, Vector> loadv();
	static void deletev(map<string, Vector>& variables);
	static void showv(const map<string, Vector>& variables);

	//式内函数
	static Vector R(const Vector& v);
	static Vector sinv(const Vector& v);
	static Vector cosv(const Vector& v);
	static Vector tanv(const Vector& v);
	static Vector sqrtv(const Vector& v);
	static Vector lnv(const Vector& v);
	static Vector dotProduct(const Vector& v1, const Vector& v2);
	static Vector ang(const Vector& v1, const Vector& v2);
	static Vector SP(const Vector& v1, const Vector& v2);
	static Vector PV(const Vector& v1, const Vector& v2);
	static Vector deg(const Vector& rad);
	static Vector rad(const Vector& deg);
};

#endif