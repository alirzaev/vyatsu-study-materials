#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif
	int pmult(int* f, size_t n1, int* s, size_t n2, int* r);
#ifdef __cplusplus
}
#endif

int main() {
	size_t as, bs;
	std::cout << "First polynom. Enter a count of coeffs" << std::endl;
	std::cin >> as;
	std::cout << "Enter coeffs separated by space" << std::endl;
	std::vector<int> a(as);
	for (int i = as - 1; i >= 0; --i) {
		std::cin >> a[i];
	}


	std::cout << "Second polynom. Enter a count of coeffs" << std::endl;
	std::cin >> bs;
	std::cout << "Enter coeffs separated by space" << std::endl;
	std::vector<int> b(bs);
	for (int i = bs - 1; i >= 0; --i) {
		std::cin >> b[i];
	}

	std::vector<int> r(as + bs);
	
	pmult(a.data(), as, b.data(), bs, r.data());

	std::cout << "The product is:" << std::endl;
	for (int i = as + bs - 1; i >= 0; --i) {
		std::cout << r[i] << ' ';
	}
	std::cout << std::endl;
	std::cin.get();
	return 0;
}
