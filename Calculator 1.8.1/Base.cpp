#include "Base.h"

using namespace std;

map<char, int>Base::bop = {
	{'&',0},{'|',0},{'^',0},{'+',1},{'-',1},{'*',2},{'/',2}
};

Base::Base(const ull& _base, const ull& _data) {
	if (_base < 2) {
		throw invalid_argument("The base cannot be less than 2.");
	}
	base = _base;
	data = _data;
}

ull Base::length()const {
	if (data == 0)return 1;
	ull temp = data, len = 0;
	while (temp > 0) {
		temp /= base;
		len++;
	}
	return len;
}

void Base::print()const {
	ull len = length();
	ull* print = new ull[len + 1];
	for (int i = 0; i < len + 1; i++) {
		print[i] = 0;
	}
	ull temp = data;
	for (int i = 0; temp > 0; i++) {
		print[i] = temp % base;
		temp /= base;
	}
	if (base <= 10) {
		for (int i = 0; i < len; i++) {
			cout << print[len - i - 1];
		}
		cout << endl;
	}
	else if (base <= 36) {
		for (int i = 0; i < len; i++) {
			if (print[len - i - 1] >= 10)
				cout << static_cast<char>(static_cast<ull>('A') - 10 + print[len - i - 1]);
			else cout << print[len - i - 1];
		}
		cout << endl;
	}
	else {
		for (int i = 0; i < len; i++) {
			cout << print[len - i - 1] << " ";
		}
		cout << endl;
	}
	delete[] print;
}

//for bases from 2 to 10
Base Base::enterBase1(const ull& base, const string& num) {
	ull len = num.length();
	ull result = 0;
	for (int i = 0; i < len; i++) {
		if (num[i] >= '0' + base || num[i] < '0')
			throw invalid_argument("Entered a character that does not exist in this numeral system.");
	}
	for (int i = 0; i < len; i++) {
		result *= base;
		result += static_cast<ull>(num[i] - '0');
	}
	return Base(base, result);
}

//for bases from 11 to 36
Base Base::enterBase2(const ull& base, const string& num) {
	ull len = num.length();
	ull result = 0;
	for (int i = 0; i < len; i++) {
		if (num[i] >= static_cast<ull>('A') - 10 + base || num[i] < '0' || (num[i] > '9' && num[i] < 'A'))
			throw invalid_argument("Entered a character that does not exist in this numeral system.");
	}
	for (int i = 0; i < len; i++) {
		result *= base;
		if (num[i] < '9' + 1)result += static_cast<ull>(num[i] - '0');
		else result += static_cast<ull>(num[i] - 'A' + 10);
	}
	return Base(base, result);
}

//for bases greater than 36
Base Base::enterBase3(const ull& base, const string& num) {
	size_t len = num.length();
	ull digit = 0, result = 0;
	for (int i = 0; i < len; i++) {
		int m = i - 1;//避免溢出
		if ((i == 0 || num[m] == ' ') && num[i] == ' ')continue;
		else if (num[i] == ' ') {
			result *= base;
			result += digit;
			digit = 0;
		}
		else if (num[i] < '0' || num[i] > '9') {
			throw invalid_argument("Entered an invalid character in this numeral system.");
		}
		else if (i == len - 1) {
			digit *= 10;
			digit += static_cast<ull>(num[i] - '0');
			if (digit >= base) {
				throw invalid_argument("Entered an invalid character in this numeral system.");
			}
			result *= base;
			result += digit;
			digit = 0;
		}
		else {
			digit *= 10;
			digit += static_cast<ull>(num[i] - '0');
			if (digit >= base) {
				throw invalid_argument("Entered an invalid character in this numeral system.");
			}
		}
	}
	return Base(base, result);
}

Base Base::operator+(const Base& b)const {
	if (base == 10) {
		return Base(b.base, data + b.data);
	}
	else if (b.base == 10) {
		return Base(base, data + b.data);
	}
	else if (base != b.base)throw invalid_argument("The bases do not match for addition.");
	Base result(base, data + b.data);
	return result;
}

