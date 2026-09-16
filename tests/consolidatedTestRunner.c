#include "../src/include/http.h"
#include "include/unitTest_localhost_http_server.h"
#include "include/unitTest_orderedStrictHashTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <uchar.h>
#define GREEN "\x1b[32m"
#define MAGENTA "\x1b[35m"
#define RESET "\x1b[0m"

/*
 * This file is a consolidated entry point for all unit/integration/e2e tests so there can be one compiled main that runs them all.
 * Performance tests will be a bit of a different situation. We will cross that bridge when we get there.
 */

int main() {

    (void)fputs(MAGENTA "Starting test suite" RESET "\n", stderr);
    // === START UNIT TESTS ==========================================================================================================================
    //                                                                                                                                               |
    (void)fputs(MAGENTA "Starting unit tests" RESET "\n", stderr); //                                                                                |
    //                                                                                                                                               |
    // == UNIT TESTS - START localhost_http_server ===============================================================================                   |
    //                                                                                                                           |                   |
    (void)fputs(MAGENTA "Starting unit tests for localhost_http_server" RESET "\n", stderr); //                                  |                   |
    //                                                                                                                           |                   |
    // === UNIT TESTS - localhost_http_server - START whole package ==========================================                   |                   |
    (void)fputs(MAGENTA "Starting unit tests for localhost_http_server whole package" RESET "\n", stderr); //|                   |                   |
    test_localhost_http_server_serves_proper_file();     //                                                  |                   |                   |
    test_localhost_http_server_start_stop_start_again(); //                                                  |                   |                   |
    test_localhost_http_server_nonASCII_UTF8();          //                                                  |                   |                   |
    test_localhost_http_server_nonexistent_file();       //                                                  |                   |                   |
    (void)fputs(GREEN "Unit tests for localhost_http_server whole package passed" RESET "\n", stderr); //    |                   |                   |
    // === UNIT TESTS - localhost_http_server - END complete package =========================================                   |                   |
    //                                                                                                                           |                   |
    // == UNIT TESTS - END localhost_http_server =================================================================================                   |
    //                                                                                                                                               |
    // === UNIT TESTS - START OrderedStrictHashTable =============================================================================                   |
    //                                                                                                                           |                   |
    (void)fputs(MAGENTA "Starting unit tests for OrderedStrictHashTable" RESET "\n", stderr); //                                 |                   |
    //                                                                                                                           |                   |
    // === UNIT TESTS - OrderedStrictHashTable - START createOSHT ============================================                   |                   |
    (void)fputs(MAGENTA "Starting unit tests for createOSHT" RESET "\n", stderr); //                         |                   |                   |
    test_createOSHT_properMemberInitializations();                                //                         |                   |                   |
    test_createOSHT_zeroBucketInitialization();                                   //                         |                   |                   |
    (void)fputs(GREEN "Unit tests for createOSHT passed" RESET "\n", stderr);     //                         |                   |                   |
    // === UNIT TESTS - OrderedStrictHashTable - END createOSHT ==============================================                   |                   |
    //                                                                                                                           |                   |
    // === UNIT TESTS - OrderedStrictHashTable - START destroyOSHT ===========================================                   |                   |
    (void)fputs(MAGENTA "Starting unit tests for destroyOSHT" RESET "\n", stderr); //                        |                   |                   |
    test_destroyOSHT_noLeaksAfterNoInserts();                                      //                        |                   |                   |
    test_destroyOSHT_noLeaksAfterOneInsert();                                      //                        |                   |                   |
    test_destroyOSHT_noLeaksAfterMultipleInserts();                                //                        |                   |                   |
    (void)fputs(GREEN "Unit tests for destroyOSHT passed" RESET "\n", stderr);     //                        |                   |                   |
    // === UNIT TESTS - OrderedStrictHashTable - END destroyOSHT =============================================                   |                   |
    //                                                                                                                           |                   |
    // === UNIT TESTS - OrderedStrictHashTable - START hashExistsInOSHT ======================================                   |                   |
    (void)fputs(MAGENTA "Starting unit tests for hashExistsInOSHT" RESET "\n", stderr); //                   |                   |                   |
    test_hashExistsInOSHT_checkNodesThatExist();                                        //                   |                   |                   |
    test_hashExistsInOSHT_checkNodesThatDoNotExist();                                   //                   |                   |                   |
    (void)fputs(GREEN "Unit tests for hashExistsInOSHT passed" RESET "\n", stderr);     //                   |                   |                   |
    // === UNIT TESTS - OrderedStrictHashTable - END hashExistsInOSHT ========================================                   |                   |
    //                                                                                                                           |                   |
    // === UNIT TESTS - OrderedStrictHashTable - START insertToOSHT ==========================================                   |                   |
    (void)fputs(MAGENTA "Starting unit tests for insertToOSHT" RESET "\n", stderr); //                       |                   |                   |
    test_insertToOSHT_basicNodeInsertions();                                        //                       |                   |                   |
    test_insertToOSHT_duplicateInsertion();                                         //                       |                   |                   |
    test_insertToOSHT_collision();                                                  //                       |                   |                   |
    test_insertToOSHT_nonASCIIUTF8();                                               //                       |                   |                   |
    (void)fputs(GREEN "Unit tests for insertToOSHT passed" RESET "\n", stderr);     //                       |                   |                   |
    // === UNIT TESTS - OrderedStrictHashTable - END insertToOSHT ============================================                   |                   |
    //                                                                                                                           |                   |
    (void)fputs(GREEN "All unit tests for OrderedStrictHashTable passed" RESET "\n", stderr); //                                 |                   |
    //                                                                                                                           |                   |
    // === UNIT TESTS - END OrderedStrictHashTable ===============================================================================                   |
    //                                                                                                                                               |
    (void)fputs(GREEN "All unit tests passed" RESET "\n", stderr); //                                                                                |
    //                                                                                                                                               |
    // === END UNIT TESTS ============================================================================================================================
    (void)fputs(MAGENTA "Complete test suite passed" RESET "\n", stderr);

// Would need to compile curl with memsan for this to pass
#ifndef WIKICYCLE_MSAN_BUILD
    const char8_t *const url = u8"www.example.com";
    char8_t *output = makeGETRequestAndReturnUTF8Response(url);
    // flawfinder: ignore
    printf("%s\n", output);
    free(output);
#endif

    return 0;
}
