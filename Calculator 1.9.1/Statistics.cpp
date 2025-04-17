#include "Statistics.h"
#include"Normal.h"

using namespace std;

map<string, double(*)(const Statistics&)>Statistics::functions1 = {
	{"n",n1},{"sum",sum},{"m",m},{"Var",Var},{"sd",sd},{"sv",sampleVar},{"s",sampleSd},
	{"max",Max},{"min",Min},{"range",range},{"QL",Q1},{"M",M},{"QU",Q3}
};
map<string, double(*)(const Statistics&, const Statistics&)>Statistics::functions2 = {
	{"Cov",Cov},{"r",r},{"Rs",R2}
};

Statistics& Statistics::operator=(const Statistics& s) {
	data = s.data;
	return *this;
}

Statistics& Statistics::operator+=(const Statistics& s) {
	for (int i = 0; i < n(s); i++) {
		data.push_back(s.data[i]);
	}
	return *this;
}

Statistics Statistics::operator+(const Statistics& s) const {
	Statistics temp(*this);
	temp += s;
	return temp;
}

void Statistics::set(const int& pos, const double& value) {
	data[pos] = value;
}

void Statistics::swap(const int& p1, const int& p2) {
	double temp = data[p1];
	data[p1] = data[p2];
	data[p2] = temp;
}

void Statistics::push_back(const double& value) {
	data.push_back(value);
}

void Statistics::pop_back() {
	data.pop_back();
}

double Statistics::get(const int& pos) const {
	return data[pos];
}

void Statistics::print() const {
	cout << "No.\tData" << endl;
	for (int i = 0; i < data.size(); i++) {
		cout << i << '\t' << data[i] << endl;
	}
}

//样本量
int Statistics::n(const Statistics& s) {
	return static_cast<int>(s.data.size());
}

double Statistics::n1(const Statistics& s) {
	return static_cast<double>(s.data.size());
}

double Statistics::sum(const Statistics& s) {
	double S = 0;
	for (auto i : s.data) S += i;
	return S;
}

//平均数
double Statistics::m(const Statistics& s) {
	return sum(s) / n(s);
}

double Statistics::Var(const Statistics& s) {
	double var = 0;
	for (auto i : s.data) {
		var += (i - m(s)) * (i - m(s));
	}
	var /= n(s);
	return var;
}

double Statistics::sd(const Statistics& s) {
	return sqrt(Var(s));
}

double Statistics::sampleVar(const Statistics& s) {
	return Var(s) * n(s) / (n(s) - 1);
}

double Statistics::sampleSd(const Statistics& s) {
	return sqrt(sampleVar(s));
}

double Statistics::Max(const Statistics& s) {
	double max = s.data[0];
	for (auto i : s.data) {
		if (i > max)max = i;
	}
	return max;
}

double Statistics::Min(const Statistics& s) {
	double min = s.data[0];
	for (auto i : s.data) {
		if (i < min)min = i;
	}
	return min;
}

double Statistics::range(const Statistics& s) {
	return Max(s) - Min(s);
}

Statistics Statistics::sort()const {
	int _n = n(*this);
	Statistics temp(*this);
	for (int i = 0; i < _n; i++) {
		int a = _n - 1 - i;
		double max = temp.data[a];
		for (int j = 0; j < _n - i; j++) {
			if (temp.data[j] > max) {
				temp.swap(j, a);
				max = temp.data[a];
			}
		}
	}
	return temp;
}

void Statistics::insert(const int& pos, const double& value) {
	if (pos<0 || pos>n(*this))throw invalid_argument("The position is out of range.");
	auto it = data.begin() + pos;
	data.insert(it, value);
}

void Statistics::erase(const int& pos) {
	if (n(*this) < 3)throw invalid_argument("The dataset is too small to be erased.");
	if (pos<0 || pos>n(*this) - 1)throw invalid_argument("The position is out of range.");
	auto it = data.begin() + pos;
	data.erase(it);
}

void Statistics::add() {
	string expr;
	double _x;
	map<string, double>variables;
	variables["PI"] = 3.14159265358979323846264;
	variables["E"] = 2.7182818284590452353602874;

	cout << "Enter \"end\" to finish addition." << endl;
	while (1) {
		cin >> expr;
		if (expr == "end" && data.size() > 1)break;
		try { _x = Normal::parsen(expr, variables); }
		catch (const exception& e) {
			cerr << "Error: " << e.what() << endl;
			continue;
		}
		data.push_back(_x);
	}
}

