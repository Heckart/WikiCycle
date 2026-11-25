// Copyright [2026] Ethan Heckart
#ifndef WIKICYCLE_SRC_INCLUDE_WIKICYCLE_H_
#define WIKICYCLE_SRC_INCLUDE_WIKICYCLE_H_

#include "orderedStrictHashTable.h"
#include <stdint.h>
#include <uchar.h>

// Parameters: Given a starting article, walk down its chain of first linked articles until a cycle is found.
// Returns:    An OrderedStrictHashTable* pointing to an OSHT that contains the full cycle that was found.
// Exits:      Exit(1) if a current article title or next article slug couldn't be found in the URL. Exit(9) if malloc of the URL pointer fails.
// Modifies:   Nothing existing. On AMD64, allocates 322 bytes on the heap for the URL pointer + (24 + 8*bucket_size) bytes on the heap for
//             construction of the OSHT + (16 + node_name's UTF-8 bytes) bytes on the heap for each inserted article URL. For each URL allocates and
//             deallocates (1 + the captured URL slug's UTF-8 bytes) bytes on the heap.
// Tests:      Not implemented yet.
// Status:     Done.
OrderedStrictHashTable *findWikiCycle(uint_least32_t bucket_size, const char8_t *const restrict pArticle_name,
                                      const char8_t *const restrict pPre_slug_url);

// Parameters: Given an OSHT, print a representation of the node order to the terminal.
// Returns:    Nothing.
// Exits:      No.
// Modifies:   Nothing
// Tests:      Not implemented yet.
// Status:     Done.
void printWikiCycle(const OrderedStrictHashTable *const restrict pWiki_cycle, const char8_t *const restrict pDuped_article);

// Parameters: Print a box to the terminal for an individual article. A max width is enforced with text wrapping for long names. Non-wrapped names are
//             centered in the box.
// Returns:    true if the node being processed is inside the cyclic nodes, else false (used for the cyclic arrows to the side of the boxes).
// Exits:      No.
// Modifies:   Nothing
// Tests:      Not implemented yet.
// Status:     Done.
bool printBox(const OrderedStrictHashTable *const restrict pWiki_cycle, const OrderedStrictHashTableNode *const restrict pWiki_node,
              const char8_t *const restrict pDuped_article, const uint_least8_t box_length, const uint_least16_t box_height, const bool inside_cycle);

#endif // WIKICYCLE_SRC_INCLUDE_WIKICYCLE_H_
