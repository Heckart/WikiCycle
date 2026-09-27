#include "include/unitTest_http.h"
#include "include/unitTest_localhost_http_server.h"
#include "include/unitTest_orderedStrictHashTable.h"
#include "include/unitTest_wikiStringSearch.h"
#include <stdio.h>
#define GREEN "\x1b[32m"
#define MAGENTA "\x1b[35m"
#define RESET "\x1b[0m"

/*
 * This file is a consolidated entry point for all unit/integration/e2e tests so there can be one compiled main that runs them all.
 * Performance tests will be a bit of a different situation. We will cross that bridge when we get there.
 */

int main(void) {

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
    (void)fputs(GREEN "All unit tests for localhost_http_server passed" RESET "\n", stderr); //                                  |                   |
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
    // === UNIT TESTS - START http ===============================================================================================                   |
    //                                                                                                                           |                   |
    (void)fputs(MAGENTA "Starting unit tests for http" RESET "\n", stderr); //                                                   |                   |
    //                                                                                                                           |                   |
    // === UNIT TESTS - http - START createCurlString ========================================================                   |                   |
    (void)fputs(MAGENTA "Starting unit tests for createCurlString" RESET "\n", stderr); //                   |                   |                   |
    test_http_createCurlString_properInitialization();                                  //                   |                   |                   |
    (void)fputs(GREEN "Unit tests for createCurlString passed" RESET "\n", stderr);     //                   |                   |                   |
    // === UNIT TESTS - http - END createCurlString ==========================================================                   |                   |
    //                                                                                                                           |                   |
    // === UNIT TESTS - http - START destroyCurlString =======================================================                   |                   |
    (void)fputs(MAGENTA "Starting unit tests for destroyCurlString" RESET "\n", stderr); //                  |                   |                   |
    test_http_destroyCurlString_noLeaksEmpty();                                          //                  |                   |                   |
    test_http_destroyCurlString_noLeaksOneInsert();                                      //                  |                   |                   |
    test_http_destroyCurlString_noLeaksManyInserts();                                    //                  |                   |                   |
    (void)fputs(GREEN "Unit tests for destroyCurlString passed" RESET "\n", stderr); //                      |                   |                   |
    // === UNIT TESTS - http - END createCurlString ==========================================================                   |                   |
    //                                                                                                                           |                   |
    // === UNIT TESTS - http - START curlWriteCallback =======================================================                   |                   |
    (void)fputs(MAGENTA "Starting unit tests for curlWriteCallback" RESET "\n", stderr); //                  |                   |                   |
    test_http_curlWriteCallback_basicInserts();                                          //                  |                   |                   |
    test_http_curlWriteCallback_NonASCIIUTF8Inserts();                                   //                  |                   |                   |
    test_http_curlWriteCallback_HTMLInsert();                                            //                  |                   |                   |
    (void)fputs(GREEN "Unit tests for curlWriteCallback passed" RESET "\n", stderr); //                      |                   |                   |
    // === UNIT TESTS - http - END curlWriteCallback =========================================================                   |                   |
    //                                                                                                                           |                   |
    // === UNIT TESTS - http - START makeGETRequestAndReturnUTF8Response =====================================                   |                   |
#ifndef MSAN_SKIP //                                                                                         |                   |                   |
    (void)fputs(MAGENTA "Starting unit tests for makeGETRequestAndReturnUTF8Response " RESET "\n", stderr); //                   |                   |
    test_http_makeGETRequestAndReturnUTF8Response_nullTerminator();                                         //                   |                   |
    test_http_makeGETRequestAndReturnUTF8Response_basicRequest();                                           //                   |                   |
    test_http_makeGETRequestAndReturnUTF8Response_nonASCIIUTF8();                                           //                   |                   |
    test_http_makeGETRequestAndReturnUTF8Response_longHTMLPage();                                           //                   |                   |
    test_http_makeGETRequestAndReturnUTF8Response_exitOnFailedRequest();                                    //                   |                   |
#endif //                                                                                                    |                   |                   |
    (void)fputs(GREEN "Unit tests for makeGETRequestAndReturnUTF8Response passed" RESET "\n", stderr); //    |                   |                   |
    // === UNIT TESTS - http - END curlWriteCallback =========================================================                   |                   |
    //                                                                                                                           |                   |
    (void)fputs(GREEN "All unit tests for http passed" RESET "\n", stderr); //                                                   |                   |
    //                                                                                                                           |                   |
    // === UNIT TESTS - END http =================================================================================================                   |
    //                                                                                                                                               |
    // === UNIT TESTS - START wikiStringSearch ===================================================================================                   |
    //                                                                                                                           |                   |
    (void)fputs(MAGENTA "Starting unit tests for wikiStringSearch" RESET "\n", stderr); //                                       |                   |
    //                                                                                                                           |                   |
    // === UNIT TESTS - wikiStringSearch - stringHasNAdditionalLength START ==================================                   |                   |
    (void)fputs(MAGENTA "Starting unit tests for stringHasNAdditionalLength" RESET "\n", stderr); //         |                   |                   |
    test_stringHasNAdditionalLength_lengthExists();                                               //         |                   |                   |
    test_stringHasNAdditionalLength_lengthDoesntExist();                                          //         |                   |                   |
    test_stringHasNAdditionalLength_nonASCIIUTF8Tests();                                          //         |                   |                   |
    (void)fputs(GREEN "Unit tests for stringHasNAdditionalLength passed" RESET "\n", stderr);     //         |                   |                   |
    // === UNIT TESTS - wikiStringSearch - END stringHasNAdditionalLength ====================================                   |                   |
    //                                                                                                                           |                   |
    // === UNIT TESTS - wikiStringSearch - START getWikiTitle ================================================                   |                   |
    (void)fputs(MAGENTA "Starting unit tests for getWikiTitle" RESET "\n", stderr); //                       |                   |                   |
    test_getWikiTitle_pagesWithTitles();                                            //                       |                   |                   |
    test_getWikiTitle_pagesWithoutTitles();                                         //                       |                   |                   |
    (void)fputs(GREEN "Unit tests for getWikiTitle passed" RESET "\n", stderr);     //                       |                   |                   |
    // === UNIT TESTS - wikiStringSearch - END getWikiTitle ==================================================                   |                   |
    //                                                                                                                           |                   |
    // === UNIT TESTS - wikiStringSearch - START getIndexOfFirstWikiParagraph ================================                   |                   |
    (void)fputs(MAGENTA "Starting unit tests for getIndexOfFirstWikiParagraph" RESET "\n", stderr); //       |                   |                   |
    test_getIndexOfFirstWikiParagraph_firstParaExists();                                            //       |                   |                   |
    test_getIndexOfFirstWikiParagraph_firstParaDoesntExist();                                       //       |                   |                   |
    (void)fputs(GREEN "Unit tests for getIndexOfFirstWikiParagraph passed" RESET "\n", stderr);     //       |                   |                   |
    // === UNIT TESTS - wikiStringSearch - END getIndexOfFirstWikiParagraph ==================================                   |                   |
    //                                                                                                                           |                   |
    // === UNIT TESTS - wikiStringSearch - START maintainPunctuationCounts ===================================                   |                   |
    (void)fputs(MAGENTA "Starting unit tests for maintainPunctuationCounts" RESET "\n", stderr); //          |                   |                   |
    test_maintainPunctuationCounts_sentences();                                                  //          |                   |                   |
    (void)fputs(GREEN "Unit tests for maintainPunctuationCounts passed" RESET "\n", stderr);     //          |                   |                   |
    // === UNIT TESTS - wikiStringSearch - END curlWriteCallback =============================================                   |                   |
    //                                                                                                                           |                   |
    // === UNIT TESTS - wikiStringSearch - START getNextWikiArticleLinkFromWikiParagraph =====================                   |                   |
    (void)fputs(MAGENTA "Starting unit tests for getNextWikiArticleLinkFromWikiParagraph" RESET "\n", stderr); //                |                   |
    test_getNextWikiArticleLinkFromWikiParagraph_linkExists();                                                 //                |                   |
    test_getNextWikiArticleLinkFromWikiParagraph_linkDoesntExist();                                            //                |                   |
    (void)fputs(GREEN "Unit tests for getNextWikiArticleLinkFromWikiParagraph passed" RESET "\n", stderr);     //                |                   |
    // === UNIT TESTS - wikiStringSearch - END getNextWikiArticleLinkFromWikiParagraph =======================                   |                   |
    //                                                                                                                           |                   |
    (void)fputs(GREEN "All unit tests for wikiStringSearch passed" RESET "\n", stderr); //                                       |                   |
    //                                                                                                                           |                   |
    // === UNIT TESTS - END wikiStringSearch =====================================================================================                   |
    //                                                                                                                                               |
    (void)fputs(GREEN "All unit tests passed" RESET "\n", stderr); //                                                                                |
    //                                                                                                                                               |
    // === END UNIT TESTS ============================================================================================================================
    (void)fputs(MAGENTA "Complete test suite passed" RESET "\n", stderr);

    return 0;
}
