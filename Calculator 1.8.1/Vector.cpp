#include"Vector.h"
#include"Normal.h"

using namespace std;

const double Vector::PI = 3.14159265358979323846264;

map<char, int>Vector::vop = { {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2} ,{'^',3} };
map<string, Vector(*)(const Vector&)>Vector::functionv1 = {
	{"R",R},{"sin",sinv},{"cos",cosv},{"tan",tanv},{"ln",lnv},{"log",lnv},{"sqrt",sqrtv},{"deg",deg},{"rad",rad }
};
map<string, Vector(*)(const Vector&, const Vector&)>Vector::functionv2 = {
	{"dot",dotProduct},{"SP",SP},{"PV",PV},{"ang",ang}
};

void Vector::print()const {
	if (n == 0)return;
	if (n == 1) {
		if (fabs(data[0]) < 1e-10)cout << 0 << endl;
		else std::cout << data[0] << endl;
		return;
	}
	std::cout << "(";
	for (int i = 0; i < n - 1; i++) {
		if (fabs(data[i]) < 1e-10)cout << 0 << ",";
		else std::cout << data[i] << ",";
	}
	int m = n - 1;//避免溢出
	if (fabs(data[m]) < 1e-10)cout << 0 << ")" << endl;
	else std::cout << data[m] << ")" << endl;
}

double Vector::modulus()const {
	double result = 0;
	for (int i = 0; i < n; i++) {
		result += data[i] * data[i];
	}
	return sqrt(result);
}

Vector Vector::operator+(const Vector& other)const {
	if (n != other.n) {
		throw invalid_argument("Vectors dimensions must match for addition.");
	}
	Vector result(n);
	for (int i = 0; i < n; i++) {
		result.data[i] = data[i] + other.data[i];
	}
	return result;
}

Vector Vector::operator-(const Vector& other)const {
	if (n != other.n) {
		throw invalid_argument("Vectors dimensions must match for subtraction.");
	}
	Vector result(n);
	for (int i = 0; i < n; i++) {
		result.data[i] = data[i] - other.data[i];
	}
	return result;
}

Vector Vector::operator-() const {
	Vector result(n);
	for (int i = 0; i < n; i++) {
		result.data[i] = -data[i];
	}
	return result;
}

double Vector::dot(const Vector& other)const {
	if (n != other.n) {
		throw invalid_argument("Vectors dimensions must match for dot production.");
	}
	double result = 0;
	for (int i = 0; i < n; i++) {
		result += data[i] * other.data[i];
	}
	return result;
}

double Vector::angle(const Vector& other)const {
	if (n != other.n) {
		throw invalid_argument("Vectors dimensions must match to get angle.");
	}
	if (modulus() == 0 || other.modulus() == 0) {
		std::cout << "The angle between the zero vector and any other vector can be any value." << endl;
		throw true;
	}
	else {
		double result = dot(other) / (modulus() * other.modulus());
		return acos(result);
	}
}

Vector Vector::operator*(const Vector& other)const {
	if (n == 1)return get(0) * other;
	else if (other.n == 1)return other.get(0) * *this;
	if (n != 3 || other.n != 3) {
		throw invalid_argument("Only three-dimensional vectors have cross/mixed product.");
	}
	Vector result(3);
	result.set(0, data[1] * other.data[2] - data[2] * other.data[1]);
	result.set(1, data[2] * other.data[0] - data[0] * other.data[2]);
	result.set(2, data[0] * other.data[1] - data[1] * other.data[0]);
	return result;
}

Vector Vector::operator*(const double& k)const {
	Vector result(n);
	for (int i = 0; i < n; i++) {
		result.set(i, k * data[i]);
	}
	return result;
}

Vector operator* (const double& k, const Vector& other) {
	Vector result(other.n);
	for (int i = 0; i < other.n; i++) {
		result.set(i, k * other.data[i]);
	}
	return result;
}

ostream& operator<<(ostream& out, const Vector& v) {
	out << v.n;
	for (size_t i = 0; i < v.n; i++) {
		out << " " << v.data[i];
	}
	return out;
}

Vector Vector::operator/(const double& k)const {
	Vector result(n);
	result.data = data;
	result = result * (1 / k);
	return result;
}

