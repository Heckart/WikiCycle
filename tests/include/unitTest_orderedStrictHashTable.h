// Copyright [2026] Ethan Heckart
#ifndef WIKICYCLE_TESTS_INCLUDE_UNITTEST_ORDEREDSTRICTHASHTABLE_H_
#define WIKICYCLE_TESTS_INCLUDE_UNITTEST_ORDEREDSTRICTHASHTABLE_H_

void test_createOSHT_properMemberInitializations(void);
void test_createOSHT_zeroBucketInitialization(void);

void test_destroyOSHT_noLeaksAfterNoInserts(void);
void test_destroyOSHT_noLeaksAfterOneInsert(void);
void test_destroyOSHT_noLeaksAfterMultipleInserts(void);

void test_hashExistsInOSHT_checkNodesThatExist(void);
void test_hashExistsInOSHT_checkNodesThatDoNotExist(void);

void test_insertToOSHT_basicNodeInsertions(void);
void test_insertToOSHT_duplicateInsertion(void);
void test_insertToOSHT_collision(void);
void test_insertToOSHT_nonASCIIUTF8(void);

#endif // WIKICYCLE_TESTS_INCLUDE_UNITTEST_ORDEREDSTRICTHASHTABLE_H_
