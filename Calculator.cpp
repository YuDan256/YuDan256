//Calculator Program
//Developed by Yu Liangyang, Tsinghua University
//Recently updated on May 3rd, 2025.

#include "Complex.h"
#include "Matrix.h"
#include "Vector.h"
#include "Base.h"
#include "Statistics.h"
#include "Function.h"
#include "Normal.h"
#include "Integer.h"
#include "Fraction.h"

using namespace std;

void instructions();
void easterEgg();
void doCalculation();
void initialize();

int main() {
	initialize();
	cout << "Junk Calculator [1.9.3] By YuDan" << endl << endl
		<< "Welcome to My Calculator!" << endl
		<< "This calculator has an extremely user-unfriendly operating system, please bear with it." << endl;
	doCalculation();
	return 0;
}

void doCalculation() {
	string mode;
	bool quit = false;

	while (1) {
		cout << endl << "Type an uppercase letter to choose a mode:" << endl
			<< "I - instructions" << endl
			<< "N - normal calculation" << endl
			<< "M - matrix" << endl
			<< "C - complex number" << endl
			<< "V - vector" << endl
			<< "L - system of linear equations" << endl
			<< "P - polynomial equation" << endl
			<< "B - base" << endl
			<< "F - function" << endl
			<< "S - statistics" << endl
			<< "Z - integer" << endl
			<< "Q - quit" << endl;

		std::cin >> mode;

		if (mode == "N") {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Normal::doNormal();
		}
		else if (mode == "M")Matrix::doMatrix();
		else if (mode == "C")Complex::doComplex();
		else if (mode == "V")Vector::doVector();
		else if (mode == "L")Matrix::doLinearEquations();
		else if (mode == "P")Complex::doPolynomial();
		else if (mode == "B")Base::doBase();
		else if (mode == "I")instructions();
		else if (mode == "F") {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Function::doFunction();
		}
		else if (mode == "S")Statistics::doStatistics();
		else if (mode == "Z")Integer::doInteger();
		else if (mode == "Q") {
			cout << "Thanks!" << endl;
			break;
		}
		else if (mode == "E")easterEgg();
		else cout << "Invalid mode." << endl;
	}
}

void instructions() {
	cout << endl << "Operating instructions:" << endl;
	cout << "1. The calculator can parse the expressions you input after you choose a mode." << endl;
	cout << "2. You can enter the commands when asked to enter an expression." << endl;
	cout << "3. Function's scope is indicated after the function name." << endl
		<< "   R - real number, C - complex number, V - vector, M - matrix, F - function, Z - integer" << endl
		<< "   N - natural number, N+ - positive integer, B - base number, S - dataset." << endl;
	cout << "4. Have a good time!" << endl << endl;
}

void easterEgg() {
	cout << "There are no Easter eggs in this calculator." << endl;
	cout << "(^u^)" << endl;
}