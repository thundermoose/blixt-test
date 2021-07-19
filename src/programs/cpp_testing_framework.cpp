#include <iostream>
#include <stdexcept>
#include "../test/test.hh"
#include <unistd.h>

auto simple_test_body = 
	 [](){
		 std :: cout << "Inside simple_test" << std::endl;
	 };
new_test(simple_test, simple_test_body);

auto catching_segfault_body =
	 [](){
		 int *null_pointer = NULL;
		 *null_pointer = 0;
	 };
new_test(catching_segfault, catching_segfault_body);


auto throwing_exception_body =
	 [](){
		throw std::runtime_error("Just a test");
	 };
new_test(throwing_exception, throwing_exception_body);

auto assertion_test_body = 
	 [](){
		assert_that(4!=4);
	 };
new_test(assertion_test,assertion_test_body);

int main()
{
	pid_t process_id = getpid();
	std::cout << "PID: " <<process_id << std::endl;	
	return 0;
}
