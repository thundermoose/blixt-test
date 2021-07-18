#include "test.hh"
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

test_assertion_failed::test_assertion_failed(std::string assertion_description)
{
	this->assertion_description = assertion_description;
}

const char* test_assertion_failed::what() const noexcept
{
	return this->assertion_description.c_str();
}

void register_test::initialize_test()
{
	int pipe_status = pipe(test_pipe);
	if (pipe_status == -1)
	{
		std::cerr << "Could not create pipe for test " 
			  << test_name 
			  << std::endl;
	}
}

void register_test::generate_test_report()
{
	int test_status = 0;
	waitpid(test_pid,&test_status,0);
	if (WIFEXITED(test_status) &&
	    WEXITSTATUS(test_status) == EXIT_SUCCESS)
	{
		std::cout << "Test " 
			  << test_name 
			  << " passed" 
			  << std::endl;
	}
	else if (WIFEXITED(test_status))
	{
		char buffer[8192];
		read(test_pipe[0],buffer,8192);
		std::cerr << "Test " 
			  << test_name 
			  << " failed" 
			  << std::endl;
		if (WEXITSTATUS(test_status) == 255)
			std::cerr << "With exception " 
				  << buffer 
				  << std::endl;
		else if (WEXITSTATUS(test_status) == 254)
			std::cerr << "At assertion "
				  << buffer
				  << std::endl;
	}
	else if (WIFSIGNALED(test_status))
	{
		std::cerr << "Test " 
			  << test_name 
			  << " failed" 
			  << std::endl;
		std::cerr << "With signal " 
			  << strsignal(WTERMSIG(test_status)) << std::endl;
	}
	if (WCOREDUMP(test_status))
		std::cerr << "Core dump with id " 
			  << test_pid 
			  << " produced" 
			  << std::endl;
	close(test_pipe[0]);
}

void register_test::test_process_failed_with_exception(const std::exception &exception)
{
	const char *message = exception.what();
	write(test_pipe[1],message,strlen(message)+1);
	close(test_pipe[1]);
	exit(-1);
}

void register_test::test_process_failed_at_assetion(const test_assertion_failed &assertion)
{
	const char *message = assertion.what();
	write(test_pipe[1],message,strlen(message)+1);
	close(test_pipe[1]);
	exit(-2);
}

void register_test::test_process_succeded()
{

	close(test_pipe[1]);
	exit(EXIT_SUCCESS);
}
