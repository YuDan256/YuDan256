#include"Complex.h"
#include"Normal.h"

using namespace std;

const double Complex::PI = 3.14159265358979323846264, Complex::E = 2.7182818284590452353602874;

map<char, int>Complex::zop = { {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2},{'^',3} };
map<string, Complex(*)(const Complex&)> Complex::functionc = {
	{"conj",conjugate},{"sqrt",sqrtc},{"log",lnc},{"ln",lnc},{"arg",arg},{"Re",Re},{"Im",Im},
	{"real",Re},{"imag",Im},{ "R",R },{"sin",sinc},{"cos",cosc},{"tan",tanc},{"sinh",sinhc},
	{"cosh",coshc},{"tanh",tanhc},{"sh",sinhc},{"ch",coshc},{"th",tanhc},{"deg",deg},{"rad",rad},
	{"asin",asinc},{"acos",acosc},{"atan",atanc},{"arcsin",asinc},{"arccos",acosc},{"arctan",atanc}
};

void Complex::print()const {
	if (fabs(real) > 1e-10) {
		if (fabs(image - 1) < 1e-10)cout << real << "+i" << endl;
		else if (fabs(image + 1) < 1e-10)cout << real << "-i" << endl;
		else if (image > 1e-10)cout << real << "+" << image << "i" << endl;
		else if (image < -1e-10) cout << real << "-" << -image << "i" << endl;
		else cout << real << endl;
	}
	else if (fabs(real) < 1e-10 && fabs(image) < 1e-10)cout << 0 << endl;
	else {
		if (fabs(image - 1) < 1e-10)cout << "i" << endl;
		else if (fabs(image + 1) < 1e-10)cout << "-i" << endl;
		else cout << image << "i" << endl;
	}
}

Complex Complex::operator+(const Complex& other)const {
	Complex result(real + other.real, image + other.image);
	return result;
}

Complex Complex::operator+(const double& p)const {
	Complex result(real + p, image);
	return result;
}

Complex operator+(const double& p, const Complex& other) {
	Complex result(other.real + p, other.image);
	return result;
}

Complex Complex::operator-(const Complex& other)const {
	Complex result(real - other.real, image - other.image);
	return result;
}

Complex Complex::operator-(const double& p)const {
	Complex result(real - p, image);
	return result;
}

Complex operator-(const double& p, const Complex& other) {
	Complex result(p - other.real, -other.image);
	return result;
}

Complex Complex::operator-()const {
	Complex result(-real, -image);
	return result;
}

Complex Complex::operator*(const Complex& other)const {
	Complex result(real * other.real - image * other.image, real * other.image + image * other.real);
	return result;
}

Complex Complex::operator*(const double& p)const {
	Complex result(real * p, image * p);
	return result;
}

Complex operator*(const double& p, const Complex& other) {
	Complex result(other.real * p, other.image * p);
	return result;
}

Complex Complex::operator/(const Complex& other)const {
	if (other.modulus < 1e-15) {
		throw runtime_error("The divisor cannot be 0.");
	}
	double r = other.modulus;
	Complex result((real * other.real + image * other.image) / (r * r), (image * other.real - real * other.image) / (r * r));
	return result;
}

Complex Complex::operator/(const double& p)const {
	if (p < 1e-15) {
		throw runtime_error("The divisor cannot be 0.");
		return Complex(0, 0);
	}
	Complex result(real / p, image / p);
	return result;
}

Complex operator/(const double& p, const Complex& other) {
	Complex temp = p;
	temp = temp / other;
	return temp;
}

Complex Complex::operator^(const Complex& other) const {
	if (getModulus() < 1e-10 && (fabs(other.image) > 1e-10 || other.real <= 1e-10)) {
		throw runtime_error("When the base is 0, the exponent can only be a positive real number.");
	}
	if (fabs(image) < 1e-15 && fabs(other.image) < 1e-15)return pow(real, other.real);
	else if (getModulus() == 0)return 0;
	double a = argument(), r = modulus;
	Complex index(log(r), a);
	index = index * other;
	r = pow(E, index.real);
	Complex result(r * cos(index.image), r * sin(index.image));
	return result;
}

