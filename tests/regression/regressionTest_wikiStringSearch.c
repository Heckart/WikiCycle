#include "../include/regressionTest_wikiStringSearch.h"
#include "../../src/include/wikiStringSearch.h"
#include "../include/read_file_to_string.h"
#include <assert.h>
#include <stddefer.h>
#include <stdint.h>
#include <stdlib.h>

void test_getIndexOfFirstWikiParagraph_pTagInTable(void) {
    /* Fixed on 11/26/26: Wikipedia articles that have tables on the side were incorrectly taking the fist link from the table instead of the first
     * link in the first paragraph. Added a contraint to getIndexOfFirstWikiParagraph to ensure table contents are ignored.
     */
    const char8_t *const pTest_string =
        u8"Tags that are inside a <table> like this one <p> shouldn't count </p>, but outside the </table> like this one <p> should count </p>";
    assert(getIndexOfFirstWikiParagraph(pTest_string, 0) == 113);

    constexpr uint_least64_t fake_global_index = 58;
    char8_t *const pTest_article = read_file_to_string(u8"tests/test_infrastructure/United_States_House_of_Representatives.json");
    defer { free(pTest_article); }
    assert(getIndexOfFirstWikiParagraph(pTest_article, fake_global_index) == 119060);
}
