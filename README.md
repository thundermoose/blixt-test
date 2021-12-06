# C++ Testing Framework

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
user desires. Not including `ìnstall_path` it will be automatically be set 
to `/usr/local/`.

## Usage

There are two macros to create tests,
```cpp
new_test(test_name/*Write your tests name here*/)
{
	// Write your test code here
}
```
and
```cpp
assert_that(condition);
```
The first macro `new_test` takes the name of the test and the test code is then
written in a normal code block after, similar to in a function implementation. 
The name should not be a string, but be formated in the same way as a variable 
or function name (since it is used to create a variable 
internally). 

The `assert_that` macro takes a conditional and allows you to test the result 
of your test body. If an assertion fails 

Once your program is compiled and runned, by default all tests will be executed 
before the main function. However, there are 4 command line flags included:
- `--help-test` shows a basic usage message listing all 4 flags.
- `--list-tests` lists all test with name and in what .cpp file they are located.
- `--run-test <test_name>` runs the test specified by `<test_name>` and possibly other tests specified with this flag. 
- `--run-tests-in <file_name>` runs all test located in the source file specified by `<file_name>`.

### Example
This illustrated by the following simple
code:
```cpp
#include <cpptestframework/test.h>

int gcd(int a, int b)
{
	return b == 0 ? a : gcd(b,a%b);
}

new_test(gcd_9_15_is_3)
{
	assert_that(gcd(9,15) == 3);
}
```

### Turning tests of
To quickly turn of all tests in your code compile the code with the `NDEBUG`
macro defined.
