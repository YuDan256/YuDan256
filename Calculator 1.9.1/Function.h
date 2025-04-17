#ifndef FUNCTION_H
#define FUNCTION_H

#include<iostream>
#include<string>
#include<stdexcept>
#include<map>

using namespace std;

class Function {
private:
	string expr;
public:
	Function(const string& expr = "0") :expr(expr) {};
	Function(const Function& f) :expr(f.expr) {};

	Function& operator=(const Function& f);

	string getExpr()const { return expr; }
	double value(const double& x0)const;
	double differentiation(const double& x0)const;
	double integration(const double& a, const double& b)const;
	double solveEquation(const double& x0)const;
	void print()const;
	void doTable(const double& x0, const double& path, const int& n)const;
	double sum(const double& i, const double& j) const;

	//新版操作系统
	static void newInput(map<string, Function>& functions);
	static void newFunction();
	static void parseFunctionf(const string& expr, const map<string, Function>& functions);
	static void storef(const map<string, Function>& functions);
	static map<string, Function> loadf();
	static void deletef(map<string, Function>& variables);
	static void showf(const map<string, Function>& variables);

	friend ostream& operator<<(ostream& out, const Function& f);
};



#endif