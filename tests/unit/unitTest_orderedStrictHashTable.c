#define _POSIX_C_SOURCE 200809L
#define LONGEST_WIKI_ARTICLE_LENGTH 255
#include "unitTest_orderedStrictHashTable.h"
#include "../../src/ds/orderedStrictHashTable.h"
#include <assert.h>
#include <stddefer.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void test_createOSHT_properMemberInitializations() {
    constexpr uint_least32_t bucket_size = 10;
    OrderedStrictHashTable *const pTestOSHT = createOSHT(bucket_size);
    defer { destroyOSHT(pTestOSHT); }

    assert(pTestOSHT->bucket_size == bucket_size);
    assert(pTestOSHT->pStart_node == nullptr);
    assert(pTestOSHT->pTail_node == nullptr);

    for (uint_fast32_t bucket = 0; bucket < bucket_size; bucket++) {
        assert(pTestOSHT->pHash_table[bucket] == nullptr);
    }
}

void test_createOSHT_zeroBucketInitialization() {
    constexpr uint_least32_t bucket_size = 0;

    /*
     * To test an exit without actually exiting, things are going to get freaky. A child process will be spawned and sacrificed, then we will inspect
     * the exit code of the sacrificed process to make sure it was what we were expecting.
     */

    const pid_t pid = fork();
    assert(pid >= 0);

    if (pid == 0) {
        // this is the child process
        OrderedStrictHashTable *const pTestOSHT = createOSHT(bucket_size);
        destroyOSHT(pTestOSHT); // SHUT UP COMPILER!
        assert(false);
    }

    // this is the parent process
    int_least32_t status = -1;
    const pid_t result = waitpid(pid, (int *)&status, 0);
    assert(result == pid);
    // cppcheck-suppress assertWithSideEffect
    assert(WEXITSTATUS(status) == EMPTY_BUCKET_RC);
}

void test_destroyOSHT_noLeaksAfterNoInserts() {
    constexpr uint_least32_t bucket_size = 500;
    OrderedStrictHashTable *const pTestOSHT = createOSHT(bucket_size);
    destroyOSHT(pTestOSHT);
}

void test_destroyOSHT_noLeaksAfterOneInsert() {
    constexpr uint_least32_t bucket_size = 500;
    OrderedStrictHashTable *const pTestOSHT = createOSHT(bucket_size);
    defer { destroyOSHT(pTestOSHT); }

    insertToOSHT(pTestOSHT, u8"Testing");
}

void test_destroyOSHT_noLeaksAfterMultipleInserts() {
    constexpr uint_least32_t bucket_size = 1000;
    OrderedStrictHashTable *const pTestOSHT = createOSHT(bucket_size);
    defer { destroyOSHT(pTestOSHT); }

    insertToOSHT(pTestOSHT, u8"Inserting");
    insertToOSHT(pTestOSHT, u8"several");
    insertToOSHT(pTestOSHT, u8"nodes");
    insertToOSHT(pTestOSHT, u8"to");
    insertToOSHT(pTestOSHT, u8"the");
    insertToOSHT(pTestOSHT, u8"table");
    insertToOSHT(pTestOSHT, u8"for");
    insertToOSHT(pTestOSHT, u8"testing");
}

void test_hashExistsInOSHT_checkNodesThatExist() {
    constexpr uint_least32_t bucket_size = 700;
    OrderedStrictHashTable *const pTestOSHT = createOSHT(bucket_size);
    defer { destroyOSHT(pTestOSHT); }

    insertToOSHT(pTestOSHT, u8"We");
    insertToOSHT(pTestOSHT, u8"shall");
    insertToOSHT(pTestOSHT, u8"see");
    insertToOSHT(pTestOSHT, u8"if");
    insertToOSHT(pTestOSHT, u8"the");
    insertToOSHT(pTestOSHT, u8"nodes");
    insertToOSHT(pTestOSHT, u8"exist");

    assert(hashExistsInOSHT(pTestOSHT, calcHash(u8"We", pTestOSHT)) == true);
    assert(hashExistsInOSHT(pTestOSHT, calcHash(u8"shall", pTestOSHT)) == true);
    assert(hashExistsInOSHT(pTestOSHT, calcHash(u8"see", pTestOSHT)) == true);
    assert(hashExistsInOSHT(pTestOSHT, calcHash(u8"if", pTestOSHT)) == true);
    assert(hashExistsInOSHT(pTestOSHT, calcHash(u8"the", pTestOSHT)) == true);
    assert(hashExistsInOSHT(pTestOSHT, calcHash(u8"nodes", pTestOSHT)) == true);
    assert(hashExistsInOSHT(pTestOSHT, calcHash(u8"exist", pTestOSHT)) == true);
}

