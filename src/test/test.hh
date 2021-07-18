#ifndef __TEST__
#define __TEST__

#include <iostream>
#include <cstdio>
#include <cstring>
#include <exception>
#include <string>
#include <unistd.h>

class test_assertion_failed :
	public std::exception
{
	public:
		test_assertion_failed(std::string assertion_description);
		~test_assertion_failed() = default;
		const char* what() const noexcept;
	private:
		std::string assertion_description;
};

class register_test
{
	public:
template<typename TEST_FUNCTION>
	register_test(TEST_FUNCTION test_function,std::string test_name) :
		test_name(test_name)
	{
		initialize_test();
		test_pid = fork();
		if (test_pid == 0)
		{
			try 
			{
				test_function();
			}
			catch (const test_assertion_failed &assertion)
			{
				test_process_failed_at_assetion(assertion);	
			}
			catch (const std::exception &exception)
			{
				test_process_failed_with_exception(exception);
			}
			test_process_succeded();
		}	
		else
			generate_test_report();
	}
	private:
		void initialize_test();
		void generate_test_report();
		void test_process_failed_with_exception(const std::exception &exception);
		void test_process_failed_at_assetion(const test_assertion_failed &exception);
		void test_process_succeded();
		pid_t test_pid;	
		int test_pipe[2];
		std::string test_name;
};

#ifndef NDEBUG
#define new_test(name,test_function...) register_test name(test_function,\
							   std::string(#name))

#define assert_that(test) \
	if (!(test)) \
		throw test_assertion_failed(#test);


#else

#define new_test(name,test_function...)

#endif

#endif
