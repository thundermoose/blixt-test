### C++ Testing Framework

This is a C++ testing framework, intended to be simple to use, yet powerfull.
Only two simple macros are needed in addition to linking your program to
this package.

## License

This software is release under GPLv2. For more details see [LICENSE](LICENSE).
However, as required, this software comes with out any guarantee to work or
any waranty.

## Installation

To install cpp_testing_framework to `$HOME/.local` run the following commands:
```
>> git clone git@github.com:thundermoose/cpp_testing_framework.git
>> cd cpp_testing_framework
>> make 
>> make install install_path=$HOME/.local/
```
By changing `install_path` it is possible to install it to where every they
user desires.

## Usage

There are two macros to create tests,
```cpp
new_test(test_name,test_function);
```
and
```cpp
assert_that(condition);
```
The first macro `new_test` takes the name of the test and a function containig
the test code. The name should not be a string, but be formated in the same
way as a variable or function name (since it is used to create a variable 
internally). The test_function can either be a lambda function or any 
object overloading `operator ()`. 
# Example
This illustrated by the following simple
code:
```cpp
#include <cpptestframework/test.h>

int gcd(int a, int b)
{
	return b == 0 ? a : gcd(b,a%b);
}

new_test(gcd_9_15_is_3,
	 []()
	 {
	 	assert_that(gcd(9,15) == 3);
	 });

```

# Turning tests of
To quickly turn of all tests in your code compile the code with the `NDEBUG`
macro defined.
