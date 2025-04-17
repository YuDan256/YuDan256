#include"Matrix.h"
#include"Vector.h"
#include"Normal.h"

using namespace std;

map<char, int>Matrix::mop = { {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2},{'\\',3}, { '^',4 } };

map<string, Matrix(*)(const Matrix&)> Matrix::functionm = {
	{"R",r},{"rank",r},{ "T",transpose },{"trans",transpose},{ "D",det },{"det",det},{"I",inverse},{"inv",inverse1},
	{"G",GaussianElimination},{"gauss",GaussianElimination},{"id",identity},{"O",SchmidtOrtho},{"ortho",SchmidtOrtho},
	{"A",adjugate},{"adj",adjugate},{"E",eigenvalue},{"eig",eigenvalue},{ "tr",tr },{"diag",diagonalize},
	{"sin",sinm},{"cos",cosm},{"tan",tanm},{"ln",lnm},{"log",lnm},{"sqrt",sqrtm},{"sum",sum},{"pro",product},
	{"deg",deg},{"rad",rad},{"row",row},{"col",col},{"ones",ones},{"zero",zero},{"exp",expm},{"P",pForDiag},
	{"N",norm},{"norm",norm},{ "sinh",sinh },{"cosh",cosh},{"tanh",tanh},{"sh",sinh},{"ch",cosh},{"th",tanh},
	{"magic",magic},{"random",random},{"randint",randint},{"abs",norm}
};

map<string, Matrix(*)(const Matrix&, const Matrix&)>Matrix::functionm2 = {
	{"integR",integR},{"integC",integC},{"ones",ones},{"zero",zero},{"getR",getRow},{"getC",getCol},{"delR",deleteRow},{"delC",deleteCol},
	{"LD",Ldivide},{"integD",integD},{"random",random},{"randint",randint}
};

map<string, Matrix(*)(const Matrix&, const Matrix&, const Matrix&)>Matrix::functionm3 = {
	{"get",get},{"swapR",swapRows},{"swapC",swapCols},{"multiR",multiplyRows},{"multiC",multiplyCols},{"sub",subMatrix},
	{"cof",cofactor},{"Acof",Acofactor}
};

map<string, Matrix(*)(const Matrix&, const Matrix&, const Matrix&, const Matrix&)>Matrix::functionm4 = {
	{"addR",addRows},{"addC",addCols},{"set",set},{"random",random},{ "randint",randint }
};

void Matrix::set(const int& row, const int& col, const double& value) {
	if (row >= 0 && row < rows && col >= 0 && col < cols) {
		data[row][col] = value;
	}
	else {
		cerr << "Index out of bounds." << endl;
	}
}

double Matrix::get(const int& row, const int& col) const {
	if (row >= 0 && row < rows && col >= 0 && col < cols) {
		return data[row][col];
	}
	else {
		cerr << "Index out of bounds." << endl;
		return 0.0; // 返回0.0作为错误指示
	}
}

Matrix Matrix::get(const Matrix& m, const Matrix& row, const Matrix& col) {
	if (row.rows != 1 || row.cols != 1 || col.rows != 1 || col.cols != 1)throw invalid_argument("The parameters must be two integers.");
	double _row = row.get(0, 0), _col = col.get(0, 0);
	if (fabs(_row - floor(_row)) > 1e-15 || fabs(_col - floor(_col)) > 1e-15)throw invalid_argument("The parameters must be two integers.");
	int r = static_cast<int>(floor(_row)), c = static_cast<int>(floor(_col));
	if (r >= 0 && r < m.rows && c >= 0 && c < m.cols)return Matrix(m.data[r][c]);
	else throw invalid_argument("Index out of bounds.");
}

Matrix Matrix::operator+(const Matrix& other) const {
	if (isNumber() && other.isNumber())return Matrix(data[0][0] + other.data[0][0]);
	if (rows == 1 && cols == 1 && other.rows == other.cols)return (0, 0) * identity(other.rows) + other;
	if (other.rows == 1 && other.cols == 1 && rows == cols)return other(0, 0) * identity(rows) + *this;
	if (rows != other.rows || cols != other.cols) {
		throw invalid_argument("Matrices dimensions must match for addition.");
	}
	Matrix result(rows, cols);
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			result.set(i, j, data[i][j] + other.get(i, j));
		}
	}
	return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
	if (isNumber() && other.isNumber())return Matrix(data[0][0] - other.data[0][0]);
	if (rows == 1 && cols == 1 && other.rows == other.cols)return (0, 0) * identity(other.rows) - other;
	if (other.rows == 1 && other.cols == 1 && rows == cols)return *this - other(0, 0) * identity(rows);
	if (rows != other.rows || cols != other.cols) {
		throw invalid_argument("Matrices dimensions must match for substraction.");
	}
	Matrix result(rows, cols);
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			result.set(i, j, data[i][j] - other.get(i, j));
		}
	}
	return result;
}

Matrix Matrix::operator-() const {
	Matrix result(rows, cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			result.data[i][j] = -data[i][j];
		}
	}
	return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
	if (rows == 1 && cols == 1)return data[0][0] * other;
	if (other.rows == 1 && other.cols == 1)return other.data[0][0] * (*this);
	if (cols != other.rows) {
		throw invalid_argument("Number of columns in the first matrix must be equal to the number of rows in the second matrix for multiplication.");
	}
	Matrix result(rows, other.cols);
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < other.cols; ++j) {
			double sum = 0.0;
			for (int k = 0; k < cols; ++k) {
				sum += data[i][k] * other.get(k, j);
			}
			result.set(i, j, sum);
		}
	}
	return result;
}

Matrix operator*(const double& scalar, const Matrix& other) {
	Matrix result(other.rows, other.cols);
	for (int i = 0; i < other.rows; ++i) {
		for (int j = 0; j < other.cols; ++j) {
			result.set(i, j, other.data[i][j] * scalar);
		}
	}
	return result;
}

ostream& operator<<(ostream& out, const Matrix& m) {
	out << m.rows << " " << m.cols;
	for (int i = 0; i < m.rows; i++) {
		for (int j = 0; j < m.cols; j++) {
			out << " " << m.data[i][j];
		}
	}
	return out;
}

Matrix Matrix::operator*(const double& scalar) const {
	Matrix result(rows, cols);
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			result.set(i, j, data[i][j] * scalar);
		}
	}
	return result;
}

Matrix Matrix::operator^(const Matrix& n) const {
	if (rows != cols || n.rows != n.cols)throw invalid_argument("Matrix power operations are only defined for square matrices.");
	if (rows == 1 && n.rows == 1)return Matrix(pow(get(0, 0), n.get(0, 0)));
	else if (rows == 1) {
		if (get(0, 0) < 1e-10)throw runtime_error("When the exponent is a matrix, the base must be positive.");
		return expm(n * log(get(0, 0)));
	}
	else if (n.isInteger()) {
		return (*this) ^ static_cast<int>(n.get(0, 0));
	}
	else {
		return expm(n * lnm(*this));
	}
}

Matrix Matrix::operator/(const double& scalar) const {
	if (fabs(scalar) < 1e-10)throw runtime_error("The divisor cannot be zero.");
	Matrix result(rows, cols);
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			result.set(i, j, data[i][j] / scalar);
		}
	}
	return result;
}

Matrix Matrix::operator/(const Matrix& other) const {
	if (other.rows != other.cols)throw invalid_argument("The divisor matrix must be square.");
	if (fabs(other.determinant()) < 1e-10) {
		if (other.rows > 1)throw runtime_error("The divisor matrix must be invertible.");
		else throw runtime_error("The divisor cannot be zero.");
	}
	if ((other.rows == 1 && other.cols == 1) || (rows == 1 && cols == 1))return (*this) * inverse(other);
	if (cols != other.rows)throw invalid_argument("Number of columns in the first matrix must be equal to the number of rows in the second matrix for division.");
	return (*this) * inverse(other);
}

Matrix Matrix::operator^(const int& n) const {
	if (rows != cols)throw invalid_argument("Matrix power operations are only defined for square matrices.");
	Matrix result = identity(rows);
	if (n > 0) {
		for (int i = 0; i < n; i++) {
			result *= (*this);
		}
		return result;
	}
	else {
		if (fabs(determinant()) < 1e-10)throw runtime_error("When the exponent is non-positive, the matrix must be invertible.");
		Matrix temp = inverse(*this);
		for (int i = 0; i < -n; i++) {
			result *= temp;
		}
		return result;
	}
}

bool Matrix::operator==(const Matrix& other)const {
	return data == other.data;
}

vector<double>& Matrix::operator[](const int& row) {
	if (row < 0 || row >= rows)throw invalid_argument("Index out of bounds.");
	return data[row];
}

const vector<double>& Matrix::operator[](const int& row) const {
	if (row < 0 || row >= rows)throw invalid_argument("Index out of bounds.");
	return data[row];
}

