#include"Normal.h"

using namespace std;

map<char, int> Normal::nop = { {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}, {'^', 3} };
map<string, double(*)(double)> Normal::functionn = {
	{"sin", sin}, {"cos", cos}, {"tan", tan}, {"log", log}, {"ln", log},{"sqrt", sqrt},
	{"arcsin", asin},{"arccos", acos},{"arctan", atan},{"asin",asin},{"acos",acos},{"atan",atan},
	{"sh",sinh},{"ch",cosh},{"th",tanh},{"sinh",sinh},{"cosh",cosh},{"tanh",tanh},{"deg",deg},{"rad",rad}
};

double Normal::deg(double rad) {
	return rad / 3.141592653589793238462643383279502 * 180;
}

double Normal::rad(double deg) {
	return deg / 180 * 3.141592653589793238462643383279502;
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
	double result = 0.0; // 初始化result
	double sign = 1.0; // 用于处理正负号

	if (currentPos < expr.length()) {
		if (expr[currentPos] == '+') {
			sign = 1.0;
			++currentPos;
		}
		else if (expr[currentPos] == '-') {
			sign = -1.0;
			++currentPos;
		}
	}

	if (currentPos < expr.length()) {
		if (expr[currentPos] == '(') {
			++currentPos;
			result = sign * parseExpressionn(expr, currentPos, variables);
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
			result = sign * stod(number);
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
						result = sign * it->second(argument);
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
					result = sign * it->second;
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
	return result;
}

void Normal::input(map<string, double>& variables) {
	string name, number;
	cout << "The name of the variable can only consist of letters." << endl;
	cout << "Enter \"end\" to finish definition." << endl;
	while (1) {
		bool invalidInput = false;
		cout << "Name:" << endl;
		cin >> name;
		if (name == "end")break;
		for (char i : name) {
			if (!isalpha(i)) {
				cout << "Invalid name." << endl;
				invalidInput = true;
				break;
			}
		}
		if (invalidInput)continue;
		cout << "Number:" << endl;
		cin >> number;
		try {
			double n = parsen(number, variables);
			variables[name] = n;
			cout << "The variable " + name + " is successfully created." << endl;
			storen(variables);
		}
		catch (const exception& e) {
			cerr << "Error: " << e.what() << endl;
		}
	}
}

//新版操作系统
void Normal::newNormal() {
	map<string, double> variables;
	char choice;

	cout << "Welcome to the Normal Calculator!" << endl;
	cout << "Do you want to use the variables you defined before?(Y/N)" << endl
		<< "If no,the variable list will be cleared." << endl;
	do {
		cin >> choice;
		if (choice == 'Y') {
			variables = loadn();
			cout << "The variables are avaliable now." << endl;
		}
		else if (choice == 'N') {
			cout << "The variable list is cleared." << endl;
		}
		else cout << "Invalid choice. Choose again." << endl;
	} while (choice != 'Y' && choice != 'N');
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	variables["PI"] = 3.14159265358979323846264;
	variables["E"] = 2.7182818284590452353602874;
	variables["ANS"] = 0;//储存上一步的结果
	storen(variables);
	string expression, name;


	cout << endl << "There are some commands you can use:" << endl;
	cout << "define - to define some variables." << endl;
	cout << "delete - to delete some variables." << endl;
	cout << "show - to find which variables are avaliable." << endl;
	cout << "function_list - to find out which functions are available." << endl;
	cout << "end - to quit this mode." << endl;
	cout << "save - to save the result in a number." << endl;
	cout << "The result of the previous calculation is automatically stored in \"ANS\"." << endl << endl;

	while (1) {
		cout << "Enter an expression: " << endl;
		getline(cin, expression);
		if (expression == "define") {
			input(variables);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (expression == "delete") {
			deleten(variables);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (expression == "show") {
			shown(variables);
			continue;
		}
		if (expression == "end") break;
		if (expression == "function_list") {
			cout << "All these functions can be used in an arthmetic expression:" << endl;
			cout << "Sine - sin(R)" << endl;
			cout << "Cosine - cos(R)" << endl;
			cout << "Tangent - tan(R)" << endl;
			cout << "Square root - sqrt(R)" << endl;
			cout << "Natural logarithm - ln(R)/log(R)" << endl;
			cout << "Arcsin - arcsin(R)/asin(R)" << endl;
			cout << "Arccos - arccos(R)/acos(R)" << endl;
			cout << "Arctan - arctan(R)/atan(R)" << endl;
			cout << "Degree - deg(R)" << endl;
			cout << "Radian - rad(R)" << endl;
			cout << "Sinh - sinh(R)/sh(R)" << endl;
			cout << "Cosh - cosh(R)/ch(R)" << endl;
			cout << "Tanh - tanh(R)/th(R)" << endl << endl;
			continue;
		}
		if (expression == "save") {
			bool invalidSave = false;
			cout << "Enter the name of number where you want to store the result:" << endl;
			cin >> name;
			for (char i : name) {
				if (!isalpha(i)) {
					cout << "Invalid name." << endl;
					invalidSave = true;
					break;
				}
			}
			if (invalidSave)continue;
			else {
				variables[name] = variables["ANS"];
				storen(variables);
				cout << "The result is successfully saved in Number " + name << endl;
			}
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		try {
			cout << "The result is: " << endl;
			double result = parsen(expression, variables);
			variables["ANS"] = result;
			storen(variables);
			cout << result << endl;
		}
		catch (const exception& e) {
			cout << "Error: " << e.what() << endl;
		}
	}
	return;
}

//旧版操作系统
void Normal::doNormal() {
	string _a, _b;
	double a, b, result;
	char operation, choice;
	bool quit = false;
	map<string, double>p = {
		{"PI",3.141592653589793238462643383279502},{"E",2.7182818284590452353602874}
	};

	cout << "Welcome to the Normal Caculator!" << endl;
	cout << "Enter the first number:";
	cin >> _a;
	cout << "Enter the second number (if you do not need it, enter 0):";
	cin >> _b;
	try {
		a = parsen(_a, p);
		b = parsen(_b, p);
	}
	catch (const exception& e) {
		cerr << "Error: " << e.what() << endl;
		doNormal();
	}

	while (1) {
		cout << "Choose an operation:" << endl;
		cout << "A - addition (2)" << endl;
		cout << "B - subtraction (2)" << endl;
		cout << "C - multiplication (2)" << endl;
		cout << "D - division (2)" << endl;
		cout << "E - power (2)" << endl;
		cout << "F - root(1)" << endl;
		cout << "G - sine (1)" << endl;
		cout << "H - cosine (1)" << endl;
		cout << "I - tangent (1)" << endl;
		cout << "J - arcsin (1)" << endl;
		cout << "K - arccos (1)" << endl;
		cout << "L - arctan (1)" << endl;
		cout << "M - sinh (1)" << endl;
		cout << "N - cosh (1)" << endl;
		cout << "O - tanh (1)" << endl;
		cout << "P - logarithm (2)" << endl;
		cout << "Q - quit" << endl;
		cin >> operation;

		try {
			switch (operation) {
			case 'A':
				result = a + b;
				if (fabs(result) < 1e-15)result = 0.0;
				cout << "The result is:" << result << endl;
				break;
			case 'B':
				result = a - b;
				if (fabs(result) < 1e-15)result = 0.0;
				cout << "The result is:" << result << endl;
				break;
			case 'C':
				result = a * b;
				if (fabs(result) < 1e-15)result = 0.0;
				cout << "The result is:" << result << endl;
				break;
			case 'D':
				if (b == 0) {
					throw invalid_argument("Math error.");
				}
				result = a / b;
				if (fabs(result) < 1e-15)result = 0.0;
				cout << "The result is:" << result << endl;
				break;
			case 'E':
				if (a == 0 && b <= 0) {
					throw invalid_argument("Math error.");
				}
				result = pow(a, b);
				if (fabs(result) < 1e-15)result = 0.0;
				cout << "The a to the power of b is:" << result << endl;
				break;
			case 'F':
				if (a < 0)throw runtime_error("The square root of a minus number is undefined.");
				result = sqrt(a);
				if (fabs(result) < 1e-15)result = 0.0;
				cout << "sqrt(a)=" << result << endl;
				break;
			case 'G':
				result = sin(a);
				if (fabs(result) < 1e-15)result = 0.0;
				cout << "sin(a)=" << result << endl;
				break;
			case 'H':
				result = cos(a);
				if (fabs(result) < 1e-15)result = 0.0;
				cout << "cos(a)=" << result << endl;
				break;
			case 'I':
				result = tan(a);
				if (fabs(result) < 1e-15)result = 0.0;
				cout << "tan(a)=" << result << endl;
				break;
			case 'J':
				if (a > 1 || a < -1) {
					throw invalid_argument("Math error.");
				}
				result = asin(a);
				if (fabs(result) < 1e-15)result = 0.0;
				cout << "arcsin(a)= " << result << endl;
				break;
			case 'K':
				if (a > 1 || a < -1) {
					throw invalid_argument("Math error.");
				}
				result = acos(a);
				if (fabs(result) < 1e-15)result = 0.0;
				cout << "arccos(a)= " << result << endl;
				break;
			case 'L':
				result = atan(a);
				if (fabs(result) < 1e-15)result = 0.0;
				cout << "arctan(a)= " << result << endl;
				break;
			case'M':
				result = sinh(a);
				if (fabs(result) < 1e-15)result = 0.0;
				cout << "sinh(a)=" << result << endl;
				break;
			case 'N':
				result = cosh(a);
				if (fabs(result) < 1e-15)result = 0.0;
				cout << "cos(h)=" << result << endl;
				break;
			case'O':
				result = tanh(a);
				if (fabs(result) < 1e-15)result = 0.0;
				cout << "tan(h)=" << result << endl;
				break;
			case 'P':
				result = log(b) / log(a);
				if (a <= 0 || b <= 0 || a == 1) {
					throw invalid_argument("Math error.");
				}
				if (fabs(result) < 1e-15)result = 0.0;
				cout << "The logarithm of b to the base a is:" << result << endl;
				break;
			case 'Q':
				quit = 1;
				break;
			default:
				result = 0.0;
				cout << "Invalid operation." << endl;
				continue;
			}
		}
		catch (const exception& e) {
			cerr << "Error: " << e.what() << endl;
			continue;
		}
		if (quit)break;
		cout << "Do you want to save this result for further operations?(Y/N)" << endl;
		cin >> choice;
		if (choice == 'N') {
			cout << "Do you want to input new Number 1? (Y/N)" << endl;
			cin >> choice;
			if (choice == 'Y') {
				cout << "Input the new Number 1:" << endl;
				cin >> _a;
				try {
					a = parsen(_a, p);
				}
				catch (const exception& e) {
					cerr << "Error: " << e.what() << endl;
				}
			}
			else if (choice == 'N') {}
			else cout << "Invalid Choice." << endl;
			cout << "Do you want to input new Number 2? (Y/N)" << endl;
			cin >> choice;
			if (choice == 'Y') {
				cout << "Input the new Number 2:" << endl;
				cin >> _b;
				try {
					b = parsen(_b, p);
				}
				catch (const exception& e) {
					cerr << "Error: " << e.what() << endl;
				}
			}
			else if (choice == 'N') {}
			else cout << "Invalid Choice." << endl;
		}
		else if (choice == 'Y') {
			cout << "Where do you want to save the result? (1/2)" << endl;
			cin >> choice;
			if (choice == '1') {
				a = result;
				cout << "Do you want to input new Number 2? (Y/N)" << endl;
				cin >> choice;
				if (choice == 'Y') {
					cout << "Input the new Number 2:" << endl;
					cin >> _b;
					try {
						b = parsen(_b, p);
					}
					catch (const exception& e) {
						cerr << "Error: " << e.what() << endl;
					}
				}
				else if (choice == 'N') {}
				else cout << "Invalid Choice." << endl;
			}
			else if (choice == '2') {
				b = result;
				cout << "Do you want to input new Number 1? (Y/N)" << endl;
				cin >> choice;
				if (choice == 'Y') {
					cout << "Input the new Number 1:" << endl;
					cin >> _a;
					try {
						a = parsen(_a, p);
					}
					catch (const exception& e) {
						cerr << "Error: " << e.what() << endl;
					}
				}
				else if (choice == 'N') {}
				else cout << "Invalid Choice." << endl;
			}
			else cout << "Invalid choice." << endl;
		}
		else {
			cout << "Invalid choice." << endl;
			continue;
		}
	}
}