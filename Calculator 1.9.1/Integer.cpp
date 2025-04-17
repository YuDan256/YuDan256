#include "Integer.h"

using namespace std;

map<char, int> Integer::iop = { {'+',1},{'-',1},{'*',2},{'/',2},{'%',2},{ '^',3 } };
map<string, Integer(*)(const Integer&)>Integer::functioni1 = {
	{"abs",fabs},{"sqrt",sqrt},{"sgn",sgn},
};
map<string, Integer(*)(const Integer&, const Integer&)>Integer::functioni2 = {
	{"gcd",gcd},{"lcm",lcm},{"pow",pow}
};

ostream& operator<<(ostream& out, const Integer& num) {
	if (!num.sign)out << '-';
	int n = static_cast<int>(num.data.size());
	for (int i = n - 1; i > -1; i--) {
		out << num.data[i];
	}
	return out;
}

istream& operator>>(istream& in, Integer& num) {
	string expr;
	in >> expr;
	if (expr[0] != '+' && expr[0] != '-' && !isdigit(expr[0])) {
		cerr << "Invalid input!" << endl;
		return in;
	}
	for (int i = 1; i < static_cast<int>(expr.length()); i++) {
		if (expr[i] < '0' || expr[i] > '9') {
			cerr << "Invalid input!" << endl;
			return in;
		}
	}
	num = Integer(expr);
	return in;
}

Integer::Integer(const char* num) {
	sign = true;
	data = { 0 };
	int len = 0;
	vector<int>result;
	if (num[0] != '+' && num[0] != '-' && !isdigit(num[0])) {
		throw invalid_argument("Invalid input!");
	}
	if ((num[0] == '+' || num[0] == '-') && num[1] == '\0') {
		throw invalid_argument("Invalid input!");
	}
	while (num[len] != '\0')len++;
	for (int i = len - 1; i > 0; i--) {
		if (num[i] < '0' || num[i] > '9') {
			throw invalid_argument("Invalid input!");
		}
		result.push_back(num[i] - '0');
	}
	if (num[0] == '+')sign = true;
	else if (num[0] == '-')sign = false;
	else {
		sign = true;
		result.push_back(num[0] - '0');
	}
	while (result.size() > 1 && result.back() == 0) {
		result.pop_back();
	}
	data = result;
}

Integer::Integer(const string& num) {
	sign = true;
	data = { 0 };
	vector<int>result;
	string n = num;
	if (num[0] == '+')erase(n, '+');
	else if (num[0] == '-') {
		sign = false;
		erase(n, '-');
	}
	for (int i = 0; i < static_cast<int>(n.length()); i++) {
		if (n[i] < '0' || n[i] > '9') {
			throw invalid_argument("Invalid input!");
		}
	}
	for (int i = static_cast<int>(n.length()) - 1; i > -1; i--) {
		result.push_back(n[i] - '0');
	}
	while (result.size() > 1 && result.back() == 0) {
		result.pop_back();
	}
	data = result;
}

bool Integer::operator==(const Integer& n) const {
	return (data == n.data && sign == n.sign);
}

bool Integer::operator>(const Integer& n) const {
	if (sign == 1 && n.sign == 1) {
		size_t a = data.size(), b = n.data.size();
		vector<int>n1 = data, n2 = n.data;
		if (a > b)return true;
		else if (a < b)return false;
		else {
			int len = static_cast<int>(a);
			for (int i = len - 1; i > -1; i--) {
				if (n1[i] > n2[i])return true;
				else if (n1[i] < n2[i])return false;
			}
			return false;
		}
	}
	else if (sign == 1 && n.sign == 0) {
		return (isZero() && n.isZero()) == false;
	}
	else if (sign == 0 && n.sign == 1)return false;
	else return (-(*this) < (-n));
}