double& Matrix::operator()(const int& row, const int& col) {
	if (row < 0 || row >= rows || col < 0 || col >= cols)throw invalid_argument("Index out of bounds.");
	return data[row][col];
}

const double& Matrix::operator()(const int& row, const int& col) const {
	if (row < 0 || row >= rows || col < 0 || col >= cols)throw invalid_argument("Index out of bounds.");
	return data[row][col];
}

Matrix& Matrix::operator=(const Matrix& other) {
	if (this == &other) return *this;
	this->rows = other.rows;
	this->cols = other.cols;
	this->data = other.data;
	return *this;
}

Matrix& Matrix::operator+=(const Matrix& other) {
	if (isNumber() && other.isNumber()) {
		data[0][0] += other.data[0][0];
		return *this;
	}
	if (rows == 1 && cols == 1 && other.rows == other.cols) {
		*this = (0, 0) * identity(other.rows) + other;
		return *this;
	}
	if (other.rows == 1 && other.cols == 1 && rows == cols) {
		*this = other(0, 0) * identity(rows) + *this;
		return *this;
	}
	if (rows != other.rows || cols != other.cols) {
		throw invalid_argument("Matrices dimensions must match for addition.");
	}
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			data[i][j] += other.get(i, j);
		}
	}
	return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
	if (isNumber() && other.isNumber()) {
		data[0][0] -= other.data[0][0];
		return *this;
	}
	if (rows == 1 && cols == 1 && other.rows == other.cols) {
		*this = (0, 0) * identity(other.rows) - other;
		return *this;
	}
	if (other.rows == 1 && other.cols == 1 && rows == cols) {
		*this = *this - other(0, 0) * identity(rows);
		return *this;
	}
	if (rows != other.rows || cols != other.cols) {
		throw invalid_argument("Matrices dimensions must match for substraction.");
	}
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			data[i][j] -= other.get(i, j);
		}
	}
	return *this;
}

Matrix& Matrix::operator*=(const Matrix& other) {
	if (rows == 1 && cols == 1) {
		*this = data[0][0] * other;
		return *this;
	}
	if (other.rows == 1 && other.cols == 1) {
		*this = other.data[0][0] * (*this);
		return *this;
	}
	if (cols != other.rows) {
		throw invalid_argument("Number of columns in the first matrix must be equal to the number of rows in the second matrix for multiplication.");
	}
	Matrix result(rows, other.cols);
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < other.cols; ++j) {
			double sum = 0.0;
			for (int k = 0; k < cols; ++k) {
				sum += data[i][k] * other.get(k, j);
			}
			result.set(i, j, sum);
		}
	}
	*this = result;
	return *this;
}

Matrix& Matrix::operator*=(const double& scalar) {
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			data[i][j] *= scalar;
		}
	}
	return *this;
}

Matrix& Matrix::operator/=(const Matrix& other) {
	if (other.rows != other.cols)throw invalid_argument("The divisor matrix must be square.");
	if (fabs(other.determinant()) < 1e-10) {
		if (other.rows > 1)throw runtime_error("The divisor matrix must be invertible.");
		else throw runtime_error("The divisor cannot be zero.");
	}
	if ((other.rows == 1 && other.cols == 1) || (rows == 1 && cols == 1)) {
		*this = (*this) * inverse(other);
		return *this;
	}
	if (cols != other.rows)throw invalid_argument("Number of columns in the first matrix must be equal to the number of rows in the second matrix for division.");
	*this = (*this) * inverse(other);
	return *this;
}

Matrix& Matrix::operator/=(const double& scalar) {
	if (fabs(scalar) < 1e-10)throw runtime_error("The divisor cannot be zero.");
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			data[i][j] /= scalar;
		}
	}
	return *this;
}

Matrix& Matrix::operator^=(const int& n) {
	if (rows != cols)throw invalid_argument("Matrix power operations are only defined for square matrices.");
	Matrix result = identity(rows);
	if (n > 0) {
		for (int i = 0; i < n; i++) {
			result *= (*this);
		}
		*this = result;
		return *this;
	}
	else {
		if (fabs(determinant()) < 1e-10)throw runtime_error("When the exponent is non-positive, the matrix must be invertible.");
		Matrix temp = inverse(*this);
		for (int i = 0; i < -n; i++) {
			result *= temp;
		}
		*this = result;
		return *this;
	}
}

Matrix& Matrix::operator^=(const Matrix& n) {
	if (rows != cols || n.rows != n.cols)throw invalid_argument("Matrix power operations are only defined for square matrices.");
	if (rows == 1 && n.rows == 1) {
		if (get(0, 0) < 1e-10)throw runtime_error("When the exponent is a matrix, the base must be positive.");
		*this = expm(n * log(get(0, 0)));
		return *this;
	}
	else if (rows == 1) {
		if (get(0, 0) < 1e-10)throw runtime_error("When the exponent is a matrix, the base must be positive.");
		*this = expm(n * log(get(0, 0)));
		return *this;
	}
	else if (n.isInteger()) {
		*this ^= static_cast<int>(n.get(0, 0));
		return *this;
	}
	else {
		*this = expm(n * lnm(*this));
		return *this;
	}
}

void Matrix::print() const {
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if (fabs(data[i][j]) < 1e-10)cout << 0 << " ";
			else cout << data[i][j] << " ";
		}
		cout << endl;
	}
}

bool Matrix::upper()const {
	Matrix A(*this);
	double limit;
	if (rows < 4)limit = 1e-9;
	else if (rows < 8)limit = 3e-9;
	else limit = 5e-9;
	if (A.rows != A.cols)throw invalid_argument("The matrix must be square.");
	for (int i = 0; i < A.rows; i++) {
		for (int j = 0; j < i; j++) {
			if (fabs(A.data[i][j]) > limit)return false;
		}
	}
	return true;
}

//检验是否有一行为基向量影响迭代
bool Matrix::hasSet() const {
	if (rows != cols)throw invalid_argument("The matrix must be square.");
	bool result = false;
	for (int i = 0; i < rows; i++) {
		int k = rows - 1 - i;
		if (data[i][k] != 0)result = true;
		for (int j = 0; j < cols; j++) {
			if ((i == j || k != j) && data[i][j] != 0)result = false;
		}
		if (result)return true;
	}
	return false;
}

bool Matrix::NoImagEig() const {
	if (rows != cols)return false;
	vector<double>evalue = eigenvalue(*this).data[0];
	if (evalue.size() != rows)return false;
	else return true;
}

bool Matrix::diagonalizable() const {
	if (!NoImagEig())return false;
	vector<double>evalue = eigenvalue(*this).data[0];
	Matrix I = identity(rows), result(rows, cols);
	map<double, int>frequency;
	int r = 0, c = 0;
	for (double i : evalue) {
		frequency[i]++;
	}
	for (auto it = frequency.begin(); it != frequency.end(); it++) {
		Matrix X = basicSolutionSet(*this - it->first * I);
		c += X.cols;
		r = X.rows;
	}
	if (r == c)return true;
	else return false;
}

Matrix Matrix::identity(const int& n) {
	Matrix I(n, n);
	for (int i = 0; i < n; i++) {
		I.data[i][i] = 1.0;
	}
	return I;
}

double Matrix::norm() const {
	return sqrt((transpose(*this) * (*this)).trace());
}

Matrix Matrix::identity(const Matrix& m) {
	if (m.rows != 1 || m.cols != 1)throw invalid_argument("This function cannot affect matrices.");
	int n = static_cast<int>(floor(m.data[0][0]));
	if (fabs(m.data[0][0] - n) > 1e-10 || n < 1)throw invalid_argument("The size of the matrix can only be a positive integer.");
	return identity(n);
}

Matrix Matrix::ones(const Matrix& m) {
	return ones(m, m);
}

Matrix Matrix::zero(const Matrix& m) {
	return zero(m, m);
}

Matrix Matrix::random(const Matrix& m) {
	return random(m, m);
}

Matrix Matrix::randint(const Matrix& m) {
	return randint(m, m);
}

Matrix Matrix::sum(const Matrix& m) {
	double result = 0.0;
	for (int i = 0; i < m.rows; i++) {
		for (int j = 0; j < m.cols; j++) {
			result += m.get(i, j);
		}
	}
	return Matrix(result);
}

Matrix Matrix::product(const Matrix& m) {
	double result = 1.0;
	for (int i = 0; i < m.rows; i++) {
		for (int j = 0; j < m.cols; j++) {
			result *= m.get(i, j);
		}
	}
	return Matrix(result);
}

Matrix Matrix::deg(const Matrix& rad) {
	if (rad.rows != 1 || rad.cols != 1)throw invalid_argument("The radian must be a real number.");
	return Matrix(rad / 3.1415926535897932384626433832 * 180);
}

Matrix Matrix::rad(const Matrix& deg) {
	if (deg.rows != 1 || deg.cols != 1)throw invalid_argument("The degree must be a real number.");
	return Matrix(deg * 3.1415926535897932384626433832 / 180);
}

