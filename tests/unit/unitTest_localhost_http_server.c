#define _POSIX_C_SOURCE 200809L
#define BUFFER_SIZE 2048
#include "../include/unitTest_localhost_http_server.h"
#include "../include/localhost_http_server.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <uchar.h>

/* How exactly do we test this? If I make another program that reads html over http, that would also have to be tested, right?
 * Well that does indeed exist, but I suppose it shouldn't be relied upon to test this since it *does* have to be tested itself.
 * Instead, I will use cURL and assume that cURL will always be a reliable, working program.
 */

void test_localhost_http_server_serves_proper_file(void) {
    char8_t buffer[BUFFER_SIZE];

    startTestServer(u8"tests/test_infrastructure/index.html");

    // flawfinder: ignore. Curl opens a localhost file we have total control over.
    FILE *curlOutput = popen("curl localhost:8080", "r");
    assert(curlOutput != nullptr);
    const char8_t *const fgets_return = (const char8_t *const)fgets((char *)buffer, BUFFER_SIZE, curlOutput);
    assert(fgets_return != nullptr);
    pclose(curlOutput);

    assert(strncmp((const char *const)buffer, "Hello, World!", 13) == 0);

    stopTestServer();
}

void test_localhost_http_server_start_stop_start_again(void) {
    char8_t buffer[BUFFER_SIZE];

    startTestServer(u8"tests/test_infrastructure/index.html");

    // flawfinder: ignore. Curl opens a localhost file we have total control over.
    FILE *curlOutput = popen("curl localhost:8080", "r");
    assert(curlOutput != nullptr);
    char8_t *fgets_return = (char8_t *const)fgets((char *)buffer, BUFFER_SIZE, curlOutput);
    assert(fgets_return != nullptr);
    pclose(curlOutput);

    assert(strncmp((const char *const)buffer, "Hello, World!", 13) == 0);

    stopTestServer();

    buffer[0] = '\0';

    startTestServer(u8"tests/test_infrastructure/index2.html");

    // flawfinder: ignore. Curl opens a localhost file we have total control over.
    curlOutput = popen("curl localhost:8080", "r");
    assert(curlOutput != nullptr);
    fgets_return = (char8_t *const)fgets((char *)buffer, BUFFER_SIZE, curlOutput);
    pclose(curlOutput);
    assert(fgets_return != nullptr);

    assert((strstr)((const char *const)fgets_return, "Localhost C Server") != nullptr);
    assert((strstr)((const char *const)fgets_return, "Hello, C!") != nullptr);
    assert((strstr)((const char *const)fgets_return, "<html>") != nullptr);
    assert((strstr)((const char *const)fgets_return, "This string does not exist") == nullptr);

    stopTestServer();
}

void test_localhost_http_server_nonASCII_UTF8(void) {
    char8_t buffer[BUFFER_SIZE];

    startTestServer(u8"tests/test_infrastructure/emoji.html");

    // flawfinder: ignore. Curl opens a localhost file we have total control over.
    FILE *curlOutput = popen("curl localhost:8080", "r");
    assert(curlOutput != nullptr);
    const char8_t *fgets_return = (const char8_t *)fgets((char *)buffer, BUFFER_SIZE, curlOutput);
    assert(fgets_return != nullptr);
    pclose(curlOutput);

    assert(strncmp((const char *const)buffer, "🫠🫨🫪🤌🇲🇶 ", 20) == 0);

    stopTestServer();

    buffer[0] = '\0';

    startTestServer(u8"tests/test_infrastructure/greek.html");

    // flawfinder: ignore. Curl opens a localhost file we have total control over.
    curlOutput = popen("curl localhost:8080", "r");
    assert(curlOutput != nullptr);
    fgets_return = (const char8_t *)fgets((char *)buffer, BUFFER_SIZE, curlOutput);
    pclose(curlOutput);
    assert(fgets_return != nullptr);

    assert(strncmp((const char *const)buffer,
                   "ὁ δὲ ἀνδρεῖος ἀνέκπληκτος ὡς ἄνθρωπος. φοβήσεται μὲν οὖν καὶ τὰ τοιαῦτα, ὡς δεῖ δὲ καὶ ὡς ὁ λόγος ὑπομενεῖ τοῦ καλοῦ ἕνεκα· "
                   "τοῦτο γὰρ τέλος τῆς ἀρετῆς.",
                   304) == 0);

    stopTestServer();
}

void test_localhost_http_server_nonexistent_file(void) {
    char8_t buffer[BUFFER_SIZE];

    startTestServer(u8"ThisFileDoesNotExist.html");

    // flawfinder: ignore. Curl opens a localhost file we have total control over.
    FILE *curlOutput = popen("curl localhost:8080", "r");
    assert(curlOutput != nullptr);
    const char8_t *const fgets_return = (const char8_t *const)fgets((char *)buffer, BUFFER_SIZE, curlOutput);
    assert(fgets_return != nullptr);
    pclose(curlOutput);

    assert(strncmp((const char *const)buffer, "Not Found", 9) == 0);

    stopTestServer();
}
