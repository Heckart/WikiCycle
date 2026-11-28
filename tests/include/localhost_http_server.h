// Copyright [2026] Ethan Heckart
#ifndef WIKICYCLE_TESTS_INCLUDE_LOCALHOST_HTTP_SERVER_H_
#define WIKICYCLE_TESTS_INCLUDE_LOCALHOST_HTTP_SERVER_H_
#include <pthread.h> // NOLINT(misc-include-cleaner)
#include <stdint.h>
#include <uchar.h>

int_least32_t startTestServer(const char8_t *const restrict pHtml_file, int_least32_t *const server_fd,
                              pthread_t *const server_thread, // NOLINT(misc-include-cleaner)
                              const uint_least16_t port);

void stopTestServer(int_least32_t *const server_fd, const pthread_t *const server_thread); // NOLINT(misc-include-cleaner)

#endif // WIKICYCLE_TESTS_INCLUDE_LOCALHOST_HTTP_SERVER_H_
