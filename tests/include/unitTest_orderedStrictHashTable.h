// Copyright [2026] Ethan Heckart
#ifndef WIKICYCLE_TESTS_INCLUDE_UNITTEST_ORDEREDSTRICTHASHTABLE_H_
#define WIKICYCLE_TESTS_INCLUDE_UNITTEST_ORDEREDSTRICTHASHTABLE_H_

void test_createOSHT_properMemberInitializations();
void test_createOSHT_zeroBucketInitialization();

void test_destroyOSHT_noLeaksAfterNoInserts();
void test_destroyOSHT_noLeaksAfterOneInsert();
void test_destroyOSHT_noLeaksAfterMultipleInserts();

void test_hashExistsInOSHT_checkNodesThatExist();
void test_hashExistsInOSHT_checkNodesThatDoNotExist();

void test_insertToOSHT_basicNodeInsertions();
void test_insertToOSHT_duplicateInsertion();
void test_insertToOSHT_collision();
void test_insertToOSHT_nonASCIIUTF8();

#endif // WIKICYCLE_TESTS_INCLUDE_UNITTEST_ORDEREDSTRICTHASHTABLE_H_