Complex Complex::operator^(const double& p) const {
	if (modulus == 0 && p <= 0) {
		throw runtime_error("When the base is 0, the exponent must be positive.");
	}
	else if (modulus == 0)return 0;
	double a = p * argument(), r = pow(modulus, p);
	Complex result(r * cos(a), r * sin(a));
	return result;
}

Complex operator^(const double& p, const Complex& other) {
	Complex z(p);
	return z ^ other;
}

ostream& operator<<(ostream& out, const Complex& z) {
	out << z.real << " " << z.image;
	return out;
}

bool Complex::operator==(const Complex& other)const {
	return(real == other.real && image == other.image);
}

Complex Complex::conjugate(const Complex& z) {
	Complex result(z.real, -z.image);
	return result;
}

Complex Complex::Re(const Complex& z) {
	return z.real;
}

Complex Complex::Im(const Complex& z) {
	return z.image;
}

Complex Complex::R(const Complex& z) {
	return z.modulus;
}

Complex Complex::arg(const Complex& z) {
	return z.argument();
}

double Complex::argument()const {
	if (modulus == 0) {
		throw runtime_error("The argument of 0 is arbitrary.");
	}
	if (real == 0 && image > 0)return PI / 2;
	else if (real == 0 && image < 0)return 3 * PI / 2;
	else if (real > 0 && image == 0)return 0;
	else if (real < 0 && image == 0)return PI;
	else return atan2(image, real);
}

Complex Complex::powc(const int& n)const {
	Complex result(1, 0);
	Complex z(real, image);
	if (n > 0) {
		for (int i = 0; i < n; i++) {
			result = result * z;
		}
		return result;
	}
	else if (n < 0) {
		if (z.modulus < 1e-15) {
			throw runtime_error("When the exponent is non-positive,the base cannot be 0.");
		}
		for (int i = 0; i < -n; i++) {
			result = result / z;
		}
		return result;
	}
	else return result;
}

void Complex::triangle()const {
	if (fabs(modulus - 1) < 1e-10)cout << "cos(" << argument() << ")+i*sin(" << argument() << ")" << endl;
	else cout << modulus << "*(cos(" << argument() << ")+i*sin(" << argument() << "))" << endl;
}

void Complex::root(const int& n)const {
	if (n <= 0)throw runtime_error("The n for nth roots must be positive.");
	vector<Complex>result(n);
	double k = n;
	Complex zero;
	if (modulus == 0) {
		for (int i = 0; i < n; i++) {
			result[i] = zero;
		}
	}
	else {
		double a = argument(), r = getModulus();
		for (int i = 0; i < n; i++) {
			double x = std::pow(r, 1 / k) * cos(((i + 1) * 2 * PI + a) / k), y = std::pow(r, 1 / k) * sin(((i + 1) * 2 * PI + a) / k);
			if (fabs(x) < 1e-5)x = 0; if (fabs(y) < 1e-5)y = 0;
			Complex root_i(x, y);
			result[i] = root_i;
		}
	}
	cout << "The all nth roots of Complex Number 1 are:" << endl;
	for (int i = 0; i < n; i++) {
		result[i].print();
	}
}

Complex Complex::sqrtc(const Complex& z) {
	return z.firstRoot(2);
}

Complex Complex::lnc(const Complex& z) {
	if (z.modulus < 1e-15)throw runtime_error("The argument of a logarithm cannot be zero.");
	if (fabs(z.image) < 1e-15)return Complex(log(z.real));
	return Complex(log(z.modulus)) + Complex(0, z.argument());
}

Complex Complex::sinc(const Complex& z) {
	if (fabs(z.image) < 1e-15)return sin(z.real);
	Complex i(0, 1);
	return ((E ^ (i * z)) - (E ^ (-i * z))) / (2 * i);
}

