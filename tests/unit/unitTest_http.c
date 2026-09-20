#define _POSIX_C_SOURCE 200809L
#include "../include/unitTest_http.h"
#include "../../src/include/http.h"
#include "../../src/include/returnCodes.h"
#include "../include/localhost_http_server.h"
#include <assert.h>
#include <stddefer.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <uchar.h>
#include <unistd.h>

void test_http_createCurlString_properInitialization(void) {
    CurlString *const pTestCurlString = createCurlString();
    defer { destroyCurlString(pTestCurlString); }

    assert(pTestCurlString->response_length == 0);
    assert(pTestCurlString->pHttp_response[0] == '\0');
}

void test_http_destroyCurlString_noLeaksEmpty(void) {
    CurlString *const pTestCurlString = createCurlString();
    destroyCurlString(pTestCurlString);
}

void test_http_destroyCurlString_noLeaksOneInsert(void) {
    CurlString *const pTestCurlString = createCurlString();
    defer { destroyCurlString(pTestCurlString); }

    const char8_t *const pInsertionString = u8"Please don't leak.";
    const uint_least64_t insertionCharacterCount = strnlen((const char *const)pInsertionString, 19);

    curlWriteCallback(pInsertionString, 1, insertionCharacterCount, pTestCurlString);
}

void test_http_destroyCurlString_noLeaksManyInserts(void) {
    CurlString *const pTestCurlString = createCurlString();
    defer { destroyCurlString(pTestCurlString); }

    const char8_t *const pFirstInsertionString = u8"If the first one doesn't leak...";
    const uint_least64_t firstInsertionCharacterCount = strnlen((const char *const)pFirstInsertionString, 33);
    curlWriteCallback(pFirstInsertionString, 1, firstInsertionCharacterCount, pTestCurlString);

    const char8_t *const pSecondInsertionString = u8"\n...the second shouldn't either!";
    const uint_least64_t secondInsertionCharacterCount = strnlen((const char *const)pSecondInsertionString, 33);
    curlWriteCallback(pSecondInsertionString, 1, secondInsertionCharacterCount, pTestCurlString);
}

void test_http_curlWriteCallback_basicInserts(void) {
    CurlString *const pTestCurlString = createCurlString();
    defer { destroyCurlString(pTestCurlString); }

    const char8_t *const pFirstInsertString = u8"This is the first insertion.\n";
    const uint_least64_t firstInsertCharacterCount = strnlen((const char *const)pFirstInsertString, 30);
    assert(curlWriteCallback(pFirstInsertString, 1, firstInsertCharacterCount, pTestCurlString) == 29);
    assert(pTestCurlString->response_length == firstInsertCharacterCount);
    assert(strncmp((const char *const)pTestCurlString->pHttp_response, (const char *const)pFirstInsertString, 30) == 0);

    const char8_t *const pSecondInsertString = u8"This is the 2nd insertion.";
    const uint_least64_t secondInsertCharacterCount = strnlen((const char *const)pSecondInsertString, 27);
    assert(curlWriteCallback(pSecondInsertString, 1, secondInsertCharacterCount, pTestCurlString) == 26);
    assert(pTestCurlString->response_length == firstInsertCharacterCount + secondInsertCharacterCount);
    assert(strncmp((const char *const)pTestCurlString->pHttp_response,
                   (const char *const)u8"This is the first insertion.\nThis is the 2nd insertion.", 56) == 0);

    const char8_t *const pThirdInsertString = u8"Insertion #3!";
    const uint_least64_t thirdInsertCharacterCount = strnlen((const char *const)pThirdInsertString, 14);
    assert(curlWriteCallback(pThirdInsertString, 1, thirdInsertCharacterCount, pTestCurlString) == 13);
    assert(pTestCurlString->response_length == firstInsertCharacterCount + secondInsertCharacterCount + thirdInsertCharacterCount);
    assert(strncmp((const char *const)pTestCurlString->pHttp_response,
                   (const char *const)u8"This is the first insertion.\nThis is the 2nd insertion.Insertion #3!", 69) == 0);
}

void test_http_curlWriteCallback_NonASCIIUTF8Inserts(void) {
    CurlString *const pTestCurlString = createCurlString();
    defer { destroyCurlString(pTestCurlString); }

    const char8_t *const pFirstInsertString = u8"ᱱᱚᱣᱟ ᱟᱹᱭᱟᱹᱛ ᱨᱮ ASCII ᱪᱤᱠᱤ ᱠᱚ ᱵᱟᱝ ᱵᱮᱵᱷᱟᱨᱚᱜ ᱠᱟᱱᱟ ᱾";
    const uint_least64_t firstInsertCharacterCount = strnlen((const char *const)pFirstInsertString, 117);
    assert(curlWriteCallback(pFirstInsertString, 1, firstInsertCharacterCount, pTestCurlString) == 116);
    assert(pTestCurlString->response_length == firstInsertCharacterCount);
    assert(strncmp((const char *const)pTestCurlString->pHttp_response, (const char *const)pFirstInsertString, 49) == 0);

    const char8_t *const pSecondInsertString = u8"🙂‍↕️🤠🫳🧏‍♂️👨‍🔧";
    const uint_least64_t secondInsertCharacterCount = strnlen((const char *const)pSecondInsertString, 46);
    assert(curlWriteCallback(pSecondInsertString, 1, secondInsertCharacterCount, pTestCurlString) == 45);
    assert(pTestCurlString->response_length == firstInsertCharacterCount + secondInsertCharacterCount);
    assert(strncmp((const char *const)pTestCurlString->pHttp_response,
                   (const char *const)u8"ᱱᱚᱣᱟ ᱟᱹᱭᱟᱹᱛ ᱨᱮ ASCII ᱪᱤᱠᱤ ᱠᱚ ᱵᱟᱝ ᱵᱮᱵᱷᱟᱨᱚᱜ ᱠᱟᱱᱟ "
                                      u8"᱾🙂‍↕️🤠🫳🧏‍♂️👨‍🔧",
                   94) == 0);
}

