#include <iostream>
#include <cpptestingframework/test.hh>

int gcd(int a, int b) {
	return b == 0 ? a : gcd(b, a%b);
}

int main() {
	std::cout << "Enter two numbers: " << std::flush;
	int a, b;
	std::cin >> a >> b;
	std::cout << "The greates common divider is: " << gcd(a, b) << std::endl;
}

new_test(testing_gcd) {
	assert_that(gcd(15,27) == 3);
	assert_that(gcd(14,21) == 7);
}
