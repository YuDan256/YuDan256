#include"Matrix.h"
#include"Vector.h"
#include"Normal.h"

using namespace std;

void Matrix::doLinearEquations() {
	cout << "Welcome to the Linear Equations Calculator!" << endl;
	char operation;
	string eNum, varNum, _value;
	int equationNum, variableNum;
	double value;
	map<string, double>p = {
		{"PI",3.14159265358979323846264},{"E",2.7182818284590452353602874}
	};
	while (1) {
		cout << "Enter the numbers of equations and variables:";
		cin >> eNum >> varNum;
		try {
			equationNum = stoi(eNum);
			variableNum = stoi(varNum);
		}
		catch (const exception& e) {
			cerr << "Error: " << e.what() << endl;
			continue;
		}

		if (equationNum <= 0 || variableNum <= 0) {
			cout << "The numbers of equations and variables must be positive." << endl;
			continue;
		}
		cout << "Enter the coefficients of the equations(row by row):" << endl;

		Matrix coefficient(equationNum, variableNum), constant(equationNum, 1);

		for (int i = 0; i < equationNum; i++) {
			for (int j = 0; j < variableNum; j++) {
				cin >> _value;
				try { value = Normal::parsen(_value, p); }
				catch (const exception& e) {
					cerr << "Error: " << e.what() << endl;
					j--;
					continue;
				}
				coefficient.set(i, j, value);
			}
		}
		cout << "Enter the constants of the equations(one per line):" << endl;
		for (int i = 0; i < equationNum; i++) {
			cin >> _value;
			try { value = Normal::parsen(_value, p); }
			catch (const exception& e) {
				cerr << "Error: " << e.what() << endl;
				i--;
				continue;
			}
			constant.set(i, 0, value);
		}
		doSolution(coefficient, constant);
		cout << "Do you still want to use this mode? (Y/N)" << endl;
		do {
			cin >> operation;
			if (operation == 'Y') {}
			else if (operation == 'N')return;
			else cout << "Invalid choice." << endl;
		} while (operation != 'Y' && operation != 'N');
	}
}


void Matrix::doSolution(const Matrix& coefficient, const Matrix& constant) {
	if (constant.getCols() != 1 || constant.getRows() != coefficient.getRows()) {
		throw invalid_argument("Invalid input for equations.");
	}
	Matrix augmented(coefficient.getRows(), coefficient.getCols() + 1);
	for (int i = 0; i < coefficient.getRows(); i++) {
		for (int j = 0; j < coefficient.getCols(); j++) {
			augmented.set(i, j, coefficient.get(i, j));
		}
		augmented.set(i, coefficient.getCols(), constant.get(i, 0));
	}
	if (coefficient.rank() != augmented.rank()) {
		cout << "The system of equations has no solution." << endl;//方程组无解,最小二乘法求近似解。
		doApproximate(coefficient, constant);
	}
	const int rank = augmented.rank();
	augmented = GaussianElimination(augmented);//高斯消元法解方程
	if (rank == coefficient.getCols()) {
		cout << "The only solution of the system of equations is:" << endl;
		for (int i = 0; i < coefficient.getCols(); i++) {
			if (fabs(augmented.get(i, augmented.getCols() - 1)) < 1e-15)cout << "x" << i + 1 << " = 0" << endl;
			else cout << "x" << i + 1 << " = " << augmented.get(i, augmented.getCols() - 1) << endl;
		}
		return;
	}

	cout << "The system of equations has infinitely many solutions." << endl;
	Vector particular(coefficient.getCols());

	//计算特解
	for (int i = 0; i < augmented.getRows(); i++) {
		for (int j = 0; j < coefficient.getCols(); j++) {
			if (augmented.get(i, j) == 0)continue;
			else {
				particular.set(j, augmented.get(i, augmented.getCols() - 1));
				break;
			}
		}
	}
	cout << "A particular solution is:" << endl;
	particular.print();
	cout << "The basic solution set is:" << endl;
	doBasicSolutionSet(coefficient);
}

void Matrix::doApproximate(const Matrix& coefficient, const Matrix& constant) {
	if (constant.getCols() != 1 || constant.getRows() != coefficient.getRows()) {
		throw invalid_argument("Invalid input for equations.");
	}
	cout << endl << "Now seeking the optimal approximate solution for the system of linear equations "
		<< "by solving the normal equations of the least squares method." << endl;
	Matrix newCoef = transpose(coefficient) * coefficient, newConst = transpose(coefficient) * constant;
	doSolution(newCoef, newConst);
}

void Matrix::doBasicSolutionSet(const Matrix& coefficient) {
	Matrix BasicSolutionSet = basicSolutionSet(coefficient);

	//打印基础解系
	for (int i = 0; i < BasicSolutionSet.getCols(); i++) {
		Vector solution(BasicSolutionSet.getRows());
		for (int j = 0; j < BasicSolutionSet.getRows(); j++) {
			solution.set(j, BasicSolutionSet.get(j, i));
		}
		solution.print();
	}
}

Matrix Matrix::basicSolutionSet(const Matrix& coefficient) {
	Matrix _coefficient(coefficient);
	_coefficient = GaussianElimination(_coefficient);
	int rank = _coefficient.rank();
	if (_coefficient.getCols() == rank)return Matrix(_coefficient.getCols(), _coefficient.getCols() - rank);
	Matrix BasicSolutionSet(_coefficient.getCols(), _coefficient.getCols() - rank);
	Matrix NonMainCoefficient(rank, _coefficient.getCols() - rank);
	vector<bool>isMainCol(_coefficient.getCols(), false);

	for (int i = 0; i < rank; i++) {
		if (_coefficient.isZeroRow(i))continue;
		for (int j = 0; j < _coefficient.getCols(); j++) {
			if (_coefficient.get(i, j) != 0) {
				isMainCol[j] = true;
				break;
			}
		}
	}

	//移项，构建基础解系
	int currentCol = 0;
	for (int i = 0; i < _coefficient.getCols(); i++) {
		if (!isMainCol[i]) {
			for (int j = 0; j < rank; j++) {
				NonMainCoefficient.set(j, currentCol, -_coefficient.get(j, i));
			}
			BasicSolutionSet.set(i, currentCol, 1);
			currentCol++;
		}
	}

	int currentRow = 0;
	for (int i = 0; i < _coefficient.getCols(); i++) {
		if (isMainCol[i]) {
			for (int j = 0; j < _coefficient.getCols() - rank; j++) {
				BasicSolutionSet.set(i, j, NonMainCoefficient.get(currentRow, j));
			}
			currentRow++;
		}
	}

	return BasicSolutionSet;
}