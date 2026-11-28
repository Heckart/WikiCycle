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
    static int_least32_t server_fd = -1;
    static pthread_t server_thread; // NOLINT(misc-include-cleaner)
    constexpr uint_least16_t port = 8080;
    char8_t buffer[BUFFER_SIZE];

    startTestServer(u8"tests/test_infrastructure/index.html", &server_fd, &server_thread, port);

    // flawfinder: ignore. Curl opens a localhost file we have total control over.
    FILE *pCurlOutput = popen("curl localhost:8080", "r");
    assert(pCurlOutput != nullptr);
    const char8_t *const fgets_return = (const char8_t *const)fgets((char *)buffer, BUFFER_SIZE, pCurlOutput);
    assert(fgets_return != nullptr);
    pclose(pCurlOutput);

    assert(strncmp((const char *const)buffer, "Hello, World!\n", BUFFER_SIZE) == 0);

    stopTestServer(&server_fd, &server_thread);
}

void test_localhost_http_server_start_stop_start_again(void) {
    static int_least32_t server_fd = -1;
    static pthread_t server_thread; // NOLINT(misc-include-cleaner)
    constexpr uint_least16_t port = 8080;
    char8_t buffer[BUFFER_SIZE];

    startTestServer(u8"tests/test_infrastructure/index.html", &server_fd, &server_thread, port);

    // flawfinder: ignore. Curl opens a localhost file we have total control over.
    FILE *pCurlOutput = popen("curl localhost:8080", "r");
    assert(pCurlOutput != nullptr);
    char8_t *pFgets_return = (char8_t *const)fgets((char *)buffer, BUFFER_SIZE, pCurlOutput);
    assert(pFgets_return != nullptr);
    pclose(pCurlOutput);

    assert(strncmp((const char *const)buffer, "Hello, World!", 13) == 0);

    stopTestServer(&server_fd, &server_thread);

    buffer[0] = '\0';

    server_fd = -1;
    startTestServer(u8"tests/test_infrastructure/index2.html", &server_fd, &server_thread, port);

    // flawfinder: ignore. Curl opens a localhost file we have total control over.
    pCurlOutput = popen("curl localhost:8080", "r");
    assert(pCurlOutput != nullptr);
    pFgets_return = (char8_t *const)fgets((char *)buffer, BUFFER_SIZE, pCurlOutput);
    pclose(pCurlOutput);
    assert(pFgets_return != nullptr);

    assert((strstr)((const char *const)pFgets_return, "Localhost C Server") != nullptr);
    assert((strstr)((const char *const)pFgets_return, "Hello, C!") != nullptr);
    assert((strstr)((const char *const)pFgets_return, "<html>") != nullptr);
    assert((strstr)((const char *const)pFgets_return, "This string does not exist") == nullptr);

    stopTestServer(&server_fd, &server_thread);
}

void test_localhost_http_server_nonASCII_UTF8(void) {
    static int_least32_t server_fd = -1;
    static pthread_t server_thread; // NOLINT(misc-include-cleaner)
    constexpr uint_least16_t port = 8080;
    char8_t buffer[BUFFER_SIZE];

    startTestServer(u8"tests/test_infrastructure/emoji.txt", &server_fd, &server_thread, port);

    // flawfinder: ignore. Curl opens a localhost file we have total control over.
    FILE *pCurlOutput = popen("curl localhost:8080", "r");
    assert(pCurlOutput != nullptr);
    const char8_t *pFgets_return = (const char8_t *)fgets((char *)buffer, BUFFER_SIZE, pCurlOutput);
    assert(pFgets_return != nullptr);
    pclose(pCurlOutput);

    assert(strncmp((const char *const)buffer, "🫠🫨🫪🤌🇲🇶\n", BUFFER_SIZE) == 0);

    stopTestServer(&server_fd, &server_thread);

    buffer[0] = '\0';

    server_fd = -1;
    startTestServer(u8"tests/test_infrastructure/greek.txt", &server_fd, &server_thread, port);

    // flawfinder: ignore. Curl opens a localhost file we have total control over.
    pCurlOutput = popen("curl localhost:8080", "r");
    assert(pCurlOutput != nullptr);
    pFgets_return = (const char8_t *)fgets((char *)buffer, BUFFER_SIZE, pCurlOutput);
    pclose(pCurlOutput);
    assert(pFgets_return != nullptr);

    assert(strncmp((const char *const)buffer,
                   "ὁ δὲ ἀνδρεῖος ἀνέκπληκτος ὡς ἄνθρωπος. φοβήσεται μὲν οὖν καὶ τὰ τοιαῦτα, ὡς δεῖ δὲ καὶ ὡς ὁ λόγος ὑπομενεῖ τοῦ καλοῦ ἕνεκα· "
                   "τοῦτο γὰρ τέλος τῆς ἀρετῆς.\n",
                   BUFFER_SIZE) == 0);

    stopTestServer(&server_fd, &server_thread);
}