double Statistics::percentile(const Statistics& s, const int& p) {
	if (p <= 0 || p >= 100)throw invalid_argument("The p for pth percentile is out of range.");
	vector<double>temp = s.sort().data;
	double P = (p / 100.0) * n(s);
	int k = static_cast<int>(ceil(P));
	double f = P - k;
	int m = k - 1;//避免溢出
	if (f != 0) return temp[m];
	else return (temp[m] + temp[k]) / 2;
}

double Statistics::Q1(const Statistics& s) {
	return percentile(s, 25);
}

//中位数
double Statistics::M(const Statistics& s) {
	return percentile(s, 50);
}

double Statistics::Q3(const Statistics& s) {
	return percentile(s, 75);
}

//众数
void Statistics::Mo(const Statistics& s) {
	vector<double>temp = s.data;
	vector<double> modes;
	map<double, int>frequency;
	for (double i : temp) {
		frequency[i]++;
	}
	int maxF = 0;
	for (const auto& pair : frequency)if (pair.second > maxF)maxF = pair.second;
	for (const auto& pair : frequency)if (pair.second == maxF)modes.push_back(pair.first);
	for (double mode : modes)cout << mode << " ";
	cout << endl;
}

void Statistics::linearRegression(const Statistics& s) {
	Statistics s1;
	s1.set(1, 1);
	for (int i = 2; i < n(s); i++) {
		s1.push_back(i);
	}
	linearRegression(s1, s);
}

double Statistics::Cov(const Statistics& s1, const Statistics& s2) {
	if (n(s1) != n(s2))throw invalid_argument("The two datasets have different data size.");
	vector<double>p(n(s1));
	Statistics temp(p);
	double m1 = m(s1), m2 = m(s2);
	for (int i = 0; i < n(s1); i++) {
		temp.data[i] = (s1.data[i] - m1) * (s2.data[i] - m2);
	}
	return m(temp);
}

//相关系数
double Statistics::r(const Statistics& s1, const Statistics& s2) {
	if (n(s1) != n(s2))throw invalid_argument("The two datasets have different data size.");
	return Cov(s1, s2) / (sd(s1) * sd(s2));
}

//决定系数
double Statistics::R2(const Statistics& s1, const Statistics& s2) {
	if (n(s1) != n(s2))throw invalid_argument("The two datasets have different data size.");
	double b = Cov(s1, s2) / Var(s1);
	double a = m(s2) - b * m(s1);
	double res = 0, tot = Var(s2) * n(s2);
	for (int i = 0; i < n(s1); i++) {
		double _y = a + b * s1.data[i];
		res += (s2.data[i] - _y) * (s2.data[i] - _y);
	}
	return 1.0 - res / tot;
}

void Statistics::linearRegression(const Statistics& s1, const Statistics& s2) {
	if (n(s1) != n(s2))throw invalid_argument("The two datasets have different data size.");
	double b = Cov(s1, s2) / Var(s1);
	double a = m(s2) - b * m(s1);
	cout << "Intercept - a: " << a << endl;
	cout << "Slope - b: " << b << endl;
	cout << "Correlation coefficient - r: " << r(s1, s2) << endl;
	cout << "Coefficient of Determination - R^2: " << R2(s1, s2) << endl;
}

void Statistics::newInput(map<string, Statistics>& datasets) {
	string expr, name;
	map<string, double>variables;
	double _x;
	variables["PI"] = 3.14159265358979323846264;
	variables["E"] = 2.7182818284590452353602874;

	cout << "The name of the datasets can only consist of letters." << endl;
	cout << "Enter \"end\" to finish definition." << endl;
	while (1) {
		bool invalidInput = false;
		cout << "Enter the name of the dataset (enter \"end\" to finish):" << endl;
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
		cout << "Enter the data (enter 'end' to finish):" << endl;
		vector<double>x;
		while (1) {
			cin >> expr;
			if (expr == "end" && x.size() > 1)break;
			else if (expr == "end") {
				cout << "More data are needed." << endl;
				continue;
			}
			try { _x = Normal::parsen(expr, variables); }
			catch (const exception& e) {
				cerr << "Error: " << e.what() << endl;
				continue;
			}
			x.push_back(_x);
		}
		Statistics X(x);
		datasets[name] = X;
		cout << "The Dataset " + name + " is successfully created." << endl;
		stores(datasets);
	}
}

