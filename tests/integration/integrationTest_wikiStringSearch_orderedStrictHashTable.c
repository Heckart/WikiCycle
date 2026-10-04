#define _POSIX_C_SOURCE 200809L
#include "../include/integrationTest_wikiStringSearch_orderedStrictHashTable.h"
#include "../../src/include/orderedStrictHashTable.h"
#include "../../src/include/returnCodes.h"
#include "../../src/include/wikiStringSearch.h"
#include "../include/read_file_to_string.h"
#include <assert.h>
#include <bsd/string.h>
#include <stddefer.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <uchar.h>
#include <unistd.h>

static char8_t *setup_next_article_file_path(const char8_t *const restrict pArticle, int_least64_t index) {
    constexpr uint_least32_t file_path_max_size = 26 + LONGEST_WIKI_ARTICLE_NAME + 1 + 5;
    char8_t *const pFile_path = u8"tests/test_infrastructure/";

    char8_t *const pNext = getNextWikiArticleSlugFromWikiParagraph(pArticle, index);
    defer { free(pNext); }
    char8_t *pNext_path = calloc(sizeof(char8_t) * file_path_max_size, sizeof(char8_t));
    strlcpy((char *const)pNext_path, (char *const)pFile_path, file_path_max_size);
    strlcat((char *const)pNext_path, (char *const)pNext, file_path_max_size);
    strlcat((char *const)pNext_path, ".json", file_path_max_size);

    return pNext_path;
}