void test_localhost_http_server_nonexistent_file(void) {
    static int_least32_t server_fd = -1;
    static pthread_t server_thread; // NOLINT(misc-include-cleaner)
    constexpr uint_least16_t port = 8080;
    char8_t buffer[BUFFER_SIZE];

    startTestServer(u8"ThisFileDoesNotExist.html", &server_fd, &server_thread, port);

    // flawfinder: ignore. Curl opens a localhost file we have total control over.
    FILE *pCurlOutput = popen("curl localhost:8080", "r");
    assert(pCurlOutput != nullptr);
    const char8_t *const pFgets_return = (const char8_t *const)fgets((char *)buffer, BUFFER_SIZE, pCurlOutput);
    assert(pFgets_return != nullptr);
    pclose(pCurlOutput);

    assert(strncmp((const char *const)buffer, "Not Found", BUFFER_SIZE) == 0);

    stopTestServer(&server_fd, &server_thread);
}

void test_localhost_http_server_serve_multiple_files(void) {
    static int_least32_t server_fd_one = -1;
    static pthread_t server_thread_one; // NOLINT(misc-include-cleaner)
    constexpr uint_least16_t port_one = 8080;
    static int_least32_t server_fd_two = -1;
    static pthread_t server_thread_two; // NOLINT(misc-include-cleaner)
    constexpr uint_least16_t port_two = 8081;
    char8_t buffer[BUFFER_SIZE];

    startTestServer(u8"tests/test_infrastructure/index.html", &server_fd_one, &server_thread_one, port_one);
    startTestServer(u8"tests/test_infrastructure/index2.html", &server_fd_two, &server_thread_two, port_two);

    // flawfinder: ignore. Curl opens a localhost file we have total control over.
    FILE *pCurlOutputOne = popen("curl localhost:8080", "r");
    assert(pCurlOutputOne != nullptr);
    const char8_t *const fgets_return_one = (const char8_t *const)fgets((char *)buffer, BUFFER_SIZE, pCurlOutputOne);
    assert(fgets_return_one != nullptr);
    pclose(pCurlOutputOne);

    // flawfinder: ignore. Curl opens a localhost file we have total control over.
    FILE *pCurlOutputTwo = popen("curl localhost:8081", "r");
    assert(pCurlOutputTwo != nullptr);
    const char8_t *const fgets_return_two = (const char8_t *const)fgets((char *)buffer, BUFFER_SIZE, pCurlOutputTwo);
    assert(fgets_return_two != nullptr);
    pclose(pCurlOutputTwo);

    assert(strncmp((const char *const)buffer,
                   "<!doctype html><html><head><title>Localhost C Server</title></head><body><h1>Hello, C!</h1></body></html>\n", BUFFER_SIZE) == 0);

    stopTestServer(&server_fd_one, &server_thread_one);
    stopTestServer(&server_fd_two, &server_thread_two);
}
