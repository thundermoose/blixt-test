-include makefile.local
ifndef compiler
compiler := g++ -std=c++17 -Wall -Werror
endif

ifndef cpp_testing_framework_library
cpp_testing_framework_library := /usr/local/lib
endif

ifndef cpp_testing_framework_headers
cpp_testing_framework_headers := /usr/local/include/cpptestingframework
endif

sources := simple_test.cpp

simple_test: $(sources)
	g++ -o $@ $^ -I $(cpp_testing_framework_headers) -L $(cpp_testing_framework_library) -lcpptestingframework

