#include "unit/unitTest_orderedStrictHashTable.h"
#include <stdio.h>
#define GREEN "\x1b[32m"
#define MAGENTA "\x1b[35m"
#define RESET "\x1b[0m"

/*
 * This file is a consolidated entry point for all unit/integration/e2e tests so there can be one compiled main that runs them all.
 * Performance tests will be a bit of a different situation. We will cross that bridge when we get there.
 */

int main() {

    (void)fputs(MAGENTA "Starting test suite" RESET "\n", stderr);
    // === START UNIT TESTS ===========================================================================================
    //                                                                                                                |
    (void)fputs(MAGENTA "Starting unit tests" RESET "\n", stderr); //                                                 |
    //                                                                                                                |
    // === UNIT TESTS - START OrderedStrictHashTable =========================================================        |
    //                                                                                                       |        |
    (void)fputs(MAGENTA "Starting unit tests for OrderedStrictHashTable" RESET "\n", stderr); //             |        |
    //                                                                                                       |        |
    // === UNIT TESTS - OrderedStrictHashTable - START createOSHT =================================          |        |
    (void)fputs(MAGENTA "Starting unit tests for createOSHT" RESET "\n", stderr); //              |          |        |
    test_createOSHT_properMemberInitializations();                                //              |          |        |
    test_createOSHT_zeroBucketInitialization();                                   //              |          |        |
    (void)fputs(GREEN "Unit tests for createOSHT passed" RESET "\n", stderr);     //              |          |        |
    // === UNIT TESTS - OrderedStrictHashTable - END createOSHT ===================================          |        |
    //                                                                                                       |        |
    // === UNIT TESTS - OrderedStrictHashTable - START destroyOSHT ================================          |        |
    (void)fputs(MAGENTA "Starting unit tests for destroyOSHT" RESET "\n", stderr); //             |          |        |
    test_destroyOSHT_noLeaksAfterNoInserts();                                      //             |          |        |
    test_destroyOSHT_noLeaksAfterOneInsert();                                      //             |          |        |
    test_destroyOSHT_noLeaksAfterMultipleInserts();                                //             |          |        |
    (void)fputs(GREEN "Unit tests for destroyOSHT passed" RESET "\n", stderr);     //             |          |        |
    // === UNIT TESTS - OrderedStrictHashTable - END destroyOSHT ==================================          |        |
    //                                                                                                       |        |
    // === UNIT TESTS - OrderedStrictHashTable - START hashExistsInOSHT ===========================          |        |
    (void)fputs(MAGENTA "Starting unit tests for hashExistsInOSHT" RESET "\n", stderr); //        |          |        |
    test_hashExistsInOSHT_checkNodesThatExist();                                        //        |          |        |
    test_hashExistsInOSHT_checkNodesThatDoNotExist();                                   //        |          |        |
    (void)fputs(GREEN "Unit tests for hashExistsInOSHT passed" RESET "\n", stderr);     //        |          |        |
    // === UNIT TESTS - OrderedStrictHashTable - END hashExistsInOSHT =============================          |        |
    //                                                                                                       |        |
    // === UNIT TESTS - OrderedStrictHashTable - START insertToOSHT ===============================          |        |
    (void)fputs(MAGENTA "Starting unit tests for insertToOSHT" RESET "\n", stderr); //            |          |        |
    test_insertToOSHT_basicNodeInsertions();                                        //            |          |        |
    test_insertToOSHT_duplicateInsertion();                                         //            |          |        |
    test_insertToOSHT_collision();                                                  //            |          |        |
    test_insertToOSHT_nonASCIIUTF8();                                               //            |          |        |
    (void)fputs(GREEN "Unit tests for insertToOSHT passed" RESET "\n", stderr);     //            |          |        |
    // === UNIT TESTS - OrderedStrictHashTable - END insertToOSHT =================================          |        |
    //                                                                                                       |        |
    (void)fputs(GREEN "All unit tests for OrderedStrictHashTable passed" RESET "\n", stderr); //             |        |
    //                                                                                                       |        |
    // === UNIT TESTS - END OrderedStrictHashTable ===========================================================        |
    //                                                                                                                |
    (void)fputs(GREEN "All unit tests passed" RESET "\n", stderr); //                                                 |
    //                                                                                                                |
    // === END UNIT TESTS =============================================================================================
    (void)fputs(MAGENTA "Complete test suite passed" RESET "\n", stderr);

    return 0;
}