Vector Vector::operator/(const Vector& other)const {
	if (other.n == 1)return (*this) * (1.0 / other.data[0]);
	if (n != other.n) {
		throw invalid_argument("Vectors dimensions must match for division.");
	}
	if (!parallel(other)) {
		throw invalid_argument("Vectors must be parallel for division.");
	}
	if (fabs(other.modulus()) < 1e-15) {
		throw runtime_error("The divisor cannot be a zero vector.");
	}
	if (dot(other) > 0)return Vector(modulus() / other.modulus());
	else return Vector(-modulus() / other.modulus());
}

Vector Vector::operator^(const Vector& other) const {
	if (n != 1)throw invalid_argument("Vectors cannot be raised to a power.");
	if (other.n != 1)throw invalid_argument("Vectors cannot be exponentiated.");
	return Vector(pow(data[0], other.data[0]));
}

bool Vector::operator==(const Vector& other)const {
	return data == other.data;
}

double Vector::mixedProduct(const Vector& v1, const Vector& v2)const {
	if (n != 3 || v1.n != 3 || v2.n != 3)throw invalid_argument("Vectors dimensions must match for mixed production.");
	return dot(v1 * v2);
}

double Vector::scalarProjection(const Vector& other)const {
	if (n != other.n) {
		throw invalid_argument("Vectors dimensions must match for projection.");
	}
	if (other.modulus() == 0) {
		throw invalid_argument("It is not possible to project a vector onto the zero vector.");
	}
	if (modulus() == 0)return 0;
	else return modulus() * cos(angle(other));
}

Vector Vector::projectionVector(const Vector& other)const {
	if (n != other.n) {
		throw invalid_argument("Vectors dimensions must match for projection.");
	}
	if (other.modulus() == 0) {
		throw invalid_argument("It is not possible to project a vector onto the zero vector.");
	}
	Vector result;
	result = other * (scalarProjection(other) / other.modulus());
	return result;
}

bool Vector::vertical(const Vector& other)const {
	if (n != other.n) {
		throw invalid_argument("Their dimensions do not match.");
	}
	return (dot(other) == 0);
}

bool Vector::parallel(const Vector& other)const {
	if (n != other.n) {
		throw invalid_argument("Their dimensions do not match.");
	}
	if (modulus() == 0 || other.modulus() == 0)return true;
	else return (angle(other) == 0 || angle(other) == PI);
}

void Vector::newInput(map<string, Vector>& vectors) {
	int n;
	map<string, double>variables = {
		{"PI",3.14159265358979323846264},{"E",2.7182818284590452353602874}
	};
	string name, _a, _n;
	double a;
	cout << "The name of vectors can only consist of letters." << endl;
	cout << "Enter \"end\" to finish definition." << endl;
	while (1) {
		bool invalidInput = false;
		cout << "Enter the name of the vector:" << endl;
		cin >> name;
		for (char i : name) {
			if (!isalpha(i)) {
				cout << "Invalid name." << endl;
				invalidInput = true;
				break;
			}
		}
		if (invalidInput)continue;
		if (name == "end")break;
		cout << "Enter the dimension of the vector:" << endl;
		cin >> _n;
		try {
			n = stoi(_n);
		}
		catch (const exception& e) {
			cerr << "Error: " << e.what() << endl;
			continue;
		}
		if (n <= 0) {
			cout << "The dimension of the vector must be positive." << endl;
			continue;
		}
		Vector v(n);
		cout << "Enter the individual components of the vector: " << endl;
		for (int i = 0; i < n; i++) {
			cin >> _a;
			try {
				a = Normal::parsen(_a, variables);
			}
			catch (const exception& e) {
				cerr << "Error: " << e.what() << endl;
				i--;
				continue;
			}
			v.set(i, a);
		}
		vectors[name] = v;
		storev(vectors);
		cout << "The Vector " + name + " is successfully created." << endl;
	}
}

Vector Vector::parseFunctionv(const string& expr, const map<string, Vector>& vectors) {
	size_t currentPos = 0;
	string identifier, vector;
	Vector v1, v2;
	while (currentPos < expr.length() && (isalpha(expr[currentPos]) || expr[currentPos] == '_')) {
		identifier += expr[currentPos++];
	}
	if (expr[currentPos] == '(') {
		++currentPos;
		v1 = parseExpressionv(expr, currentPos, vectors);
		if (expr[currentPos] == ',') {
			vector = "";
			++currentPos;
			v2 = parseExpressionv(expr, currentPos, vectors);
			if (expr[currentPos] == ')' && currentPos + 1 == expr.length()) {
				if (identifier == "para") {
					cout << (v1.parallel(v2) ? "The two vectors are parallel." : "The two vectors are not parallel.") << endl;
					throw true;
				}
				else if (identifier == "vert") {
					cout << (v1.vertical(v2) ? "The two vectors are vertical." : "The two vectors are not vertical.") << endl;
					throw true;
				}
			}
		}
	}
	currentPos = 0;
	Vector result = parseExpressionv(expr, currentPos, vectors);
	if (currentPos == expr.length())return result;
	else throw invalid_argument("Unexpected character: " + string(1, expr[currentPos]));
}

