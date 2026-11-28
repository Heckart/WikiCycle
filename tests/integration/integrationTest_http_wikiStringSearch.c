#define _POSIX_C_SOURCE 200809L
#include "../include/integrationTest_http_wikiStringSearch.h"
#include "../../src/include/http.h"
#include "../../src/include/wikiStringSearch.h"
#include "../include/localhost_http_server.h"
#include <assert.h>
#include <stddefer.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <uchar.h>
#include <unistd.h>

void test_http_wikiStringSearch_correctTitle(void) {
    static int_least32_t server_fd = -1;
    static pthread_t server_thread; // NOLINT(misc-include-cleaner)
    constexpr uint_least16_t port = 8080;

    startTestServer(u8"tests/test_infrastructure/Unit_testing.json", &server_fd, &server_thread, port);
    char8_t *const pHTML_request_one = makeGETRequestAndReturnUTF8Response(u8"localhost:8080");
    defer { free(pHTML_request_one); }

    int_least64_t fake_global_index = 0;
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_one = getWikiTitle(pHTML_request_one, &fake_global_index);
    defer { free(pTitle_one); }

    assert(strnlen((const char *const)pTitle_one, 13) == 12);
    assert(strncmp((const char *const)pTitle_one, "Unit testing", 13) == 0);

    fake_global_index = 0;

    stopTestServer(&server_fd, &server_thread);

    server_fd = -1;
    startTestServer(u8"tests/test_infrastructure/Knowledge.json", &server_fd, &server_thread, port);
    char8_t *const pHTML_request_two = makeGETRequestAndReturnUTF8Response(u8"localhost:8080");
    defer { free(pHTML_request_two); }

    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_two = getWikiTitle(pHTML_request_two, &fake_global_index);
    defer { free(pTitle_two); }

    assert(strnlen((const char *const)pTitle_two, 10) == 9);
    assert(strncmp((const char *const)pTitle_two, "Knowledge", 10) == 0);

    stopTestServer(&server_fd, &server_thread);
}

void test_http_wikiStringSearch_correctSlug(void) {
    static int_least32_t server_fd = -1;
    static pthread_t server_thread; // NOLINT(misc-include-cleaner)
    constexpr uint_least16_t port = 8080;
    startTestServer(u8"tests/test_infrastructure/Unit_testing.json", &server_fd, &server_thread, port);
    char8_t *const pHTML_request_one = makeGETRequestAndReturnUTF8Response(u8"localhost:8080");
    defer { free(pHTML_request_one); }

    int_least64_t fake_global_index = 0;
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pSlug_one = getNextWikiArticleSlugFromWikiParagraph(pHTML_request_one, fake_global_index);
    defer { free(pSlug_one); }

    assert(strnlen((const char *const)pSlug_one, 17) == 16);
    assert(strncmp((const char *const)pSlug_one, "Software_testing", 17) == 0);

    fake_global_index = 0;

    stopTestServer(&server_fd, &server_thread);

    server_fd = -1;
    startTestServer(u8"tests/test_infrastructure/Knowledge.json", &server_fd, &server_thread, port);
    char8_t *const pHTML_request_two = makeGETRequestAndReturnUTF8Response(u8"localhost:8080");
    defer { free(pHTML_request_two); }

    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pSlug_two = getNextWikiArticleSlugFromWikiParagraph(pHTML_request_two, fake_global_index);
    defer { free(pSlug_two); }

    assert(strnlen((const char *const)pSlug_two, 7) == 6);
    assert(strncmp((const char *const)pSlug_two, "Belief", 7) == 0);

    stopTestServer(&server_fd, &server_thread);
}