Matrix Matrix::norm(const Matrix& m) {
	return Matrix(m.norm());
}

Matrix Matrix::magic(const Matrix& m) {
	if (!m.isInteger() || m.get(0, 0) <= 0) {
		throw invalid_argument("The size of the matrix must be a positive integer.");
	}
	int n = static_cast<int>(m.get(0, 0));
	if (n < 3)throw runtime_error("The size must be greater than 2.");
	Matrix result(n, n);

	if (n % 2 == 1) {
		// N 为奇数时
		int num = 1;
		int i = 0, j = n / 2;
		result.set(i, j, num++);

		while (num <= n * n) {
			int newi = (i - 1 + n) % n;
			int newj = (j + 1) % n;

			if (result.get(newi, newj) != 0) {
				newi = (i + 1) % n;
				newj = j;
			}

			result.set(newi, newj, num++);
			i = newi;
			j = newj;
		}
	}
	else if (n % 4 == 0) {
		// N 为4的倍数时
		int num = 1;
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				result.set(i, j, num++);
			}
		}
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n / 2; j++) {
				if (i % 4 == j % 4)result.swap(i, j, n - 1 - i, n - 1 - j);
				if (i % 4 + j % 4 == 3)result.swap(i, j, n - 1 - i, n - 1 - j);
			}
		}
	}
	else {
		// N 为其它偶数时 (4n+2)
		int u = n / 2, v = n * n / 4, t = (n - 2) / 4;
		Matrix subSquare = magic(u);
		for (int i = 0; i < u; i++) {
			for (int j = 0; j < u; j++) {
				result.set(i, j, subSquare.get(i, j));
				result.set(i + u, j + u, subSquare.get(i, j) + v);
				result.set(i + u, j, subSquare.get(i, j) + 3 * v);
				result.set(i, j + u, subSquare.get(i, j) + 2 * v);
			}
		}
		for (int i = 0; i < u; i++) {
			for (int j = 0; j < t; j++) {
				if (i == (u - 1) / 2)result.swap(i, (u - 1) / 2 + j, u + i, (u - 1) / 2 + j);
				else result.swap(i, j, u + i, j);
			}
		}
		for (int j = 0; j < t - 1; j++) {
			for (int i = 0; i < u; i++) {
				result.swap(i, u + (u - 1) / 2 - j, u + i, u + (u - 1) / 2 - j);
			}
		}
	}
	return result;
}

Matrix Matrix::expm(const Matrix& m) {
	if (m.rows != m.cols)throw invalid_argument("The matrix must be square.");
	if (m.rows == 1 && m.cols == 1)return Matrix(exp(m.get(0, 0)));
	else {
		Matrix result = identity(m.rows);
		Matrix temp = identity(m.rows);
		int i = 1;
		while (true) {
			temp *= (m / i);
			result += temp;
			if (temp.norm() < 1e-20)break;
			i++;
			if (i == 1e6)throw invalid_argument("The matrix does not converge.");
		}
		return result;
	}
}

Matrix Matrix::integR(const Matrix& m1, const Matrix& m2) {
	return m1.integR(m2);
}

Matrix Matrix::integC(const Matrix& m1, const Matrix& m2) {
	return m1.integC(m2);
}

Matrix Matrix::integD(const Matrix& m1, const Matrix& m2) {
	return m1.integD(m2);
}

Matrix Matrix::Ldivide(const Matrix& m1, const Matrix& m2) {
	if (m1.rows != m1.cols)throw invalid_argument("The divisor must be square.");
	if (m1.cols != m2.rows)throw invalid_argument("Number of columns in the first matrix must be equal to the number of rows in the second matrix for division.");
	if (fabs(m1.determinant()) < 1e-15)throw runtime_error("The divisor must be invertible.");
	if (m1.rows == 1 && m1.cols == 1)return m2 / m1;
	return inverse(m1) * m2;
}

Matrix Matrix::ones(const Matrix& r, const Matrix& c) {
	if (!r.isInteger() || !c.isInteger())throw invalid_argument("The numbers of rows and columns must be integers.");
	int _r = static_cast<int>(r.get(0, 0)), _c = static_cast<int>(c.get(0, 0));
	if (_r < 1 || _c < 1)throw invalid_argument("The numbers of rows and columns must be positive integers.");
	Matrix result(_r, _c);
	for (int i = 0; i < _r; i++) {
		for (int j = 0; j < _c; j++) {
			result.set(i, j, 1);
		}
	}
	return result;
}

Matrix Matrix::zero(const Matrix& r, const Matrix& c) {
	if (!r.isInteger() || !c.isInteger())throw invalid_argument("The numbers of rows and columns must be integers.");
	int _r = static_cast<int>(r.get(0, 0)), _c = static_cast<int>(c.get(0, 0));
	if (_r < 1 || _c < 1)throw invalid_argument("The numbers of rows and columns must be positive integers.");
	return Matrix(_r, _c);
}

Matrix Matrix::random(const Matrix& r, const Matrix& c) {
	return random(r, c, 0, 1);
}

Matrix Matrix::randint(const Matrix& r, const Matrix& c) {
	return randint(r, c, 0, 10);
}

Matrix Matrix::getRow(const Matrix& m, const Matrix& row) {
	if (!row.isInteger())throw invalid_argument("The row number must be an integer.");
	int r = static_cast<int>(row.get(0, 0));
	if (r < 0 || r >= m.rows)throw invalid_argument("Index out of bounds.");
	Matrix result(1, m.cols);
	for (int i = 0; i < m.cols; i++) {
		result.set(0, i, m.get(r, i));
	}
	return result;
}

Matrix Matrix::getCol(const Matrix& m, const Matrix& col) {
	if (!col.isInteger())throw invalid_argument("The column number must be an integer.");
	int c = static_cast<int>(col.get(0, 0));
	if (c < 0 || c >= m.cols)throw invalid_argument("Index out of bounds.");
	Matrix result(m.rows, 1);
	for (int i = 0; i < m.rows; i++) {
		result.set(i, 0, m.get(i, c));
	}
	return result;
}

Matrix Matrix::deleteRow(const Matrix& m, const Matrix& row) {
	if (m.rows == 1)throw invalid_argument("The result matrix must have at least one row.");
	if (!row.isInteger())throw invalid_argument("The row number must be an integer.");
	int r = static_cast<int>(row.get(0, 0));
	if (r < 0 || r >= m.rows)throw invalid_argument("Index out of bounds.");
	Matrix result(m.rows - 1, m.cols);
	int _r = 0;
	for (int i = 0; i < m.rows; i++) {
		if (i == r)continue;
		for (int j = 0; j < m.cols; j++) {
			result.set(_r, j, m.get(i, j));
		}
		++_r;
	}
	return result;
}

Matrix Matrix::deleteCol(const Matrix& m, const Matrix& col) {
	if (m.cols == 1)throw invalid_argument("The result matrix must have at least one column.");
	if (!col.isInteger())throw invalid_argument("The column number must be an integer.");
	int c = static_cast<int>(col.get(0, 0));
	if (c < 0 || c >= m.cols)throw invalid_argument("Index out of bounds.");
	Matrix result(m.rows, m.cols - 1);
	for (int i = 0; i < m.rows; i++) {
		int _c = 0;
		for (int j = 0; j < m.cols; j++) {
			if (j == c)continue;
			result.set(i, _c++, m.get(i, j));
		}
	}
	return result;
}

double Matrix::determinant() const {
	if (rows != cols)throw invalid_argument("Determinent is only defined of square matrices.");

	Matrix temp(*this);
	int swapNum = 0, lastRow = rows - 1;
	double result = 1;

	for (int i = 0; i < temp.cols; i++) {
		if (isZeroCol(i))return 0;
		if (fabs(temp.data[i][i]) < 1e-10) {
			for (int j = i + 1; j < temp.rows; j++) {
				if (fabs(temp.data[j][i]) > 1e-10) {
					temp.swapRows(i, j);
					swapNum++;
					break;
				}
			}
		}
		for (int j = i + 1; j < temp.cols; j++) {
			if (fabs(temp.data[i][i]) < 1e-10)return 0;
			temp.addRows(j, i, -temp.data[j][i] / temp.data[i][i]);
		}
	}
	if (fabs(temp.data[lastRow][lastRow]) < 1e-10)return 0;
	for (int i = 0; i < temp.cols; i++) {
		result *= temp.data[i][i];
	}
	result *= pow(-1, swapNum);
	return result;
}

Matrix Matrix::subMatrix(const int& excludeRow, const int& excludeCol) const {
	Matrix subMatrix(rows - 1, cols - 1);
	int subRow = 0;
	for (int i = 0; i < rows; ++i) {
		if (i == excludeRow) continue;
		int subCol = 0;
		for (int j = 0; j < cols; ++j) {
			if (j == excludeCol) continue;
			subMatrix.set(subRow, subCol++, data[i][j]);
		}
		++subRow;
	}
	for (int i = 0; i < subMatrix.getRows(); i++) {
		for (int j = 0; j < subMatrix.getCols(); j++) {
			if (fabs(subMatrix.data[i][j]) < 1e-10) subMatrix.data[i][j] = 0;
		}
	}
	return subMatrix;
}

