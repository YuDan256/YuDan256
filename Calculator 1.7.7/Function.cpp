#include "Function.h"
#include "Normal.h"

using namespace std;

double Function::differentiation(const double& x0)const {
	double delta = 1e-6;
	double x1 = x0 + delta, x2 = x0 - delta;
	double y1 = value(x1);
	double y2 = value(x2);
	double result = (y1 - y2) / (2 * delta);//用定义计算
	if (fabs(result) < 1e-6)return 0;
	else return result;
}

double Function::integration(const double& a, const double& b)const {
	double division = 1e6;
	double result = 0;
	//定义法计算定积分，步长设为区间长度的1/1e6.
	for (int i = 0; i < 1e6; i++) {
		double x1 = a + (b - a) * i / division, x2 = a + (b - a) * (i + 1) / division;
		double y1 = value(x1);
		double y2 = value(x2);
		result += (y1 + y2) * (b - a) / (2 * division);
	}
	if (fabs(result) < 1e-6)return 0;
	else return result;
}

double Function::solveEquation(const double& x0)const {
	int num = 0;
	double x = x0, k = 0, y = 0;
	for (int i = 1; i < 1e6; i++) {
		for (int j = 1; j < 1e4; j++) {
			k = differentiation(x);
			if (k == 0)x += 1e-4;
			else break;
		}
		if (k == 0) {
			throw runtime_error("The solution cannot be found.");
		}
		y = value(x);
		if (fabs(y) < 1e-6) {
			if (fabs(x) < 1e-6)return 0.0;
			else return x;
		}
		else x -= y / k;
	}
	throw runtime_error("The solution cannot be found.");
}

void Function::print() const {
	cout << "y = " << expr << endl;
}

void Function::doTable(const double& x0, const double& path, const int& n)const {
	cout << "The function value table is:" << endl;
	cout << "No.\tx\ty" << endl;
	double y = 0.0;
	for (int i = 0; i < n; i++) {
		double x = (x0 + path * i);
		y = value(x);
		if (fabs(y) < 1e-6)y = 0;
		cout << i << "\t" << x << "\t" << y << endl;
	}
}

double Function::sum(const double& i, const double& j) const {
	if (i != floor(i) || j != floor(j))throw invalid_argument("The two parameters need to be integer.");
	double s = 0;
	for (int k = static_cast<int>(i); k < static_cast<int>(j) + 1; k++) {
		s += value(k);
	}
	return s;
}

void Function::doFunction() {
	char operation;
	string expr, variable, _x0, _a, _b, _path, _n;
	map<string, double>variables;
	double x0, a, b, path;
	int n;

	variables["PI"] = 3.14159265358979323846264;
	variables["E"] = 2.7182818284590452353602874;

	cout << "Welcome to the Function Calculator!" << endl;
	cout << "All of the functions you enter should be with respect to the independent variable x," << endl
		<< "and please do not omit any mutiplication sign!" << endl;

	while (1) {
		cout << "Choose an operation:" << endl
			<< "A - differentation" << endl
			<< "B - definite integration" << endl
			<< "C - numerical solution" << endl
			<< "D - Function value table" << endl
			<< "E - Function value" << endl
			<< "F - Sum" << endl
			<< "Q - quit" << endl;
		cin >> operation;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');;
		if (operation == 'A') {
			cout << "Enter the function that you want to differentiate:" << endl;
			cin >> expr;
			Function f(expr);
			cout << "Enter the specific x0 at which you want to find the derivative:" << endl;
			cin >> _x0;
			try {
				x0 = Normal::parsen(_x0, variables);
				cout << "The result is:" << endl;
				cout << f.differentiation(x0) << endl;
			}
			catch (const exception& e) {
				cerr << "Error: " << e.what() << endl;
				continue;
			}
		}
		else if (operation == 'B') {
			cout << "Enter the integrand:" << endl;
			cin >> expr;
			Function f(expr);
			cout << "Enter the lower limits of integration:" << endl;
			cin >> _a;
			cout << "Enter the upper limits of integration:" << endl;
			cin >> _b;
			try {
				a = Normal::parsen(_a, variables);
				b = Normal::parsen(_b, variables);
				cout << "The result is (you may wait for a while):" << endl;
				if (a < b)cout << f.integration(a, b) << endl;
				else if (a == b)cout << 0 << endl;
				else cout << f.integration(b, a) << endl;
			}
			catch (const exception& e) {
				cerr << "Error: " << e.what() << endl;
				continue;
			}
		}
		else if (operation == 'C') {
			cout << "Enter the function for which you want to find the zero point:" << endl;
			cin >> expr;
			Function f(expr);
			cout << "Enter a point close to the zero point:" << endl;
			cin >> _x0;
			try {
				x0 = Normal::parsen(_x0, variables);
				cout << "One of the solution is:" << endl;
				cout << f.solveEquation(x0) << endl;
			}
			catch (const exception& e) {
				cerr << "Error: " << e.what() << endl;
				continue;
			}
		}
		else if (operation == 'D') {
			cout << "Enter the function for the function value table:" << endl;
			cin >> expr;
			Function f(expr);
			cout << "Enter the initial value:" << endl;
			cin >> _x0;
			cout << "Enter the step value:" << endl;
			cin >> _path;
			cout << "Enter the length of the table:" << endl;
			cin >> _n;
			try {
				x0 = Normal::parsen(_x0, variables);
				path = Normal::parsen(_path, variables);
				n = static_cast<int>(Normal::parsen(_n, variables));
				f.doTable(x0, path, n);
			}
			catch (const exception& e) {
				cerr << "Error: " << e.what() << endl;
				continue;
			}
		}
		else if (operation == 'E') {
			cout << "Enter the function for the function value:" << endl;
			cin >> expr;
			Function f(expr);
			cout << "Enter the point you want to find the function value:" << endl;
			cin >> _x0;
			try {
				x0 = Normal::parsen(_x0, variables);
				cout << f.value(x0) << endl;
			}
			catch (const exception& e) {
				cerr << "Error: " << e.what() << endl;
				continue;
			}
		}
		else if (operation == 'F') {
			cout << "Enter the function for the sum:" << endl;
			cin >> expr;
			Function f(expr);
			cout << "Enter the lower and upper limits of the sum:" << endl;
			cin >> _a >> _b;
			try {
				a = Normal::parsen(_a, variables);
				b = Normal::parsen(_b, variables);
				cout << f.sum(a, b) << endl;
			}
			catch (const exception& e) {
				cerr << "Error: " << e.what() << endl;
				continue;
			}
		}
		else if (operation == 'Q') {
			return;
		}
		else {
			cout << "Invalid operation." << endl;
			continue;
		}
	}
}