bool Integer::operator<(const Integer& n) const {
	if (sign == 1 && n.sign == 1) {
		size_t a = data.size(), b = n.data.size();
		vector<int>n1 = data, n2 = n.data;
		if (a < b)return true;
		else if (a > b)return false;
		else {
			int len = static_cast<int>(a);
			for (int i = len - 1; i > -1; i--) {
				if (n1[i] < n2[i])return true;
				else if (n1[i] > n2[i])return false;
			}
			return false;
		}
	}
	else if (sign == 0 && n.sign == 1) {
		return (isZero() && n.isZero()) == false;
	}
	else if (sign == 1 && n.sign == 0)return false;
	else return (-(*this) > (-n));
}

bool Integer::operator>=(const Integer& n) const {
	return !(*this < n);
}

bool Integer::operator<=(const Integer& n) const {
	return !(*this > n);
}

bool Integer::operator!=(const Integer& n) const {
	return data != n.data;
}

Integer Integer::operator+(const Integer& n) const {
	if (sign && n.sign) {
		size_t a = data.size(), b = n.data.size();
		vector<int>n1 = data, n2 = n.data;
		if (a > b)for (size_t i = b; i < a; i++)n2.push_back(0);
		else for (size_t i = a; i < b; i++)n1.push_back(0);
		n1.push_back(0);
		n2.push_back(0);

		size_t pos = 0, len = n1.size();
		vector<int>result(len);
		while (pos < len) {
			int a = n1[pos] + n2[pos];
			if (a > 9) {
				a -= 10;
				n1[pos + 1]++;
			}
			result[pos] = a;
			pos++;
		}
		while (result.size() > 1 && result.back() == 0) {
			result.pop_back();
		}
		return Integer(result, true);
	}
	else if (sign == 1 && n.sign == 0) return *this - (-n);
	else if (sign == 0 && n.sign == 1) return n - (-*this);
	else return -((-*this) + (-n));
}

Integer Integer::operator-(const Integer& n) const {
	if (sign && n.sign) {
		if (*this >= n) {
			size_t a = data.size(), b = n.data.size();
			vector<int>n1 = data, n2 = n.data;
			for (size_t i = b; i < a; i++)n2.push_back(0);

			size_t pos = 0, len = n1.size();
			vector<int>result(len);
			while (pos < len) {
				char a = n1[pos] - (n2[pos] - 0);
				if (a < 0) {
					a += 10;
					n1[pos + 1]--;
				}
				result[pos] = a;
				pos++;
			}
			while (result.size() > 1 && result.back() == 0) {
				result.pop_back();
			}
			return Integer(result, true);
		}
		else return -(n - *this);
	}
	else if (sign == 1 && n.sign == 0) return *this + (-n);
	else if (sign == 0 && n.sign == 1) return -(n + (-*this));
	else return (-n) - (-*this);
}

Integer Integer::operator*(const Integer& n) const {
	Integer a = fabs(*this), b = fabs(n);
	bool result_sign = sign == n.sign;
	size_t a_size = a.data.size();
	size_t b_size = b.data.size();
	if (a_size == 0 || b_size == 0 || (a.data[0] == 0 && a_size == 1) || (b.data[0] == 0 && b_size == 1)) {
		return Integer(vector<int>{0}, true);
	}
	vector<int> result(a_size + b_size, 0);
	for (size_t i = 0; i < a_size; i++) {
		for (size_t j = 0; j < b_size; j++) {
			result[i + j] += a.data[i] * b.data[j];
		}
	}
	for (size_t i = 0; i < a_size + b_size - 1; i++) {
		int carry = result[i] / 10;
		result[i] %= 10;
		result[i + 1] += carry;
	}
	size_t first_non_zero = result.size() - 1;
	while (first_non_zero > 0 && result[first_non_zero] == 0) {
		first_non_zero--;
	}
	result.resize(first_non_zero + 1);

	return Integer(result, result_sign);
}

Integer Integer::operator/(const int& divisor) const {
	if (divisor == 0) {
		throw invalid_argument("Division by zero error.");
	}
	bool b = divisor >= 0;
	int d = static_cast<int>(std::fabs(divisor));
	vector<int> result;
	int remainder = 0;
	for (int i = static_cast<int>(data.size()) - 1; i >= 0; i--) {
		remainder = remainder * 10 + data[i];
		result.insert(result.begin(), remainder / d);
		remainder %= d;
	}

	while (result.size() > 1 && result.back() == 0) {
		result.pop_back();
	}

	return Integer(result, sign == b);
}