Matrix Matrix::row(const Matrix& m) {
	return Matrix(m.rows);
}

Matrix Matrix::col(const Matrix& m) {
	return Matrix(m.cols);
}

Matrix Matrix::det(const Matrix& m) {
	return Matrix(m.determinant());
}

Matrix Matrix::r(const Matrix& m) {
	return Matrix(m.rank());
}

Matrix Matrix::transpose(const Matrix& m) {
	Matrix result(m.cols, m.rows);
	for (int i = 0; i < m.rows; ++i) {
		for (int j = 0; j < m.cols; ++j) {
			result.set(j, i, m.data[i][j]);
		}
	}
	for (int i = 0; i < result.getRows(); i++) {
		for (int j = 0; j < result.getCols(); j++) {
			if (fabs(result.data[i][j]) < 1e-10) result.data[i][j] = 0;
		}
	}
	return result;
}

Matrix Matrix::adjugate(const Matrix& m) {
	if (m.rows != m.cols) {
		throw invalid_argument("Adjugate matrix is only defined for square matrices.");
	}
	if (m.rows == 1) {
		Matrix result(1, 1);
		result.set(0, 0, 1);
		return result;
	}
	Matrix cofactorMatrix(m.rows, m.cols);
	for (int i = 0; i < m.rows; ++i) {
		for (int j = 0; j < m.cols; ++j) {
			Matrix subMatrix = m.subMatrix(i, j);
			cofactorMatrix.set(i, j, pow(-1, i + j) * subMatrix.determinant());
		}
	}
	Matrix result = transpose(cofactorMatrix);
	for (int i = 0; i < result.getRows(); i++) {
		for (int j = 0; j < result.getCols(); j++) {
			if (fabs(result.data[i][j]) < 1e-10) result.data[i][j] = 0;
		}
	}
	return result;
}

Matrix Matrix::inverse1(const Matrix& m) {
	if (m.rows != m.cols) {
		throw invalid_argument("Inverse is only defined for square matrices.");
	}
	double det = m.determinant();
	if (fabs(det) < 1e-15) {
		throw runtime_error("Matrix is singular and cannot be inverted.");
	}
	Matrix adjugateMatrix = adjugate(m);
	Matrix inverseMatrix(m.rows, m.cols);
	for (int i = 0; i < m.rows; ++i) {
		for (int j = 0; j < m.cols; ++j) {
			inverseMatrix.set(i, j, adjugateMatrix.get(i, j) / det);
		}
	}
	for (int i = 0; i < inverseMatrix.getRows(); i++) {
		for (int j = 0; j < inverseMatrix.getCols(); j++) {
			if (fabs(inverseMatrix.data[i][j]) < 1e-10) inverseMatrix.data[i][j] = 0;
		}
	}
	return inverseMatrix;
}

Matrix Matrix::inverse(const Matrix& m) {
	Matrix result(m.rows, m.cols);
	if (m.rows != m.cols)throw invalid_argument("Inverse is only defined for square matrices.");
	if (fabs(m.determinant()) < 1e-10)throw runtime_error("Matrix is singular and cannot be inverted.");
	Matrix temp = m.integR(identity(m.rows));
	temp = GaussianElimination(temp);
	for (int i = 0; i < m.rows; i++) {
		for (int j = 0; j < m.cols; j++) {
			result.set(i, j, temp.get(i, m.cols + j));
		}
	}
	return result;
}

int Matrix::rank() const {
	if (rows < 1 || cols < 1) {
		return 0;
	}
	int rank = 0;
	Matrix temp;
	temp = GaussianElimination(*this);
	for (int i = 0; i < rows; i++) {
		if (!temp.isZeroRow(i))rank++;
	}
	return rank;
}

double Matrix::trace() const {
	if (rows != cols)throw invalid_argument("The matrix must be square.");
	double result = 0;
	for (int i = 0; i < rows; i++) {
		result += data[i][i];
	}
	return result;
}

void Matrix::swap(const int& row1, const int& col1, const int& row2, const int& col2) {
	double temp;
	temp = get(row1, col1);
	set(row1, col1, get(row2, col2));
	set(row2, col2, temp);
}

void Matrix::swapRows(const int& row1, const int& row2) {
	for (int i = 0; i < cols; i++) {
		swap(row1, i, row2, i);
	}
}
void Matrix::swapCols(const int& col1, const int& col2) {
	for (int i = 0; i < rows; i++) {
		swap(i, col1, i, col2);
	}
}

void Matrix::multiplyRows(const int& row, const double& scalar) {
	if (scalar == 0) {
		throw invalid_argument("The scalar cannot be 0.");
	}
	for (int i = 0; i < cols; i++) {
		set(row, i, scalar * data[row][i]);
	}
}

void Matrix::multiplyCols(const int& col, const double& scalar) {
	if (scalar == 0) {
		throw invalid_argument("The scalar cannot be 0.");
	}
	for (int i = 0; i < rows; i++) {
		set(i, col, scalar * data[i][col]);
	}
}

void Matrix::addRows(const int& row1, const int& row2, const double& scalar) {
	for (int i = 0; i < cols; i++) {
		set(row1, i, data[row1][i] + scalar * data[row2][i]);
	}
}

void Matrix::addCols(const int& col1, const int& col2, const double& scalar) {
	for (int i = 0; i < rows; i++) {
		set(i, col1, data[i][col1] + scalar * data[i][col2]);
	}
}

Matrix Matrix::swapRows(const Matrix& m, const Matrix& row1, const Matrix& row2) {
	if (!row1.isInteger() || !row2.isInteger())throw invalid_argument("The two parameters must be integers.");
	Matrix result = m;
	int r1 = static_cast<int>(row1.get(0, 0)), r2 = static_cast<int>(row2.get(0, 0));
	result.swapRows(r1, r2);
	return result;
}

Matrix Matrix::swapCols(const Matrix& m, const Matrix& col1, const Matrix& col2) {
	if (!col1.isInteger() || !col2.isInteger())throw invalid_argument("The two parameters must be integers.");
	Matrix result = m;
	int c1 = static_cast<int>(col1.get(0, 0)), c2 = static_cast<int>(col2.get(0, 0));
	result.swapCols(c1, c2);
	return result;
}

Matrix Matrix::multiplyRows(const Matrix& m, const Matrix& row, const Matrix& scalar) {
	if (!row.isInteger())throw invalid_argument("The number of row must be an integer.");
	if (!scalar.isNumber())throw invalid_argument("The scalar must be a real number.");
	int r = static_cast<int>(row.get(0, 0));
	double s = scalar.get(0, 0);
	Matrix result = m;
	result.multiplyRows(r, s);
	return result;
}

Matrix Matrix::multiplyCols(const Matrix& m, const Matrix& col, const Matrix& scalar) {
	if (!col.isInteger())throw invalid_argument("The number of column must be an integer.");
	if (!scalar.isNumber())throw invalid_argument("The scalar must be a real number.");
	int c = static_cast<int>(col.get(0, 0));
	double s = scalar.get(0, 0);
	Matrix result = m;
	result.multiplyCols(c, s);
	return result;
}

Matrix Matrix::subMatrix(const Matrix& m, const Matrix& excludeRow, const Matrix& excludeCol) {
	if (!excludeRow.isInteger() || !excludeCol.isInteger())throw invalid_argument("The two parameters must be integers.");
	int r = static_cast<int>(excludeRow.get(0, 0)), c = static_cast<int>(excludeCol.get(0, 0));
	if (r < 0 || r >= m.rows || c < 0 || c >= m.cols)throw invalid_argument("Index out of bounds.");
	Matrix result = m.subMatrix(r, c);
	return result;
}

Matrix Matrix::cofactor(const Matrix& m, const Matrix& row, const Matrix& col) {
	if (!row.isInteger() || !col.isInteger())throw invalid_argument("The two parameters must be integers.");
	int r = static_cast<int>(row.get(0, 0)), c = static_cast<int>(col.get(0, 0));
	if (r < 0 || r >= m.rows || c < 0 || c >= m.cols)throw invalid_argument("Index out of bounds.");
	Matrix subMatrix = m.subMatrix(r, c);
	double result = subMatrix.determinant();
	return Matrix(result);
}

Matrix Matrix::Acofactor(const Matrix& m, const Matrix& row, const Matrix& col) {
	if (!row.isInteger() || !col.isInteger())throw invalid_argument("The two parameters must be integers.");
	int r = static_cast<int>(row.get(0, 0)), c = static_cast<int>(col.get(0, 0));
	if (r < 0 || r >= m.rows || c < 0 || c >= m.cols)throw invalid_argument("Index out of bounds.");
	Matrix subMatrix = m.subMatrix(r, c);
	double result = pow(-1, r + c) * subMatrix.determinant();
	return Matrix(result);
}

