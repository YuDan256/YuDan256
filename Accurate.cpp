#include "Accurate.h"

ostream& operator<<(ostream& out, const Interger& num) {
	int n = static_cast<int>(num.data.size());
	for (int i = n - 1; i > -1; i--) {
		out << num.data[i];
	}
	return out;
}

Interger::Interger(const ull& num) {
	ull n = num;
	vector<int>result;
	do {
		int a = n % 10;
		result.push_back(a);
		n /= 10;
	} while (n > 0);
	data = result;
}

bool Interger::operator==(const Interger& n) const {
	return (data == n.data);
}

bool Interger::operator>(const Interger& n) const {
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

bool Interger::operator<(const Interger& n) const {
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

bool Interger::operator>=(const Interger& n) const {
	return !(*this < n);
}

bool Interger::operator<=(const Interger& n) const {
	return !(*this > n);
}

bool Interger::operator!=(const Interger& n) const {
	return data != n.data;
}

Interger Interger::operator+(const Interger& n) const {
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
	return Interger(result);
}

Interger Interger::operator-(const Interger& n) const {
	if (*this < n) {
		cerr << "The first number cannot be less than the second!" << endl;
		return Interger();//´íÎóÌáÊ¾
	}

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
	return Interger(result);
}

Interger Interger::operator*(const Interger& n) const {
	size_t a = data.size(), b = n.data.size();
	vector<int>result(a + b, 0);
	for (size_t i = 0; i < a; i++) {
		for (size_t j = 0; j < b; j++) {
			int c = (data[i]) * (data[j]);
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
	return Interger(result);
}

Interger Interger::operator/(const Interger& n) const {
	Interger n1 = *this;
	vector<int>result;
	do {
	} while (n1 >= n);
	return Interger(result);
}
