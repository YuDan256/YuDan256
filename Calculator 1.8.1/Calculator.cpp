//Calculator Program
//Developed by Yu Liangyang, Tsinghua University
//Recently updated on March 24th, 2025.

#include "Complex.h"
#include "Matrix.h"
#include "Vector.h"
#include "Base.h"
#include "Statistics.h"
#include "Function.h"
#include "Normal.h"

using namespace std;

void newInstructions();
void easterEgg();
void newCalculation();
void initialize();

int main() {
	initialize();
	char choice = 0;
	cout << "Junk Calculator [1.8.1] By YuDan" << endl << endl
		<< "Welcome to My Calculator!" << endl
		<< "This calculator has an extremely user-unfriendly operating system, please bear with it." << endl;
	newCalculation();
	return 0;
}

void newCalculation() {
	char mode;
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
			<< "Q - quit" << endl;
		std::cin >> mode;

		switch (mode) {
		case'N':
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Normal::newNormal();
			break;
		case'M':
			Matrix::newMatrix();
			break;
		case'C':
			Complex::newComplex();
			break;
		case'V':
			Vector::newVector();
			break;
		case'L':
			Matrix::doLinearEquations();
			break;
		case'P':
			Complex::doPolynomial();
			break;
		case'B':
			Base::newBase();
			break;
		case'I':
			newInstructions();
			break;
		case'F':
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Function::newFunction();
			break;
		case'S':
			Statistics::newStatistics();
			break;
		case'Q':
			quit = 1;
			break;
		case'E':
			easterEgg();
			break;
		default:
			cout << "Invalid mode." << endl;
			continue;
		}
		if (quit == 1) {
			cout << "Thanks!" << endl;
			break;
		}
	}
}

void newInstructions() {
	cout << endl << "Operating instructions:" << endl;
	cout << "1. The calculator can parse the expressions you input after you choose a mode." << endl;
	cout << "2. You can enter the commands when asked to enter an expression." << endl;
	cout << "3. Function's scope is indicated after the function name." << endl
		<< "   R - real number, C - complex number, V - vector, M - matrix, F - function" << endl
		<< "   N - natural number, N+ - positive integer, B - base number, S - dataset." << endl;
	cout << "4. Have a good time!" << endl << endl;
}

void easterEgg() {
	cout << "There are no Easter eggs in this calculator." << endl;
	cout << "(^u^)" << endl;
}