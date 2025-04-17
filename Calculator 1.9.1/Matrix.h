#ifndef MATRIX_H
#define MATRIX_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class Matrix {
	int rows, cols;
	vector<vector<double>> data;
	//映射
	static map<char, int>mop;
	static map<string, Matrix(*)(const Matrix&)>functionm;
	static map<string, Matrix(*)(const Matrix&, const Matrix&)>functionm2;
	static map<string, Matrix(*)(const Matrix&, const Matrix&, const Matrix&)>functionm3;
	static map<string, Matrix(*)(const Matrix&, const Matrix&, const Matrix&, const Matrix&)>functionm4;
public:
	Matrix(const int& rows, const int& cols, const vector<vector<double>>& data) : rows(rows), cols(cols), data(data) {};
	Matrix(const int& r, const int& c) : rows(r), cols(c), data(r, vector<double>(c, 0.0)) {};
	Matrix(const double& num = 0.0) :rows(1), cols(1), data(1, vector<double>(1, num)) {};
	Matrix(const Matrix& m) :rows(m.rows), cols(m.cols), data(m.data) {};

	//基本操作
	int getRows() const { return rows; }
	int getCols() const { return cols; }
	void set(const int& row, const int& col, const double& value);
	double get(const int& row, const int& col) const;

	void swap(const int& row1, const int& col1, const int& row2, const int& col2);
	bool isZeroRow(const int& row)const;
	bool isZeroCol(const int& col)const;
	bool isNumber()const;
	bool isInteger()const;
	void print() const;

	//初等变换
	void swapRows(const int& row1, const int& row2);
	void swapCols(const int& col1, const int& col2);
	void multiplyRows(const int& row, const double& scalar);
	void multiplyCols(const int& col, const double& scalar);
	void addRows(const int& row1, const int& row2, const double& scalar);
	void addCols(const int& col1, const int& col2, const double& scalar);

	//运算符重载
	Matrix operator+(const Matrix& other) const;
	Matrix operator-(const Matrix& other)const;
	Matrix operator-()const;
	Matrix operator*(const Matrix& other) const;
	Matrix operator*(const double& scalar) const;
	friend Matrix operator*(const double& scalar, const Matrix& rhs);
	Matrix operator/(const double& scalar) const;
	Matrix operator/(const Matrix& other)const;
	Matrix operator^(const int& n)const;
	Matrix operator^(const Matrix& n)const;
	bool operator==(const Matrix& other)const;
	friend ostream& operator<<(ostream& out, const Matrix& m);
	vector<double>& operator[](const int& row);
	const vector<double>& operator[](const int& row)const;
	double& operator()(const int& row, const int& col);
	const double& operator()(const int& row, const int& col)const;
	Matrix& operator=(const Matrix& other);
	Matrix& operator+=(const Matrix& other);
	Matrix& operator-=(const Matrix& other);
	Matrix& operator*=(const Matrix& other);
	Matrix& operator*=(const double& scalar);
	Matrix& operator/=(const Matrix& other);
	Matrix& operator/=(const double& scalar);
	Matrix& operator^=(const int& n);
	Matrix& operator^=(const Matrix& n);

	//高级操作
	double determinant() const;
	Matrix subMatrix(const int& excludeRow, const int& excludeCol) const;
	int rank() const;
	double trace()const;
	void LUdecomposition(map<string, Matrix>& matrices)const;
	void QRdecomposition(map<string, Matrix>& matrices)const;
	Matrix Hessenberg()const;
	bool upper()const;
	bool hasSet() const;
	bool NoImagEig()const;
	bool diagonalizable()const;
	void eigenvector()const;
	Matrix ev() const;
	Matrix integR(const Matrix& other)const;
	Matrix integC(const Matrix& other)const;
	Matrix integD(const Matrix& other)const;
	static Matrix identity(const int& n);
	double norm()const;

	//新版操作系统
	static void newInput(map<string, Matrix>& matrices);
	static Matrix parseFunctionm(const string& expr, map<string, Matrix>& matrices);
	static Matrix parseExpressionm(const string& expr, size_t& currentPos, const map<string, Matrix>& matrices);
	static Matrix parseTermm(const string& expr, size_t& currentPos, const map<string, Matrix>& matrices);
	static Matrix parseLDivisionm(const string& expr, size_t& currentPos, const map<string, Matrix>& matrices);
	static Matrix parsePowerm(const string& expr, size_t& currentPos, const map<string, Matrix>& matrices);
	static void newMatrix();
	static void storem(const map<string, Matrix>& matrices);
	static map<string, Matrix> loadm();
	static void deletem(map<string, Matrix>& variables);
	static void showm(const map<string, Matrix>& variables);

	//单参数函数
	static Matrix row(const Matrix& m);
	static Matrix col(const Matrix& m);
	static Matrix det(const Matrix& m);
	static Matrix r(const Matrix& m);
	static Matrix transpose(const Matrix& m);
	static Matrix inverse(const Matrix& m);
	static Matrix inverse1(const Matrix& m);
	static Matrix adjugate(const Matrix& m);
	static Matrix GaussianElimination(const Matrix& m);
	static Matrix SchmidtOrtho(const Matrix& m);
	static Matrix sinm(const Matrix& m);
	static Matrix cosm(const Matrix& m);
	static Matrix tanm(const Matrix& m);
	static Matrix sinh(const Matrix& m);
	static Matrix cosh(const Matrix& m);
	static Matrix tanh(const Matrix& m);
	static Matrix lnm(const Matrix& m);
	static Matrix sqrtm(const Matrix& m);
	static Matrix eigenvalue(const Matrix& m);
	static Matrix tr(const Matrix& m);
	static Matrix diagonalize(const Matrix& m);
	static Matrix pForDiag(const Matrix& m);
	static Matrix identity(const Matrix& m);
	static Matrix ones(const Matrix& m);
	static Matrix zero(const Matrix& m);
	static Matrix random(const Matrix& m);
	static Matrix randint(const Matrix& m);
	static Matrix magic(const Matrix& m);
	static Matrix sum(const Matrix& m);
	static Matrix product(const Matrix& m);
	static Matrix deg(const Matrix& rad);
	static Matrix rad(const Matrix& deg);
	static Matrix norm(const Matrix& m);
	static Matrix expm(const Matrix& m);

	//二参数函数
	static Matrix integR(const Matrix& m1, const Matrix& m2);
	static Matrix integC(const Matrix& m1, const Matrix& m2);
	static Matrix integD(const Matrix& m1, const Matrix& m2);
	static Matrix Ldivide(const Matrix& m1, const Matrix& m2);
	static Matrix ones(const Matrix& r, const Matrix& c);
	static Matrix zero(const Matrix& r, const Matrix& c);
	static Matrix random(const Matrix& r, const Matrix& c);
	static Matrix randint(const Matrix& r, const Matrix& c);
	static Matrix getRow(const Matrix& m, const Matrix& row);
	static Matrix getCol(const Matrix& m, const Matrix& col);
	static Matrix deleteRow(const Matrix& m, const Matrix& row);
	static Matrix deleteCol(const Matrix& m, const Matrix& col);

	//三参数函数
	static Matrix get(const Matrix& m, const Matrix& row, const Matrix& col);
	static Matrix swapRows(const Matrix& m, const Matrix& row1, const Matrix& row2);
	static Matrix swapCols(const Matrix& m, const Matrix& col1, const Matrix& col2);
	static Matrix multiplyRows(const Matrix& m, const Matrix& row, const Matrix& scalar);
	static Matrix multiplyCols(const Matrix& m, const Matrix& col, const Matrix& scalar);
	static Matrix subMatrix(const Matrix& m, const Matrix& excludeRow, const Matrix& excludeCol);
	static Matrix cofactor(const Matrix& m, const Matrix& row, const Matrix& col);
	static Matrix Acofactor(const Matrix& m, const Matrix& row, const Matrix& col);

	//四参数函数
	static Matrix addRows(const Matrix& m, const Matrix& row1, const Matrix& row2, const Matrix& scalar);
	static Matrix addCols(const Matrix& m, const Matrix& col1, const Matrix& col2, const Matrix& scalar);
	static Matrix set(const Matrix& m, const Matrix& row, const Matrix& col, const Matrix& num);
	static Matrix random(const Matrix& r, const Matrix& c, const Matrix& min, const Matrix& max);
	static Matrix randint(const Matrix& r, const Matrix& c, const Matrix& min, const Matrix& max);

	//线性方程组求解
	static void doLinearEquations();
	static void doSolution(const Matrix& coefficient, const Matrix& constant);
	static void doApproximate(const Matrix& coefficient, const Matrix& constant);
	static void doBasicSolutionSet(const Matrix& coefficient);
	static Matrix basicSolutionSet(const Matrix& coefficient);
};

#endif