Base Base::operator-(const Base& b)const {
	if (base == 10) {
		return Base(b.base, data - b.data);
	}
	else if (b.base == 10) {
		return Base(base, data - b.data);
	}
	else if (base != b.base)throw invalid_argument("The bases do not match for subtraction.");
	Base result(base, data - b.data);
	return result;
}

Base Base::operator*(const Base& b)const {
	if (base == 10) {
		return Base(b.base, data * b.data);
	}
	else if (b.base == 10) {
		return Base(base, data * b.data);
	}
	else if (base != b.base)throw invalid_argument("The bases do not match for multiplication.");
	Base result(base, data * b.data);
	return result;
}

Base Base::operator/(const Base& b)const {
	if (b.data == 0)throw runtime_error("The divisor cannot be 0.");
	if (base == 10) {
		return Base(b.base, data / b.data);
	}
	else if (b.base == 10) {
		return Base(base, data / b.data);
	}
	else if (base != b.base)throw invalid_argument("The bases do not match for division.");
	Base result(base, data / b.data);
	return result;
}

bool Base::operator>(const Base& b)const {
	if (data > b.data)return true;
	else return false;
}

bool Base::operator<(const Base& b)const {
	if (data < b.data)return true;
	else return false;
}

bool Base::operator==(const Base& b)const {
	if (data == b.data)return true;
	else return false;
}

bool Base::operator!=(const Base& b) const {
	if (data == b.data)return false;
	else return true;
}

bool Base::operator>=(const Base& b)const {
	if (data >= b.data)return true;
	else return false;
}

bool Base::operator<=(const Base& b)const {
	if (data <= b.data)return true;
	else return false;
}

Base Base::operator&(const Base& bin)const {
	if (base != 2 || bin.base != 2)throw invalid_argument("Bitwise AND operation is only defined for binary.");
	ull a = data, b = bin.data;
	ull result = a & b;
	return Base(2, result);
}

Base Base::operator|(const Base& bin)const {
	if (base != 2 || bin.base != 2)throw invalid_argument("Bitwise OR operation is only defined for binary.");
	ull a = data, b = bin.data;
	ull result = a | b;
	return Base(2, result);
}

Base Base::operator^(const Base& bin) const {
	if (base != 2 || bin.base != 2)throw invalid_argument("Bitwise XOR operation is only defined for binary.");
	ull a = data, b = bin.data;
	ull result = a ^ b;
	return Base(2, result);
}

Base Base::operator~() const {
	if (base != 2)throw invalid_argument("Bitwise NOT operation is only defined for binary.");
	ull result = ~data;
	return Base(2, result);
}

Base Base::powb(const Base& b1, const Base& b2) {
	if (b1.base != b2.base && b2.base != 10 && b1.base != 10)throw invalid_argument("The bases do not match for power.");
	ull result = static_cast<ull>(pow(b1.data, b2.data));
	if (b1.base == 10)return Base(b2.base, result);
	else return Base(b1.base, result);
}

void Base::compareb(const Base& b1, const Base& b2) {
	if (b1 > b2)cout << "The first number is greater than the second." << endl;
	else if (b1 < b2)cout << "The first number is less than the second." << endl;
	else cout << "The two numbers are equal." << endl;
}

Base Base::enterBase(const ull& base, const string& num) {
	if (base < 2)throw	runtime_error("The base cannot less than 2.");
	else if (base < 11)return enterBase1(base, num);
	else if (base < 37)return enterBase2(base, num);
	else return enterBase3(base, num);
}

void Base::newInput(map<string, Base>& baseNumbers) {
	cout << "The name of the base numbers can only consist of letters" << endl;
	cout << "Enter end to finish definition." << endl;
	string name, expr, _base;
	ull base;
	Base result;
	while (1) {
		bool invalidInput = false;
		cout << "Enter the name of the number:" << endl;
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
		cout << "Enter the base of the number:" << endl;
		cin >> _base;
		try {
			base = stoull(_base);
		}
		catch (const exception& e) {
			cerr << "Error: " << e.what() << endl;
			continue;
		}
		if (base < 2) {
			cout << "The base cannot less than 2." << endl;
			continue;
		}
		cout << "Enter the number in this numeral system:" << endl;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, expr);
		try {
			result = enterBase(base, expr);
		}
		catch (const exception& e) {
			cerr << "Error: " << e.what() << endl;
			continue;
		}
		baseNumbers[name] = result;
		cout << "The Base Number " + name + " is successfully created." << endl;
		storeb(baseNumbers);
	}
}