Integer Integer::operator/(const Integer& n) const {
	if (n.isZero()) {
		throw invalid_argument("Division by zero error.");
	}
	if (n <= numeric_limits<ull>::max() - 1 && *this <= numeric_limits<ull>::max() - 1) return division1(n);
	else return division2(n);
}

Integer Integer::operator%(const Integer& n) const {
	bool result_sign = sign == n.sign;
	if (n.isZero()) {
		throw invalid_argument("Division by zero error.");
	}
	Integer a = fabs(*this), b = fabs(n);
	Integer result = a - (a / b * b);
	return result * (result_sign ? Integer(1) : Integer(-1));
}

Integer Integer::operator^(const Integer& n) const {
	return value() ^ n.value();
}

Integer Integer::operator|(const Integer& n) const {
	return value() | n.value();
}

Integer Integer::operator&(const Integer& n) const {
	return value() & n.value();
}

Integer Integer::operator~() const {
	return ~value();
}

Integer& Integer::operator+=(const Integer& n) {
	*this = *this + n;
	return *this;
}

Integer& Integer::operator-=(const Integer& n) {
	*this = *this - n;
	return *this;
}

Integer& Integer::operator*=(const Integer& n) {
	*this = *this * n;
	return *this;
}

Integer& Integer::operator/=(const Integer& n) {
	*this = *this / n;
	return *this;
}

Integer& Integer::operator%=(const Integer& n) {
	*this = *this % n;
	return *this;
}

Integer& Integer::operator^=(const Integer& n) {
	*this = *this ^ n;
	return *this;
}


Integer& Integer::operator|=(const Integer& n) {
	*this = *this | n;
	return *this;
}

Integer& Integer::operator&=(const Integer& n) {
	*this = *this & n;
	return *this;
}

Integer& Integer::operator++() {
	*this = *this + 1;
	return *this;
}

Integer& Integer::operator--() {
	*this = *this - 1;
	return *this;
}

Integer Integer::operator++(int) {
	Integer temp = *this;
	*this = *this + 1;
	return temp;
}

Integer Integer::operator--(int) {
	Integer temp = *this;
	*this = *this - 1;
	return temp;
}

Integer Integer::operator-() const {
	Integer temp = *this;
	temp.sign = !temp.sign;
	return temp;
}

Integer Integer::operator+() const {
	return *this;
}

Integer Integer::fabs(const Integer& n) {
	return Integer(n.data, true);
}

bool Integer::getSign() const {
	return sign;
}

vector<int> Integer::getData() const {
	return data;
}

void Integer::print() const {
	cout << *this << endl;
}

ull Integer::uvalue()const {
	if (*this > numeric_limits<ull>::max())throw invalid_argument("The integer is too large.");
	ull p = 1, result = 0;
	for (int i : data) {
		result += p * i;
		p *= 10;
	}
	return result;
}

long long Integer::value()const {
	if (*this > numeric_limits<long long>::max())throw invalid_argument("The integer is too large.");
	long long p = 1, result = 0;
	for (int i : data) {
		result += p * i;
		p *= 10;
	}
	if (!sign)result *= -1;
	return result;
}

bool Integer::isZero() const {
	if (data.size() > 1)return false;
	return data[0] == 0;
}

Integer Integer::division1(const Integer& n) const {
	bool result_sign = sign == n.sign;
	ull n1 = uvalue(), n2 = n.uvalue();
	return Integer(n1 / n2) * (result_sign ? 1 : -1);
}

