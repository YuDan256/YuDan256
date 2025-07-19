#include "Normal.h"
#include <cmath>

using namespace std;

map<char, int> Normal::nop = { {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}, {'^', 3} };

map<string, double(*)(double)> Normal::functionn = {
	{"sin", sin}, {"cos", cos}, {"tan", tan}, {"log", log}, {"ln", log},{"sqrt", sqrt},{"exp", exp},
	{"arcsin", asin},{"arccos", acos},{"arctan", atan},{"asin",asin},{"acos",acos},{"atan",atan},
	{"sh",sinh},{"ch",cosh},{"th",tanh},{"sinh",sinh},{"cosh",cosh},{"tanh",tanh},{"deg",deg},{"rad",rad},
	{"abs",fabs},{"sgn",sgn},{"floor",floor},{"ceil",ceil},{"round",round},
};

double Normal::deg(double rad) {
	return rad / 3.141592653589793238462643383279502 * 180;
}

double Normal::rad(double deg) {
	return deg / 180 * 3.141592653589793238462643383279502;
}

double Normal::sgn(double x) {
	if (x > 0)return 1;
	else if (x < 0)return -1;
	else return 0;
}

double Normal::parsen(const string& expr, const map<string, double>& variables) {
	size_t currentPos = 0;
	double result = parseExpressionn(expr, currentPos, variables);
	if (currentPos == expr.length())return result;
	else throw invalid_argument("Unexpected character: " + string(1, expr[currentPos]));
}

double Normal::parseExpressionn(const string& expr, size_t& currentPos, const map<string, double>& variables) {
	double result = parseTermn(expr, currentPos, variables);

	while (currentPos < expr.length()) {
		char op = expr[currentPos];
		if (nop.find(op) != nop.end()) {
			++currentPos;
			double rhs = parseTermn(expr, currentPos, variables);
			switch (op) {
			case '+': result += rhs; break;
			case '-': result -= rhs; break;
			default: throw runtime_error("Unknown operator.");
			}
		}
		else break;
	}
	return result;
}

double Normal::parseTermn(const string& expr, size_t& currentPos, const map<string, double>& variables) {
	double result = parsePowern(expr, currentPos, variables);
	char op;

	while (currentPos < expr.length()) {
		op = expr[currentPos];
		if (nop.find(op) != nop.end()) {
			if (nop[op] == 2) { // 只处理乘法和除法
				++currentPos;
				double rhs = parsePowern(expr, currentPos, variables);
				switch (op) {
				case '*':
					result *= rhs;
					break;
				case '/':
					if (rhs == 0) throw runtime_error("Division by zero error.");
					result /= rhs;
					break;
				default:
					throw runtime_error("Unknown operator.");
				}
			}// 如果遇到加法或减法，应该停止解析当前项
			else break;
		}
		else break;
	}
	return result;
}

double Normal::parsePowern(const string& expr, size_t& currentPos, const map<string, double>& variables) {
	if (currentPos == expr.size())throw invalid_argument("Invalid syntax.");
	double result = 0.0; // 初始化result
	double sign = 1.0; // 用于处理正负号

	while (currentPos < expr.length()) {
		if (expr[currentPos] == '+') {
			++currentPos;
		}
		else if (expr[currentPos] == '-') {
			sign *= -1.0;
			++currentPos;
		}
		else break;
	}

	if (currentPos < expr.length()) {
		if (expr[currentPos] == '(') {
			++currentPos;
			result = parseExpressionn(expr, currentPos, variables);
			if (expr[currentPos] == ')') {
				++currentPos;
			}
			else {
				throw runtime_error("Missing closing parenthesis.");
			}
		}
		else if (isdigit(expr[currentPos]) || expr[currentPos] == '.') {
			string number;
			while (currentPos < expr.length() && (isdigit(expr[currentPos]) || expr[currentPos] == '.')) {
				number += expr[currentPos++];
			}
			result = stod(number);
		}
		else if (isalpha(expr[currentPos])) { // 支持函数和变量
			string identifier;
			while (currentPos < expr.length() && (isalpha(expr[currentPos]) || expr[currentPos] == '_')) {
				identifier += expr[currentPos++];
			}
			if (expr[currentPos] == '(') { // 检查是否为函数
				++currentPos; // 消耗 '('
				double argument = parseExpressionn(expr, currentPos, variables);
				if (expr[currentPos] == ')') {
					++currentPos; // 消耗 ')'
					auto it = functionn.find(identifier);
					if (it != functionn.end()) {
						result = it->second(argument);
					}
					else {
						throw runtime_error("Unknown function: " + identifier);
					}
				}
				else {
					throw runtime_error("Missing closing parenthesis.");
				}
			}
			else { // 否则视为变量
				auto it = variables.find(identifier);
				if (it != variables.end()) {
					result = it->second;
				}
				else {
					throw runtime_error("Undefined variable: " + identifier);
				}
			}
		}
		else if (!isspace(expr[currentPos])) {
			throw runtime_error("Unexpected character: " + string(1, expr[currentPos]));
		}
	}

	while (currentPos < expr.length() && expr[currentPos] == '^') {
		++currentPos;
		double rhs = parsePowern(expr, currentPos, variables);
		result = pow(result, rhs);
	}

	return sign * result;
}
