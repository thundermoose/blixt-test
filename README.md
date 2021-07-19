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
```c++
new_test(test_name,test_function);
```
and
```c++
assert_that(test)
```