Integer Integer::division2(const Integer& n) const {
	if (n.isZero()) {
		throw invalid_argument("Division by zero error.");
	}

	Integer dividend = fabs(*this);
	Integer divisor = fabs(n);
	vector<int> quotient_data;
	bool quotient_sign = sign == n.sign;

	if (dividend < divisor) {
		return Integer(vector<int>{0}, true);
	}

	Integer temp;
	Integer power = 1;
	Integer remainder = 0;

	for (int i = static_cast<int>(dividend.data.size()) - 1; i >= 0; i--) {
		remainder = remainder * 10 + Integer(vector<int>{dividend.data[i]}, true);
		int q = 0;

		Integer low(0), high(remainder);
		while (low <= high) {
			Integer mid = (low + high) / 2;
			Integer product = mid * divisor;
			if (product <= remainder) {
				q = mid.data[0];
				low = mid + 1;
			}
			else {
				high = mid - 1;
			}
		}

		quotient_data.insert(quotient_data.begin(), q);
		remainder = remainder - (divisor * q);
	}

	while (quotient_data.size() > 1 && quotient_data.back() == 0) {
		quotient_data.pop_back();
	}

	return Integer(quotient_data, true) * (quotient_sign ? 1 : -1);
}

vector<Integer> Integer::read_primes(const string& filename, const Integer& max_size) {
	vector<Integer> primes;
	ifstream file(filename);
	if (!file.is_open()) {
		throw runtime_error("无法打开文件: " + filename);
	}

	string line;

	while (getline(file, line)) {
		if (!line.empty()) {
			Integer prime(line);
			if (prime < max_size) {
				primes.emplace_back(prime);
			}
			else break;
		}
	}

	file.close();
	return primes;
}

Integer Integer::pow(const Integer& n) const {
	if (isZero() && n <= 0)throw invalid_argument("When the base is zero, the exponent must be positive.");
	if (n < 0)return 0;
	Integer result = 1;
	for (Integer i = 0; i < n; i++) {
		result *= *this;
	}
	return result;
}

Integer Integer::gcd(const Integer& n) const {
	Integer a = fabs(*this);
	Integer b = fabs(n);

	if (b.isZero()) {
		return a;
	}

	while (true) {
		Integer remainder = a % b;
		if (remainder.isZero()) {
			return b;
		}
		a = b;
		b = remainder;
	}
}

Integer Integer::lcm(const Integer& n) const {
	Integer a = fabs(*this), b = fabs(n);
	return a * b / gcd(a, b);
}

map<Integer, Integer> Integer::factorization() const {
	Integer num = *this;
	if (num <= 1) {
		throw invalid_argument("The number must be greater than 1.");
	}

	map<Integer, Integer> result;

	while (num % 2 == 0) {
		result[2]++;
		num /= 2;
	}

	ifstream file("D:\\Prime\\Prime.txt");
	if (!file.is_open()) {
		throw runtime_error("Failed to open file: D:\\Prime\\Prime.txt");
	}

	string line;
	Integer current_prime;

	while (getline(file, line)) {
		if (line.empty()) continue;

		current_prime = Integer(line);
		if (current_prime * current_prime > num) {
			break;
		}

		while (num % current_prime == 0) {
			result[current_prime]++;
			num /= current_prime;
		}
	}

	file.close();
	Integer i = 3;
	if (!current_prime.isZero()) {
		i = current_prime + 2;
	}

	while (i * i <= num) {
		if (i.getData()[0] == 5 && i.getData().size() != 1) {
			i += 2;
			continue;
		}

		while (num % i == 0) {
			result[i]++;
			num /= i;
		}

		i += 2;
	}
	if (num > 1) {
		result[num]++;
	}
	return result;
}

void Integer::factor() const {
	map<Integer, Integer>factors = factorization();
	cout << "Factor\tPower" << endl;
	for (auto& pair : factors) {
		cout << pair.first << '\t' << pair.second << endl;
	}
}

Integer Integer::pow(const Integer& n1, const Integer& n2) {
	Integer result = 1;
	if (n2 <= 0 && n1 == 0)throw invalid_argument("When the base is zero, the exponent must be positive.");
	if (n2 < 0)return 0;
	for (Integer i = 0; i < n2; i++) {
		result *= n1;
	}
	return result;
}

Integer Integer::sqrt(const Integer& n) {
	return n.sqrt();
}

Integer Integer::gcd(const Integer& n1, const Integer& n2) {
	return n1.gcd(n2);
}