void Function::newInput(map<string, Function>& functions) {
	cout << "Enter \"end\" to finish definition." << endl;
	cout << "The names of functions can only consist of letters." << endl;
	cout << "Ensure the function has x as the independent variable." << endl;
	cout << "Do not omit any multiplication signs!" << endl;
	Function f;
	string name, expr;

	while (1) {
		bool invalidSave = false;
		cout << "Name:" << endl;
		cin >> name;
		for (char i : name) {
			if (!isalpha(i)) {
				invalidSave = true;
				cout << "Invalid name." << endl;
			}
		}
		if (invalidSave)continue;
		if (name == "end")break;
		cout << "Function:" << endl;
		cout << "y = ";
		cin >> expr;
		Function f(expr);
		functions[name] = f;
		storef(functions);
		cout << "The function " + name + " is successfully created." << endl;
	}
}

void Function::newFunction() {
	char choice;
	map<string, Function>functions;

	cout << "Welcome to the Function Calculator!" << endl;
	cout << "This mode only supports operations on predefined functions." << endl;
	cout << "Do you want to use the functions you defined before?(Y/N)" << endl
		<< "If no,the function list will be cleared." << endl;
	do {
		cin >> choice;
		if (choice == 'Y') {
			functions = loadf();
			cout << "The functions are avaliable now." << endl;
		}
		else if (choice == 'N') cout << "The function list is cleared." << endl;
		else cout << "Invalid choice. Choose again." << endl;
	} while (choice != 'Y' && choice != 'N');
	storef(functions);

	string expression, name;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl << "There are some commands you can use:" << endl;
	cout << "define - to define some functions." << endl;
	cout << "delete - to delete some functions." << endl;
	cout << "show - to find out which functions are avaliable." << endl;
	cout << "operation_list - to find out which operations are available." << endl;
	cout << "function_list - to find out which functions can be parsed." << endl;
	cout << "end - to quit this mode." << endl << endl;

	while (1) {
		cout << "Enter an expression:" << endl;
		getline(cin, expression);
		if (expression == "define") {
			newInput(functions);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (expression == "delete") {
			deletef(functions);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (expression == "show") {
			showf(functions);
			continue;
		}
		if (expression == "operation_list") {
			cout << endl
				<< "Differentation - D(F,R):" << endl
				<< "    The first parameter is the function." << endl
				<< "    The second parameter is the point where you want to find the derivative." << endl
				<< "Definite intergration - I(F,R,R):" << endl
				<< "    The first parameter is the intergrand." << endl
				<< "    The second and the third parameters are the lower and upper limits." << endl
				<< "Numerical solution - S(F,R):" << endl
				<< "    The first parameter is the function of which you want to find a zero point." << endl
				<< "    The second parameter is the initial value, as close to zero as possible." << endl
				<< "Function value table - T(F,R,R,N+):" << endl
				<< "    The first parameter is the function of which you want to generate a table." << endl
				<< "    The second parameter is the initial x-value of the table." << endl
				<< "    The third parameter is the path, meaning the distance between two neighbour x's." << endl
				<< "    The fourth parameter is the length of the table." << endl
				<< "Function value - V(F,R):" << endl
				<< "    The first parameter is the function." << endl
				<< "    The second parameter is the point where you want to find the function value." << endl
				<< "Sum - sum(F,Z,Z):" << endl
				<< "    The first parameter is the function." << endl
				<< "    The second and the third parameter is the lower and upper limits of the sum." << endl
				<< "Print - print(F):" << endl
				<< "    The parameter is the function of which you want to print the expression." << endl
				<< endl;
			continue;
		}
		if (expression == "function_list") {
			cout << "All these functions can be parsed in an arthmetic expression:" << endl;
			cout << "Sine - sin(R)" << endl;
			cout << "Cosine - cos(R)" << endl;
			cout << "Tangent - tan(R)" << endl;
			cout << "Square root - sqrt(R)" << endl;
			cout << "Natural logarithm - ln(R)/log(R)" << endl;
			cout << "Arcsin - arcsin(R)/asin(R)" << endl;
			cout << "Arccos - arccos(R)/acos(R)" << endl;
			cout << "Arctan - arctan(R)/atan(R)" << endl;
			cout << "Sinh - sinh(R)/sh(R)" << endl;
			cout << "Cosh - cosh(R)/ch(R)" << endl;
			cout << "Tanh - tanh(R)/th(R)" << endl << endl;
			continue;
		}
		if (expression == "end") break;
		try {
			cout << "The result is:" << endl;
			parseFunctionf(expression, functions);
		}
		catch (const exception& e) {
			cout << "Error: " << e.what() << endl;
		}
	}
}

void Function::parseFunctionf(const string& expr, const map<string, Function>& functions) {
	size_t currentPos = 0;
	string identifier, name, _num3;
	double num1, num2;
	int num3;
	map<string, double>variables = {
		{"PI",3.14159265358979323846264},{"E",2.7182818284590452353602874}
	};
	while (currentPos < expr.length() && isalpha(expr[currentPos])) {
		identifier += expr[currentPos++];
	}
	if (expr[currentPos] == '(') {
		currentPos++;
		while (currentPos < expr.length() && isalpha(expr[currentPos])) {
			name += expr[currentPos++];
		}
		auto it = functions.find(name);
		if (it == functions.end())throw runtime_error("Undefined function: " + name);
		Function f = it->second;
		if (expr[currentPos] == ',') {
			currentPos++;
			num1 = Normal::parseExpressionn(expr, currentPos, variables);
			if (expr[currentPos] == ')'&&currentPos+1==expr.length()) {
				if (identifier == "D")cout << f.differentiation(num1) << endl;
				else if (identifier == "S")cout << f.solveEquation(num1) << endl;
				else if (identifier == "V")cout << f.value(num1) << endl;
				else throw runtime_error("Unknown operation: " + identifier);
			}
			else if (expr[currentPos] == ',') {
				currentPos++;
				num2 = Normal::parseExpressionn(expr, currentPos, variables);
				if (expr[currentPos] == ')' && currentPos + 1 == expr.length()) {
					if (identifier == "I")cout << f.integration(num1, num2) << endl;
					else if (identifier == "sum")cout << f.sum(num1, num2) << endl;
					else throw runtime_error("Unknown operation: " + identifier);
				}
				else if (expr[currentPos] == ',') {
					currentPos++;
					while (currentPos < expr.length() && isdigit(expr[currentPos])) {
						_num3 += expr[currentPos++];
					}
					num3 = stoi(_num3);
					if (expr[currentPos] == ')' && currentPos + 1 == expr.length()) {
						if (identifier == "T")f.doTable(num1, num2, num3);
						else throw runtime_error("Unknown operation: " + identifier);
					}
					else throw runtime_error("Missing closing parenthesis.");
				}
				else throw runtime_error("Unexpected character: " + string(1, expr[currentPos]));
			}
			else throw runtime_error("Unexpected character: " + string(1, expr[currentPos]));
		}
		else if (expr[currentPos] == ')') {
			if (identifier == "print")f.print();
			else throw runtime_error("Unknown operation: " + identifier);
		}
		else throw runtime_error("Unexpected character: " + string(1, expr[currentPos]));
	}
	else throw invalid_argument("The expression cannot be recognized as an operation.");
}

double Function::value(const double& x0)const {
	int num = 0;
	double y = 0.0;
	string f = expr;
	map<string, double>variables = {
		{"PI",3.14159265358979323846264},{"E",2.7182818284590452353602874},{"x",x0}
	};
	y = Normal::parsen(f, variables);
	if (fabs(y) < 1e-15)y = 0;
	return y;
}

ostream& operator<<(ostream& out, const Function& f) {
	out << f.expr;
	return out;
}
