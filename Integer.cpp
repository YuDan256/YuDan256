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
	return (data == n.data);
}

bool Integer::operator>(const Integer& n) const {
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

bool Integer::operator<(const Integer& n) const {
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
	size_t a = data.size(), b = n.data.size();
	vector<int>result(a + b, 0);
	for (size_t i = 0; i < a; i++) {
		for (size_t j = 0; j < b; j++) {
			int c = (data[i]) * (n.data[j]);
			result[i + j] += c % 10;
			result[i + j + 1] += c / 10;
		}
	}
	for (size_t i = 0; i < a + b - 1; i++) {
		if (result[i] > 9) {
			int j = result[i];
			result[i] = j % 10;
			result[i + 1] += j / 10;
		}
	}
	while (true) {
		if (result.back() == 0 && result.size() > 1)result.pop_back();
		else break;
	}
	if (sign == n.sign)return Integer(result, true);
	else return Integer(result, false);
}

Integer Integer::operator/(const Integer& n) const {
	Integer n1 = *this;
	vector<int>temp, result;
	int last, current = 0;//�����м����
	while (true) {
		int a = 0;
		last = current;
		current = 0;
		Integer n2 = n;
		while (n2 * 10 <= n1) {
			n2 = n2 * 10;
			current++;
		}
		while (n1 >= n2) {
			n1 = n1 - n2;
			a++;
		}
		for (int i = 0; i < last - current - 1; i++)temp.push_back(0);
		temp.push_back(a);
		if (n1 < n) {
			for (int i = 0; i < current; i++)temp.push_back(0);
			break;
		}
	}
	for (int i = static_cast<int>(temp.size()) - 1; i > -1; i--) {
		result.push_back(temp[i]);
	}
	while (true) {
		if (result.back() == 0 && result.size() > 1)result.pop_back();
		else break;
	}
	if (sign == n.sign)return Integer(result, true);
	else return Integer(result, false);
}

Integer Integer::operator%(const Integer& n) const {
	Integer a = fabs(*this), b = fabs(n);
	Integer result = a - a / b * b;
	if (sign == n.sign)return result;
	else return -result;
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

Integer Integer::fabs(const Integer& n){
	return Integer(n.data, true);
}