Integer Integer::lcm(const Integer& n1, const Integer& n2) {
	return n1.lcm(n2);
}

Integer Integer::sgn(const Integer& n) {
	if (n.isZero())return 0;
	else if (n > 0)return 1;
	else return -1;
}

Integer Integer::sqrt() const {
	if (*this < 0) {
		throw invalid_argument("Square root of negative number.");
	}
	if (*this == 0 || *this == 1) {
		return *this;
	}

	Integer low = 1;
	Integer high = *this;
	Integer result = 0;

	while (low <= high) {
		Integer mid = (low + high) / 2;
		Integer mid_squared = mid * mid;

		if (mid_squared == *this) {
			return mid;
		}
		else if (mid_squared < *this) {
			result = mid;
			low = mid + 1;
		}
		else {
			high = mid - 1;
		}
	}

	return result;
}

void Integer::input(map<string, Integer>& variables) {
	cout << "The name of the Integers can only consist of letters." << endl;
	cout << "Enter \"end\" to finish definition." << endl;
	string name, _a;
	Integer a;
	while (1) {
		bool invalidInput = false;
		cout << "Name (enter \"end\" to finish): " << endl;
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
		cout << "Integer: " << endl;
		cin >> _a;
		try {
			a = Integer(_a);
		}
		catch (const exception& e) {
			cerr << "Error: " << e.what() << endl;
			continue;
		}
		variables[name] = a;
		cout << "The Integer Number " + name + " is successfully created." << endl;
		storei(variables);
	}
}

Integer Integer::parseFunctioni(const string& expr, const map<string, Integer>& numbers) {
	size_t currentPos = 0;
	string identifier, number;
	Integer z;
	int n = 0;
	while (currentPos < expr.length() && (isalpha(expr[currentPos]) || expr[currentPos] == '_')) {
		identifier += expr[currentPos++];
	}
	if (expr[currentPos] == '(') {
		++currentPos;
		z = parseExpressioni(expr, currentPos, numbers);
		if (expr[currentPos] == ')' && currentPos + 1 == expr.length()) {
			if (identifier == "print") {
				z.print();
				throw true;
			}
			else if (identifier == "fact") {
				z.factor();
				throw true;
			}
		}
	}
	currentPos = 0;
	Integer result = parseExpressioni(expr, currentPos, numbers);
	if (currentPos == expr.length())return result;
	else throw invalid_argument("Unexpected character: " + string(1, expr[currentPos]));
}

Integer Integer::parseExpressioni(const string& expr, size_t& currentPos, const map<string, Integer>& numbers) {
	Integer result = parseTermi(expr, currentPos, numbers);

	while (currentPos < expr.length()) {
		char op = expr[currentPos];
		if (iop.find(op) != iop.end()) {
			++currentPos;
			Integer rhs = parseTermi(expr, currentPos, numbers);
			switch (op) {
			case '+':
				result += rhs;
				break;
			case '-':
				result -= rhs;
				break;
			default: throw runtime_error("Unknown operator.");
			}
		}
		else break;
	}
	return result;
}

