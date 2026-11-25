#define _POSIX_C_SOURCE 200809L
#include "../include/wikiCycle.h"
#include "../include/http.h"
#include "../include/orderedStrictHashTable.h"
#include "../include/returnCodes.h"
#include "../include/wikiStringSearch.h"
#include <bsd/string.h>
#include <stddefer.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <uchar.h>
#include <unistd.h>

OrderedStrictHashTable *findWikiCycle(uint_least32_t bucket_size, const char8_t *const restrict pArticle_name,
                                      const char8_t *const restrict pPre_slug_url) {
    // for real-world usage, pre_slug_url is always "https://en.wikipedia.org/w/api.php?action=parse&page=", which is 54 characters. During testing it
    // will be shorter.
    constexpr uint_least32_t max_pre_slug_length_limit = 54;
    // format tacked onto the end should always be "&format=json"
    constexpr uint_least32_t max_format_length_limit = 12;
    const uint_least32_t max_url_size = max_pre_slug_length_limit + LONGEST_WIKI_ARTICLE_NAME + max_format_length_limit + 1;

    char8_t *pFull_url = malloc(max_url_size);
    if (pFull_url == NULL) {
        exit(1);
    }
    defer { free(pFull_url); }
    // cppcheck-suppress deallocuse
    pFull_url[0] = '\0';
    strlcat((char *const)pFull_url, (const char *const)pPre_slug_url, max_url_size);
    strlcat((char *const)pFull_url, (const char *const)pArticle_name, max_url_size);
    strlcat((char *const)pFull_url, "&format=json", max_url_size);

    OrderedStrictHashTable *const wiki_cycle_tracker = createOSHT(bucket_size);

    constexpr uint_fast64_t SAFE_SLEEP_INTERVAL = 350'000'000;
    struct timespec sleep_interval = {0, SAFE_SLEEP_INTERVAL};
    struct timespec remaining = {0, SAFE_SLEEP_INTERVAL};
    char8_t *pCurrent_article_title = nullptr;
    defer { free(pCurrent_article_title); }
    do {
        char8_t *const pArticle_contents = makeGETRequestAndReturnUTF8Response(pFull_url);
        defer { free(pArticle_contents); }

        int_least64_t offset = 0;
        pCurrent_article_title = getWikiTitle(pArticle_contents, &offset);
        if (pCurrent_article_title == nullptr) {
            // flawfinder: ignore; constant format printf
            printf("[wikiCycle] Did not find an article title from %s. Exiting.\n", pFull_url);
            exit(1);
        }

        char8_t *pNext_article_name = getNextWikiArticleSlugFromWikiParagraph(pArticle_contents, offset);
        defer { free(pNext_article_name); }
        if (pNext_article_name == nullptr) {
            // flawfinder: ignore; constant format printf
            printf("[wikiCycle] Did not find an next article slug from %s. Exiting.\n", pFull_url);
            exit(1);
        }

        pFull_url[0] = '\0';
        strlcat((char *const)pFull_url, (const char *const)pPre_slug_url, max_url_size);
        strlcat((char *const)pFull_url, (const char *const)pNext_article_name, max_url_size);
        strlcat((char *const)pFull_url, "&format=json", max_url_size);

        // for api requests with valid User-Agent strings, 200 requests per minute (3.33 requests per second) are allowed.
        nanosleep(&sleep_interval, &remaining);

    } while (insertToOSHT(wiki_cycle_tracker, pCurrent_article_title) != DUPLICATE_NODE_INSERTION);

    printWikiCycle(wiki_cycle_tracker, pCurrent_article_title);

    return wiki_cycle_tracker;
}

