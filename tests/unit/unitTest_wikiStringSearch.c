#include "../include/unitTest_wikiStringSearch.h"
#include "../../src/include/wikiStringSearch.h"
#include "../include/read_file_to_string.h"
#include <assert.h>
#include <stddefer.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <uchar.h>

void test_stringHasNAdditionalLength_lengthExists(void) {
    const char8_t *const pTestString = u8"This is a test string.";

    assert(stringHasNAdditionalLength(pTestString, 0, 0));
    assert(stringHasNAdditionalLength(pTestString, 0, 3));
    assert(stringHasNAdditionalLength(pTestString, 0, 6));
    assert(stringHasNAdditionalLength(pTestString, 0, 9));
    assert(stringHasNAdditionalLength(pTestString, 0, 12));
    assert(stringHasNAdditionalLength(pTestString, 0, 15));
}

void test_stringHasNAdditionalLength_lengthDoesntExist(void) {
    const char8_t *const pTestString = u8"Another test string.";

    assert(!stringHasNAdditionalLength(pTestString, 0, 30));
    assert(!stringHasNAdditionalLength(pTestString, 0, 35));
    assert(!stringHasNAdditionalLength(pTestString, 0, 40));
}

void test_stringHasNAdditionalLength_nonASCIIUTF8Tests(void) {
    const char8_t *const pTestString = u8"זה לא אנגלית!";

    assert(stringHasNAdditionalLength(pTestString, 0, 3));
    assert(stringHasNAdditionalLength(pTestString, 0, 6));
    assert(stringHasNAdditionalLength(pTestString, 0, 9));
    assert(!stringHasNAdditionalLength(pTestString, 0, 25));
    assert(!stringHasNAdditionalLength(pTestString, 0, 30));
}

