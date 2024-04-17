#include "test.hh"
#include <cstring>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

static bool timedout_read(int file_descriptor, char *buffer,
                          size_t buffer_size) {
        fd_set read_file_descriptor;
        FD_ZERO(&read_file_descriptor);
        FD_SET(file_descriptor, &read_file_descriptor);
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 10000;
        int status = select(file_descriptor + 1, &read_file_descriptor, NULL,
                            NULL, &timeout);
        if (status <= 0)
                return false;
        else {
                if (read(file_descriptor, buffer, buffer_size) < 0) {
                        std::cerr << "timedout_read::Could not read to buffer"
                                  << std::endl;
                }
                return true;
        }
}

namespace blixt {
int test::num_arguments = 0;
char **test::argument_list = NULL;

test_assertion_failed::test_assertion_failed(
    std::string assertion_description) {
        this->assertion_description = assertion_description;
}

const char *test_assertion_failed::what() const noexcept {
        return this->assertion_description.c_str();
}

void test::initialize_test() {
        should_run = true;
        parse_argument_list();
        int pipe_status = pipe(test_pipe);
        if (pipe_status == -1) {
                std::cerr << "Could not create pipe for test " << test_name
                          << std::endl;
        }
}

void test::parse_argument_list() {
        bool run_all_tests = true;
        bool run_this_test = false;
        for (int argument_index = 1; argument_index < num_arguments;
             argument_index++) {
                if (strcmp(argument_list[argument_index], "--run-test") == 0) {
                        run_all_tests = false;
                        if (test_name ==
                            std::string(argument_list[++argument_index]))
                                run_this_test = true;
                } else if (strcmp(argument_list[argument_index],
                                  "--run-tests-in") == 0) {
                        run_all_tests = false;
                        if (test_file ==
                            std::string(argument_list[++argument_index]))
                                run_this_test = true;
                } else if (strcmp(argument_list[argument_index],
                                  "--list-tests") == 0) {
                        run_all_tests = false;
                        run_this_test = false;
                        std::cout << test_name << " @ " << test_file
                                  << std::endl;
                        break;
                } else if (strcmp(argument_list[argument_index],
                                  "--help-test") == 0) {
                        run_all_tests = false;
                        run_this_test = false;
                        display_help_text();
                        break;
                }
        }
        should_run = run_all_tests || run_this_test;
}

void test::generate_test_report() {
        int test_status = 0;
        waitpid(test_pid, &test_status, 0);
        if (WIFEXITED(test_status) &&
            WEXITSTATUS(test_status) == EXIT_SUCCESS) {
                std::cout << "Test " << test_name << " passed" << std::endl;
        } else if (WIFEXITED(test_status)) {
                std::cerr << "Test " << test_name << " failed" << std::endl;
                char buffer[8192];
                if (timedout_read(test_pipe[0], buffer, 8192)) {
                        if (WEXITSTATUS(test_status) == 255)
                                std::cerr << "With exception "
                                          << "\"" << buffer

                                          << "\"" << std::endl;
                        else if (WEXITSTATUS(test_status) == 254)
                                std::cerr << "At assertion " << buffer
                                          << std::endl;
                } else {
                        std::cerr << "With exit status " << test_status
                                  << std::endl;
                }
        } else if (WIFSIGNALED(test_status)) {
                std::cerr << "Test " << test_name << " failed" << std::endl;
                std::cerr << "With signal " << strsignal(WTERMSIG(test_status))
                          << std::endl;
        }
        if (WCOREDUMP(test_status))
                std::cerr << "Core dump with id " << test_pid << " produced"
                          << std::endl;
        close(test_pipe[0]);
}

void test::test_process_failed_with_exception(const std::exception &exception) {
        const char *message = exception.what();
        if (write(test_pipe[1], message, strlen(message) + 1) < 0) {
                std::cerr << "Couldn't send message from exception"
                          << std::endl;
        }
        close(test_pipe[1]);
        exit(-1);
}

void test::test_process_failed_at_assetion(
    const test_assertion_failed &assertion) {
        const char *message = assertion.what();
        if (write(test_pipe[1], message, strlen(message) + 1) < 0) {
                std::cerr << "Couldn't send message from assertion"
                          << std::endl;
        }
        close(test_pipe[1]);
        exit(-2);
}

void test::test_process_succeded() {
        close(test_pipe[1]);
        exit(EXIT_SUCCESS);
}

void test::display_help_text() {
        std::cout << "Usage: " << std::string(argument_list[0])
                  << " [--list-tests] [--help-test]"
                  << " [--run-test <test-name>]"
                  << " [--run-tests-in <source-file>]" << std::endl;
}

void test::set_command_arguments(int num_arguments, char **argument_list) {
        test::num_arguments = num_arguments;
        test::argument_list = argument_list;
}

void initialize_test_environment(int num_arguments, char **argument_list) {
        test::set_command_arguments(num_arguments, argument_list);
}

} // namespace blixt
