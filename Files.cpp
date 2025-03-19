#include<fstream>
#include<filesystem>
#include"Complex.h"
#include"Base.h"
#include"Matrix.h"
#include"Vector.h"
#include"Statistics.h"
#include"Normal.h"
#include "Function.h"

using namespace std;

void initialize() {
	filesystem::path p = R"(D:\Calculator)";
	filesystem::create_directories(p);

	vector<string> fileNames = {
		"Complex.txt", "Vector.txt", "Matrix.txt",
		"Base.txt", "Statistics.txt", "Normal.txt", "Function.txt"
	};

	for (const auto& fileName : fileNames) {
		ofstream file(p / fileName, ios::app);
		if (!file.is_open()) {
			throw invalid_argument("The file " + fileName + " cannot be opened.");
		}
		file.close();
	}
}

//存储变量

template<typename T>
void store(const map<string, T>& variables, const string& fileName) {
	string filename = R"(D:\Calculator\)" + fileName;
	ofstream fout(filename);
	if (!fout.is_open()) {
		throw invalid_argument("The file " + filename + " cannot be opened.");
	}
	for (const auto& [name, value] : variables) {
		fout << name << " " << value << endl;
	}
}

void Complex::storec(const map<string, Complex>& complexes) {
	store(complexes, "Complex.txt");
}

void Matrix::storem(const map<string, Matrix>& matrices) {
	store(matrices, "Matrix.txt");
}

void Vector::storev(const map<string, Vector>& vectors) {
	store(vectors, "Vector.txt");
}

void Base::storeb(const map<string, Base>& baseNumbers) {
	store(baseNumbers, "Base.txt");
}

void Statistics::stores(const map<string, Statistics>& datasets) {
	store(datasets, "Statistics.txt");
}

void Normal::storen(const map<string, double>& numbers) {
	store(numbers, "Normal.txt");
}

void Function::storef(const map<string, Function>& functions) {
	store(functions, "Function.txt");
}

//分解字符串
static vector<string>getTokens(const string& _line) {
	vector<string>tokens;
	size_t pos = 0;
	string token, line = _line;
	while ((pos = line.find(' ')) != string::npos) {
		token = line.substr(0, pos);
		tokens.push_back(token);
		line.erase(0, pos + 1);
	}
	tokens.push_back(line);
	return tokens;
}

//读取变量
map<string, Complex>Complex::loadc() {
	string filename = R"(D:\Calculator\Complex.txt)";
	ifstream fin(filename);
	if (!fin.is_open())throw invalid_argument("The file " + filename + " cannot be opened.");
	map<string, Complex>complexs;
	string complex;
	while (getline(fin, complex)) {
		vector<string>tokens = getTokens(complex);
		string name = tokens[0];
		double real = stod(tokens[1]), image = stod(tokens[2]);
		complexs[name] = Complex(real, image);
	}
	return complexs;
}

map<string, Matrix>Matrix::loadm() {
	string filename = R"(D:\Calculator\Matrix.txt)";
	ifstream fin(filename);
	if (!fin.is_open())throw invalid_argument("The file " + filename + " cannot be opened.");
	map<string, Matrix>matrices;
	string matrix;
	while (getline(fin, matrix)) {
		vector<string>tokens = getTokens(matrix);
		string name = tokens[0];
		int rows = stoi(tokens[1]), cols = stoi(tokens[2]);
		Matrix m(rows, cols);
		size_t pos = 3;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				m.data[i][j] = stod(tokens[pos++]);
			}
		}
		matrices[name] = m;
	}
	return matrices;
}

map<string, Vector>Vector::loadv() {
	string filename = R"(D:\Calculator\Vector.txt)";
	ifstream fin(filename);
	if (!fin.is_open())throw invalid_argument("The file " + filename + " cannot be opened.");
	map<string, Vector>vectors;
	string _v;
	while (getline(fin, _v)) {
		vector<string>tokens = getTokens(_v);
		string name = tokens[0];
		int n = stoi(tokens[1]);
		Vector v(n);
		size_t pos = 2;
		for (int i = 0; i < n; i++) {
			v.data[i] = stod(tokens[pos++]);
		}
		vectors[name] = v;
	}
	return vectors;
}

map<string, Base>Base::loadb() {
	string filename = R"(D:\Calculator\Base.txt)";
	ifstream fin(filename);
	if (!fin.is_open())throw invalid_argument("The file " + filename + " cannot be opened.");
	map<string, Base>bases;
	string base;
	while (getline(fin, base)) {
		vector<string>tokens = getTokens(base);
		string name = tokens[0];
		unsigned long long b = stoull(tokens[1]),
			data = stoull(tokens[2]);
		bases[name] = Base(b, data);
	}
	return bases;
}

