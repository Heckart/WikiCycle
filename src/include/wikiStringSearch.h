// Copyright [2026] Ethan Heckart
#ifndef WIKICYCLE_SRC_INCLUDE_WIKISTRINGSEARCH_H_
#define WIKICYCLE_SRC_INCLUDE_WIKISTRINGSEARCH_H_
#include <stdint.h>
#include <uchar.h>

// Purpose:    Determine if a given null-terminated string has required additional length.
// Parameters: A char8_t* to inspect the length of, a int_fast64_t with the offset to start searching from, a uint_fast8_t with the required length.
// Returns:    true if the needed length exists, false if it doesn't.
// Exits:      No.
// Modifies:   Nothing.
// Tests:      tests/unit.
// Status:     Done.
bool stringHasNAdditionalLength(const char8_t *const restrict pString, const int_fast64_t offset, const uint_fast8_t needed_length);

// Purpose:    Extract a Wikipedia article title from a wiki api request's null-terminated response and update an offset tracking how much of the
//             string has been inspected.
// Parameters: A char8_t* with the wiki api response to inspect, an int_least64_t* representing how much of the string has been inspected before the
//             function is called (usually 0) title is found.
// Returns:    A null-terminated char8_t* with the article title if one is found, else nullptr.
// Exits:      No.
// Modifies:   Updates pGlobal_offset. On AMD64, allocates (1 + pWiki_article's title's UTF-8 bytes) bytes to the heap.
// Tests:      tests/unit.
// Status:     Done.
char8_t *getWikiTitle(const char8_t *const restrict pWiki_article, int_least64_t *const restrict pGlobal_offset);

// Purpose:    Helper function to find the index after the first <p> tag in a null-terminated wiki api request response.
// Parameters: A char8_t* with the wiki api response to inspect, a int_least64_t* representing how much of the string has been inspected before the
//             function is called.
// Returns:    An int_least64_t with the index after the first <p> tag, else -1
// Exits:      No.
// Modifies:   Nothing.
// Tests:      tests/unit.
// Status:     Done.
int_least64_t getIndexOfFirstWikiParagraph(const char8_t *const restrict pWiki_article, const int_least64_t global_offset);

// Purpose:    Helper function to maintain paranthesis, square bracket, and curly bracket counts. Opening means adding to the count, closing means
//             subtracting from the count.
// Parameters: A char8_t with the character to be inspected, three uint_fast_8s representing the symbol counts.
// Returns:    true if the passed char8_t is a paranthesis, square bracker, or curly bracker, else false.
// Exits:      No.
// Modifies:   The appropriate symbol counting variable will be incremented if the char8_t is a left/opening version and decremented if it's a
//             right/closing version.
// Tests:      tests/unit.
// Status:     Done.
bool maintainPunctuationCounts(const char8_t wiki_char, uint_fast8_t *const restrict pParanthesis_count,
                               uint_fast8_t *const restrict pSquare_bracket_count, uint_fast8_t *const restrict pCurly_bracket_count);

// Purpose:    Extract the URL slug of the first linked wikipedia article that is not inside parenthesis, square brackets, or curly brackets from a
//             wiki api response.
// Parameters: A char8_t* with the wiki api response to inspect, an int_least64_t with the offset from which to start searching (should be the index
//             after the article title).
// Returns:    A null-terminated char8_t* with the URL slug of the next wiki article if found, else nullptr.
// Exits:      No.
// Modifies:   On AMD64, allocates (1 + the captured URL slug's UTF-8 bytes) bytes to the heap.
// Tests:      tests/unit.
// Status:     Done.
char8_t *getNextWikiArticleSlugFromWikiParagraph(const char8_t *const restrict pWiki_article, const int_least64_t global_offset);

#endif // WIKICYCLE_SRC_INCLUDE_WIKISTRINGSEARCH_H_
