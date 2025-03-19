#ifndef NORMAL_H
#define NORMAL_H

#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <stdexcept>

using namespace std;

class Normal {
private:
	static map<char, int> nop;
	static map<string, double(*)(double)> functionn;
public:
	static double parsen(const string& expr, const map<string, double>& variables);
	static double parseExpressionn(const string& expr, size_t& currentPos, const map<string, double>& variables);
	static double parseTermn(const string& expr, size_t& currentPos, const map<string, double>& variables);
	static double parsePowern(const string& expr, size_t& currentPos, const map<string, double>& variables);
	static void input(map<string, double>& variables);
	static void newNormal();
	static void doNormal();
	static void storen(const map<string, double>& numbers);
	static map<string, double>loadn();
	static void deleten(map<string, double>& variables);
	static void shown(const map<string, double>& variables);

	static double deg(double rad);
	static double rad(double deg);
};

#endif