Complex Complex::cosc(const Complex& z) {
	if (fabs(z.image) < 1e-15)return cos(z.real);
	Complex i(0, 1);
	return ((E ^ (i * z)) + (E ^ (-i * z))) / 2;
}

Complex Complex::tanc(const Complex& z) {
	if (cosc(z).modulus < 1e-15)throw runtime_error("The tangent value is undefined.");
	return sinc(z) / cosc(z);
}

Complex Complex::sinhc(const Complex& z) {
	return ((E ^ z) - (E ^ (-z))) / 2;
}

Complex Complex::coshc(const Complex& z) {
	return ((E ^ z) + (E ^ (-z))) / 2;
}

Complex Complex::tanhc(const Complex& z) {
	if (coshc(z).getModulus() < 1e-15)throw runtime_error("Division by zero error.");
	return sinhc(z) / coshc(z);
}

Complex Complex::asinc(const Complex& z) {
	Complex i(0, 1);
	return -i * lnc(i * z + sqrtc(1 - (z ^ 2)));
}

Complex Complex::acosc(const Complex& z) {
	Complex i(0, 1);
	return -i * lnc(z + i * sqrtc(1 - (z ^ 2)));
}

Complex Complex::atanc(const Complex& z) {
	Complex i(0, 1);
	return 0.5 * i * (lnc(1 + i * z) - lnc(1 - i * z));
}

Complex Complex::deg(const Complex& rad) {
	if (fabs(rad.image) > 1e-15)throw invalid_argument("The radian must be a real number.");
	return Complex(rad / PI * 180);
}

Complex Complex::rad(const Complex& deg) {
	if (fabs(deg.image) > 1e-15)throw invalid_argument("The degree must be a real number.");
	return Complex(deg * PI / 180);
}

Complex Complex::input() {
	map<string, double>p = { {"PI",PI},{"E",E} };
	string _a, _b;
	double a, b;
	cout << "Enter the real and imaginary parts of the complex number:" << endl;
	cin >> _a >> _b;
	try {
		a = Normal::parsen(_a, p);
		b = Normal::parsen(_b, p);
	}
	catch (const exception& e) {
		cerr << "Error: " << e.what() << endl;
		return input();
	}
	return Complex(a, b);
}