void test_getWikiTitle_pagesWithTitles(void) {
    int_least64_t fake_global_index = 0;
    char8_t *const pUnit_testing = read_file_to_string(u8"tests/test_infrastructure/unit_testing.json");
    defer { free(pUnit_testing); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle = getWikiTitle(pUnit_testing, &fake_global_index);
    defer { free(pTitle); }
    assert(strncmp((const char *const)pTitle, "Unit testing", 13) == 0);
    fake_global_index = 0;

    char8_t *const pKnowledge = read_file_to_string(u8"tests/test_infrastructure/knowledge.json");
    defer { free(pKnowledge); }
    char8_t *const pTitle2 = getWikiTitle(pKnowledge, &fake_global_index);
    defer { free(pTitle2); }
    assert(strncmp((const char *const)pTitle2, "Knowledge", 10) == 0);
}

void test_getWikiTitle_pagesWithoutTitles(void) {
    int_least64_t fake_global_index = 0;
    char8_t *const pEmoji = read_file_to_string(u8"tests/test_infrastructure/emoji.txt");
    defer { free(pEmoji); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    // cppcheck-suppress deallocuse
    assert(getWikiTitle(pEmoji, &fake_global_index) == nullptr);
    fake_global_index = 0;

    char8_t *const pGreek = read_file_to_string(u8"tests/test_infrastructure/greek.txt");
    defer { free(pGreek); }
    // cppcheck-suppress deallocuse
    assert(getWikiTitle(pGreek, &fake_global_index) == nullptr);
    fake_global_index = 0;

    char8_t *const pIndex = read_file_to_string(u8"tests/test_infrastructure/index.html");
    defer { free(pIndex); }
    // cppcheck-suppress deallocuse
    assert(getWikiTitle(pIndex, &fake_global_index) == nullptr);
    fake_global_index = 0;

    char8_t *const pIndex2 = read_file_to_string(u8"tests/test_infrastructure/index2.html");
    defer { free(pIndex2); }
    // cppcheck-suppress deallocuse
    assert(getWikiTitle(pIndex2, &fake_global_index) == nullptr);
    fake_global_index = 0;
}

void test_getIndexOfFirstWikiParagraph_firstParaExists(void) {
    constexpr int_least64_t one_hundred = 100;
    constexpr int_least64_t one_thousand = 1000;
    constexpr int_least64_t ten_thousand = 10000;

    int_least64_t fake_global_index = 0;
    char8_t *const pUnit_testing = read_file_to_string(u8"tests/test_infrastructure/unit_testing.json");
    defer { free(pUnit_testing); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    // cppcheck-suppress deallocuse
    assert(getIndexOfFirstWikiParagraph(pUnit_testing, fake_global_index) == 20692);
    fake_global_index = one_hundred;
    // cppcheck-suppress deallocuse
    assert(getIndexOfFirstWikiParagraph(pUnit_testing, fake_global_index) == 20692);
    fake_global_index = one_thousand;
    // cppcheck-suppress deallocuse
    assert(getIndexOfFirstWikiParagraph(pUnit_testing, fake_global_index) == 20692);
    fake_global_index = ten_thousand;
    // cppcheck-suppress deallocuse
    assert(getIndexOfFirstWikiParagraph(pUnit_testing, fake_global_index) == 20692);

    fake_global_index = 0;

    char8_t *const pKnowledge = read_file_to_string(u8"tests/test_infrastructure/knowledge.json");
    defer { free(pKnowledge); }
    // cppcheck-suppress deallocuse
    assert(getIndexOfFirstWikiParagraph(pKnowledge, fake_global_index) == 17739);
    fake_global_index = one_hundred;
    // cppcheck-suppress deallocuse
    assert(getIndexOfFirstWikiParagraph(pKnowledge, fake_global_index) == 17739);
    fake_global_index = one_thousand;
    // cppcheck-suppress deallocuse
    assert(getIndexOfFirstWikiParagraph(pKnowledge, fake_global_index) == 17739);
    fake_global_index = ten_thousand;
    // cppcheck-suppress deallocuse
    assert(getIndexOfFirstWikiParagraph(pKnowledge, fake_global_index) == 17739);
}

void test_getIndexOfFirstWikiParagraph_firstParaDoesntExist(void) {
    int_least64_t fake_global_index = 0;
    char8_t *const pEmoji = read_file_to_string(u8"tests/test_infrastructure/emoji.txt");
    defer { free(pEmoji); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    // cppcheck-suppress deallocuse
    assert(getIndexOfFirstWikiParagraph(pEmoji, fake_global_index) == -1);
    fake_global_index = 0;

    char8_t *const pGreek = read_file_to_string(u8"tests/test_infrastructure/greek.txt");
    defer { free(pGreek); }
    // cppcheck-suppress deallocuse
    assert(getIndexOfFirstWikiParagraph(pGreek, fake_global_index) == -1);
    fake_global_index = 0;

    char8_t *const pIndex = read_file_to_string(u8"tests/test_infrastructure/index.html");
    defer { free(pIndex); }
    // cppcheck-suppress deallocuse
    assert(getIndexOfFirstWikiParagraph(pIndex, fake_global_index) == -1);
    fake_global_index = 0;

    char8_t *const pIndex2 = read_file_to_string(u8"tests/test_infrastructure/index2.html");
    defer { free(pIndex2); }
    // cppcheck-suppress deallocuse
    assert(getIndexOfFirstWikiParagraph(pIndex2, fake_global_index) == -1);
}

void test_maintainPunctuationCounts_sentences(void) {
    uint_fast8_t paranthesis_count = 0;
    uint_fast8_t square_count = 0;
    uint_fast8_t curly_count = 0;

    const char8_t *const pSentence_one = u8"This is a sentence (it has paranthesis). It also has a [.";
    for (int_fast16_t index = 0; pSentence_one[index] != '\0'; ++index) {
        maintainPunctuationCounts(pSentence_one[index], &paranthesis_count, &square_count, &curly_count);
    }
    assert(paranthesis_count == 0);
    assert(square_count == 1);
    assert(curly_count == 0);

    paranthesis_count = 0;
    square_count = 0;
    curly_count = 0;

    const char8_t *const pSentence_two = u8"{Curley brackets are easy (and fun to draw), but [(square brakets] are easier.";
    for (int_fast16_t index = 0; pSentence_two[index] != '\0'; ++index) {
        maintainPunctuationCounts(pSentence_two[index], &paranthesis_count, &square_count, &curly_count);
    }
    assert(paranthesis_count == 1);
    assert(square_count == 0);
    assert(curly_count == 1);

    paranthesis_count = 0;
    square_count = 0;
    curly_count = 0;

    const char8_t *const pTorture_test = u8"((((){{}}{[[])))([[]}";
    for (int_fast16_t index = 0; pTorture_test[index] != '\0'; ++index) {
        maintainPunctuationCounts(pTorture_test[index], &paranthesis_count, &square_count, &curly_count);
    }
    assert(paranthesis_count == 1);
    assert(square_count == 2);
    assert(curly_count == 0);
}

void test_getNextWikiArticleLinkFromWikiParagraph_linkExists(void) {
    char8_t *const pUnit_testing = read_file_to_string(u8"tests/test_infrastructure/unit_testing.json");
    defer { free(pUnit_testing); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle = getNextWikiArticleSlugFromWikiParagraph(pUnit_testing, 0);
    defer { free(pTitle); }
    assert(strncmp((const char *const)pTitle, "Software_testing", 17) == 0);

    char8_t *const pKnowledge = read_file_to_string(u8"tests/test_infrastructure/knowledge.json");
    defer { free(pKnowledge); }
    char8_t *const pTitle2 = getNextWikiArticleSlugFromWikiParagraph(pKnowledge, 0);
    defer { free(pTitle2); }
    assert(strncmp((const char *const)pTitle2, "Belief", 7) == 0);
}

void test_getNextWikiArticleLinkFromWikiParagraph_linkDoesntExist(void) {
    char8_t *const pEmoji = read_file_to_string(u8"tests/test_infrastructure/emoji.txt");
    defer { free(pEmoji); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    // cppcheck-suppress deallocuse
    assert(getNextWikiArticleSlugFromWikiParagraph(pEmoji, 0) == nullptr);

    char8_t *const pGreek = read_file_to_string(u8"tests/test_infrastructure/greek.txt");
    defer { free(pGreek); }
    // cppcheck-suppress deallocuse
    assert(getNextWikiArticleSlugFromWikiParagraph(pGreek, 0) == nullptr);

    char8_t *const pIndex = read_file_to_string(u8"tests/test_infrastructure/index.html");
    defer { free(pIndex); }
    // cppcheck-suppress deallocuse
    assert(getNextWikiArticleSlugFromWikiParagraph(pIndex, 0) == nullptr);

    char8_t *const pIndex2 = read_file_to_string(u8"tests/test_infrastructure/index2.html");
    defer { free(pIndex2); }
    // cppcheck-suppress deallocuse
    assert(getNextWikiArticleSlugFromWikiParagraph(pIndex2, 0) == nullptr);
}