void test_hashExistsInOSHT_checkNodesThatDoNotExist() {
    constexpr uint_least32_t bucket_size = 357;
    OrderedStrictHashTable *const pTestOSHT = createOSHT(bucket_size);
    defer { destroyOSHT(pTestOSHT); }

    insertToOSHT(pTestOSHT, u8"Won't");
    insertToOSHT(pTestOSHT, u8"find");
    insertToOSHT(pTestOSHT, u8"these");
    insertToOSHT(pTestOSHT, u8"nodes!");

    assert(hashExistsInOSHT(pTestOSHT, calcHash(u8"Nope", pTestOSHT)) == false);
    assert(hashExistsInOSHT(pTestOSHT, calcHash(u8"Try again", pTestOSHT)) == false);
    assert(hashExistsInOSHT(pTestOSHT, calcHash(u8"Not gonna happen!", pTestOSHT)) == false);
    assert(hashExistsInOSHT(pTestOSHT, calcHash(u8":(", pTestOSHT)) == false);
}

void test_insertToOSHT_basicNodeInsertions() {
    constexpr uint_least32_t bucket_size = 10000;
    OrderedStrictHashTable *const pTestOSHT = createOSHT(bucket_size);
    defer { destroyOSHT(pTestOSHT); }

    assert(insertToOSHT(pTestOSHT, u8"The first node") == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTestOSHT->pStart_node->pNode_name, (const char *const)u8"The first node", LONGEST_WIKI_ARTICLE_LENGTH + 1) ==
           0);
    assert(strncmp((const char *const)pTestOSHT->pTail_node->pNode_name, (const char *const)u8"The first node", LONGEST_WIKI_ARTICLE_LENGTH + 1) ==
           0);
    assert(pTestOSHT->pStart_node->pNext_node == nullptr);
    assert(pTestOSHT->pTail_node->pNext_node == nullptr);

    assert(insertToOSHT(pTestOSHT, u8"It's node number 2") == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTestOSHT->pStart_node->pNode_name, (const char *const)u8"The first node", LONGEST_WIKI_ARTICLE_LENGTH + 1) ==
           0);
    assert(strncmp((const char *const)pTestOSHT->pTail_node->pNode_name, (const char *const)u8"It's node number 2",
                   LONGEST_WIKI_ARTICLE_LENGTH + 1) == 0);
    assert(pTestOSHT->pStart_node != pTestOSHT->pTail_node);
    assert(pTestOSHT->pStart_node->pNext_node == pTestOSHT->pTail_node);
    assert(pTestOSHT->pTail_node->pNext_node == nullptr);

    assert(insertToOSHT(pTestOSHT, u8"Node tres is pretty cool") == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTestOSHT->pStart_node->pNode_name, (const char *const)u8"The first node", LONGEST_WIKI_ARTICLE_LENGTH + 1) ==
           0);
    assert(strncmp((const char *const)pTestOSHT->pStart_node->pNext_node->pNode_name, (const char *const)u8"It's node number 2",
                   LONGEST_WIKI_ARTICLE_LENGTH + 1) == 0);
    assert(strncmp((const char *const)pTestOSHT->pTail_node->pNode_name, (const char *const)u8"Node tres is pretty cool",
                   LONGEST_WIKI_ARTICLE_LENGTH + 1) == 0);
    assert(pTestOSHT->pStart_node != pTestOSHT->pTail_node);
    assert(pTestOSHT->pTail_node->pNext_node == nullptr);
    assert(pTestOSHT->pStart_node->pNext_node->pNext_node == pTestOSHT->pTail_node);

    assert(insertToOSHT(pTestOSHT, u8"") == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTestOSHT->pStart_node->pNode_name, (const char *const)u8"The first node", LONGEST_WIKI_ARTICLE_LENGTH + 1) ==
           0);
    assert(strncmp((const char *const)pTestOSHT->pTail_node->pNode_name, (const char *const)u8"", LONGEST_WIKI_ARTICLE_LENGTH + 1) == 0);
    assert(pTestOSHT->pStart_node != pTestOSHT->pTail_node);
    assert(pTestOSHT->pTail_node->pNext_node == nullptr);
}

void test_insertToOSHT_duplicateInsertion() {
    constexpr uint_least32_t bucket_size = 77;
    OrderedStrictHashTable *const pTestOSHT = createOSHT(bucket_size);
    defer { destroyOSHT(pTestOSHT); }

    assert(insertToOSHT(pTestOSHT, u8"deja vu") == SUCCESSFUL_NODE_INSERTION);
    assert(insertToOSHT(pTestOSHT, u8"deja vu") == DUPLICATE_NODE_INSERTION);
    assert(insertToOSHT(pTestOSHT, u8"Haven't seen this one") == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTestOSHT->pStart_node->pNode_name, (const char *const)u8"deja vu", LONGEST_WIKI_ARTICLE_LENGTH + 1) == 0);
    assert(strncmp((const char *const)pTestOSHT->pTail_node->pNode_name, (const char *const)u8"Haven't seen this one",
                   LONGEST_WIKI_ARTICLE_LENGTH + 1) == 0);
}

