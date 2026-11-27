// Copyright [2026] Ethan Heckart
#ifndef WIKICYCLE_SRC_INCLUDE_WIKICYCLE_H_
#define WIKICYCLE_SRC_INCLUDE_WIKICYCLE_H_

#include "orderedStrictHashTable.h"
#include <stdint.h>
#include <uchar.h>

// Purpose:    Given a starting article, walk down its chain of first linked articles until a cycle is found.
// Parameters: A uint_least32_t for the bucket size of the OSHT, a char8_t* with the wiki article name to find the cycle of, a char8_t* with the URL
//             contents before the article name.
// Returns:    An OrderedStrictHashTable* pointing to an OSHT that contains the full cycle that was found.
// Exits:      Exit(1) if a current article title or next article slug couldn't be found in the URL. Exit(9) if malloc of the URL pointer fails.
// Modifies:   Nothing existing. On AMD64, allocates 322 bytes on the heap for the URL pointer + (24 + 8*bucket_size) bytes on the heap for
//             construction of the OSHT + (16 + node_name's UTF-8 bytes) bytes on the heap for each inserted article URL. For each URL allocates and
//             deallocates (1 + the captured URL slug's UTF-8 bytes) bytes on the heap.
// Tests:      Not implemented yet.
// Status:     Done.
OrderedStrictHashTable *findWikiCycle(uint_least32_t bucket_size, const char8_t *const restrict pArticle_name,
                                      const char8_t *const restrict pPre_slug_url);

// Purpose:    Given an OSHT, print a representation of the node order to the terminal.
// Parameters: An OrderedStrictHashTable* containing the wiki cycle, a char8_t* with the name of the duplicated article.
// Returns:    Nothing.
// Exits:      No.
// Modifies:   Nothing
// Tests:      Not implemented yet.
// Status:     Done.
void printWikiCycle(const OrderedStrictHashTable *const restrict pWiki_cycle, const char8_t *const restrict pDuped_article);

// Purpose:    Print a box to the terminal for an individual article. A max width is enforced with text wrapping for long names. Non-wrapped names are
//             centered in the box.
// Parameters: An OrderedStrictHashTable* containing the wiki cycle, an OrderedStrictHashTableNode* with the article to be printed, a char8_t* with
//             the name of the duplicated article, a uint_least8_t with the total length of the box to be printed, a uint_least16_t with the height of
//             the box to be printed, a bool representing whether or not the article being printed is inside the cycle.
// Returns:    true if the node being processed is inside the cyclic nodes, else false (used for the cyclic arrows to the side of the boxes).
// Exits:      No.
// Modifies:   Nothing
// Tests:      Not implemented yet.
// Status:     Done.
bool printBox(const OrderedStrictHashTable *const restrict pWiki_cycle, const OrderedStrictHashTableNode *const restrict pWiki_node,
              const char8_t *const restrict pDuped_article, const uint_least8_t box_length, const uint_least16_t box_height, const bool inside_cycle);

// Purpose:    After the box itslef has been printed, determine if the box is inside the cycle and print the cycle arrow if necessary. This is
//             separate from printBox to simplify it a bit.
// Parameters: uint_least8_ts for the length of the current article title and max length, a uint_fast16t for the current line count of the box being
//             printed, an OrderedStrictHashTableNode* of the article being printed, an OrderedStrictHashTable* of the wiki cycle being printed, a
//             uint_least16_t with the height of the box in question, a char8_t* with the name of the duplicated article, and a bool for whether or
//             not the article has already been determined to be inside the cycle.
// Returns:    true if the node being processed is inside the cyclic nodes, else false (used for the cyclic arrows to the side of the boxes).
// Exits:      No.
// Modifies:   Nothing
// Tests:      Not implemented yet.
// Status:     Done.
bool handleCyclePrinting(const uint_least8_t title_length, const uint_least8_t max_title_line_length, const uint_fast16_t line_count,
                         const OrderedStrictHashTableNode *const restrict pWiki_node, const OrderedStrictHashTable *const restrict pWiki_cycle,
                         const uint_least16_t box_height, const char8_t *const restrict pDuped_article, const bool inside_cycle);

#endif // WIKICYCLE_SRC_INCLUDE_WIKICYCLE_H_
