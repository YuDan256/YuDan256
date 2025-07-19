#include "Real.h"

map<char, int> Real::rop = { {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}, {'^', 3} };
map<string, Real(*)(const Real&)>Real::functionr1;

Real Real::operator+(const Real& r) {
	return Real(value + r.value);
}

Real Real::operator-(const Real& r) {
	return Real(value - r.value);
}

Real Real::operator*(const Real& r) {
	return Real(value * r.value);
}

Real Real::operator/(const Real& r) {
	if (fabs(r.value) < 1e-15) throw runtime_error("Division by zero.");
	return Real(value / r.value);
}

Real Real::operator^(const Real& r) {
	return Real(pow(value, r.value));
}

Real operator+(const double& p, const Real& r) {
	return Real(p + r.value);
}

Real operator-(const double& p, const Real& r) {
	return Real(p - r.value);
}

Real operator*(const double& p, const Real& r) {
	return Real(p * r.value);
}

Real operator/(const double& p, const Real& r) {
	if (fabs(r.value) < 1e-15) throw runtime_error("Division by zero.");
	return Real(p / r.value);
}

Real operator^(const double& p, const Real& r) {
	return Real(pow(p, r.value));
}

Real& Real::operator=(const Real& r) {
	value = r.value;
	return *this;
}

Real& Real::operator+=(const Real& r) {
	value += r.value;
	return *this;
}

Real& Real::operator-=(const Real& r) {
	value -= r.value;
	return *this;
}

Real& Real::operator*=(const Real& r) {
	value *= r.value;
	return *this;
}

Real& Real::operator/=(const Real& r) {
	if (fabs(r.value) < 1e-15) throw runtime_error("Division by zero.");
	value /= r.value;
	return *this;
}

Real& Real::operator^=(const Real& r) {
	value = pow(value, r.value);
	return *this;
}

Real& Real::operator++() {
	value++;
	return *this;
}

Real& Real::operator--() {
	value--;
	return *this;
}

Real Real::operator++(int) {
	Real temp = *this;
	value++;
	return temp;
}

Real Real::operator--(int) {
	Real temp = *this;
	value--;
	return temp;
}

bool Real::operator==(const Real& r) const {
	return fabs(value - r.value) < 1e-15;
}

bool Real::operator!=(const Real& r) const {
	return !(*this == r);
}

bool Real::operator<(const Real& r) const {
	return value < r.value;
}

bool Real::operator<=(const Real& r) const {
	return value <= r.value;
}

bool Real::operator>(const Real& r) const {
	return value > r.value;
}

bool Real::operator>=(const Real& r) const {
	return value >= r.value;
}

double Real::getValue() const {
	return value;
}

void Real::print(const string& end) const {
	cout << value << end;
}

Real Real::parseFunctionr(const string& expr, const map<string, Real>& variables) {
	size_t currentPos = 0;
	string identifier, number;
	Real z;
	int n = 0;
	while (currentPos < expr.length() && (isalpha(expr[currentPos]) || expr[currentPos] == '_')) {
		identifier += expr[currentPos++];
	}
	if (expr[currentPos] == '(') {
		++currentPos;
		z = parseExpressionr(expr, currentPos, variables);
		if (expr[currentPos] == ')' && currentPos + 1 == expr.length()) {
			if (identifier == "print") {
				z.print();
				throw true;
			}
			else throw runtime_error("Unknown function: " + identifier);
		}
		else throw runtime_error("Missing closing parenthesis.");
	}
	currentPos = 0;
	Real result = parseExpressionr(expr, currentPos, variables);
	if (currentPos == expr.length())return result;
	else throw invalid_argument("Unexpected character: " + string(1, expr[currentPos]));
}

