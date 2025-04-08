#include "Integer.h"

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
	int len = 0;
	vector<int>result;
	if (num[0] != '+' && num[0] != '-' && !isdigit(num[0])) {
		cerr << "Invalid input!" << endl;
		return;
	}
	if ((num[0] == '+' || num[0] == '-') && num[1] == '\0') {
		cerr << "Invalid input!" << endl;
		return;
	}
	while (num[len] != '\0')len++;
	for (int i = len - 1; i > 0; i--) {
		if (num[i] < '0' || num[i] > '9') {
			cerr << "Invalid input!" << endl;
			return;
		}
		result.push_back(num[i] - '0');
	}
	if (num[0] == '+')sign = true;
	else if (num[0] == '-')sign = false;
	else {
		sign = true;
		result.push_back(num[0] - '0');
	}
	data = result;
}

Integer::Integer(const string& num) {
	sign = true;
	vector<int>result;
	string n = num;
	if (num[0] == '+')erase(n, 0);
	else if (num[0] == '-') {
		sign = false;
		erase(n, 0);
	}
	for (int i = 0; i < static_cast<int>(num.length()); i++) {
		if (num[i] < '0' || num[i] > '9') {
			cerr << "Invalid input!" << endl;
			return;
		}
	}
	for (int i = static_cast<int>(num.length()) - 1; i > -1; i--) {
		result.push_back(num[i] - '0');
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
		while (true) {
			if (result.back() == 0 && result.size() > 1)result.pop_back();
			else break;
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
			while (true) {
				if (result.back() == 0 && result.size() > 1)result.pop_back();
				else break;
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
	bool result_sign = sign == n.sign;
	return Integer(result, result_sign);
}

Integer Integer::operator/(const Integer& n) const {
	if (n.uvalue() == 0) {
		throw invalid_argument("Division by zero error.");
	}

	Integer n1 = fabs(*this);
	vector<int> temp, result;
	int last = 0, current = 0;

	while (true) {
		int a = 0;
		last = current;
		current = 0;
		Integer n2 = fabs(n);
		while (n2 * 10 <= n1) {
			n2 = n2 * 10;
			current++;
		}
		while (n1 >= n2) {
			n1 = n1 - n2;
			a++;
		}
		for (int i = 0; i < last - current - 1; i++) {
			temp.push_back(0);
		}
		temp.push_back(a);
		if (n1 < n) {
			for (int i = 0; i < current; i++) {
				temp.push_back(0);
			}
			break;
		}
	}
	for (int i = static_cast<int>(temp.size()) - 1; i >= 0; i--) {
		result.push_back(temp[i]);
	}
	while (result.size() > 1 && result.back() == 0) {
		result.pop_back();
	}
	bool result_sign = sign == n.sign;
	return Integer(result, result_sign);
}

Integer Integer::operator%(const Integer& n) const {
	if (n.uvalue() == 0)throw invalid_argument("Division by zero error.");
	Integer a = fabs(*this), b = fabs(n);
	Integer result = a - a / b * b;
	if (sign == n.sign)return result;
	else return -result;
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

Integer Integer::pow(const Integer& n1, const Integer& n2) {
	Integer result = 1;
	if (n2 <= 0 && n1 == 0)throw invalid_argument("When the base is zero, the exponent must be positive.");
	if (n2 < 0)return 0;
	for (Integer i = 0; i < n2; i++) {
		result *= n1;
	}
	return result;
}