void test_http_curlWriteCallback_HTMLInsert(void) {
    CurlString *const pTestCurlString = createCurlString();
    defer { destroyCurlString(pTestCurlString); }

    const char8_t *const pFirstInsertString = u8"<!doctype html><html><head><title>C HTML Page</title></head><body><h1>Hello, C!</h1></body></html>";
    const uint_least64_t firstInsertCharacterCount = strnlen((const char *const)pFirstInsertString, 99);
    assert(curlWriteCallback(pFirstInsertString, 1, firstInsertCharacterCount, pTestCurlString) == 98);
    assert(pTestCurlString->response_length == firstInsertCharacterCount);
    assert(strncmp((const char *const)pTestCurlString->pHttp_response, (const char *const)pFirstInsertString, 108) == 0);
}

void test_http_makeGETRequestAndReturnUTF8Response_nullTerminator(void) {
    startTestServer(u8"tests/test_infrastructure/index.html");
    char8_t *const pHTMLRequestOne = makeGETRequestAndReturnUTF8Response(u8"localhost:8080");
    assert(pHTMLRequestOne[14] == '\0');
    stopTestServer();
    free(pHTMLRequestOne);

    startTestServer(u8"tests/test_infrastructure/index2.html");
    char8_t *const pHTMLRequestTwo = makeGETRequestAndReturnUTF8Response(u8"localhost:8080");
    assert(pHTMLRequestTwo[107] == '\0');
    stopTestServer();
    free(pHTMLRequestTwo);

    startTestServer(u8"tests/test_infrastructure/emoji.html");
    char8_t *const pHTMLRequestThree = makeGETRequestAndReturnUTF8Response(u8"localhost:8080");
    assert(pHTMLRequestThree[25] == '\0');
    stopTestServer();
    free(pHTMLRequestThree);

    startTestServer(u8"tests/test_infrastructure/greek.html");
    char8_t *const pHTMLRequestFour = makeGETRequestAndReturnUTF8Response(u8"localhost:8080");
    assert(pHTMLRequestFour[305] == '\0');
    stopTestServer();
    free(pHTMLRequestFour);

    startTestServer(u8"tests/test_infrastructure/unit_testing.html");
    char8_t *const pHTMLRequestFive = makeGETRequestAndReturnUTF8Response(u8"localhost:8080");
    assert(pHTMLRequestFive[341160] == '\0');
    stopTestServer();
    free(pHTMLRequestFive);
}

void test_http_makeGETRequestAndReturnUTF8Response_basicRequest(void) {
    startTestServer(u8"tests/test_infrastructure/index.html");
    char8_t *const pHTMLRequestOne = makeGETRequestAndReturnUTF8Response(u8"localhost:8080");
    assert(strncmp((const char *const)pHTMLRequestOne, "Hello, World!\n", 15) == 0);
    // cppcheck-suppress assertWithSideEffect
    assert(strnlen((const char *const)pHTMLRequestOne, 15) == 14);
    stopTestServer();
    free(pHTMLRequestOne);

    startTestServer(u8"tests/test_infrastructure/index2.html");
    char8_t *const pHTMLRequestTwo = makeGETRequestAndReturnUTF8Response(u8"localhost:8080");
    assert((strstr)((const char *const)pHTMLRequestTwo, "Localhost C Server") != nullptr);
    assert((strstr)((const char *const)pHTMLRequestTwo, "Hello, C!") != nullptr);
    assert((strstr)((const char *const)pHTMLRequestTwo, "<html>") != nullptr);
    assert((strstr)((const char *const)pHTMLRequestTwo, "This string does not exist") == nullptr);
    // cppcheck-suppress assertWithSideEffect
    assert(strnlen((const char *const)pHTMLRequestTwo, 108) == 107);
    stopTestServer();
    free(pHTMLRequestTwo);
}