Matrix Matrix::addRows(const Matrix& m, const Matrix& row1, const Matrix& row2, const Matrix& scalar) {
	if (!row1.isInteger() || !row2.isInteger())throw invalid_argument("The numbers of rows must be integers.");
	if (!scalar.isNumber())throw invalid_argument("The scalar must be a real number.");
	int r1 = static_cast<int>(row1.get(0, 0)), r2 = static_cast<int>(row2.get(0, 0));
	double s = scalar.get(0, 0);
	Matrix result = m;
	result.addRows(r1, r2, s);
	return result;
}

Matrix Matrix::addCols(const Matrix& m, const Matrix& col1, const Matrix& col2, const Matrix& scalar) {
	if (!col1.isInteger() || !col2.isInteger())throw invalid_argument("The numbers of columns must be integers.");
	if (!scalar.isNumber())throw invalid_argument("The scalar must be a real number.");
	int c1 = static_cast<int>(col1.get(0, 0)), c2 = static_cast<int>(col2.get(0, 0));
	double s = scalar.get(0, 0);
	Matrix result = m;
	result.addCols(c1, c2, s);
	return result;
}

Matrix Matrix::set(const Matrix& m, const Matrix& row, const Matrix& col, const Matrix& num) {
	if (!row.isInteger() || !col.isInteger())throw invalid_argument("The coordinates of the nummber must be integers.");
	if (!num.isNumber())throw invalid_argument("The number cannot be a matrix.");
	int r = static_cast<int>(row.get(0, 0)), c = static_cast<int>(col.get(0, 0));
	double n = num.get(0, 0);
	Matrix result = m;
	result.set(r, c, n);
	return result;
}

Matrix Matrix::random(const Matrix& r, const Matrix& c, const Matrix& min, const Matrix& max) {
	if (!r.isInteger() || !c.isInteger())throw invalid_argument("The numbers of rows and columns must be integers.");
	if (!min.isNumber() || !max.isNumber())throw invalid_argument("The minimum and maximum values must be real numbers.");
	int _r = static_cast<int>(r.get(0, 0)), _c = static_cast<int>(c.get(0, 0));
	double _min = min.get(0, 0), _max = max.get(0, 0);
	if (_r < 1 || _c < 1)throw invalid_argument("The numbers of rows and columns must be positive integers.");
	if (_min > _max)throw invalid_argument("The minimum value must be less than or equal to the maximum value.");
	Matrix result(_r, _c);
	srand(static_cast<unsigned int>(time(0)));
	for (int i = 0; i < _r; i++) {
		for (int j = 0; j < _c; j++) {
			result.set(i, j, _min + (double)rand() / RAND_MAX * (_max - _min));
		}
	}
	return result;
}

Matrix Matrix::randint(const Matrix& r, const Matrix& c, const Matrix& min, const Matrix& max) {
	if (!r.isInteger() || !c.isInteger())throw invalid_argument("The numbers of rows and columns must be integers.");
	if (!min.isInteger() || !max.isInteger())throw invalid_argument("The minimum and maximum values must be integers.");
	int _r = static_cast<int>(r.get(0, 0)), _c = static_cast<int>(c.get(0, 0));
	int _min = static_cast<int>(min.get(0, 0)), _max = static_cast<int>(max.get(0, 0));
	if (_r < 1 || _c < 1)throw invalid_argument("The numbers of rows and columns must be positive integers.");
	if (_min > _max)throw invalid_argument("The minimum value must be less than or equal to the maximum value.");
	Matrix result(_r, _c);
	srand(static_cast<unsigned int>(time(0)));
	for (int i = 0; i < _r; i++) {
		for (int j = 0; j < _c; j++) {
			result.set(i, j, _min + rand() % (_max - _min + 1));
		}
	}
	return result;
}

bool Matrix::isZeroRow(const int& row)const {
	for (int i = 0; i < cols; i++) {
		if (fabs(data[row][i]) > 1e-15)return false;
	}
	return true;
}

bool Matrix::isZeroCol(const int& col)const {
	for (int i = 0; i < rows; i++) {
		if (fabs(data[i][col]) > 1e-15)return false;
	}
	return true;
}

bool Matrix::isNumber() const {
	if (rows != 1 || cols != 1)return false;
	else return true;
}

bool Matrix::isInteger() const {
	if (!isNumber())return false;
	double a = data[0][0];
	if (fabs(a - floor(a)) > 1e-15)return false;
	else return true;
}

Matrix Matrix::GaussianElimination(const Matrix& m) {
	Matrix temp(m);
	int currentRow = 0;
	double mainElement;
	for (int i = 0; i < m.cols; i++) {
		// 部分选主元
		if (currentRow >= m.rows)break;
		int maxRow = currentRow;
		for (int j = currentRow + 1; j < m.rows; j++) {
			if (fabs(temp.data[j][i]) > fabs(temp.data[maxRow][i])) {
				maxRow = j;
			}
		}
		if (fabs(temp.data[maxRow][i]) < 1e-10) continue; // 如果主元接近0，跳过此列
		temp.swapRows(currentRow, maxRow);

		mainElement = temp.data[currentRow][i];
		temp.multiplyRows(currentRow, 1.0 / mainElement);
		for (int j = currentRow + 1; j < m.rows; j++) {
			temp.addRows(j, currentRow, -temp.data[j][i]);
		}
		currentRow += 1;
	}
	for (int i = m.cols - 1; i >= 0; i--) {
		if (temp.isZeroCol(i)) continue;
		int nonZeroRow = -1;
		for (int j = m.rows - 1; j >= 0; j--) {
			if (fabs(temp.data[j][i]) > 1e-10) {
				nonZeroRow = j;
				break;
			}
		}
		if (nonZeroRow == -1) continue;
		for (int j = nonZeroRow - 1; j >= 0; j--) {
			temp.addRows(j, nonZeroRow, -temp.data[j][i]);
		}
	}
	for (int i = 0; i < temp.getRows(); i++) {
		for (int j = 0; j < temp.getCols(); j++) {
			if (fabs(temp.data[i][j]) < 1e-10) temp.data[i][j] = 0;
		}
	}
	return temp;
}

void Matrix::LUdecomposition(map<string, Matrix>& matrices) const {
	if (rows != cols || fabs(determinant()) < 1e-10)throw invalid_argument("Only invertible matrices can be decomposed into LU.");
	for (int i = 0; i < rows; i++) {
		if (fabs(data[i][i]) < 1e-10)throw invalid_argument("One of the main elements is zero.");
	}
	Matrix temp(rows, 2 * cols), L(rows, cols), U(rows, cols);
	for (int i = 0; i < rows; i++) {
		temp.set(i, cols + i, 1.0);
		for (int j = 0; j < cols; j++) {
			temp.set(i, j, get(i, j));
		}
	}
	for (int i = 0; i < cols - 1; i++) {
		for (int j = i + 1; j < rows; j++) {
			temp.addRows(j, i, -temp.get(j, i) / temp.get(i, i));
		}
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			U.set(i, j, temp.get(i, j));
		}
		for (int j = cols; j < 2 * cols; j++) {
			L.set(i, j - cols, temp.get(i, j));
		}
	}
	L = inverse(L);
	matrices["L"] = L;
	matrices["U"] = U;
	cout << "The matrix L is:" << endl;
	L.print();
	cout << "The matrix U is:" << endl;
	U.print();
}

Matrix Matrix::SchmidtOrtho(const Matrix& m) {
	if (m.rank() < m.cols)throw invalid_argument("The column vectors of the matrix are linearly dependent.");

	Matrix result(m.rows, m.cols), temp(m);
	vector<Vector>a(m.cols);//列向量组
	vector<Vector>b(m.cols);//正交向量组

	for (int i = 0; i < m.cols; i++) {
		Vector colvector(m.rows, transpose(temp).data[i]);
		a[i] = colvector;
	}
	b[0] = a[0];
	for (int i = 1; i < m.cols; i++) {
		Vector sum(m.rows);
		for (int j = 0; j < i; j++) {
			sum = (a[i].dot(b[j]) / b[j].dot(b[j])) * b[j] + sum;
		}
		b[i] = a[i] - sum;
	}
	for (int i = 0; i < m.cols; i++) {
		b[i] = b[i] / b[i].modulus();
	}
	for (int i = 0; i < m.rows; i++) {
		for (int j = 0; j < m.cols; j++) {
			result.set(i, j, b[j].get(i));
		}
	}
	return result;
}

Matrix Matrix::sinm(const Matrix& m) {
	if (m.rows != m.cols)throw invalid_argument("The matrix must be square.");
	if (m.rows == 1) {
		return Matrix(sin(m.data[0][0]));
	}
	else {
		Matrix temp = m;
		Matrix result(m.rows, m.cols);
		int i = 1;
		while (true) {
			result += temp;
			temp = -temp * m * m / (2 * i * (2 * i + 1));
			if (temp.norm() < 1e-20)break;
			i++;
			if (i > 1e6)throw invalid_argument("The matrix does not converge.");
		}
		return result;
	}
}