void Complex::doComplex() {
	char operation, choice;
	int n;
	double a;
	Complex result;
	cout << "Welcome to the Complex Number Caculator!" << endl;
	cout << "Input Complex Number 1." << endl;
	Complex z1 = input();
	cout << "Input Complex Number 2 (if you do not need it, type 0 0)." << endl;
	Complex z2 = input();

	while (1) {
		bool invalidSave = false, quit = false;
		cout << "Choose an operation:" << endl;
		cout << "A - addition (2)" << endl;
		cout << "B - subtraction (2)" << endl;
		cout << "C - multiplication (2)" << endl;
		cout << "D - division (2)" << endl;
		cout << "E - modulus (1*)" << endl;
		cout << "F - argument (1*)" << endl;
		cout << "G - conjugation (1)" << endl;
		cout << "H - power (2)" << endl;
		cout << "I - root (1*)" << endl;
		cout << "J - sine (1)" << endl;
		cout << "K - cosine (1)" << endl;
		cout << "L - tangent (1)" << endl;
		cout << "M - natural logarithm (1)" << endl;
		cout << "N - triangle representation (1*)" << endl;
		cout << "Q - quit" << endl;
		cin >> operation;

		try {
			switch (operation) {
			case 'A':
				result = z1 + z2;
				cout << "The result is:" << endl;
				result.print();
				break;
			case 'B':
				result = z1 - z2;
				cout << "The result is:" << endl;
				result.print();
				break;
			case 'C':
				result = z1 * z2;
				cout << "The result is:" << endl;
				result.print();
				break;
			case 'D':
				result = z1 / z2;
				cout << "The result is:" << endl;
				result.print();
				break;
			case 'E':
				cout << "The modulus of Complex Number 1 is:" << endl << z1.getModulus() << endl;
				invalidSave = 1;
				break;
			case 'F':
				a = z1.argument();
				cout << "The argument of Complex Number 1 is:" << endl << a << endl;
				invalidSave = 1;
				break;
			case 'G':
				result = conjugate(z1);
				cout << "The conjugate of Complex Number 1 is:" << endl;
				result.print();
				break;
			case 'H':
				result = z1 ^ z2;
				cout << "The Complex Number 1 raised to the power of Complex Number 2 is:" << endl;
				result.print();
				break;
			case 'I':
				cout << "Enter the n for the nth roots:" << endl;
				cin >> n;
				z1.root(n);
				invalidSave = 1;
				break;
			case 'J':
				cout << "The sine value of Complex Number 1 is:" << endl;
				result = sinc(z1);
				result.print();
				break;
			case 'K':
				cout << "The cosine value of Complex Number 1 is:" << endl;
				result = cosc(z1);
				result.print();
				break;
			case 'L':
				cout << "The tangent value of Complex Number 1 is:" << endl;
				result = tanc(z1);
				result.print();
				break;
			case 'M':
				cout << "The natural logarithm of Complex Number 1 is:" << endl;
				result = lnc(z1);
				result.print();
				break;
			case 'N':
				z1.triangle();
				invalidSave = 1;
				break;
			case 'Q':
				quit = 1;
				break;
			default:
				cout << "Invalid operation" << endl;
				continue;
			}
		}
		catch (const exception& e) {
			cerr << "Error: " << e.what() << endl;
			continue;
		}
		if (quit)break;
		cout << "Do you want to save this result for further operations?(Y/N)" << endl;
		std::cin >> choice;
		if (choice == 'N') {
			cout << "Do you want to input new Number 1? (Y/N)" << endl;
			std::cin >> choice;
			if (choice == 'Y') z1 = input();
			else if (choice == 'N') {}
			else cout << "Invalid Choice." << endl;
			cout << "Do you want to input new Number 2? (Y/N)" << endl;
			std::cin >> choice;
			if (choice == 'Y') z2 = input();
			else if (choice == 'N') {}
			else cout << "Invalid Choice." << endl;
		}
		else if (choice == 'Y') {
			cout << "Where do you want to save the result? (1/2)" << endl;
			cin >> choice;
			if (choice == '1') {
				z1 = result;
				cout << "Do you want to input new Number 2? (Y/N)" << endl;
				std::cin >> choice;
				if (choice == 'Y')z2 = input();
				else if (choice == 'N') {}
				else cout << "Invalid Choice." << endl;
			}
			else if (choice == '2') {
				z2 = result;
				cout << "Do you want to input new Number 1? (Y/N)" << endl;
				std::cin >> choice;
				if (choice == 'Y')z1 = input();
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

Complex Complex::firstRoot(const int& n)const {
	if (n <= 0)throw runtime_error("The n for nth roots must be positive.");
	if ((modulus) < 1e-15)return Complex(0);
	double k = n;
	double a = argument(), r = modulus;
	double x = std::pow(r, 1 / k) * cos(a / k), y = std::pow(r, 1 / k) * sin(a / k);
	Complex result(x, y);
	return result;
}

void Complex::newInput(map<string, Complex>& numbers) {
	cout << "Enter the name, real and imaginary part of the complex numbers you need." << endl;
	cout << "The name of the complex numbers can only consist of letters." << endl;
	cout << "Enter 'end' to finish definition." << endl;
	map<string, double>p = { {"PI",PI},{"E",E} };
	string name, _a, _b;
	double a, b;
	while (1) {
		bool invalidInput = false;
		cout << "Name: " << endl;
		cin >> name;
		for (char i : name) {
			if (!isalpha(i)) {
				cout << "Invalid name." << endl;
				invalidInput = true;
				break;
			}
		}
		if (name == "end")break;
		if (invalidInput)continue;
		cout << "Real and imaginary parts: " << endl;
		cin >> _a;
		cin >> _b;
		try {
			a = Normal::parsen(_a, p);
			b = Normal::parsen(_b, p);
		}
		catch (const exception& e) {
			cerr << "Error: " << e.what() << endl;
			continue;
		}
		Complex z(a, b);
		numbers[name] = z;
		cout << "The Complex Number " + name + " is successfully created." << endl;
		storec(numbers);
	}
}

Complex Complex::parseFunctionc(const string& expr, const map<string, Complex>& numbers) {
	size_t currentPos = 0;
	string identifier, number;
	Complex z;
	int n = 0;
	while (currentPos < expr.length() && (isalpha(expr[currentPos]) || expr[currentPos] == '_')) {
		identifier += expr[currentPos++];
	}
	if (expr[currentPos] == '(') {
		++currentPos;
		z = parseExpressionc(expr, currentPos, numbers);
		if (expr[currentPos] == ',') {
			++currentPos;
			number = "";
			while (currentPos < expr.length() && isdigit(expr[currentPos])) {
				number += expr[currentPos++];
			}
			n = stoi(number);
			if (expr[currentPos] == ')' && currentPos + 1 == expr.length()) {
				if (identifier == "root") {
					z.root(n);
					throw true;
				}
			}
		}
		else if (expr[currentPos] == ')' && currentPos + 1 == expr.length()) {
			if (identifier == "tri") {
				z.triangle();
				throw true;
			}
		}
	}
	currentPos = 0;
	Complex result = parseExpressionc(expr, currentPos, numbers);
	if (currentPos == expr.length())return result;
	else throw invalid_argument("Unexpected character: " + string(1, expr[currentPos]));
}

Complex Complex::parseExpressionc(const string& expr, size_t& currentPos, const map<string, Complex>& numbers) {
	Complex result = parseTermc(expr, currentPos, numbers);

	while (currentPos < expr.length()) {
		char op = expr[currentPos];
		if (zop.find(op) != zop.end()) {
			++currentPos;
			Complex rhs = parseTermc(expr, currentPos, numbers);
			switch (op) {
			case '+': result = result + rhs; break;
			case '-': result = result - rhs; break;
			default: throw runtime_error("Unknown operator.");
			}
		}
		else break;
	}
	return result;
}

Complex Complex::parseTermc(const string& expr, size_t& currentPos, const map<string, Complex>& numbers) {
	Complex result = parsePowerc(expr, currentPos, numbers);
	char op;

	while (currentPos < expr.length()) {
		op = expr[currentPos];
		if (zop.find(op) != zop.end()) {
			if (zop[op] == 2) { // 只处理乘法和除法
				++currentPos;
				Complex rhs = parsePowerc(expr, currentPos, numbers);
				switch (op) {
				case '*':
					result = result * rhs;
					break;
				case '/':
					if (rhs == 0) throw runtime_error("Division by zero error.");
					result = result / rhs;
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

Complex Complex::parsePowerc(const string& expr, size_t& currentPos, const map<string, Complex>& numbers) {
	Complex result = 0.0; // 初始化result
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
			result = sign * parseExpressionc(expr, currentPos, numbers);
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
				Complex argument = parseExpressionc(expr, currentPos, numbers);
				if (expr[currentPos] == ')') {
					++currentPos; // 消耗 ')'
					auto it = functionc.find(identifier);
					if (it != functionc.end()) {
						result = sign * it->second(argument);
					}
					else {
						throw runtime_error("Unknown function: " + identifier);
					}
				}
				else if (expr[currentPos] == ',') {
					++currentPos;
					string number;
					while (currentPos < expr.length() && isdigit(expr[currentPos])) number += expr[currentPos++];
					int n = stoi(number);
					if (expr[currentPos] == ')') {
						++currentPos;
						if (identifier == "fr") {
							result = sign * argument.firstRoot(n);
						}
						else throw runtime_error("Unknown function: " + identifier);
					}
					else throw runtime_error("Missing closing parenthesis.");
				}
				else throw runtime_error("Missing closing parenthesis.");
			}
			else { // 否则视为复数
				auto it = numbers.find(identifier);
				if (it != numbers.end()) {
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
		Complex rhs = parsePowerc(expr, currentPos, numbers);
		result = result ^ rhs;
	}

	return result;
}

void Complex::newComplex() {
	map<string, Complex>numbers;
	char choice;

	cout << "Welcome to the Complex Number Calculator!" << endl;
	cout << "Do you want to use the variables you defined before?(Y/N)" << endl
		<< "If no,the variable list will be cleared." << endl;

	do {
		cin >> choice;
		if (choice == 'Y') {
			numbers = loadc();
			cout << "The variables are avaliable now." << endl;
		}
		else if (choice == 'N') {
			cout << "The variable list is cleared." << endl;
		}
		else cout << "Invalid choice. Choose again." << endl;
	} while (choice != 'Y' && choice != 'N');

	numbers["i"] = Complex(0, 1);
	numbers["E"] = Complex(E, 0);
	numbers["PI"] = Complex(PI, 0);
	numbers["ANS"] = Complex();
	storec(numbers);
	string expression, name;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl << "There are some commands you can use:" << endl;
	cout << "define - to define some complex variables." << endl;
	cout << "delete - to delete some complex variables." << endl;
	cout << "show - to find out which variables are available." << endl;
	cout << "function_list - to find out which functions are available." << endl;
	cout << "end - to quit this mode." << endl;
	cout << "save - to save the result in a complex number." << endl;
	cout << "The result of the previous calculation is automatically stored in \"ANS\"." << endl << endl;

	while (1) {
		cout << "Enter an expression:" << endl;
		getline(cin, expression);
		if (expression == "define") {
			newInput(numbers);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (expression == "delete") {
			deletec(numbers);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (expression == "show") {
			showc(numbers);
			continue;
		}
		if (expression == "function_list") {
			cout << endl << "The following functions can only be used individually:" << endl;
			cout << "Root - root(C,N+)" << endl;
			cout << "Triangle representation - tri(C)" << endl << endl;
			cout << "The following functions can be used with expressions:" << endl;
			cout << "First root - fr(C,N+)" << endl;
			cout << "Real part - Re(C)" << endl;
			cout << "Imaginary part - Im(C)" << endl;
			cout << "Modulus - R(C)" << endl;
			cout << "Conjugation - conj(C)" << endl;
			cout << "Argument - arg(C)" << endl;
			cout << "Sine - sin(C)" << endl;
			cout << "Cosine - cos(C)" << endl;
			cout << "Tangent - tan(C)" << endl;
			cout << "Natural logarithm - ln(C) or log(C)" << endl;
			cout << "Arcsin - arcsin(C) or asin(C)" << endl;
			cout << "Arccos - arccos(C) or acos(C)" << endl;
			cout << "Arctan - arctan(C) or atan(C)" << endl;
			cout << "Sinh - sinh(C) or sh(C)" << endl;
			cout << "Cosh - cosh(C) or ch(C)" << endl;
			cout << "Tanh - tanh(C) or th(C)" << endl;
			cout << "Degree - deg(C)" << endl;
			cout << "Radian - rad(C)" << endl << endl;
			cout << "Square root - sqrt(C)" << endl << endl;
			continue;
		}
		if (expression == "save") {
			bool invalidSave = false;
			cout << "Enter the name of complex number where you want to store the result:" << endl;
			cin >> name;
			for (char i : name) {
				if (!isalpha(i)) {
					cout << "Invalid name." << endl;
					invalidSave = true;
					break;
				}
			}
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (invalidSave)continue;
			else {
				numbers[name] = numbers["ANS"];
				cout << "The result is successfully saved in Complex Number " + name << endl;
				storec(numbers);
			}
			continue;
		}
		if (expression == "end") break;
		try {
			cout << "The result is: " << endl;
			Complex result = parseFunctionc(expression, numbers);
			numbers["ANS"] = result;
			storec(numbers);
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