void test_wikiStringSearch_orderedStrictHashTable_findCycleOne(void) {
    constexpr uint_least32_t bucket_size = 1000;
    OrderedStrictHashTable *const pTable = createOSHT(bucket_size);
    defer { destroyOSHT(pTable); }

    char8_t *const pArticle_one = read_file_to_string(u8"tests/test_infrastructure/Unit_testing.json");
    defer { free(pArticle_one); }
    int_least64_t fake_global_index = 0;
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_one = getWikiTitle(pArticle_one, &fake_global_index);
    defer { free(pTitle_one); }
    assert(insertToOSHT(pTable, pTitle_one) == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pStart_node->pNode_name, (const char *const)pTitle_one, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_one, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    char8_t *const pNext_path_one = setup_next_article_file_path(pArticle_one, fake_global_index);
    defer { free(pNext_path_one); }

    fake_global_index = 0;
    char8_t *const pArticle_two = read_file_to_string(pNext_path_one);
    defer { free(pArticle_two); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_two = getWikiTitle(pArticle_two, &fake_global_index);
    defer { free(pTitle_two); }
    assert(insertToOSHT(pTable, pTitle_two) == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pStart_node->pNode_name, (const char *const)pTitle_one, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_two, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    char8_t *const pNext_path_two = setup_next_article_file_path(pArticle_two, fake_global_index);
    defer { free(pNext_path_two); }

    fake_global_index = 0;
    char8_t *const pArticle_three = read_file_to_string(pNext_path_two);
    defer { free(pArticle_three); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_three = getWikiTitle(pArticle_three, &fake_global_index);
    defer { free(pTitle_three); }
    assert(insertToOSHT(pTable, pTitle_three) == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_three, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    char8_t *const pNext_path_three = setup_next_article_file_path(pArticle_three, fake_global_index);
    defer { free(pNext_path_three); }

    fake_global_index = 0;
    char8_t *const pArticle_four = read_file_to_string(pNext_path_three);
    defer { free(pArticle_four); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_four = getWikiTitle(pArticle_four, &fake_global_index);
    defer { free(pTitle_four); }
    assert(insertToOSHT(pTable, pTitle_four) == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_four, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    char8_t *const pNext_path_four = setup_next_article_file_path(pArticle_four, fake_global_index);
    defer { free(pNext_path_four); }

    fake_global_index = 0;
    char8_t *const pArticle_five = read_file_to_string(pNext_path_four);
    defer { free(pArticle_five); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_five = getWikiTitle(pArticle_five, &fake_global_index);
    defer { free(pTitle_five); }
    assert(insertToOSHT(pTable, pTitle_five) == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_five, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    char8_t *const pNext_path_five = setup_next_article_file_path(pArticle_five, fake_global_index);
    defer { free(pNext_path_five); }

    fake_global_index = 0;
    char8_t *const pArticle_six = read_file_to_string(pNext_path_five);
    defer { free(pArticle_six); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_six = getWikiTitle(pArticle_six, &fake_global_index);
    defer { free(pTitle_six); }
    assert(insertToOSHT(pTable, pTitle_six) == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_six, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    char8_t *const pNext_path_six = setup_next_article_file_path(pArticle_six, fake_global_index);
    defer { free(pNext_path_six); }

    fake_global_index = 0;
    char8_t *const pArticle_redirect = read_file_to_string(pNext_path_six);
    defer { free(pArticle_redirect); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_redirect = getWikiTitle(pArticle_redirect, &fake_global_index);
    defer { free(pTitle_redirect); }
    assert(insertToOSHT(pTable, pTitle_redirect) == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_redirect, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    char8_t *const pNext_path_redirect = setup_next_article_file_path(pArticle_redirect, fake_global_index);
    defer { free(pNext_path_redirect); }

    fake_global_index = 0;
    char8_t *const pArticle_seven = read_file_to_string(pNext_path_redirect);
    defer { free(pArticle_seven); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_seven = getWikiTitle(pArticle_seven, &fake_global_index);
    defer { free(pTitle_seven); }
    assert(insertToOSHT(pTable, pTitle_seven) == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_seven, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    char8_t *const pNext_path_seven = setup_next_article_file_path(pArticle_seven, fake_global_index);
    defer { free(pNext_path_seven); }

    fake_global_index = 0;
    char8_t *const pArticle_eight = read_file_to_string(pNext_path_seven);
    defer { free(pArticle_eight); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_eight = getWikiTitle(pArticle_eight, &fake_global_index);
    defer { free(pTitle_eight); }
    assert(insertToOSHT(pTable, pTitle_eight) == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_eight, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    char8_t *const pNext_path_eight = setup_next_article_file_path(pArticle_eight, fake_global_index);
    defer { free(pNext_path_eight); }

    fake_global_index = 0;
    char8_t *const pArticle_nine = read_file_to_string(pNext_path_eight);
    defer { free(pArticle_nine); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_nine = getWikiTitle(pArticle_nine, &fake_global_index);
    defer { free(pTitle_nine); }
    assert(insertToOSHT(pTable, pTitle_nine) == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_nine, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    char8_t *const pNext_path_nine = setup_next_article_file_path(pArticle_nine, fake_global_index);
    defer { free(pNext_path_nine); }

    fake_global_index = 0;
    char8_t *const pArticle_ten = read_file_to_string(pNext_path_nine);
    defer { free(pArticle_ten); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_ten = getWikiTitle(pArticle_ten, &fake_global_index);
    fake_global_index = 0;
    defer { free(pTitle_ten); }
    assert(insertToOSHT(pTable, pTitle_ten) == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_ten, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    char8_t *const pNext_path_ten = setup_next_article_file_path(pArticle_ten, fake_global_index);
    defer { free(pNext_path_ten); }

    fake_global_index = 0;
    char8_t *const pArticle_eleven = read_file_to_string(pNext_path_ten);
    defer { free(pArticle_eleven); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_eleven = getWikiTitle(pArticle_eleven, &fake_global_index);
    fake_global_index = 0;
    defer { free(pTitle_eleven); }
    assert(insertToOSHT(pTable, pTitle_eleven) == DUPLICATE_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_ten, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
}

void test_wikiStringSearch_orderedStrictHashTable_findCycleTwo(void) {
    constexpr uint_least32_t bucket_size = 1000;
    OrderedStrictHashTable *const pTable = createOSHT(bucket_size);
    defer { destroyOSHT(pTable); }

    char8_t *const pArticle_one = read_file_to_string(u8"tests/test_infrastructure/Knowledge.json");
    defer { free(pArticle_one); }
    int_least64_t fake_global_index = 0;
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_one = getWikiTitle(pArticle_one, &fake_global_index);
    defer { free(pTitle_one); }
    assert(insertToOSHT(pTable, pTitle_one) == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pStart_node->pNode_name, (const char *const)pTitle_one, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_one, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    char8_t *const pNext_path_one = setup_next_article_file_path(pArticle_one, fake_global_index);
    defer { free(pNext_path_one); }

    fake_global_index = 0;
    char8_t *const pArticle_two = read_file_to_string(pNext_path_one);
    defer { free(pArticle_two); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_two = getWikiTitle(pArticle_two, &fake_global_index);
    defer { free(pTitle_two); }
    assert(insertToOSHT(pTable, pTitle_two) == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pStart_node->pNode_name, (const char *const)pTitle_one, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_two, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    char8_t *const pNext_path_two = setup_next_article_file_path(pArticle_two, fake_global_index);
    defer { free(pNext_path_two); }

    fake_global_index = 0;
    char8_t *const pArticle_three = read_file_to_string(pNext_path_two);
    defer { free(pArticle_three); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_three = getWikiTitle(pArticle_three, &fake_global_index);
    defer { free(pTitle_three); }
    assert(insertToOSHT(pTable, pTitle_three) == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_three, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    char8_t *const pNext_path_three = setup_next_article_file_path(pArticle_three, fake_global_index);
    defer { free(pNext_path_three); }

    fake_global_index = 0;
    char8_t *const pArticle_four = read_file_to_string(pNext_path_three);
    defer { free(pArticle_four); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_four = getWikiTitle(pArticle_four, &fake_global_index);
    defer { free(pTitle_four); }
    assert(insertToOSHT(pTable, pTitle_four) == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_four, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    char8_t *const pNext_path_four = setup_next_article_file_path(pArticle_four, fake_global_index);
    defer { free(pNext_path_four); }

    fake_global_index = 0;
    char8_t *const pArticle_five = read_file_to_string(pNext_path_four);
    defer { free(pArticle_five); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_five = getWikiTitle(pArticle_five, &fake_global_index);
    defer { free(pTitle_five); }
    assert(insertToOSHT(pTable, pTitle_five) == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_five, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    char8_t *const pNext_path_five = setup_next_article_file_path(pArticle_five, fake_global_index);
    defer { free(pNext_path_five); }

    fake_global_index = 0;
    char8_t *const pArticle_six = read_file_to_string(pNext_path_five);
    defer { free(pArticle_six); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_six = getWikiTitle(pArticle_six, &fake_global_index);
    defer { free(pTitle_six); }
    assert(insertToOSHT(pTable, pTitle_six) == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_six, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    char8_t *const pNext_path_six = setup_next_article_file_path(pArticle_six, fake_global_index);
    defer { free(pNext_path_six); }

    fake_global_index = 0;
    char8_t *const pArticle_seven = read_file_to_string(pNext_path_six);
    defer { free(pArticle_seven); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_seven = getWikiTitle(pArticle_seven, &fake_global_index);
    defer { free(pTitle_seven); }
    assert(insertToOSHT(pTable, pTitle_seven) == SUCCESSFUL_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_seven, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
    char8_t *const pNext_path_seven = setup_next_article_file_path(pArticle_seven, fake_global_index);
    defer { free(pNext_path_seven); }

    fake_global_index = 0;
    char8_t *const pArticle_eight = read_file_to_string(pNext_path_seven);
    defer { free(pArticle_eight); }
    [[clang::suppress]] // analyzer doesnt understand defer yet
    char8_t *const pTitle_eight = getWikiTitle(pArticle_eight, &fake_global_index);
    fake_global_index = 0;
    defer { free(pTitle_eight); }
    assert(insertToOSHT(pTable, pTitle_eight) == DUPLICATE_NODE_INSERTION);
    assert(strncmp((const char *const)pTable->pTail_node->pNode_name, (const char *const)pTitle_seven, LONGEST_WIKI_ARTICLE_NAME + 1) == 0);
}