Integer Integer::parseTermi(const string& expr, size_t& currentPos, const map<string, Integer>& numbers) {
	Integer result = parsePoweri(expr, currentPos, numbers);
	char op;

	while (currentPos < expr.length()) {
		op = expr[currentPos];
		if (iop.find(op) != iop.end()) {
			if (iop[op] == 2) { // 只处理乘法和除法
				++currentPos;
				Integer rhs = parsePoweri(expr, currentPos, numbers);
				switch (op) {
				case '*':
					result *= rhs;
					break;
				case '/':
					if (rhs.isZero()) throw runtime_error("Division by zero error.");
					result /= rhs;
					break;
				case '%':
					if (rhs.isZero()) throw runtime_error("Division by zero error.");
					result %= rhs;
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

Integer Integer::parsePoweri(const string& expr, size_t& currentPos, const map<string, Integer>& numbers) {
	Integer result = 0, sign = 1;

	if (currentPos < expr.length()) {
		if (expr[currentPos] == '+') {
			sign = 1;
			++currentPos;
		}
		else if (expr[currentPos] == '-') {
			sign = -1;
			++currentPos;
		}
	}

	if (currentPos < expr.length()) {
		if (expr[currentPos] == '(') {
			++currentPos;
			result = sign * parseExpressioni(expr, currentPos, numbers);
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
			result = sign * Integer(number);
		}
		else if (isalpha(expr[currentPos])) { // 支持函数和变量
			string identifier;
			while (currentPos < expr.length() && (isalpha(expr[currentPos]) || expr[currentPos] == '_')) {
				identifier += expr[currentPos++];
			}
			if (expr[currentPos] == '(') { // 检查是否为函数
				++currentPos; // 消耗 '('
				Integer argument = parseExpressioni(expr, currentPos, numbers);
				if (expr[currentPos] == ')') {
					++currentPos; // 消耗 ')'
					auto it = functioni1.find(identifier);
					if (it != functioni1.end()) {
						result = sign * it->second(argument);
					}
					else {
						throw runtime_error("Unknown function: " + identifier);
					}
				}
				else if (expr[currentPos] == ',') {
					++currentPos;
					string number;
					Integer argument2 = parseExpressioni(expr, currentPos, numbers);
					if (expr[currentPos] == ')') {
						++currentPos;
						auto it = functioni2.find(identifier);
						if (it != functioni2.end()) {
							result = sign * it->second(argument, argument2);
						}
						else {
							throw runtime_error("Unknown function: " + identifier);
						}
					}
					else throw runtime_error("Missing closing parenthesis.");
				}
				else throw runtime_error("Missing closing parenthesis.");
			}
			else { // 否则视为整数
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
		Integer rhs = parsePoweri(expr, currentPos, numbers);
		result = result.pow(rhs);
	}

	return result;
}

void Integer::newInteger() {
	map<string, Integer>numbers;
	char choice;

	cout << "Welcome to the Integer Calculator!" << endl << endl;
	cout << "Do you want to use the variables you defined before?(Y/N)" << endl
		<< "If no,the variable list will be cleared." << endl;

	do {
		cin >> choice;
		if (choice == 'Y') {
			numbers = loadi();
			cout << "The variables are avaliable now." << endl;
		}
		else if (choice == 'N') {
			cout << "The variable list is cleared." << endl;
		}
		else cout << "Invalid choice. Choose again." << endl;
	} while (choice != 'Y' && choice != 'N');

	numbers["ANS"] = Integer();
	storei(numbers);
	string expression, name;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl << "There are some commands you can use:" << endl;
	cout << "define - to define some integer variables." << endl;
	cout << "delete - to delete some integer variables." << endl;
	cout << "show - to find out which variables are available." << endl;
	cout << "function_list - to find out which functions are available." << endl;
	cout << "end - to quit this mode." << endl;
	cout << "save - to save the result in an integer variable." << endl;
	cout << "The result of the previous calculation is automatically stored in \"ANS\"." << endl << endl;

	while (1) {
		cout << "Enter an expression:" << endl;
		getline(cin, expression);
		if (expression == "define") {
			input(numbers);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (expression == "delete") {
			deletei(numbers);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (expression == "show") {
			showi(numbers);
			continue;
		}
		if (expression == "function_list") {
			cout << endl << "The following functions can only be used individually:" << endl;
			cout << "Print - print(Z)" << endl;
			cout << "Factorization - fact(Z)" << endl;
			cout << "The following functions can be used with expressions:" << endl;
			cout << "Absolute value - abs(Z)" << endl;
			cout << "Square root - sqrt(Z)" << endl;
			cout << "Sign - sgn(Z)" << endl;
			cout << "Power - pow(Z,Z)" << endl;
			cout << "GCD - gcd(Z,Z)" << endl;
			cout << "LCM - lcm(Z,Z)" << endl;
			cout << endl;
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
				storei(numbers);
			}
			continue;
		}
		if (expression == "end") break;
		try {
			cout << "The result is: " << endl;
			Integer result = parseFunctioni(expression, numbers);
			numbers["ANS"] = result;
			storei(numbers);
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