void test_insertToOSHT_collision() {
    constexpr uint_least32_t bucket_size = 2;
    OrderedStrictHashTable *const pTestOSHT = createOSHT(bucket_size);
    assert(insertToOSHT(pTestOSHT, u8"not a lotta space in here") == SUCCESSFUL_NODE_INSERTION);

    /*
     * See exit test comment in test_createOSHT_zeroBucketInsitialization()
     */
    const pid_t pid = fork();
    assert(pid >= 0);
    if (pid == 0) {
        insertToOSHT(pTestOSHT, u8"no there is not!");
        destroyOSHT(pTestOSHT); // SHUT UP COMPILER!
        assert(false);
    }

    int_least32_t status = -1;
    const pid_t result = waitpid(pid, (int *)&status, 0);
    assert(result == pid);
    // cppcheck-suppress assertWithSideEffect
    assert(WEXITSTATUS(status) == INSERTION_COLLISION);
    destroyOSHT(pTestOSHT);
}

void test_insertToOSHT_nonASCIIUTF8() {
    constexpr uint_least32_t bucket_size = 1967;
    OrderedStrictHashTable *const pTestOSHT = createOSHT(bucket_size);
    defer { destroyOSHT(pTestOSHT); }

    assert(insertToOSHT(pTestOSHT, u8"這句話是中文的。") == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTestOSHT->pStart_node->pNode_name, (const char *const)u8"這句話是中文的。", LONGEST_WIKI_ARTICLE_LENGTH + 1) ==
           0);
    assert(strncmp((const char *const)pTestOSHT->pTail_node->pNode_name, (const char *const)u8"這句話是中文的。", LONGEST_WIKI_ARTICLE_LENGTH + 1) ==
           0);
    assert(pTestOSHT->pStart_node->pNext_node == nullptr);
    assert(pTestOSHT->pTail_node->pNext_node == nullptr);

    assert(insertToOSHT(pTestOSHT, u8"یہ اردو میں ہے۔") == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTestOSHT->pStart_node->pNode_name, (const char *const)u8"這句話是中文的。", LONGEST_WIKI_ARTICLE_LENGTH + 1) ==
           0);
    assert(strncmp((const char *const)pTestOSHT->pTail_node->pNode_name, (const char *const)u8"یہ اردو میں ہے۔", LONGEST_WIKI_ARTICLE_LENGTH + 1) ==
           0);
    assert(pTestOSHT->pStart_node != pTestOSHT->pTail_node);
    assert(pTestOSHT->pStart_node->pNext_node == pTestOSHT->pTail_node);
    assert(pTestOSHT->pTail_node->pNext_node == nullptr);

    assert(insertToOSHT(pTestOSHT, u8"🧌🪤🗿") == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTestOSHT->pStart_node->pNode_name, (const char *const)u8"這句話是中文的。", LONGEST_WIKI_ARTICLE_LENGTH + 1) ==
           0);
    assert(strncmp((const char *const)pTestOSHT->pStart_node->pNext_node->pNode_name, (const char *const)u8"یہ اردو میں ہے۔",
                   LONGEST_WIKI_ARTICLE_LENGTH + 1) == 0);
    assert(strncmp((const char *const)pTestOSHT->pTail_node->pNode_name, (const char *const)u8"🧌🪤🗿", LONGEST_WIKI_ARTICLE_LENGTH + 1) == 0);
    assert(pTestOSHT->pStart_node != pTestOSHT->pTail_node);
    assert(pTestOSHT->pTail_node->pNext_node == nullptr);
    assert(pTestOSHT->pStart_node->pNext_node->pNext_node == pTestOSHT->pTail_node);

    assert(insertToOSHT(pTestOSHT, u8"Ñ") == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTestOSHT->pStart_node->pNode_name, (const char *const)u8"這句話是中文的。", LONGEST_WIKI_ARTICLE_LENGTH + 1) ==
           0);
    assert(strncmp((const char *const)pTestOSHT->pTail_node->pNode_name, (const char *const)u8"Ñ", LONGEST_WIKI_ARTICLE_LENGTH + 1) == 0);
    assert(pTestOSHT->pStart_node != pTestOSHT->pTail_node);
    assert(pTestOSHT->pTail_node->pNext_node == nullptr);
}