void Statistics::newStatistics() {
	map<string, Statistics>datasets;
	char choice;

	cout << "Welcome to the Statistics Calculator!" << endl << endl;
	cout << "You can only utilize functions with defined datasets in this mode." << endl;
	cout << "Do you want to use the variables you defined before?(Y/N)" << endl
		<< "If no,the variable list will be cleared." << endl;
	do {
		cin >> choice;
		if (choice == 'Y') {
			datasets = loads();
			cout << "The variables are avaliable now." << endl;
		}
		else if (choice == 'N') {
			cout << "The variable list is cleared." << endl;
		}
		else cout << "Invalid choice. Choose again." << endl;
	} while (choice != 'Y' && choice != 'N');
	stores(datasets);

	string expression, name;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl << "There are some commands you can use:" << endl;
	cout << "define - to define some datasets." << endl;
	cout << "delete - to delete some datasets." << endl;
	cout << "show - to find out which datasets are avaliable." << endl;
	cout << "function_list - to find out which functions are available." << endl;
	cout << "end - to quit this mode." << endl << endl;

	while (1) {
		cout << "Enter an expression:" << endl;
		getline(cin, expression);
		if (expression == "define") {
			newInput(datasets);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (expression == "delete") {
			deletes(datasets);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (expression == "show") {
			shows(datasets);
			continue;
		}
		if (expression == "function_list") {
			cout << endl
				<< "Print - print(S)" << endl
				<< "Sort - sort(S)" << endl
				<< "Erase - erase(S,N)" << endl
				<< "Insert - insert(S,N,R)" << endl
				<< "Set - set(S,N,R)" << endl
				<< "Sample size - n(S)" << endl
				<< "Sum - sum(S)" << endl
				<< "Mean - m(S)" << endl
				<< "Variance - Var(S)" << endl
				<< "Standard deviation - sd(S)" << endl
				<< "Sample variance - sv(S)" << endl
				<< "Sample standard deviation - s(S)" << endl
				<< "Max - max(S)" << endl
				<< "Min - min(S)" << endl
				<< "Range - range(S)" << endl
				<< "Lower quartile - QL(S)" << endl
				<< "Median - M(S)" << endl
				<< "Upper quartile - QU(S)" << endl
				<< "Modes - Mo(S)" << endl
				<< "Single-variable statistics - sing(S)" << endl
				<< "percentile - per(S,N+)" << endl
				<< "Covariance - Cov(S,S)" << endl
				<< "Correlation coefficient - r(S,S)" << endl
				<< "Coefficient of Determination - Rs(S,S)" << endl
				<< "Linear regression - reg(S,S)" << endl
				<< "Linear regression with natural number - reg(S)" << endl
				<< "Bivariate statistics - bi(S,S)" << endl << endl;
			continue;
		}
		if (expression == "end") break;
		try {
			cout << "The result is:" << endl;
			parseFunctions(expression, datasets);
			stores(datasets);
		}
		catch (const exception& e) {
			cout << "Error: " << e.what() << endl;
		}
	}
}

void Statistics::parseFunctions(const string& expr, map<string, Statistics>& datasets) {
	size_t currentPos = 0;
	string identifier, argument1, argument2;
	int num1 = 0;
	double num2 = 0;
	map<string, double>variables;
	variables["PI"] = 3.14159265358979323846264;
	variables["E"] = 2.7182818284590452353602874;
	while (currentPos < expr.length() && (isalpha(expr[currentPos]) || expr[currentPos] == '_')) {
		identifier += expr[currentPos++];
	}
	if (expr[currentPos] == '(') {
		currentPos++;
		while (currentPos < expr.length() && isalpha(expr[currentPos])) {
			argument1 += expr[currentPos++];
		}
		auto it1 = datasets.find(argument1);
		if (it1 != datasets.end()) {
			Statistics X = it1->second;
			if (expr[currentPos] == ')' && currentPos + 1 == expr.length()) {
				auto that1 = functions1.find(identifier);
				if (that1 != functions1.end()) {
					cout << that1->second(X) << endl;
				}
				else if (identifier == "print") {
					X.print();
				}
				else if (identifier == "Mo") {
					Mo(X);
				}
				else if (identifier == "sort") {
					X = X.sort();
					X.print();
					datasets[argument1] = X;
				}
				else if (identifier == "sing") {
					cout << "The statistical results are as follows:" << endl;
					newSingle(X);
				}
				else if (identifier == "reg") {
					linearRegression(X);
				}
				else if (identifier == "add") {
					X.add();
					datasets[argument1] = X;
					X.print();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
				else throw runtime_error("Invalid function: " + identifier);
			}
			else if (expr[currentPos] == ',') {
				++currentPos;
				if (isalpha(expr[currentPos])) {
					while (currentPos < expr.length() && isalpha(expr[currentPos])) {
						argument2 += expr[currentPos++];
					}
					auto it2 = datasets.find(argument2);
					if (it2 != datasets.end()) {
						Statistics Y = it2->second;
						if (expr[currentPos] == ')' && currentPos + 1 == expr.length()) {
							auto that2 = functions2.find(identifier);
							if (that2 != functions2.end()) {
								cout << that2->second(X, Y) << endl;
							}
							else if (identifier == "reg") {
								cout << "Function model: Y = a + bX" << endl;
								linearRegression(X, Y);
							}
							else if (identifier == "bi") {
								newBivariate(X, Y);
							}
							else throw runtime_error("Invalid function: " + identifier);
						}
						else throw runtime_error("Invalid input");
					}
					else throw runtime_error("Undefined dataset: " + argument2);
				}
				else if (isdigit(expr[currentPos])) {
					while (currentPos < expr.length() && isdigit(expr[currentPos])) {
						argument2 += expr[currentPos++];
					}
					num1 = stoi(argument2);
					if (expr[currentPos] == ')' && currentPos + 1 == expr.length()) {
						if (identifier == "per") {
							cout << percentile(X, num1) << endl;
						}
						else if (identifier == "erase") {
							X.erase(num1);
							X.print();
							datasets[argument1] = X;
						}
						else throw runtime_error("Invalid function: " + identifier);
					}
					else if (expr[currentPos] == ',') {
						++currentPos;
						num2 = Normal::parseExpressionn(expr, currentPos, variables);
						if (expr[currentPos] == ')' && currentPos + 1 == expr.length()) {
							if (identifier == "insert") {
								X.insert(num1, num2);
								X.print();
								datasets[argument1] = X;
							}
							else if (identifier == "set") {
								X.set(num1, num2);
								X.print();
								datasets[argument1] = X;
							}
							else if (identifier == "swap") {
								if (num2 != floor(num2))throw invalid_argument("The second parameter must be an integer.");
								int _num2 = static_cast<int>(num2);
								X.swap(num1, _num2);
								X.print();
								datasets[argument1] = X;
							}
							else throw runtime_error("Invalid function: " + identifier);
						}
						else throw runtime_error("Unexpected character: " + string(1, expr[currentPos]));
					}
					else throw runtime_error("Unexpected character: " + string(1, expr[currentPos]));
				}
				else throw runtime_error("Invalid input");
			}
			else throw runtime_error("Invalid input");
		}
		else throw runtime_error("Undefined dataset: " + argument1);
	}
	else throw invalid_argument("The expression cannot be recognized as an operation.");
}

void Statistics::newSingle(const Statistics& X) {
	cout << "Sample size - n: " << n(X) << endl
		<< "Sum - sum: " << sum(X) << endl
		<< "Mean - m: " << m(X) << endl
		<< "Variance - Var: " << Var(X) << endl
		<< "Standard deviation - sd: " << sd(X) << endl
		<< "Sample variance - s^2: " << sampleVar(X) << endl
		<< "Sample standard deviation - s: " << sampleSd(X) << endl
		<< "Max - max: " << Max(X) << endl
		<< "Min - min: " << Min(X) << endl
		<< "Range - range: " << range(X) << endl
		<< "Lower quartile - Q1: " << Q1(X) << endl
		<< "Median - M: " << M(X) << endl
		<< "Upper quartile - Q3: " << Q3(X) << endl
		<< "Modes - Mo: ";
	Mo(X);
}

void Statistics::newBivariate(const Statistics& _X, const Statistics& _Y) {
	Statistics X(_X), Y(_Y);
	cout << "Bivariate analysis results (insufficient data will be supplemented with 0):" << endl;
	int xn = n(X), yn = n(Y);
	if (xn > yn)for (int i = 0; i < xn - yn; i++) Y.push_back(0);
	else for (int i = 0; i < yn - xn; i++) X.push_back(0);
	cout << "Covariance - Cov: " << Cov(X, Y) << endl;
	cout << "Function model: Y = a + bX" << endl;
	linearRegression(X, Y);
}

ostream& operator<<(ostream& out, const Statistics& s) {
	for (int i = 0; i < Statistics::n(s) - 1; i++) {
		out << s.get(i) << " ";
	}
	out << s.get(Statistics::n(s) - 1);
	return out;
}