void Base::newBase() {
	map<string, Base>numbers;
	char choice;

	cout << "Welcome to the Base Calculator!" << endl;
	cout << "Do you want to use the variables you defined before?(Y/N)" << endl
		<< "If no,the variable list will be cleared." << endl;

	do {
		cin >> choice;
		if (choice == 'Y') {
			numbers = loadb();
			cout << "The variables are avaliable now." << endl;
		}
		else if (choice == 'N') {
			cout << "The variable list is cleared." << endl;
		}
		else {
			cout << "Invalid choice. Choose again." << endl;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	} while (choice != 'Y' && choice != 'N');

	numbers["ANS"] = Base();
	storeb(numbers);
	string expression, name;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl << "There are some commands you can use:" << endl;
	cout << "define - to define some base number variables." << endl;
	cout << "delete - to delete some base number variables." << endl;
	cout << "show - to find out which variables are avaliable." << endl;
	cout << "function_list - to find out which functions are available." << endl;
	cout << "end - to quit this mode." << endl;
	cout << "save - to save the result in a base number." << endl;
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
			deleteb(numbers);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (expression == "show") {
			showb(numbers);
			continue;
		}
		if (expression == "function_list") {
			cout << "The following function can only be used individually." << endl;
			cout << "Compare - compare(B,B)" << endl;
			cout << "The following function can be used in an arthmetic expression." << endl;
			cout << "Change base - change(B,N+)" << endl;
			cout << "Power - pow(B,B)" << endl;
			cout << "Check base - base(B)" << endl;
			continue;
		}
		if (expression == "save") {
			bool invalidSave = false;
			cout << "Enter the name of base number where you want to store the result:" << endl;
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
				cout << "The result is successfully saved in Base Number " + name << endl;
				storeb(numbers);
			}
			continue;
		}
		if (expression == "end") break;
		try {
			cout << "The result is: " << endl;
			Base result = parseFunctionb(expression, numbers);
			numbers["ANS"] = result;
			storeb(numbers);
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

Base Base::parseFunctionb(const string& expr, const map<string, Base>& baseNumbers) {
	size_t currentPos = 0;
	string identifier, argument1, argument2;
	while (currentPos < expr.length() && (isalpha(expr[currentPos]) || expr[currentPos] == '_')) {
		identifier += expr[currentPos++];
	}
	if (expr[currentPos] == '(') {
		currentPos++;
		Base b1 = parseExpressionb(expr, currentPos, baseNumbers);
		if (expr[currentPos] == ',') {
			currentPos++;
			Base b2 = parseExpressionb(expr, currentPos, baseNumbers);
			if (expr[currentPos] == ')' && currentPos + 1 == expr.length()) {
				if (identifier == "compare") {
					compareb(b1, b2);
					throw true;
				}
			}
		}
	}
	currentPos = 0;
	Base result=parseExpressionb(expr, currentPos, baseNumbers);
	if (currentPos == expr.length())return result;
	else throw invalid_argument("Unexpected character: " + string(1, expr[currentPos]));
}

Base Base::parseExpressionb(const string& expr, size_t& currentPos, const map<string, Base>& baseNumbers) {
	Base result = parseAdditionb(expr, currentPos, baseNumbers);
	while (currentPos < expr.length()) {
		char op = expr[currentPos];
		if (bop.find(op) != bop.end()) {
			++currentPos;
			Base rhs = parseAdditionb(expr, currentPos, baseNumbers);
			switch (op) {
			case'&': result = result & rhs; break;
			case'|': result = result | rhs; break;
			case'^': result = result ^ rhs; break;
			default: throw runtime_error("Unknown operator.");
			}
		}
		else {
			break;
		}
	}
	return result;
}

Base Base::parseAdditionb(const string& expr, size_t& currentPos, const map<string, Base>& baseNumbers) {
	Base result = parseTermb(expr, currentPos, baseNumbers);

	while (currentPos < expr.length()) {
		char op = expr[currentPos];
		if (bop.find(op) != bop.end()) {
			if (bop[op] == 1) {//只处理加法和减法
				++currentPos;
				Base rhs = parseTermb(expr, currentPos, baseNumbers);
				switch (op) {
				case '+': result = result + rhs; break;
				case '-': result = result - rhs; break;
				default: throw runtime_error("Unknown operator.");
				}
			}//如遇到位运算，则停止解析该多项式
			else break;
		}
		else break;
	}

	return result;
}

Base Base::parseTermb(const string& expr, size_t& currentPos, const map<string, Base>& baseNumbers) {
	Base result = parsePowerb(expr, currentPos, baseNumbers);
	char op;

	while (currentPos < expr.length()) {
		op = expr[currentPos];
		if (bop.find(op) != bop.end()) {
			if (bop[op] == 2) { // 只处理乘法和除法
				++currentPos;
				Base rhs = parsePowerb(expr, currentPos, baseNumbers);
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

Base Base::parsePowerb(const string& expr, size_t& currentPos, const map<string, Base>& baseNumbers) {
	Base result;
	ull num;
	if (expr[currentPos] == '~') {
		++currentPos;
		result = ~parsePowerb(expr, currentPos, baseNumbers);
	}
	else if (expr[currentPos] == '(') {
		++currentPos;
		result = parseExpressionb(expr, currentPos, baseNumbers);
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
		result = stoull(number);
	}
	else if (isalpha(expr[currentPos])) { // 支持函数和变量
		string identifier;
		while (currentPos < expr.length() && (isalpha(expr[currentPos]) || expr[currentPos] == '_')) {
			identifier += expr[currentPos++];
		}
		if (expr[currentPos] == '(') { // 检查是否为函数
			currentPos++;
			if (identifier == "base") {
				Base b1 = parseExpressionb(expr, currentPos, baseNumbers);
				if (expr[currentPos] == ')') {
					currentPos++;
					result = Base(b1.base);
				}
				else throw invalid_argument("Missing closing parenthesis.");
			}
			else if (identifier == "num") {
				Base b1 = parseExpressionb(expr, currentPos, baseNumbers);
				if (expr[currentPos] == ')') {
					currentPos++;
					result = Base(b1.data);
				}
				else throw invalid_argument("Missing closing parenthesis.");
			}
			else if (identifier == "pow") {
				Base b1 = parseExpressionb(expr, currentPos, baseNumbers);
				if (expr[currentPos] == ',') {
					currentPos++;
					Base b2 = parseExpressionb(expr, currentPos, baseNumbers);
					if (expr[currentPos] == ')') {
						currentPos++;
						result = powb(b1, b2);
					}
					else throw invalid_argument("Missing closing parenthesis.");
				}
				else throw invalid_argument("Unexpected character: " + string(1, expr[currentPos]));
			}
			else if (identifier == "change") {
				Base b1 = parseExpressionb(expr, currentPos, baseNumbers);
				if (expr[currentPos] == ',') {
					string _num;
					currentPos++;
					while (currentPos < expr.length() && isdigit(expr[currentPos])) {
						_num += expr[currentPos++];
					}
					num = stoull(_num);
					if (expr[currentPos] == ')') {
						currentPos++;
						result = Base(num, b1.data);
					}
					else throw invalid_argument("Missing closing parenthesis.");
				}
				else throw invalid_argument("Unexpected character: " + string(1, expr[currentPos]));
			}
			else throw invalid_argument("Unknown function: " + identifier);
		}
		else { // 否则视为变量
			auto it = baseNumbers.find(identifier);
			if (it != baseNumbers.end()) {
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
	return result;
}

ostream& operator<<(ostream& out, const Base& b) {
	out << b.base << " " << b.data;
	return out;
}