Real Real::parseExpressionr(const string& expr, size_t& currentPos, const map<string, Real>& variables) {
	Real result = parseTermr(expr, currentPos, variables);

	while (currentPos < expr.length()) {
		char op = expr[currentPos];
		if (rop.find(op) != rop.end()) {
			++currentPos;
			Real rhs = parseTermr(expr, currentPos, variables);
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

Real Real::parseTermr(const string& expr, size_t& currentPos, const map<string, Real>& variables) {
	Real result = parsePowerr(expr, currentPos, variables);
	char op;

	while (currentPos < expr.length()) {
		op = expr[currentPos];
		if (rop.find(op) != rop.end()) {
			if (rop[op] == 2) { // 只处理乘法和除法
				++currentPos;
				Real rhs = parsePowerr(expr, currentPos, variables);
				switch (op) {
				case '*':
					result *= rhs;
					break;
				case '/':
					if (rhs == Real(0.0)) throw runtime_error("Division by zero error.");
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

Real Real::parsePowerr(const string& expr, size_t& currentPos, const map<string, Real>& variables) {
	if (currentPos == expr.size())throw invalid_argument("Invalid syntax.");
	Real result = 0, sign = 1;

	while (currentPos < expr.length()) {
		if (expr[currentPos] == '+') {
			++currentPos;
		}
		else if (expr[currentPos] == '-') {
			sign *= -1;
			++currentPos;
		}
		else break;
	}

	if (currentPos < expr.length()) {
		if (expr[currentPos] == '(') {
			++currentPos;
			result = parseExpressionr(expr, currentPos, variables);
			if (expr[currentPos] == ')') {
				++currentPos;
			}
			else {
				throw runtime_error("Missing closing parenthesis.");
			}
		}
		else if (isdigit(expr[currentPos])) {
			string number;
			while (currentPos < expr.length() && isdigit(expr[currentPos])) {
				number += expr[currentPos++];
			}
			result = Real(stod(number));
		}
		else if (isalpha(expr[currentPos])) { // 支持函数和变量
			string identifier;
			while (currentPos < expr.length() && (isalpha(expr[currentPos]) || expr[currentPos] == '_')) {
				identifier += expr[currentPos++];
			}
			if (expr[currentPos] == '(') { // 检查是否为函数
				++currentPos; // 消耗 '('
				Real argument = parseExpressionr(expr, currentPos, variables);
				if (expr[currentPos] == ')') {
					++currentPos; // 消耗 ')'
					auto it = functionr1.find(identifier);
					if (it != functionr1.end()) {
						result = it->second(argument);
					}
					else {
						throw runtime_error("Unknown function: " + identifier);
					}
				}
				else throw runtime_error("Missing closing parenthesis.");
			}
			else { // 否则视为整数
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
		Real rhs = parsePowerr(expr, currentPos, variables);
		result = pow(result.value, rhs.value);
	}

	return sign * result;
}

ostream& operator<<(ostream& out, const Real& r) {
	out << r.value;
	return out;
}

istream& operator>>(istream& in, Real& r) {
	in >> r.value;
	if (in.fail()) {
		throw runtime_error("Invalid input for Real number.");
	}
	return in;
}

void Real::input(map<string, Real>& variables) {
	string line;
	while (getline(cin, line)) {
		if (line.empty()) continue;
		size_t pos = line.find('=');
		if (pos == string::npos) {
			throw runtime_error("Invalid input format. Expected 'name = value'.");
		}
		string name = line.substr(0, pos);
		string valueStr = line.substr(pos + 1);
		if (!isValidName(name)) {
			throw runtime_error("Invalid variable name: " + name);
		}
		variables[name] = Real(stod(valueStr));
	}
}

Real Real::stor(const string& expr){
	return Real(stod(expr));
}

void Real::doReal(){
	map<string, Real>variables;
	string choice;

	cout << "Welcome to the Real Calculator!" << endl << endl;
	cout << "Do you want to use the variables you defined before?(Y/N)" << endl
		<< "If no,the variable list will be cleared." << endl;
	do {
		cin >> choice;
		if (choice == "Y") {
			variables = loadr();
			cout << "The variables are avaliable now." << endl;
		}
		else if (choice == "N") {
			cout << "The variable list is cleared." << endl;
		}
		else cout << "Invalid choice. Choose again." << endl;
	} while (choice != "Y" && choice != "N");

	variables["PI"] = Real(3.141592653589793238462643383279502);
	variables["E"] = Real(2.7182818284590452353602874);
	variables["ANS"] = Real();
	storer(variables);
	string expression, name;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl << "There are some commands you can use:" << endl;
	cout << "define - to define some matrix variables." << endl;
	cout << "delete - to delete some matrix variables." << endl;
	cout << "show - to find out which variables are available." << endl;
	cout << "function_list - to find out which functions are available." << endl;
	cout << "end - to quit this mode." << endl;
	cout << "save - to save the result in a matrix." << endl;
	cout << "The result of the previous calculation is automatically stored in \"ANS\"." << endl << endl;

	while (1) {
		cout << "Enter an expression:" << endl;
		getline(cin, expression);
		expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());
		if (expression.find('=') != string::npos) {
			processr(expression, variables);
			continue;
		}
		if (expression == "define") {
			input(variables);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (expression == "delete") {
			deleter(variables);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (expression == "show") {
			showr(variables);
			continue;
		}
		if (expression == "function_list") {
			cout << endl;
			cout << "The following functions can only be used individually with defined variables:" << endl;
			cout  << endl;
			cout << "The following functions can be used with expressions:" << endl;
			cout  << endl;
			continue;
		}
		if (expression == "save") {
			bool invalidSave = false;
			cout << "Enter the name of real number where you want to store the result:" << endl;
			cin >> name;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (!isValidName(name))continue;
			else {
				variables[name] = variables["ANS"];
				cout << "The result is successfully saved in Number " + name << endl;
				storer(variables);
			}
			continue;
		}
		if (expression == "end") break;
		try {
			cout << "The result is: " << endl;
			Real result = parseFunctionr(expression, variables);
			variables["ANS"] = result;
			storer(variables);
			result.print();
		}
		catch (const exception& e) {
			cout << "Error: " << e.what() << endl;
		}
		catch (const bool& b) {
			if (b)cout << "The result cannot be saved." << endl;
		}
	}
}

bool Real::isValidName(const string& name) {
	for (char c : name) {
		if (!isalpha(c) && c != '_') {
			cerr << "Error: Invalid variable name '" << name << "'." << endl;
			return false;
		}
	}
	if (name.empty()) {
		cerr << "Error: Variable name cannot be empty." << endl;
		return false;
	}
	return true;
}