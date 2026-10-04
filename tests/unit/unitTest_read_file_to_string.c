#define _POSIX_C_SOURCE 200809L
#include "../include/unitTest_read_file_to_string.h"
#include "../include/read_file_to_string.h"
#include <assert.h>
#include <stddefer.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <uchar.h>
#include <unistd.h>

void test_read_file_to_string_basic_file_exists(void) {
    char8_t *const pTest_one = read_file_to_string(u8"tests/test_infrastructure/index.html");
    defer { free(pTest_one); }
    [[clang::suppress]]
    // cppcheck-suppress assertWithSideEffect
    assert(strnlen((const char *const)pTest_one, 15) == 14);
    assert(strncmp((const char *const)pTest_one, "Hello, World!\n", 15) == 0);

    char8_t *const pTest_two = read_file_to_string(u8"tests/test_infrastructure/index2.html");
    defer { free(pTest_two); }
    // cppcheck-suppress assertWithSideEffect
    assert(strnlen((const char *const)pTest_two, 108) == 107);
    assert((strstr)((const char *const)pTest_two, "Localhost C Server") != nullptr);
    assert((strstr)((const char *const)pTest_two, "Hello, C!") != nullptr);
    assert((strstr)((const char *const)pTest_two, "<html>") != nullptr);
    assert((strstr)((const char *const)pTest_two, "This string does not exist") == nullptr);

    char8_t *const pTest_three = read_file_to_string(u8"tests/test_infrastructure/Unit_testing.json");
    defer { free(pTest_three); }
    // cppcheck-suppress assertWithSideEffect
    assert(strnlen((const char *const)pTest_three, 210889) == 210888);
    assert((strstr)((const char *const)pTest_three,
                    "<p><b>Unit testing</b>, also known as <b>component</b> or <b>module testing</b>, is a form of <a "
                    "href=\\\"/wiki/Software_testing\\\" title=\\\"Software testing\\\">software testing</a> by which isolated <a "
                    "href=\\\"/wiki/Source_code\\\" "
                    "title=\\\"Source code\\\">source code</a> is tested to validate expected behavior.") != nullptr);
    assert((strstr)((const char *const)pTest_three,
                    "<p>In <a href=\\\"/wiki/Software_engineering\\\" title=\\\"Software engineering\\\">software engineering</a>, a <a "
                    "href=\\\"/wiki/Test_case_(software)\\\" title=\\\"Test case (software)\\\">test case</a> is a specification of the inputs, "
                    "execution "
                    "conditions, testing procedure, and expected results that define a single test to be executed to achieve a particular <a "
                    "href=\\\"/wiki/Software_testing\\\" title=\\\"Software testing\\\">software testing</a> objective, such as to exercise a "
                    "particular "
                    "program path or to verify compliance with a specific requirement.") != nullptr);
    assert((strstr)((const char *const)pTest_three, "Unit tests can be performed manually") != nullptr);
    assert((strstr)((const char *const)pTest_three, "Test cases underlie testing that is methodical rather than haphazard.") != nullptr);
    assert((strstr)((const char *const)pTest_three, "Use of parametrized tests can reduce test code duplication.") != nullptr);
    assert((strstr)((const char *const)pTest_three,
                    "There is some debate among developers, as to whether it is wise to test private methods and data anyway.") != nullptr);
    assert((strstr)((const char *const)pTest_three, "Unit testing enables more frequent releases in software development.") != nullptr);
    assert((strstr)((const char *const)pTest_three, "Some programming languages directly support unit testing.") != nullptr);
    assert((strstr)((const char *const)pTest_three, "Unit testing, also known as component or module testing, is a form of software testing by "
                                                    "which isolated source code is tested to validate expected behavior.") == nullptr);
}

void test_read_file_to_string_nonASCII_UTF8(void) {
    char8_t *const pTest_one = read_file_to_string(u8"tests/test_infrastructure/emoji.txt");
    defer { free(pTest_one); }
    [[clang::suppress]]
    // cppcheck-suppress assertWithSideEffect
    assert(strnlen((const char *const)pTest_one, 26) == 25);
    assert(strncmp((const char *const)pTest_one, "🫠🫨🫪🤌🇲🇶\n", 26) == 0);

    char8_t *const pTest_two = read_file_to_string(u8"tests/test_infrastructure/greek.txt");
    defer { free(pTest_two); }
    // cppcheck-suppress assertWithSideEffect
    assert(strnlen((const char *const)pTest_two, 306) == 305);
    assert(strncmp((const char *const)pTest_two,
                   "ὁ δὲ ἀνδρεῖος ἀνέκπληκτος ὡς ἄνθρωπος. φοβήσεται μὲν οὖν καὶ τὰ τοιαῦτα, ὡς δεῖ δὲ καὶ ὡς ὁ λόγος ὑπομενεῖ τοῦ καλοῦ ἕνεκα· "
                   "τοῦτο γὰρ τέλος τῆς ἀρετῆς.\n",
                   306) == 0);
}

void test_read_file_to_string_nullTerminator(void) {
    char8_t *const pTest_one = read_file_to_string(u8"tests/test_infrastructure/index.html");
    defer { free(pTest_one); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    // cppcheck-suppress deallocuse
    assert(pTest_one[14] == '\0');

    char8_t *const pTest_two = read_file_to_string(u8"tests/test_infrastructure/index2.html");
    defer { free(pTest_two); }
    // cppcheck-suppress deallocuse
    assert(pTest_two[107] == '\0');

    char8_t *const pTest_three = read_file_to_string(u8"tests/test_infrastructure/emoji.txt");
    defer { free(pTest_three); }
    // cppcheck-suppress deallocuse
    assert(pTest_three[25] == '\0');

    char8_t *const pTest_four = read_file_to_string(u8"tests/test_infrastructure/greek.txt");
    defer { free(pTest_four); }
    // cppcheck-suppress deallocuse
    assert(pTest_four[305] == '\0');

    char8_t *const pTest_five = read_file_to_string(u8"tests/test_infrastructure/Unit_testing.json");
    defer { free(pTest_five); }
    // cppcheck-suppress deallocuse
    assert(pTest_five[210888] == '\0');
}

void test_read_file_to_string_nonexistent_file(void) {
    const pid_t pid = fork();
    assert(pid >= 0);

    if (pid == 0) {
        char8_t *const pTest_one = read_file_to_string(u8"tests/test_infrastructure/thisfiledoesnotexist.html");
        free(pTest_one); // SHUT UP COMPILER!
        assert(false);
    }

    int_least32_t status = -1;
    const pid_t result = waitpid(pid, (int *)&status, 0);
    assert(result == pid);
    // cppcheck-suppress assertWithSideEffect
    assert(WEXITSTATUS(status) == 1);
}
