#ifndef STATISTICS_H
#define STATISTICS_H

#include<iostream>
#include<cmath>
#include<vector>
#include<stdexcept>
#include<map>
#include <string>

using namespace std;

class Statistics {
private:
	vector<double>data;
	//映射
	static map<string, double(*)(const Statistics&)>functions1;
	static map<string, double(*)(const Statistics&, const Statistics&)>functions2;
public:
	Statistics(const vector<double>& data):data(data){}
	Statistics():data(vector<double>(2,0.0)){}
	Statistics(const Statistics& s):data(s.data){}

	//数据组操作
	void set(const int& pos, const double& value);
	void swap(const int& p1, const int& p2);
	void push_back(const double& value);
	void pop_back();
	double get(const int& pos)const;
	void print()const;
	Statistics sort() const;
	void insert(const int& pos, const double& value);
	void erase(const int& pos);

	//单变量统计
	static int n(const Statistics&s);
	static double n1(const Statistics& s);
	static double sum(const Statistics& s);
	static double m(const Statistics& s);
	static double Var(const Statistics& s);
	static double sd(const Statistics& s);
	static double sampleVar(const Statistics& s);
	static double sampleSd(const Statistics& s);
	static double Max(const Statistics& s);
	static double Min(const Statistics& s);
	static double range(const Statistics& s);
	static double Q1(const Statistics& s);
	static double M(const Statistics& s);
	static double Q3(const Statistics& s);
	static double percentile(const Statistics& s,const int& p);
	static void Mo(const Statistics& s);
	static void linearRegression(const Statistics& s);

	//双变量统计
	static double Cov(const Statistics& s1,const Statistics& s2);
	static double r(const Statistics& s1, const Statistics& s2);
	static double R2(const Statistics& s1, const Statistics& s2);
	static void linearRegression(const Statistics& s1, const Statistics& s2);

	//新版统计操作
	static void newInput(map<string, Statistics>& datasets);
	static void newStatistics();
	static void parseFunctions(const string&expr, map<string,Statistics>& datasets);
	static void newSingle(const Statistics & X);
	static void newBivariate(const Statistics & _X, const Statistics & _Y);

	friend ostream& operator<<(ostream& out, const Statistics& s);
	static void stores(const map<string,Statistics>& datasets);
	static map<string, Statistics> loads();
	static void deletes(map<string, Statistics>& variables);
	static void shows(const map<string,Statistics>& variables);
};

#endif