void test_http_makeGETRequestAndReturnUTF8Response_nonASCIIUTF8(void) {
    startTestServer(u8"tests/test_infrastructure/emoji.html");
    char8_t *const pHTMLRequestOne = makeGETRequestAndReturnUTF8Response(u8"localhost:8080");
    assert(strncmp((const char *const)pHTMLRequestOne, "🫠🫨🫪🤌🇲🇶\n", 21) == 0);
    // cppcheck-suppress assertWithSideEffect
    assert(strnlen((const char *const)pHTMLRequestOne, 26) == 25);
    stopTestServer();
    free(pHTMLRequestOne);

    startTestServer(u8"tests/test_infrastructure/greek.html");
    char8_t *const pHTMLRequestTwo = makeGETRequestAndReturnUTF8Response(u8"localhost:8080");
    assert(strncmp((const char *const)pHTMLRequestTwo,
                   "ὁ δὲ ἀνδρεῖος ἀνέκπληκτος ὡς ἄνθρωπος. φοβήσεται μὲν οὖν καὶ τὰ τοιαῦτα, ὡς δεῖ δὲ καὶ ὡς ὁ λόγος ὑπομενεῖ τοῦ καλοῦ ἕνεκα· "
                   "τοῦτο γὰρ τέλος τῆς ἀρετῆς.\n",
                   306) == 0);
    // cppcheck-suppress assertWithSideEffect
    assert(strnlen((const char *const)pHTMLRequestTwo, 306) == 305);
    stopTestServer();
    free(pHTMLRequestTwo);
}

void test_http_makeGETRequestAndReturnUTF8Response_longHTMLPage(void) {
    startTestServer(u8"tests/test_infrastructure/unit_testing.html");
    char8_t *const pHTMLRequestOne = makeGETRequestAndReturnUTF8Response(u8"localhost:8080");
    assert(
        (strstr)((const char *const)pHTMLRequestOne,
                 "<p id=\"mwCA\"><b id=\"mwCQ\">Unit testing</b>, also known as <b id=\"mwCg\">component</b> or <b id=\"mwCw\">module testing</b>, "
                 "is a form of <a rel=\"mw:WikiLink\" href=\"https://en.wikipedia.org/wiki/Software_testing\" title=\"Software testing\" "
                 "id=\"mwDA\">software testing</a> by which isolated <a rel=\"mw:WikiLink\" href=\"https://en.wikipedia.org/wiki/Source_code\" "
                 "title=\"Source code\" id=\"mwDQ\">source code</a> is tested to validate expected behavior.") != nullptr);
    assert((strstr)((const char *const)pHTMLRequestOne,
                    "<p>In <a rel=\"mw:WikiLink\" href=\"https://en.wikipedia.org/wiki/Software_engineering\" title=\"Software "
                    "engineering\">software engineering</a>, a <a rel=\"mw:WikiLink\" href=\"https://en.wikipedia.org/wiki/Test_case_(software)\" "
                    "title=\"Test case (software)\">test case</a> is a specification of the inputs, execution conditions, testing procedure, and "
                    "expected results that define a single test to be executed to achieve a particular <a rel=\"mw:WikiLink\" "
                    "href=\"https://en.wikipedia.org/wiki/Software_testing\" title=\"Software testing\">software testing</a> objective, such as to "
                    "exercise a particular program path or to verify compliance with a specific requirement.") != nullptr);
    assert((strstr)((const char *const)pHTMLRequestOne, "Unit tests can be performed manually") != nullptr);
    assert((strstr)((const char *const)pHTMLRequestOne, "Test cases underlie testing that is methodical rather than haphazard.") != nullptr);
    assert((strstr)((const char *const)pHTMLRequestOne, "Use of parametrized tests can reduce test code duplication.") != nullptr);
    assert((strstr)((const char *const)pHTMLRequestOne,
                    "There is some debate among developers, as to whether it is wise to test private methods and data anyway.") != nullptr);
    assert((strstr)((const char *const)pHTMLRequestOne, "Unit testing enables more frequent releases in software development.") != nullptr);
    assert((strstr)((const char *const)pHTMLRequestOne, "Some programming languages directly support unit testing.") != nullptr);
    assert((strstr)((const char *const)pHTMLRequestOne, "Unit testing, also known as component or module testing, is a form of software testing by "
                                                        "which isolated source code is tested to validate expected behavior.") == nullptr);
    // cppcheck-suppress assertWithSideEffect
    assert(strnlen((const char *const)pHTMLRequestOne, 341161) == 341160);
    stopTestServer();
    free(pHTMLRequestOne);
}

void test_http_makeGETRequestAndReturnUTF8Response_exitOnFailedRequest(void) {
    const pid_t pid = fork();
    assert(pid >= 0);

    if (pid == 0) {
        // this is the child process
        char8_t *const pHTMLRequestOne = makeGETRequestAndReturnUTF8Response(u8"localhost:8080");
        free(pHTMLRequestOne); // SHUT UP COMPILER!
        assert(false);
    }

    // this is the parent process
    int_least32_t status = -1;
    const pid_t result = waitpid(pid, (int *)&status, 0);
    assert(result == pid);
    // cppcheck-suppress assertWithSideEffect
    assert(WEXITSTATUS(status) == CURL_PERFORM_FAILURE_RC);
}
