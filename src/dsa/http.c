#define _POSIX_C_SOURCE 200809L
#include "../include/http.h"
#include "../include/returnCodes.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>
#include <iso646.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uchar.h>

static CurlString *createCurlString() {
    CurlString *const curlString = (CurlString *)malloc(sizeof(CurlString));
    if (curlString == nullptr) {
        perror("Allocation of curlString in createCurlString failed. Exiting now.");
        exit(MALLOC_FAILURE_RC);
    }

    curlString->response_length = 0;

    curlString->pHttp_response = (char8_t *)malloc(curlString->response_length + 1);
    if (curlString->pHttp_response == nullptr) {
        perror("Allocation of curlString->http_response in createCurlString failed. Exiting now.");
        exit(MALLOC_FAILURE_RC);
    }

    curlString->pHttp_response[0] = '\0';

    return curlString;
}

static size_t curlWriteCallback(const void *const pReceived_data, const size_t input_count, const size_t data_length, void *const pOutput) {
    CurlString *const pResponse_string = (CurlString *const)pOutput;
    char8_t *const pUpdated_string = realloc(pResponse_string->pHttp_response, pResponse_string->response_length + (input_count * data_length) + 1);
    if (pUpdated_string == nullptr) {
        perror("Allocation of pUpdated_string in curlWriteCallback failed. Program ending.");
        exit(MALLOC_FAILURE_RC);
    }

    pResponse_string->pHttp_response = pUpdated_string;
    // flawfinder: ignore. Bounds check input_count * data_length is used during realloc eariler.
    memcpy(&(pResponse_string->pHttp_response[pResponse_string->response_length]), (const char8_t *const)pReceived_data, (input_count * data_length));
    pResponse_string->response_length += data_length;
    pResponse_string->pHttp_response[pResponse_string->response_length] = 0;

    return data_length;
}

char8_t *makeGETRequestAndReturnUTF8Response(const char8_t *const pUrl) {
    // TODO(ethan): This should have better error handling since internet connections can fail. Duh.
    CURL *pCurl_client;
    CURLcode curl_rc = curl_global_init(CURL_GLOBAL_ALL);
    if (curl_rc != CURLE_OK) {
        perror("curl_global_init returned non-ok rc. Exiting now.");
        exit((int)curl_rc);
    }

    CurlString *const pResponse_string = createCurlString();

    pCurl_client = curl_easy_init();
    if (pCurl_client) {
        curl_easy_setopt(pCurl_client, CURLOPT_USERAGENT, "WikiCycle/1.0");
        curl_easy_setopt(pCurl_client, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(pCurl_client, CURLOPT_URL, pUrl);
        curl_easy_setopt(pCurl_client, CURLOPT_WRITEFUNCTION, curlWriteCallback);
        curl_easy_setopt(pCurl_client, CURLOPT_WRITEDATA, pResponse_string);
        curl_rc = curl_easy_perform(pCurl_client);
        if (curl_rc != CURLE_OK) {
            perror("Request to URL failed. Exiting now.");
            exit(CURL_PERFORM_FAILURE_RC);
        }

        curl_easy_cleanup(pCurl_client);

    } else {
        perror("pCurl_client in makeGETRequestAndReturnUTF8Response was not properly initialized. Exiting now.");
        exit(CURL_INIT_FAILURE_RC);
    }

    char8_t *const pReturn_value = (char8_t *const)strndup((char *)pResponse_string->pHttp_response, pResponse_string->response_length);

    free(pResponse_string->pHttp_response);
    pResponse_string->pHttp_response = nullptr;
    free(pResponse_string);

    return pReturn_value;
}
