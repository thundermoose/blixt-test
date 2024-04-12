#ifndef __TEST__
#define __TEST__

#include <cstdio>
#include <cstring>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>

namespace tests {
/*
   The following exception is thrown when ever a test fails due to an
   assertion.
 */
class test_assertion_failed : public std::exception {
      public:
        test_assertion_failed(std::string assertion_description);
        ~test_assertion_failed() = default;
        const char *what() const noexcept;

      private:
        std::string assertion_description;
};

/*
   This class represents a test.
 */
class test {
      public:
        template <typename TEST_FUNCTION>
        test(TEST_FUNCTION test_function, std::string test_name,
             std::string test_file)
            : test_name(test_name), test_file(test_file) {
                initialize_test();
                if (should_run) {
                        test_pid = fork();
                        if (test_pid == 0) {
                                try {
                                        test_function();
                                } catch (
                                    const test_assertion_failed &assertion) {
                                        test_process_failed_at_assetion(
                                            assertion);
                                } catch (const std::exception &exception) {
                                        test_process_failed_with_exception(
                                            exception);
                                }
                                test_process_succeded();
                        } else
                                generate_test_report();
                }
        }
        static void set_command_arguments(int num_arguments,
                                          char **argument_list);

      private:
        void initialize_test();
        void parse_argument_list();
        void generate_test_report();
        void
        test_process_failed_with_exception(const std::exception &exception);
        void
        test_process_failed_at_assetion(const test_assertion_failed &exception);
        void test_process_succeded();
        static void display_help_text();
        pid_t test_pid;
        int test_pipe[2];
        std::string test_name;
        std::string test_file;
        static int num_arguments;
        static char **argument_list;
        bool should_run;
};
} // namespace tests

#ifndef NDEBUG
/**
 * @brief Macro to create a new named test. The body of the test is written as a
 * normal code block following the macro
 *
 * @param name is the name of the test. The name parameter should not be a
 * string.
 *
 * @example simple_test.cpp
 */
#define new_test(name)                                                         \
        void test_function_##name();                                           \
        tests::test name(test_function_##name, std::string(#name),             \
                         std::string(__FILE__));                               \
        void test_function_##name()

/**
 * @brief Macro to do an assertion in a test.
 *
 * @param test is the condition to be tested, for a succefull test it should be
 * true.
 */
#define assert_that(test)                                                      \
        if (!(test))                                                           \
                throw tests::test_assertion_failed(#test);

namespace tests {
__attribute__((constructor(101))) extern void
initialize_test_environment(int num_arguments, char **argument_list);
}
#else

#define new_test(name) [[maybe_unused]] void test_function_##name()

#define assert_that(test)
#endif

#endif