Vector Vector::parseExpressionv(const string& expr, size_t& currentPos, const map<string, Vector>& vectors) {
	Vector result = parseTermv(expr, currentPos, vectors);

	while (currentPos < expr.length()) {
		char op = expr[currentPos];
		if (vop.find(op) != vop.end()) {
			++currentPos;
			Vector rhs = parseTermv(expr, currentPos, vectors);
			switch (op) {
			case '+': result = result + rhs; break;
			case '-': result = result - rhs; break;
			default: throw runtime_error("Unknown operator.");
			}
		}
		else {
			break;
		}
	}

	return result;
}

Vector Vector::parseTermv(const string& expr, size_t& currentPos, const map<string, Vector>& vectors) {
	Vector result = parsePowerv(expr, currentPos, vectors);
	char op;

	while (currentPos < expr.length()) {
		op = expr[currentPos];
		if (vop.find(op) != vop.end()) {
			if (vop[op] == 2) { // 只处理乘法和除法
				++currentPos;
				Vector rhs = parsePowerv(expr, currentPos, vectors);
				switch (op) {
				case '*':
					result = result * rhs;
					break;
				case '/':
					if (rhs == Vector(0.0)) throw runtime_error("Division by zero error.");
					result = result / rhs;
					break;
				default:
					throw runtime_error("Unknown operator.");
				}
			}
			else {
				// 如果遇到加法或减法，应该停止解析当前项
				break;
			}
		}
		else {
			break;
		}
	}
	return result;
}

Vector Vector::parsePowerv(const string& expr, size_t& currentPos, const map<string, Vector>& vectors) {
	Vector result = 0.0; // 初始化result
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
			result = sign * parseExpressionv(expr, currentPos, vectors);
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
				Vector argument1 = parseExpressionv(expr, currentPos, vectors);
				if (expr[currentPos] == ')') {
					++currentPos; // 消耗 ')'
					auto it = functionv1.find(identifier);
					if (it != functionv1.end()) {
						result = sign * it->second(argument1);
					}
					else {
						throw runtime_error("Unknown function: " + identifier);
					}
				}
				else if (expr[currentPos] == ',') {
					++currentPos;
					Vector argument2 = parseExpressionv(expr, currentPos, vectors);
					if (expr[currentPos] == ')') {
						++currentPos;
						auto it = functionv2.find(identifier);
						if (it != functionv2.end()) {
							result = sign * it->second(argument1, argument2);
						}
						else {
							throw runtime_error("Unknown function: " + identifier);
						}
					}
					else if (expr[currentPos] == ',') {
						++currentPos;
						Vector argument3 = parseExpressionv(expr, currentPos, vectors);
						if (expr[currentPos] == ')') {
							++currentPos;
							if (identifier == "mix") {
								result = sign * argument1.mixedProduct(argument2, argument3);
							}
							else {
								throw runtime_error("Unknown function : " + identifier);
							}
						}
						else {
							throw runtime_error("Missing closing parenthesis.");
						}
					}
					else {
						throw runtime_error("Missing closing parenthesis.");
					}
				}
				else {
					throw runtime_error("Missing closing parenthesis.");
				}
			}
			else { // 否则视为复数
				auto it = vectors.find(identifier);
				if (it != vectors.end()) {
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
		Vector rhs = parsePowerv(expr, currentPos, vectors);
		result = result ^ rhs;
	}
	return result;
}

