#include <iostream>
#include <stdexcept>
#include "../test/test.hh"
#include <unistd.h>

/* This test will not fail unless something is terrible wrong.
 * The expected output is that it writes "Inside simple_test"
 */
new_test(simple_test)
{
	std :: cout << "Inside simple_test" << std::endl;
}

/* This test tests what happens if a segmentation fault occurs in a test.
 * The expectation is that testing framework will detect it and output:
 * Test catching_segfault failed
 * With signal Segmentation fault
 * Core dump with id <PID> produced
 */ 
new_test(catching_segfault)
{
	int *null_pointer = NULL;
	*null_pointer = 0;
}

/* This test tests what happen if there is a division by zero, which should
 * produce a floating point exception. The expected output is:
 * Test catching_floating_point_exception failed
 * With signal Floating point exception
 * Core dump with id <PID> produced 
 */
new_test(catching_floating_point_exception)
{
	int nominator = 34;
	int denominator = 0;
	assert_that((nominator/denominator)*denominator == nominator);
}

/* This test tests what happen if a C++ exception is thrown inside a test.
 * The expected output is:
 * Test throwing_exception failed
 * With exception "Just a test"
 */
new_test(throwing_exception)
{
	throw std::runtime_error("Just a test");
}

/* This test tests what happen when a test assertion fails inside a test.
 * The expected output is:
 * Test assertion_test failed
 * At assertion 4!=4
 */
new_test(assertion_test)
{
	assert_that(4!=4);
}

int main()
{
	/* This main body is used to check what PID the mother process has.
	 * This is used to check that the catching_segfault and 
	 * the catching_floating_point_exception tests are not runned in the
	 * mother process.
	 */
	pid_t process_id = getpid();
	std::cout << "Mother process PID: " <<process_id << std::endl;	
	return 0;
}
