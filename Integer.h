#ifndef INTEGER_H
#define INTEGER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <random>

#define INT_T template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>

using namespace std;
typedef unsigned long long ull;

class Integer {
private:
	vector<int> data;
	bool sign;

	static map<char, int>iop;
	static map<std::string, Integer(*)(const Integer&)>functioni1;
	static map<std::string, Integer(*)(const Integer&, const Integer&)>functioni2;

public:
	Integer(const vector<int>& data, const bool& sign) : data(data), sign(sign) {};
	Integer() : data({ 0 }), sign(true) {};
	Integer(const char* num);
	Integer(const string& num);

	INT_T Integer(const T& num) {
		ull n = 0;
		vector<int>result;
		if (num < 0) {
			sign = false;
			n = static_cast<ull>(num * (-1));
		}
		else {
			sign = true;
			n = static_cast<ull>(num);
		}
		do {
			result.push_back(n % 10);
			n /= 10;
		} while (n > 0);
		data = result;
	};

	friend ostream& operator<<(ostream& out, const Integer& num);
	friend istream& operator>>(istream& in, Integer& num);

	bool operator==(const Integer& n) const;
	bool operator>(const Integer& n) const;
	bool operator<(const Integer& n) const;
	bool operator>=(const Integer& n) const;
	bool operator<=(const Integer& n) const;
	bool operator!=(const Integer& n) const;

	Integer operator+(const Integer& n) const;
	Integer operator-(const Integer& n) const;
	Integer operator*(const Integer& n) const;
	Integer operator/(const int& divisor) const;
	Integer operator/(const Integer& n) const;
	Integer operator%(const Integer& n) const;
	Integer operator^(const Integer& n)const;
	Integer operator|(const Integer& n)const;
	Integer operator&(const Integer& n)const;
	Integer operator~() const;
	Integer operator<<(const Integer& n) const;
	Integer operator>>(const Integer& n) const;

	INT_T friend Integer operator+(const T& n, const Integer& m) { return m + n; }
	INT_T friend Integer operator-(const T& n, const Integer& m) { return Integer(n) - m; }
	INT_T friend Integer operator*(const T& n, const Integer& m) { return m * n; }
	INT_T friend Integer operator/(const T& n, const Integer& m) { return Integer(n) / m; }
	INT_T friend Integer operator%(const T& n, const Integer& m) { return Integer(n) % m; }
	INT_T friend Integer operator^(const T& n, const Integer& m) { return Integer(n) ^ m; }
	INT_T friend Integer operator|(const T& n, const Integer& m) { return Integer(n) | m; }
	INT_T friend Integer operator&(const T& n, const Integer& m) { return Integer(n) & m; }
	INT_T friend Integer operator<<(const T& n, const Integer& m) { return Integer(n) << m; }
	INT_T friend Integer operator>>(const T& n, const Integer& m) { return Integer(n) >> m; }

	Integer& operator=(const Integer& n);
	Integer& operator+=(const Integer& n);
	Integer& operator-=(const Integer& n);
	Integer& operator*=(const Integer& n);
	Integer& operator/=(const Integer& n);
	Integer& operator%=(const Integer& n);
	Integer& operator^=(const Integer& n);
	Integer& operator|=(const Integer& n);
	Integer& operator&=(const Integer& n);

	Integer& operator++();
	Integer& operator--();
	Integer operator++(int);
	Integer operator--(int);
	Integer operator-()const;
	Integer operator+()const;

	bool getSign()const;
	vector<int>getData()const;
	void print(const string& end = "\n")const;

	ull uvalue()const;
	size_t size()const;
	long long value() const;
	bool isZero()const;
	Integer sqrt() const;
	Integer division1(const Integer& n)const;
	Integer division2(const Integer& n)const;
	Integer pow(const Integer& n) const;
	Integer gcd(const Integer& n) const;
	Integer lcm(const Integer& n)const;
	map<Integer, Integer> factor()const;
	void factorization()const;
	static vector<Integer> read_primes(const string& filename, const Integer& max_size);
	static string to_string(const Integer& n);

	static Integer abs(const Integer& n);
	static Integer pow(const Integer& n1, const Integer& n2);
	static Integer sqrt(const Integer& n);
	static Integer gcd(const Integer& n1, const Integer& n2);
	static Integer lcm(const Integer& n1, const Integer& n2);
	static Integer sgn(const Integer& n);
	static Integer randint(const Integer& n1, const Integer& n2);
	static Integer length(const Integer& n);
	static Integer prime(const Integer& n);
	static Integer primePi(const Integer& n);
	static Integer log(const Integer& n1, const Integer& n2);
	static Integer log(const Integer& n);
	static Integer phi(const Integer& n);
	static Integer factorial(const Integer& n);
	static Integer d(const Integer& n);
	static Integer omega(const Integer& n);
	static Integer Omega(const Integer& n);
	static Integer fibonacci(const Integer& n);
	static Integer sigma(const Integer& n);
	static Integer sigma(const Integer& n1, const Integer& n2);

	static void storei(const map<string, Integer>& numbers);
	static map<string, Integer> loadi();
	static void deletei(map<string, Integer>& variables);
	static void showi(const map<string, Integer>& variables);
	static void processi(const string& expression, map<string, Integer>& variables);
	static void input(map<string, Integer>& variables);
	static void doInteger();

	static std::vector<ull> generateSmallPrimes(ull maxNumber);
	static std::vector<ull> segmentedSieve(ull low, ull high);
	static void writePrimesToFile(const std::vector<ull>& primes, const std::string& filePath, bool append);
	static void add_prime(const ull& interval);
	static ull getLastPrime(const std::string& filePath);

	static Integer parseFunctioni(const string& expr, const map<string, Integer>& numbers);
	static Integer parseExpressioni(const string& expr, size_t& currentPos, const map<string, Integer>& numbers);
	static Integer parseTermi(const string& expr, size_t& currentPos, const map<string, Integer>& numbers);
	static Integer parsePoweri(const string& expr, size_t& currentPos, const map<string, Integer>& numbers);
};

#endif