void Vector::newVector() {
	map<string, Vector>vectors;
	char choice;

	cout << "Welcome to the Vector Calculator!" << endl;
	cout << "Do you want to use the variables you defined before?(Y/N)" << endl
		<< "If no,the variable list will be cleared." << endl;
	do {
		cin >> choice;
		if (choice == 'Y') {
			vectors = loadv();
			cout << "The variables are avaliable now." << endl;
		}
		else if (choice == 'N') {
			cout << "The variable list is cleared." << endl;
		}
		else cout << "Invalid choice. Choose again." << endl;
	} while (choice != 'Y' && choice != 'N');

	vectors["PI"] = Vector(3.14159265358979323846264);
	vectors["E"] = Vector(2.7182818284590452353602874);
	vectors["ANS"] = Vector();
	storev(vectors);
	string expression, name;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl << "There are some commands you can use:" << endl;
	cout << "define - to define some vector variables." << endl;
	cout << "delete - to delete some vector variables." << endl;
	cout << "show - to find out which variables are avaliable." << endl;
	cout << "function_list - to find out which functions are available." << endl;
	cout << "end - to quit this mode." << endl;
	cout << "save - to save the result in a vector." << endl;
	cout << "The result of the previous calculation is automatically stored in \"ANS\"." << endl << endl;

	while (1) {
		cout << "Enter an expression:" << endl;
		getline(cin, expression);
		if (expression == "define") {
			newInput(vectors);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (expression == "delete") {
			deletev(vectors);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (expression == "show") {
			showv(vectors);
			continue;
		}
		if (expression == "function_list") {
			cout << endl;
			cout << "The following functions can only be used individually with defined vectors:" << endl;
			cout << "Parallel determination - para(V,V)" << endl;
			cout << "Perpendicular determination - vert(V,V)" << endl;
			cout << "The following functions can be used with expressions:" << endl;
			cout << "Modulus - R(V)" << endl;
			cout << "Angle - ang(V,V)" << endl;
			cout << "Scalar projection - SP(V,V)" << endl;
			cout << "Projection vector - PV(V,V)" << endl;
			cout << "Dot production - dot(V,V)" << endl;
			cout << "Mix production - mix(V,V,V)" << endl;
			cout << "Sine - sin(R)" << endl;
			cout << "Cosine - cos(R)" << endl;
			cout << "Tangent - tan(R)" << endl;
			cout << "Square root - sqrt(R)" << endl;
			cout << "Degree - deg(R)" << endl;
			cout << "Radian - rad(R)" << endl;
			cout << "Natural logarithm - ln(R) or log(R)" << endl << endl;
			continue;
		}
		if (expression == "save") {
			bool invalidSave = false;
			cout << "Enter the name of vector where you want to store the result:" << endl;
			cin >> name;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			for (char i : name) {
				if (!isalpha(i)) {
					cout << "Invalid name." << endl;
					invalidSave = true;
					break;
				}
			}
			if (invalidSave)continue;
			else {
				vectors[name] = vectors["ANS"];
				storev(vectors);
				cout << "The result is successfully saved in Vector " + name << endl;
			}
			continue;
		}
		if (expression == "end") break;
		try {
			cout << "The result is: " << endl;
			Vector result = parseFunctionv(expression, vectors);
			vectors["ANS"] = result;
			storev(vectors);
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

Vector Vector::R(const Vector& v) {
	return Vector(v.modulus());
}

Vector Vector::sinv(const Vector& v) {
	if (v.n != 1)throw runtime_error("Invalid operation for vectors.");
	return Vector(sin(v.data[0]));
}

Vector Vector::cosv(const Vector& v) {
	if (v.n != 1)throw runtime_error("Invalid operation for vectors.");
	return Vector(cos(v.data[0]));
}

Vector Vector::tanv(const Vector& v) {
	if (v.n != 1)throw runtime_error("Invalid operation for vectors.");
	return Vector(tan(v.data[0]));
}

Vector Vector::lnv(const Vector& v) {
	if (v.n != 1)throw runtime_error("Invalid operation for vectors.");
	return Vector(log(v.data[0]));
}

Vector Vector::dotProduct(const Vector& v1, const Vector& v2) {
	return Vector(v1.dot(v2));
}

Vector Vector::ang(const Vector& v1, const Vector& v2) {
	return Vector(v1.angle(v2));
}

Vector Vector::SP(const Vector& v1, const Vector& v2) {
	return Vector(v1.scalarProjection(v2));
}

Vector Vector::PV(const Vector& v1, const Vector& v2) {
	return v1.projectionVector(v2);
}

Vector Vector::deg(const Vector& rad) {
	if (rad.getDimension() != 1)throw invalid_argument("The radian must be a real number.");
	return Vector(rad / PI * 180);
}

Vector Vector::rad(const Vector& deg) {
	if (deg.getDimension() != 1)throw invalid_argument("The degree must be a real number.");
	return Vector(deg / 180 * PI);
}

Vector Vector::sqrtv(const Vector& v) {
	if (v.n != 1)throw runtime_error("Invalid operation for vectors.");
	return Vector(sqrt(v.data[0]));
}
