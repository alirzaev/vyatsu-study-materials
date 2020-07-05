#include <bits\stdc++.h>

template<class T>
double exp_template(T);

// exp(x), x : double
template<>
double exp_template(double x) {
	double res = 1;
	double val = x;
	uint64_t fac = 1;
	for (uint64_t n = 1; n < 15; ++n, val *= x, fac *= n) {
		res += val / fac;
	}

	return res;
}

// exp(x), x : uint64_t
template<>
double exp_template(uint64_t x) {
	double res = exp_template(1.0);
	double val = res;
	for (uint64_t n = 1; n < x; ++n) {
		res *= val;
	}

	return res;
}

// sin_overload
double sincos_overload(double x) {
	double res = 0;
	double val = x;
	int sign = 1;
	uint64_t fac = 1;
	uint64_t n = 1;
	while (n < 15) {
		if (n % 2 != 0) {
			res += val / fac * sign;
			sign *= -1;
		}
		++n;
		fac *= n;
		val *= x;
	}

	return res;
}

// sin_overload
double sincos_overload(int64_t x) {
	double res = 1;
	double val = static_cast<double>(x * x);
	int sign = -1;
	uint64_t fac = 2;
	uint64_t n = 2;
	while (n < 15) {
		res += val / fac * sign;

		n += 2;
		fac *= n * (n - 1);
		sign *= -1;
		val *= x * x;
	}

	return res;
}

void print_help(std::ostream& os) {
	os
		<< "What do you want to do (enter number between 0 and 4):" << std::endl
		<< "1. Compute exp(x) where x is a float point value" << std::endl
		<< "2. Compute exp(x) where x is an unsigned integer value" << std::endl
		<< "3. Compute sin(x) where x is a float point value" << std::endl
		<< "4. Compute cos(x) where x is a signed integer value" << std::endl
		<< "0. Exit" << std::endl;
}

template<class T>
void print_result(std::ostream& os, const T& val) {
	os << "The result of computation is " << val << std::endl;
}

void print_error(std::ostream& os) {
	os << "Unable to obtain value" << std::endl;
}

bool try_get_value(std::istream& is, double& val) {
	std::string s;
	bool ok = true;

	is >> s;
	size_t err = 0;
	try {
		val = std::stod(s, &err);
	}
	catch (std::exception& ex) {
		ok = false;
	}

	ok = ok && !(err < s.size());
	std::getline(is, s);

	return ok;
}

bool try_get_value(std::istream& is, uint64_t& val) {
	std::string s;
	bool ok = true;

	is >> s;
	size_t err = 0;
	try {
		val = std::stoull(s, &err);
	}
	catch (std::exception& ex) {
		ok = false;
	}

	ok = ok && !(err < s.size());
	std::getline(is, s);

	return ok;
}

bool try_get_value(std::istream& is, int64_t& val) {
	std::string s;
	bool ok = true;

	is >> s;
	size_t err = 0;
	try {
		val = std::stoll(s, &err);
	}
	catch (std::exception& ex) {
		ok = false;
	}

	ok = ok && !(err < s.size());
	std::getline(is, s);

	return ok;
}

int main(void) {
	using std::cin;
	using std::cout;

	while (true) {
		print_help(cout);
		int64_t n = 0;
		if (!try_get_value(cin, n) || !(n >= 0 && n < 5)) {
			cout << "Invalid code of operation" << std::endl;
			continue;
		}
		if (n == 0)
			break;
		
		cout << "Enter a number: " << std::endl;

		if (n == 1) {
			double val;
			if (try_get_value(cin, val))
				print_result(cout, exp_template(val));
			else
				print_error(cout);
		}
		else if (n == 2) {
			uint64_t val;
			if (try_get_value(cin, val))
				print_result(cout, exp_template(val));
			else
				print_error(cout);
		}
		else if (n == 3) {
			double val;
			if (try_get_value(cin, val))
				print_result(cout, sincos_overload(val));
			else
				print_error(cout);
		}
		else if (n == 4) {
			int64_t val;
			if (try_get_value(cin, val))
				print_result(cout, sincos_overload(val));
			else
				print_error(cout);
		}
	}
	return 0;
}