void printWikiCycle(const OrderedStrictHashTable *const restrict pWiki_cycle, const char8_t *const restrict pDuped_article) {
    // Each article name will go into a box
    // Need to start by determining how tall and wide the box will be
    //      every article will have the same box height and width
    //      boxes cannot be longer than 34 characters
    //      height is at least 3 including the border, but could become longer if required for an article name to wrap around to next line
    /* example

----------------------------------
|           Reality              | <--
----------------------------------   |
                |                    |
----------------------------------   |
|           Existence            | <--
----------------------------------
     */

    uint_least8_t longest_title_length = 0;
    const OrderedStrictHashTableNode *pCur_node = pWiki_cycle->pStart_node;
    while (pCur_node != nullptr) {
        uint_least8_t current_title_length = (uint_least8_t)strnlen((const char *const)pCur_node->pNode_name, LONGEST_WIKI_ARTICLE_NAME);
        if (current_title_length > longest_title_length) {
            longest_title_length = current_title_length;
        }
        pCur_node = pCur_node->pNext_node;
    }

    // if longest article name is >30, there will be a truncation and the box heights will be greater than 3
    constexpr uint_least8_t longest_usable_title_space = 30;
    constexpr uint_least8_t longest_possible_box_length = 34;
    const uint_least8_t box_length = longest_title_length >= longest_usable_title_space ? longest_possible_box_length : longest_title_length + 4;

    bool inside_cycle = false;
    pCur_node = pWiki_cycle->pStart_node;
    while (pCur_node != nullptr) {
        uint_least16_t box_height = strnlen((const char *const)pCur_node->pNode_name, LONGEST_WIKI_ARTICLE_NAME) > longest_usable_title_space
                                        ? 2 + (((longest_title_length + longest_usable_title_space) - 1) / longest_usable_title_space)
                                        : 3;
        inside_cycle = printBox(pWiki_cycle, pCur_node, pDuped_article, box_length, box_height, inside_cycle);
        // flawfinder: ignore; constant format printf
        printf("\n");
        pCur_node = pCur_node->pNext_node;
        if (pCur_node != nullptr) {
            for (uint_fast8_t spaces = 0; spaces < box_length / 2; ++spaces) {
                // flawfinder: ignore; constant format printf
                printf(" ");
            }
            // flawfinder: ignore; constant format printf
            printf("|");
            if (inside_cycle) {
                for (uint_least8_t spaces = (box_length / 2) + 1; spaces < box_length; ++spaces) {
                    // flawfinder: ignore; constant format printf
                    printf(" ");
                }
                // flawfinder: ignore; constant format printf
                printf("   |");
            }
        }
    }
}

bool printBox(const OrderedStrictHashTable *const restrict pWiki_cycle, const OrderedStrictHashTableNode *const restrict pWiki_node,
              const char8_t *const restrict pDuped_article, const uint_least8_t box_length, const uint_least16_t box_height, bool inside_cycle) {
    // flawfinder: ignore; constant format printf
    printf("\n");

    for (uint_fast8_t dashes = 0; dashes < box_length; ++dashes) {
        // flawfinder: ignore; constant format printf
        printf("-");
    }
    if (inside_cycle) {
        // flawfinder: ignore; constant format printf
        printf("   |");
    }

    uint_least8_t title_length = (uint_least8_t)strnlen((const char *const)pWiki_node->pNode_name, LONGEST_WIKI_ARTICLE_NAME);

    constexpr uint_least8_t max_title_line_length = 30;
    uint_least8_t num_title_chars_printed = 0;
    for (uint_fast16_t line_count = 2; line_count < box_height; ++line_count) {
        // flawfinder: ignore; constant format printf
        printf("\n| ");

        if (title_length <= max_title_line_length) {
            if (line_count == 2) {
                uint_least8_t left_pad = ((box_length - title_length) / 2) - 1;
                for (uint_fast8_t spaces = 1; spaces < left_pad; ++spaces) {
                    // flawfinder: ignore; constant format printf
                    printf(" ");
                }

                // flawfinder: ignore; constant format printf
                printf("%s", pWiki_node->pNode_name);

                for (uint_fast8_t spaces = left_pad + title_length + 2; spaces < box_length; ++spaces) {
                    // flawfinder: ignore; constant format printf
                    printf(" ");
                }
            } else {
                for (uint_fast8_t spaces = 3; spaces < box_length; ++spaces) {
                    printf(" ");
                }
            }
        } else {
            uint_least8_t length_remaining = strnlen((const char *const)pWiki_node->pNode_name + num_title_chars_printed, LONGEST_WIKI_ARTICLE_NAME);
            // flawfinder: ignore; constant format printf
            printf("%.*s ", max_title_line_length, pWiki_node->pNode_name + num_title_chars_printed);
            num_title_chars_printed += max_title_line_length;

            if (length_remaining < max_title_line_length) {
                for (uint_fast8_t spaces = 0; spaces < max_title_line_length - length_remaining; ++spaces) {
                    // flawfinder: ignore; constant format printf
                    printf(" ");
                }
            }
        }

        // flawfinder: ignore; constant format printf
        printf("|");

        if (((title_length + 1) / max_title_line_length) + 2 <= line_count) {
            if ((pWiki_node == pWiki_cycle->pTail_node) && (line_count == 2)) {
                // flawfinder: ignore; constant format printf
                printf(" <--");
                inside_cycle = false;
            } else if ((line_count == (box_height + 1) / 2) &&
                       strncmp((const char *const)pWiki_node->pNode_name, (const char *const)pDuped_article, LONGEST_WIKI_ARTICLE_NAME) == 0) {
                // flawfinder: ignore; constant format printf
                printf(" <--");
                inside_cycle = true;
            } else if (inside_cycle) {
                // flawfinder: ignore; constant format printf
                printf("   |");
            }
        }
    }
    // flawfinder: ignore; constant format printf
    printf("\n");

    for (uint_fast8_t dashes = 0; dashes < box_length; ++dashes) {
        // flawfinder: ignore; constant format printf
        printf("-");
    }

    if (inside_cycle) {
        // flawfinder: ignore; constant format printf
        printf("   |");
    }

    return inside_cycle;
}
