//Calculator Program
//Developed by Yu Liangyang, Tsinghua University
//Recently updated on March 19th, 2025.

#include "Complex.h"
#include "Matrix.h"
#include "Vector.h"
#include "Base.h"
#include "Statistics.h"
#include "Function.h"
#include "Normal.h"

using namespace std;

void instructions();
void newInstructions();
void easterEgg();
void doCalculation();
void newCalculation();
void initialize();

int main() {
	initialize();
	char choice = 0;
	cout << "Junk Calculator [1.7.7] By YuDan" << endl << endl
		<< "Welcome to My Calculator!" << endl
		<< "This calculator has an extremely user-unfriendly operating system, please bear with it." << endl << endl;

	cout << "Which operating system do you prefer?(A/B)" << endl;
	cout << "A - operating by entering uppercase letters." << endl;
	cout << "B - operating by entering expressions." << endl;
	while (choice != 'A' && choice != 'B') {
		cin >> choice;
		if (choice == 'A')doCalculation();
		else if (choice == 'B')newCalculation();
		else cout << "Invalid choice. Choose again." << endl;
	}
	return 0;
}

void doCalculation() {
	char mode, use;
	bool quit = false;

	while (1) {
		cout << "Now operating by entering uppercase letters." << endl;
		cout << "Type an uppercase letter to choose a mode:" << endl
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
			<< "O - change operating system" << endl
			<< "Q - quit" << endl;
		std::cin >> mode;

		switch (mode) {
		case'N':
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Normal::doNormal();
			break;
		case'M':
			Matrix::doMatrix();
			break;
		case'C':
			Complex::doComplex();
			break;
		case'V':
			Vector::doVector();
			break;
		case'L':
			Matrix::doLinearEquations();
			break;
		case'P':
			Complex::doPolynomial();
			break;
		case'B':
			Base::doBase();
			break;
		case'I':
			instructions();
			break;
		case'F':
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Function::doFunction();
			break;
		case'S':
			Statistics::doStatistics();
			break;
		case'Q':
			quit = 1;
			break;
		case'E':
			easterEgg();
			break;
		case'O':
			newCalculation();
			return;
		default:
			cout << "Invalid mode." << endl;
			continue;
		}
		if (quit == 1) {
			cout << "Thanks!" << endl;
			break;
		}
		cout << "Do you still need to use the calculator?(Y/N)" << endl;
		std::cin >> use;
		if (use == 'N') {
			cout << "Thanks!" << endl;
			break;
		}
		else if (use == 'Y') {}
		else {
			cout << "Invalid choice." << endl;
			continue;
		}
	}
}

void newCalculation() {
	char mode, use;
	bool quit = false;

	while (1) {
		cout << "Now operating by entering expressions." << endl;
		cout << "Type an uppercase letter to choose a mode:" << endl
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
			<< "O - change operating system" << endl
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
		case'O':
			doCalculation();
			return;
		default:
			cout << "Invalid mode." << endl;
			continue;
		}
		if (quit == 1) {
			cout << "Thanks!" << endl;
			break;
		}
		cout << "Do you still need to use the calculator?(Y/N)" << endl;
		std::cin >> use;
		if (use == 'N') {
			cout << "Thanks!" << endl;
			break;
		}
		else if (use == 'Y') {}
		else {
			cout << "Invalid choice." << endl;
			continue;
		}
	}
}

void instructions() {
	cout << endl << "Operating instructions:" << endl;
	cout << "1. The calculator is operated by entering uppercase letters." << endl;
	cout << "2. You will be asked to enter all of the objects you may use," << endl
		<< "   but some of the operations may only affect the first or the first two." << endl
		<< "   The number of objects the operation affects is put after the operation name." << endl;
	cout << "3. You can save the result if it can be saved." << endl
		<< "   If the result cannot be saved, a '*' will be put after the operation name." << endl;
	cout << "4. When you are asked to enter a number, you can also enter an arthmetic expression." << endl;
	cout << "5. Have a good time!" << endl << endl;
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