#include <iostream>
#include <stdexcept>
#include "../test/test.hh"
#include <unistd.h>


new_test(simple_test,
	 [](){
	 std :: cout << "Inside simple_test" << std::endl;
	 });

new_test(catching_segfault,
	 [](){
	 int *null_pointer = NULL;
	 *null_pointer = 0;
	 });



new_test(throwing_exception,
	 [](){
		throw std::runtime_error("Just a test");
	 });

new_test(assertion_test,
	 [](){
		assert_that(4!=4);
	 });

int main()
{
	pid_t process_id = getpid();
	std::cout << "PID: " <<process_id << std::endl;	
	return 0;
}