Matrix Matrix::cosm(const Matrix& m) {
	if (m.rows != m.cols)throw invalid_argument("The matrix must be square.");
	if (m.rows == 1) {
		return Matrix(cos(m.data[0][0]));
	}
	else {
		Matrix temp = -m * m / 2;
		Matrix result = identity(m.rows);
		int i = 2;
		while (true) {
			result += temp;
			temp = -temp * m * m / (2 * i * (2 * i - 1));
			if (temp.norm() < 1e-20)break;
			i++;
			if (i > 1e6)throw invalid_argument("The matrix does not converge.");
		}
		return result;
	}
}

Matrix Matrix::tanm(const Matrix& m) {
	if (m.rows != m.cols)throw invalid_argument("The matrix must be square.");
	if (m.rows == 1) {
		if (cos(m.data[0][0]) == 0)throw invalid_argument("The tangent value is undefined.");
		return Matrix(tan(m.data[0][0]));
	}
	else {
		if ((cosm(m)).determinant() == 0)throw invalid_argument("The tangent value is undefined.");
		Matrix result = sinm(m) / cosm(m);
		return result;
	}
}

Matrix Matrix::sinh(const Matrix& m) {
	return (expm(m) - expm(-m)) / 2;
}

Matrix Matrix::cosh(const Matrix& m) {
	return (expm(m) + expm(-m)) / 2;
}

Matrix Matrix::tanh(const Matrix& m) {
	return sinh(m) / cosh(m);
}

Matrix Matrix::lnm(const Matrix& m) {
	if (m.rows != m.cols)throw invalid_argument("The matrix must be square.");
	if (m.rows == 1) {
		if (m.data[0][0] <= 0)throw invalid_argument("The number must be positive.");
		return Matrix(log(m.data[0][0]));
	}
	else {
		Matrix result(m.rows, m.cols);
		Matrix temp = m - identity(m.rows);
		if (temp.norm() < 1) {
			int i = 1;
			while (true) {
				if (((temp ^ i) / i).norm() < 1e-20)break;
				result += (pow(-1, i + 1) * (temp ^ i) / i);
				i++;
			}
			return result;
		}
		else if (m.diagonalizable()) {
			Matrix P = pForDiag(m), D = diagonalize(m);
			if (D.get(0, 0) < 1e-10)throw invalid_argument("The matrix must be positive definite.");
			if ((inverse(P) * m * P - D).norm() < 1e-11) {
				Matrix result(m.rows, m.cols);
				for (int i = 0; i < m.rows; i++) {
					result.set(i, i, log(D.data[i][i]));
				}
				result = P * result * inverse(P);
				return result;
			}
			else throw invalid_argument("The logarithm cannot be calculated");
		}
		else throw invalid_argument("The logarithm cannot be calculated");
	}
}

Matrix Matrix::sqrtm(const Matrix& m) {
	if (m.rows != m.cols)throw invalid_argument("The matrix must be square.");
	if (m.rows == 1) {
		return Matrix(sqrt(m.get(0, 0)));
	}
	else if (m.diagonalizable()) {
		Matrix P = pForDiag(m), D = diagonalize(m);
		Matrix result(m.rows, m.cols);
		if (D.get(0, 0) < 0)throw runtime_error("The matrix must be positive semi-definite.");
		for (int i = 0; i < m.rows; i++) {
			result.set(i, i, sqrt(D.get(i, i)));
		}
		result = P * result * inverse(P);
		return result;
	}
	else if ((m - identity(m.rows)).norm() < 1)return m ^ Matrix(0.5);
	else throw runtime_error("The square root of the matrix cannot be calculated.");
}

Matrix Matrix::Hessenberg()const {
	Matrix m(*this);
	if (m.rows != m.cols)throw invalid_argument("The matrix must be square.");
	Matrix A(m);
	int i = 0;
	while (i < 1e6) {
		Matrix Q = SchmidtOrtho(A), R = inverse(Q) * A;
		A = R * Q;
		i++;
		if (A.upper()) {
			return A;
		}
	}
	throw runtime_error("The matrix does not converge for eigenvalues.");
}

Matrix Matrix::eigenvalue(const Matrix& m) {
	if (m.rows != m.cols)throw invalid_argument("The matrix must be square.");
	Matrix I = identity(m.rows);
	int n = 0; Matrix A(1, 0), B(1, 1);
	while ((A.cols != B.cols || A.cols == 0) && n < 100) {
		B = A;
		A = (m - n * I).ev();
		for (int i = 0; i < A.cols; i++) {
			A.data[0][i] += n;
		}
		n++;
	}
	if (n == 100)throw runtime_error("The matrix does not converge for eigenvalues.");
	for (int i = 0; i < A.cols; i++) {
		if (fabs(A.data[0][i]) < 1e-11)A.data[0][i] = 0;
	}
	return A;
}

Matrix Matrix::ev()const {
	Matrix m(*this);
	if (m.rows != m.cols)throw invalid_argument("The matrix must be square.");
	int n = 0;
	vector<double>B(m.rows);
	Matrix I = identity(m.rows);
	while (n < 1000) {
		Matrix C = m - n * I;
		bool a(fabs((C).determinant()) > 1e-10);
		if (a) {
			Matrix Q = SchmidtOrtho(C), R = inverse(Q) * C;
			if (C.upper())break;
			if (!C.hasSet() && !(Q * R == R * Q))break;
		}
		n++;
	}
	if (n == 1000)throw invalid_argument("The matrix does not converge for eigenvalues.");
	Matrix A = (m - n * I).Hessenberg();
	for (int i = 0; i < A.rows; i++) B[i] = A.data[i][i] + n;
	sort(B.begin(), B.end());
	for (int i = 1; i < m.rows; i++) {
		int j = i - 1;
		if (fabs(B[j] - B[i]) < 1e-7)B[i] = B[j];
	}
	int k = static_cast<int>(B.size());
	for (int i = 0; i < k; i++) {
		if (basicSolutionSet(m - B[i] * I).cols == 0) {
			B.erase(B.begin() + i);
			i--; k--;
		}
	}
	int cols = static_cast<int>(B.size());
	return Matrix(1, cols, vector<vector<double>>(1, B));
}

Matrix Matrix::integR(const Matrix& other) const {
	if (rows != other.rows)throw invalid_argument("The rows of the matrices do not match.");
	Matrix result(rows, cols + other.cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			result.set(i, j, data[i][j]);
		}
		for (int j = 0; j < other.cols; j++) {
			result.set(i, j + cols, other.data[i][j]);
		}
	}
	return result;
}

Matrix Matrix::integC(const Matrix& other) const {
	if (cols != other.cols)throw invalid_argument("The cols of the matrices do not match.");
	Matrix result(rows + other.rows, cols);
	for (int i = 0; i < rows; i++) {
		result.data[i] = data[i];
	}
	for (int i = 0; i < other.rows; i++) {
		int j = i + rows;
		result.data[j] = other.data[i];
	}
	return result;
}

Matrix Matrix::integD(const Matrix& other) const {
	Matrix result(rows + other.rows, cols + other.cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			result.set(i, j, get(i, j));
		}
	}
	for (int i = 0; i < other.rows; i++) {
		for (int j = 0; j < other.cols; j++) {
			result.set(i + rows, j + cols, other.get(i, j));
		}
	}
	return result;
}

Matrix Matrix::tr(const Matrix& m) {
	return Matrix(m.trace());
}

Matrix Matrix::diagonalize(const Matrix& m) {
	if (m.rows != m.cols)throw invalid_argument("The matrix must be square.");
	vector<double>evalue = eigenvalue(m).data[0];
	if (evalue.size() != m.rows)throw runtime_error("The matrix might have imaginary eigenvalues.");
	Matrix result(m.rows, m.cols);
	if (!m.diagonalizable())throw invalid_argument("The matrix is not diagonalizable.");
	else {
		for (int i = 0; i < m.rows; i++) {
			result.set(i, i, evalue[i]);
		}
	}
	return result;
}

Matrix Matrix::pForDiag(const Matrix& m) {
	if (!m.diagonalizable())throw invalid_argument("The matrix cannot be diagonalized.");
	Matrix D = diagonalize(m), I = identity(D.rows);
	vector<double>evalue;
	for (int i = 0; i < D.rows; i++) {
		if (evalue.empty() || evalue.back() != D.get(i, i))evalue.push_back(D.get(i, i));
		else continue;
	}
	Matrix result(D.rows, 0);
	for (double e : evalue) {
		result = integR(result, basicSolutionSet(m - e * I));
	}
	return result;
}

