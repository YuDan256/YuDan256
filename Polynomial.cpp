#include"Complex.h"

using namespace std;

void Complex::doPolynomial() {
	int degree;
	char operation;
	string _degree, _a, _b, _c, _d, _e;
	map<string, Complex>p = {
		{"PI",Complex(PI)},{"E",Complex(E)},{"i",Complex(0.0,1.0)}
	};
	cout << "Welcome to the Polynomial Equation Calculator!" << endl
		<< "This calculator will solve polynomial equations for you in the complex number domain." << endl;
	while (1) {
		cout << "Enter the degree of the polynomial (1~4):";
		cin >> _degree;
		try {
			degree = stoi(_degree);
		}
		catch (const exception& e) {
			cerr << "Error: " << e.what() << endl;
			continue;
		}
		if (degree >= 5) {
			cout << "Equations of degree five and higher have no algebraic solutions." << endl;
			continue;
		}
		cout << "Enter the coefficients and constant of the polynomial:" << endl;
		Complex a = 0.0, b = 0.0, c = 0.0, d = 0.0, e = 0.0;
		switch (degree) {
		case 4:
			cin >> _a >> _b >> _c >> _d >> _e;
			try {
				a = parseFunctionc(_a, p);
				b = parseFunctionc(_b, p);
				c = parseFunctionc(_c, p);
				d = parseFunctionc(_d, p);
				e = parseFunctionc(_e, p);
			}
			catch (const exception& e) {
				cerr << "Error: " << e.what() << endl;
				continue;
			}
			doDegreeFour(a, b, c, d, e);
			break;
		case 3:
			cin >> _a >> _b >> _c >> _d;
			try {
				a = parseFunctionc(_a, p);
				b = parseFunctionc(_b, p);
				c = parseFunctionc(_c, p);
				d = parseFunctionc(_d, p);
			}
			catch (const exception& e) {
				cerr << "Error: " << e.what() << endl;
				continue;
			}
			doDegreeThree(a, b, c, d);
			break;
		case 2:
			cin >> _a >> _b >> _c;
			try {
				a = parseFunctionc(_a, p);
				b = parseFunctionc(_b, p);
				c = parseFunctionc(_c, p);
			}
			catch (const exception& e) {
				cerr << "Error: " << e.what() << endl;
				continue;
			}
			doDegreeTwo(a, b, c);
			break;
		case 1:
			cin >> _a >> _b;
			try {
				a = parseFunctionc(_a, p);
				b = parseFunctionc(_b, p);
			}
			catch (const exception& e) {
				cerr << "Error: " << e.what() << endl;
				continue;
			}
			doDegreeOne(a, b);
			break;
		default:
			cout << "The degree of the polynomial cannot be less than 1." << endl;
			continue;
		}
		cout << "Do you still want to use this mode? (Y/N)" << endl;
		do {
			cin >> operation;
			if (operation == 'Y') {}
			else if (operation == 'N')return;
			else cout << "Invalid choice." << endl;
		} while (operation != 'Y' && operation != 'N');
	}
}

void Complex::doDegreeOne(const Complex& a, const Complex& b) {
	if (a == 0 && b == 0) {
		cout << "The solution is any complex number." << endl;
	}
	else if (a == 0 && !(b == 0)) {
		cout << "The equation has no solution." << endl;
	}
	else {
		Complex result = -b / a;
		cout << "The solution of the equation is:" << endl;
		cout << "x=";
		result.print();
	}
}

void Complex::doDegreeTwo(const Complex& a, const Complex& b, const Complex& c) {
	if (a == 0) {
		doDegreeOne(b, c);
		return;
	}
	Complex result1, result2;
	result1 = (Complex::sqrtc(b * b - 4 * a * c) - b) / (2 * a);
	result2 = (-Complex::sqrtc(b * b - 4 * a * c) - b) / (2 * a);
	cout << "The solutions of the equation are:" << endl;
	cout << "x1 = ";
	result1.print();
	cout << "x2 = ";
	result2.print();
}

void Complex::doDegreeThree(const Complex& a, const Complex& b, const Complex& c, const Complex& d) {
	if (a == 0) {
		doDegreeTwo(b, c, d);
		return;
	}
	Complex u = (9 * a * b * c - 27 * (a ^ 2) * d - 2 * (b ^ 3)) / (54 * (a ^ 3)),
		v = sqrtc(3 * (4 * a * (c ^ 3) - (b ^ 2) * (c ^ 2) - 18 * a * b * c * d + 27 * (a ^ 2) * (d ^ 2) + 4 * (b ^ 3) * d)) / (18 * (a ^ 2)),
		m, n, w(-0.5, sqrt(3) / 2.0);
	if ((u + v).modulus > (u - v).modulus)m = (u + v).firstRoot(3);
	else m = (u - v).firstRoot(3);
	if (m.modulus == 0)n = 0;
	else n = ((b ^ 2) - 3 * a * c) / (9 * (a ^ 2) * m);
	Complex x1 = m + n - b / (3 * a),
		x2 = w * m + (w ^ 2) * n - b / (3 * a),
		x3 = (w ^ 2) * m + w * n - b / (3 * a);
	cout << "The solutions of the equation are:" << endl;
	cout << "x1 = ";
	x1.print();
	cout << "x2 = ";
	x2.print();
	cout << "x3 = ";
	x3.print();
}

void Complex::doDegreeFour(const Complex& a, const Complex& b, const Complex& c, const Complex& d, const Complex& e) {
	if (a == 0) {
		doDegreeThree(b, c, d, e);
		return;
	}
	Complex P = (c * c + 12 * a * e - 3 * b * d) / 9,
		Q = (27 * a * d * d + 2 * c * c * c + 27 * b * b * e - 72 * a * c * e - 9 * b * c * d) / 54,
		D, u, v, w(-0.5, sqrt(3) / 2);
	D = Complex::sqrtc(Q * Q - P * P * P);
	Complex s1 = Q + D, s2 = Q - D;
	if (s1.getModulus() > s2.getModulus())u = s1.firstRoot(3);
	else u = s2.firstRoot(3);
	if (fabs(u.getModulus()) < 1e-5) {
		u = 0;
	}
	else v = P / u;
	Complex m = 0, S = 0, T = 0, t = 0;
	int k = 1;
	for (int i = 1; i < 4; i++) {
		t = b * b - 8 * a * c / 3 + 4 * a * (w.powc(i - 1) * u + w.powc(4 - i) * v);
		t = t.firstRoot(2);
		if (t.getModulus() > m.getModulus()) {
			m = t;
			k = i;
		}
	}
	if (fabs(m.getModulus()) < 1e-5) {
		m = 0;
		S = b * b - 8 * a * c / 3;
		T = 0;
	}
	else {
		S = 2 * b * b - 16 * a * c / 3 - 4 * a * (w.powc(k - 1) * u + w.powc(4 - k) * v);
		T = (8 * a * b * c - 16 * a * a * d - 2 * b * b * b) / m;
	}
	Complex x1 = 0, x2 = 0, x3 = 0, x4 = 0;
	Complex p1 = (S - T).firstRoot(2), p2 = (S + T).firstRoot(2);
	x1 = (-m + p1 - b) / (4 * a);
	x2 = (-m - p1 - b) / (4 * a);
	x3 = (m + p2 - b) / (4 * a);
	x4 = (m - p2 - b) / (4 * a);
	cout << "The solutions of the equation are:" << endl;
	cout << "x1 = ";
	x1.print();
	cout << "x2 = ";
	x2.print();
	cout << "x3 = ";
	x3.print();
	cout << "x4 = ";
	x4.print();
}