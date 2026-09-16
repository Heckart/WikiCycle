// Copyright [2026] Ethan Heckart
#ifndef WIKICYCLE_TESTS_INCLUDE_LOCALHOST_HTTP_SERVER_H_
#define WIKICYCLE_TESTS_INCLUDE_LOCALHOST_HTTP_SERVER_H_
#include <stdint.h>
#include <uchar.h>

int_least32_t startTestServer(char8_t *const restrict html_file);

void stopTestServer(void);

#endif // WIKICYCLE_TESTS_INCLUDE_LOCALHOST_HTTP_SERVER_H_