void Matrix::eigenvector()const {
	Matrix m(*this);
	if (m.rows != m.cols)throw invalid_argument("The matrix must be square.");
	vector<double>evalue = eigenvalue(m).data[0];
	Matrix I = identity(m.rows);
	map<double, int>frequency;
	for (double i : evalue) {
		frequency[i]++;
	}
	double a;
	for (auto it = frequency.begin(); it != frequency.end(); it++) {
		a = it->first;
		cout << "For eigenvalue " << a << " (" << it->second << "-fold):" << endl;
		Matrix A = m - a * I;
		doBasicSolutionSet(A);
	}
}

void Matrix::QRdecomposition(map<string, Matrix>& matrices) const {
	if (rank() < cols)throw invalid_argument("The column vectors of the matrix are linearly dependent.");

	Matrix Q(rows, cols), R(cols, cols), temp(*this);
	vector<Vector>a(cols);//列向量组
	vector<Vector>b(cols);//正交向量组

	for (int i = 0; i < cols; i++) {
		Vector colvector(rows, transpose(temp).data[i]);
		a[i] = colvector;
	}
	b[0] = a[0];
	R.set(0, 0, 1.0);
	for (int i = 1; i < cols; i++) {
		R.set(i, i, 1.0);
		Vector sum(rows);
		for (int j = 0; j < i; j++) {
			sum = (a[i].dot(b[j]) / b[j].dot(b[j])) * b[j] + sum;
			R.set(j, i, a[i].dot(b[j]) / b[j].dot(b[j]));
		}
		b[i] = a[i] - sum;
	}
	for (int i = 0; i < cols; i++) {
		R.multiplyRows(i, b[i].modulus());
		b[i] = b[i] / b[i].modulus();
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			Q.set(i, j, b[j].get(i));
		}
	}
	matrices["Q"] = Q;
	matrices["R"] = R;
	cout << "The matrix Q is:" << endl;
	Q.print();
	cout << "The matrix R is:" << endl;
	R.print();
}

void Matrix::newInput(map<string, Matrix>& matrices) {
	int rows, cols;
	map<string, double>variables = {
		{"PI",3.14159265358979323846264},{"E",2.7182818284590452353602874}
	};
	string name, _rows, _cols, _a;
	double a;
	cout << "The name of the matrices can only consist of letters." << endl;
	cout << "Enter \"end\" to finish definition." << endl;
	while (1) {
		bool invalidInput = false;
		cout << "Enter the name of the matrix (enter \"end\" to finish):" << endl;
		cin >> name;
		for (char i : name) {
			if (!isalpha(i)) {
				cout << "Invalid name." << endl;
				invalidInput = true;
				break;
			}
		}
		if (name == "end")break;
		if (invalidInput)continue;
		cout << "Enter the number of rows and columns for the matrix: ";
		std::cin >> _rows >> _cols;
		for (char i : _rows) {
			if (!isdigit(i)) {
				invalidInput = true;
				break;
			}
		}
		for (char i : _cols) {
			if (!isdigit(i)) {
				invalidInput = true;
				break;
			}
		}
		if (invalidInput) {
			cout << "Invalid input.";
			continue;
		}
		rows = stoi(_rows);
		cols = stoi(_cols);
		if (rows <= 0 || cols <= 0) {
			cout << "The dimensions of the matrix must be positive." << endl;
			continue;
		}
		cout << "Enter the elements of the matrix (row by row): " << endl;
		Matrix matrix(rows, cols);
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				std::cin >> _a;
				try {
					a = Normal::parsen(_a, variables);
				}
				catch (const exception& e) {
					cerr << "Error: " << e.what() << endl;
					j--;
					continue;
				}
				matrix.set(i, j, a);
			}
		}
		matrices[name] = matrix;
		cout << "The Matrix " + name + " is successfully created." << endl;
		storem(matrices);
	}
}

Matrix Matrix::parseFunctionm(const string& expr, map<string, Matrix>& matrices) {
	size_t currentPos = 0;
	string identifier, number;
	Matrix m, p;
	int n = 0;
	while (currentPos < expr.length() && (isalpha(expr[currentPos]) || expr[currentPos] == '_')) {
		identifier += expr[currentPos++];
	}
	if (expr[currentPos] == '(') {
		++currentPos;
		m = parseExpressionm(expr, currentPos, matrices);
		if (expr[currentPos] == ')' && currentPos + 1 == expr.length()) {
			if (identifier == "LU") {
				m.LUdecomposition(matrices);
				throw true;
			}
			else if (identifier == "QR") {
				m.QRdecomposition(matrices);
				throw true;
			}
			else if (identifier == "V" || identifier == "eigenvector") {
				m.eigenvector();
				throw true;
			}
			else if (identifier == "solve") {
				p = Matrix(m.rows, 1);
				doSolution(m, p);
				throw true;
			}
			else if (identifier == "print") {
				m.print();
				throw true;
			}
		}
		else if (expr[currentPos] == ',') {
			++currentPos;
			p = parseExpressionm(expr, currentPos, matrices);
			if (expr[currentPos] == ')' && currentPos + 1 == expr.length()) {
				if (identifier == "solve") {
					doSolution(m, p);
					throw true;
				}
			}
		}
	}
	currentPos = 0;
	Matrix result = parseExpressionm(expr, currentPos, matrices);
	if (currentPos == expr.length())return result;
	else throw invalid_argument("Unexpected character: " + string(1, expr[currentPos]));
}

Matrix Matrix::parseExpressionm(const string& expr, size_t& currentPos, const map<string, Matrix>& matrices) {
	Matrix result = parseTermm(expr, currentPos, matrices);

	while (currentPos < expr.length()) {
		char op = expr[currentPos];
		if (mop.find(op) != mop.end()) {
			++currentPos;
			Matrix rhs = parseTermm(expr, currentPos, matrices);
			switch (op) {
			case '+': result += rhs; break;
			case '-': result -= rhs; break;
			default: throw runtime_error("Unknown operator.");
			}
		}
		else break;
	}
	return result;
}

Matrix Matrix::parseTermm(const string& expr, size_t& currentPos, const map<string, Matrix>& matrices) {
	Matrix result = parseLDivisionm(expr, currentPos, matrices);
	char op;

	while (currentPos < expr.length()) {
		op = expr[currentPos];
		if (mop.find(op) != mop.end()) {
			if (mop[op] == 2) { // 只处理乘法和除法
				++currentPos;
				Matrix rhs = parseLDivisionm(expr, currentPos, matrices);
				switch (op) {
				case '*':
					result *= rhs;
					break;
				case '/':
					if (rhs == Matrix(0.0)) throw runtime_error("Division by zero error.");
					result /= rhs;
					break;
				default:
					throw runtime_error("Unknown operator.");
				}
			}// 如果遇到加法或减法，应该停止解析当前项
			else break;
		}
		else break;
	}
	return result;
}

Matrix Matrix::parseLDivisionm(const string& expr, size_t& currentPos, const map<string, Matrix>& matrices) {
	Matrix result = parsePowerm(expr, currentPos, matrices);
	char op;
	while (currentPos < expr.length()) {
		op = expr[currentPos];
		if (mop.find(op) != mop.end()) {
			if (mop[op] == 3) { // 只处理乘法和除法
				++currentPos;
				Matrix rhs = parseLDivisionm(expr, currentPos, matrices);
				switch (op) {
				case '\\': result = Ldivide(result, rhs); break;
				default: throw runtime_error("Unknown operator.");
				}
			}
			else break;
		}
		else break;
	}
	return result;
}

