#include "Accurate.h"

ostream& operator<<(ostream& out, const Accurate& num) {
	int n = static_cast<int>(num.data.size());
	for (int i = n - 1; i > -1; i--) {
		out << num.data[i];
	}
	return out;
}

Accurate::Accurate(const ull& num) {
	ull n = num;
	vector<char>result;
	do {
		char a = '0' + n % 10;
		result.push_back(a);
		n /= 10;
	} while (n > 0);
	data = result;
}

Accurate Accurate::operator+(const Accurate& n) const {
	size_t a = data.size(), b = n.data.size();
	vector<char>n1 = data, n2 = n.data;
	if (a > b)for (size_t i = b; i < a; i++)n2.push_back('0');
	else for (size_t i = a; i < b; i++)n1.push_back('0');
	n1.push_back('0');
	n2.push_back('0');

	size_t pos = 0, len = n1.size();
	vector<char>result(len);
	while (pos < len) {
		char a = n1[pos] - '0' + n2[pos];
		if (a > '9') {
			a -= 10;
			n1[pos + 1]++;
		}
		result[pos] = a;
		pos++;
	}
	while (true) {
		if (result.back() == '0')result.pop_back();
		else break;
	}
	return Accurate(result);
}
