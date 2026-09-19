// Copyright 2026 Ethan Heckart
#ifndef WIKICYCLE_SRC_INCLUDE_HTTP_H_
#define WIKICYCLE_SRC_INCLUDE_HTTP_H_
#include <stddef.h>
#include <stdint.h>
#include <uchar.h>

/*
 * libcurl requires a string-like datatype that tracks the string size
 */
typedef struct CurlString {
    char8_t *pHttp_response;
    uint_least32_t response_length;
} CurlString;

// Purpose:    Instantiate an empty, null terminated CurlString.
// Parameters: None.
// Returns:    A CurlString* with size 0, containing a null byte.
// Exits:      Exit(9) if malloc of the CurlString char8_t pointers fail.
// Modifies:   Nothing existing. On AMD64, allocates (20) bytes on the heap.
// Tests:      Not implemented yet.
// Status:     Done.
CurlString *createCurlString();

// Purpose:    Destroys a CurlString
// Parameters: A CurlString* pointing to the CurlString to be destroyed.
// Returns:    Nothing.
// Exits:      No.
// Modifies:   Frees all memory pointed to by the passed CurlString. On AMD64, deallocates (20 + pHTTP_response's UTF-8 character byte count) bytes
//             from the heap.
// Tests:      Not implemented yet.
// Status:     Done.
void destroyCurlString(CurlString *const restrict pCurlString);

// Purpose:    This callback function gets called by libcurl as soon as there is data received that needs to be saved. For most transfers, this
//             callback gets called many times and each invokation delivers another chunk of data. The data passed to this function is not
//             null-terminated.
// Parameters: A void* to the delivered data, a size_t with the number of inputs (always one), a size_t with the length of the delivered data. A void*
//             to the output CurlString.
// Returns:    A size_t with the length of the delivered data (the same as the input parameter).
// Exits:      Exit(9) if realloc of the updated output CurlString fails.
// Modifies:   The output CurlString is expanded with the newly delivered data. On AMD64, reallocates an additional (input_count * data_length)
//             bytes to pOutput on the heap.
// Tests:      Not implemented yet.
// Status:     Done.
size_t curlWriteCallback(const void *const pReceived_data, const size_t input_count, const size_t data_length, void *const pOutput);

// Purpose:    Make an HTTP connection to a URL and save its returned data as UTF-8
// Parameters: A char8_t pointing to a website URL.
// Returns:    A null-terminated char8_t* with the returned response of the requested webpage, often the page's HTML content.
// Exits:      Exit(curl_rc) if curl_global_init fails. Exit(8) if curl_easy_init fails. Exit(9) if curl_easy_perform fails. Also calls
//             curlWriteCallBack and createCurlString which can fail and Exit().
// Modifies:   Nothing existing. Calls curlWriteCallBack and createCurlString which, on AMD64, allocates total of (20 + pUrl response's UTF-8
//             character byte count) bytes on the heap.
// Tests:      Not implemented yet.
// Status:     Add timeout logic or some other type of more graceful error handling if any curl steps fail. Make a better effort to fully follow the
//             wiki policy https://wikitech.wikimedia.org/wiki/Robot_policy
char8_t *makeGETRequestAndReturnUTF8Response(const char8_t *const pUrl);

#endif // WIKICYCLE_SRC_INCLUDE_HTTP_H_