map<string, Statistics>Statistics::loads() {
	string filename = R"(D:\Calculator\Statistics.txt)";
	ifstream fin(filename);
	if (!fin.is_open())throw invalid_argument("The file " + filename + " cannot be opened.");
	map<string, Statistics>datasets;
	string dataset;
	while (getline(fin, dataset)) {
		vector<string>tokens = getTokens(dataset);
		string name = tokens[0];
		vector<double>data;
		for (size_t i = 1; i < tokens.size(); i++) {
			data.push_back(stod(tokens[i]));
		}
		datasets[name] = Statistics(data);
	}
	return datasets;
}

map<string, double>Normal::loadn() {
	string filename = R"(D:\Calculator\Normal.txt)";
	ifstream fin(filename);
	if (!fin.is_open())throw invalid_argument("The file " + filename + " cannot be opened.");
	map<string, double>variables;
	string variable;
	while (getline(fin, variable)) {
		vector<string>tokens = getTokens(variable);
		string name = tokens[0];
		double num = stod(tokens[1]);
		variables[name] = num;
	}
	return variables;
}

map<string, Function>Function::loadf() {
	string filename = R"(D:\Calculator\Function.txt)";
	ifstream fin(filename);
	if (!fin.is_open())throw invalid_argument("The file " + filename + " cannot be opened.");
	map<string, Function>functions;
	string function;
	while (getline(fin, function)) {
		vector<string>tokens = getTokens(function);
		string name = tokens[0], expr = tokens[1];
		functions[name] = Function(expr);
	}
	return functions;
}

//删除变量
template<typename T>
void deleteVariable(map<string, T>& variables, const string& name, const string& fileName) {
	if (variables.erase(name) == 1) {
		cout << "The variable " + name + " is successfully delete." << endl;
		store(variables, fileName);
	}
	else cout << "The variable " + name + " is not found." << endl;
}

void Complex::deletec(map<string, Complex>& variables) {
	string name;
	cout << "Enter end to finish delete." << endl;
	while (1) {
		cout << "Enter the name of the variable you want to delete:" << endl;
		cin >> name;
		if (name == "end")break;
		else deleteVariable(variables, name, "Complex.txt");
	}
}

void Matrix::deletem(map<string, Matrix>& variables) {
	string name;
	cout << "Enter end to finish delete." << endl;
	while (1) {
		cout << "Enter the name of the variable you want to delete:" << endl;
		cin >> name;
		if (name == "end")break;
		else deleteVariable(variables, name, "Matrix.txt");
	}
}

void Vector::deletev(map<string, Vector>& variables) {
	string name;
	cout << "Enter end to finish delete." << endl;
	while (1) {
		cout << "Enter the name of the variable you want to delete:" << endl;
		cin >> name;
		if (name == "end")break;
		else deleteVariable(variables, name, "Vector.txt");
	}
}

void Base::deleteb(map<string, Base>& variables) {
	string name;
	cout << "Enter end to finish delete." << endl;
	while (1) {
		cout << "Enter the name of the variable you want to delete:" << endl;
		cin >> name;
		if (name == "end")break;
		else deleteVariable(variables, name, "Base.txt");
	}
}

void Statistics::deletes(map<string, Statistics>& variables) {
	string name;
	cout << "Enter end to finish delete." << endl;
	while (1) {
		cout << "Enter the name of the variable you want to delete:" << endl;
		cin >> name;
		if (name == "end")break;
		else deleteVariable(variables, name, "Statistics.txt");
	}
}

void Normal::deleten(map<string, double>& variables) {
	string name;
	cout << "Enter end to finish delete." << endl;
	while (1) {
		cout << "Enter the name of the variable you want to delete:" << endl;
		cin >> name;
		if (name == "end")break;
		else deleteVariable(variables, name, "Normal.txt");
	}
}

void Function::deletef(map<string, Function>& variables) {
	string name;
	cout << "Enter end to finish delete." << endl;
	while (1) {
		cout << "Enter the name of the variable you want to delete:" << endl;
		cin >> name;
		if (name == "end")break;
		else deleteVariable(variables, name, "Function.txt");
	}
}

//展示变量
template<typename T>
void show(const map<string, T>& variables) {
	cout << "All the variables you store are:" << endl;
	for (auto it = variables.begin(); it != variables.end(); it++) {
		cout << "Name: " << it->first << endl;
		it->second.print();
	}
}

void Complex::showc(const map<string, Complex>& variables) {
	show(variables);
}

void Matrix::showm(const map<string, Matrix>& variables) {
	show(variables);
}

void Vector::showv(const map<string, Vector>& variables) {
	show(variables);
}

void Base::showb(const map<string, Base>& variables) {
	show(variables);
}

void Statistics::shows(const map<string, Statistics>& variables) {
	show(variables);
}

void Normal::shown(const map<string, double>& variables) {
	cout << "All the variables you store are:" << endl;
	for (auto it = variables.begin(); it != variables.end(); it++) {
		cout << "Name: " << it->first << endl << it->second << endl;
	}
}

void Function::showf(const map<string, Function>& variables) {
	show(variables);
}