Matrix Matrix::parsePowerm(const string& expr, size_t& currentPos, const map<string, Matrix>& matrices) {
	Matrix result = 0.0; // 初始化result
	double sign = 1.0; // 用于处理正负号

	if (currentPos < expr.length()) {
		if (expr[currentPos] == '+') {
			sign = 1.0;
			++currentPos;
		}
		else if (expr[currentPos] == '-') {
			sign = -1.0;
			++currentPos;
		}
	}

	if (currentPos < expr.length()) {
		if (expr[currentPos] == '(') {
			++currentPos;
			result = sign * parseExpressionm(expr, currentPos, matrices);
			if (expr[currentPos] == ')') {
				++currentPos;
			}
			else {
				throw runtime_error("Missing closing parenthesis.");
			}
		}
		else if (isdigit(expr[currentPos]) || expr[currentPos] == '.') {
			string number;
			while (currentPos < expr.length() && (isdigit(expr[currentPos]) || expr[currentPos] == '.')) {
				number += expr[currentPos++];
			}
			result = sign * stod(number);
		}
		else if (isalpha(expr[currentPos])) { // 支持函数和变量
			string identifier;
			while (currentPos < expr.length() && (isalpha(expr[currentPos]) || expr[currentPos] == '_')) {
				identifier += expr[currentPos++];
			}
			if (expr[currentPos] == '(') { // 检查是否为函数
				++currentPos; // 消耗 '('
				Matrix argument = parseExpressionm(expr, currentPos, matrices);
				if (expr[currentPos] == ')') {
					++currentPos; // 消耗 ')'
					auto it = functionm.find(identifier);
					if (it != functionm.end())result = sign * it->second(argument);
					else throw runtime_error("Unknown function: " + identifier);
				}
				else if (expr[currentPos] == ',') {
					++currentPos;
					Matrix a2 = parseExpressionm(expr, currentPos, matrices);
					if (expr[currentPos] == ')') {
						++currentPos;
						auto it = functionm2.find(identifier);
						if (it != functionm2.end())result = sign * it->second(argument, a2);
						else throw runtime_error("Unknown function: " + identifier);
					}
					else if (expr[currentPos] == ',') {
						++currentPos;
						Matrix a3 = parseExpressionm(expr, currentPos, matrices);
						if (expr[currentPos] == ')') {
							++currentPos;
							auto it = functionm3.find(identifier);
							if (it != functionm3.end())result = sign * it->second(argument, a2, a3);
							else throw runtime_error("Unknown function: " + identifier);
						}
						else if (expr[currentPos] == ',') {
							++currentPos;
							Matrix a4 = parseExpressionm(expr, currentPos, matrices);
							if (expr[currentPos] == ')') {
								++currentPos;
								auto it = functionm4.find(identifier);
								if (it != functionm4.end()) result = sign * it->second(argument, a2, a3, a4);
								else throw runtime_error("Unknown function: " + identifier);
							}
							else throw runtime_error("Missing closing parenthesis.");
						}
						else throw runtime_error("Missing closing parenthesis.");
					}
					else throw runtime_error("Missing closing parenthesis.");
				}
				else throw runtime_error("Missing closing parenthesis.");
			}
			else { // 否则视为矩阵
				auto it = matrices.find(identifier);
				if (it != matrices.end()) {
					result = sign * it->second;
				}
				else {
					throw runtime_error("Undefined variable: " + identifier);
				}
			}
		}
		else if (!isspace(expr[currentPos])) {
			throw runtime_error("Unexpected character: " + string(1, expr[currentPos]));
		}
	}

	while (currentPos < expr.length() && expr[currentPos] == '^') {
		++currentPos;
		Matrix rhs = parsePowerm(expr, currentPos, matrices);
		result ^= rhs;
	}
	return result;
}

void Matrix::newMatrix() {
	map<string, Matrix>matrices;
	char choice;

	cout << "Welcome to the Matrix Calculator!" << endl << endl;
	cout << "Do you want to use the variables you defined before?(Y/N)" << endl
		<< "If no,the variable list will be cleared." << endl;
	do {
		cin >> choice;
		if (choice == 'Y') {
			matrices = loadm();
			cout << "The variables are avaliable now." << endl;
		}
		else if (choice == 'N') {
			cout << "The variable list is cleared." << endl;
		}
		else cout << "Invalid choice. Choose again." << endl;
	} while (choice != 'Y' && choice != 'N');

	matrices["PI"] = Matrix(3.141592653589793238462643383279502);
	matrices["E"] = Matrix(2.7182818284590452353602874);
	matrices["ANS"] = Matrix();
	storem(matrices);
	string expression, name;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl << "There are some commands you can use:" << endl;
	cout << "define - to define some matrix variables." << endl;
	cout << "delete - to delete some matrix variables." << endl;
	cout << "show - to find out which variables are available." << endl;
	cout << "function_list - to find out which functions are available." << endl;
	cout << "end - to quit this mode." << endl;
	cout << "save - to save the result in a matrix." << endl;
	cout << "The result of the previous calculation is automatically stored in \"ANS\"." << endl << endl;

	while (1) {
		cout << "Enter an expression:" << endl;
		getline(cin, expression);
		if (expression == "define") {
			newInput(matrices);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (expression == "delete") {
			deletem(matrices);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (expression == "show") {
			showm(matrices);
			continue;
		}
		if (expression == "function_list") {
			cout << endl;
			cout << "The following functions can only be used individually with defined matrices:" << endl;
			cout << "Print - print(M)" << endl;
			cout << "LU decomposition - LU(M)" << endl;
			cout << "QR decomposition - QR(M)" << endl;
			cout << "Eigenvector - V(M) or eigenvector(M)" << endl;
			cout << "Solve linear equations - solve(M,M)" << endl;
			cout << "Solve homogeneous linear equations - solve(M)" << endl << endl;
			cout << "The following functions can be used with expressions:" << endl;
			cout << "Determinant - D(M) or det(M)" << endl;
			cout << "Rank - R(M) or rank(M)" << endl;
			cout << "Transpose - T(M) or trans(M)" << endl;
			cout << "Inverse with Gaussian Elimination - I(M)" << endl;
			cout << "Inverse with adjugate matrix - inv(M)" << endl;
			cout << "Adjugate matrix - A(M) or adj(M)" << endl;
			cout << "Reduced row echelon form - G(M) or gauss(M)" << endl;
			cout << "Orthogonalization - O(M) or ortho(M)" << endl;
			cout << "Norm - N(M) or norm(M)" << endl;
			cout << "Diagonalization - diag(M)" << endl;
			cout << "Eigenvalue - E(M) or eig(M)" << endl;
			cout << "Sum of all elements - sum(M)" << endl;
			cout << "Product of all elements - pro(M)" << endl;
			cout << "Trace - tr(M)" << endl;
			cout << "Sine - sin(M)" << endl;
			cout << "Cosine - cos(M)" << endl;
			cout << "Tangent - tan(M)" << endl;
			cout << "Sinh - sinh(M) or sh(M)" << endl;
			cout << "Cosh - cosh(M) or ch(M)" << endl;
			cout << "Tanh - tanh(M) or th(M)" << endl;
			cout << "Square root - sqrt(M)" << endl;
			cout << "Natural logarithm - ln(M) or log(M)" << endl;
			cout << "Exponential function - exp(M)" << endl;
			cout << "Integrate by rows - integR(M)" << endl;
			cout << "Integrate by columns - integC(M)" << endl;
			cout << "Integrate by diagonals - integD(M)" << endl;
			cout << "Swap Rows - swapR(M,N,N)" << endl;
			cout << "Swap Columns - swapC(M,N,N)" << endl;
			cout << "Mutiply Rows - multiR(M,N,R)" << endl;
			cout << "Mutiply Columns - multiC(M,N,R)" << endl;
			cout << "Add Rows - addR(M,N,N,R)" << endl;
			cout << "Add Columns - addC(M,N,N,R)" << endl;
			cout << "Get Rows - row(M)" << endl;
			cout << "Get Columns - col(M)" << endl;
			cout << "Get a Row - getR(M,N)" << endl;
			cout << "Get a Column - getC(M,N)" << endl;
			cout << "delete a Row - delR(M,N)" << endl;
			cout << "delete a Column - delC(M,N)" << endl;
			cout << "Get Element - get(M,N,N)" << endl;
			cout << "Set Element - set(M,N,N,R)" << endl;
			cout << "Random matrix with max and min - random(N+,N+,Z,Z)" << endl;
			cout << "Random matrix (0~1) - random(N+,N+)" << endl;
			cout << "Random square matrix (0~1) - random(N+,N+)" << endl;
			cout << "Random integer matrix with max and min - randint(N+,N+,Z,Z)" << endl;
			cout << "Random integer matrix (0~10) - randint(N+,N+)" << endl;
			cout << "Random integer square matrix (0~10) - randint(N+,N+)" << endl;
			cout << "Cofactor - cof(M,N,N)" << endl;
			cout << "Algebric cofactor - Acof(M,N,N)" << endl;
			cout << "All one matrix - ones(N+,N+)" << endl;
			cout << "All zero matrix - zero(N+,N+)" << endl;
			cout << "All one square matrix - ones(N+)" << endl;
			cout << "All zero square matrix - zero(N+)" << endl;
			cout << "Identity matrix - id(N+)" << endl;
			cout << "Magic matrix - magic(N+)" << endl;
			cout << "Degree - deg(R)" << endl;
			cout << "Radian - rad(R)" << endl << endl;
			continue;
		}
		if (expression == "save") {
			bool invalidSave = false;
			cout << "Enter the name of matrix where you want to store the result:" << endl;
			cin >> name;
			for (char i : name) {
				if (!isalpha(i)) {
					cout << "Invalid name." << endl;
					invalidSave = true;
					break;
				}
			}
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (invalidSave)continue;
			else {
				matrices[name] = matrices["ANS"];
				cout << "The result is successfully saved in Matrix " + name << endl;
				storem(matrices);
			}
			continue;
		}
		if (expression == "end") break;
		try {
			cout << "The result is: " << endl;
			Matrix result = parseFunctionm(expression, matrices);
			matrices["ANS"] = result;
			storem(matrices);
			result.print();
		}
		catch (const exception& e) {
			cout << "Error: " << e.what() << endl;
		}
		catch (const bool& b) {
			if (b)cout << "The result cannot be saved." << endl;
		}
	}
}
