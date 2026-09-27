#include "../include/wikiStringSearch.h"
#include "../include/returnCodes.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <uchar.h>

bool stringHasNAdditionalLength(const char8_t *const restrict pString, const int_fast64_t offset, const uint_fast8_t needed_length) {
    for (uint_fast8_t index = 1; index <= needed_length; ++index) {
        if (pString[offset + index] == '\0') {
            return false;
        }
    }
    return true;
}

char8_t *getWikiTitle(const char8_t *const restrict pWiki_article, int_least64_t *const restrict pGlobal_offset) {
    // The article title is found immeditely after the first occurrence of the string title":" and continues until the next " character
    constexpr uint_least64_t title_json_length = 7;
    int_least64_t title_index = -1;

    for (int_fast64_t index = 0; pWiki_article[index] != '\0'; ++index) {
        if (pWiki_article[index] == 't') {
            if (!stringHasNAdditionalLength(pWiki_article, index, title_json_length)) {
                return nullptr;
            }
            if (strncmp((const char *const)pWiki_article + index + 1, "itle\":\"", title_json_length) == 0) {
                title_index = index + (int_least64_t)title_json_length + 1;
                break;
            }
        }
    }

    if (title_index == -1) {
        return nullptr;
    }

    uint_fast8_t article_title_length = 0;
    for (int_fast64_t index = title_index; pWiki_article[index] != '\"'; ++index) {
        if (pWiki_article[index] == '\0' || article_title_length > LONGEST_WIKI_ARTICLE_NAME) {
            return nullptr;
        }
        ++article_title_length;
    }

    *pGlobal_offset = title_index + article_title_length;
    char8_t *const pArticle_title = malloc(article_title_length + 1);
    // flawfinder: ignore; we know that everything passed to this function will be null-terminated
    strncpy((char *const)pArticle_title, (const char *const)pWiki_article + title_index, article_title_length);
    pArticle_title[article_title_length] = '\0';

    return pArticle_title;
}

int_least64_t getIndexOfFirstWikiParagraph(const char8_t *const restrict pWiki_article, const int_least64_t global_offset) {
    // The first article paragraph is found immeditely after the first occurrence of the string <p>
    constexpr int_fast64_t p_tag_length = 2;
    for (int_fast64_t index = global_offset; pWiki_article[index] != '\0'; ++index) {
        if (pWiki_article[index] == '<') {
            if (!stringHasNAdditionalLength(pWiki_article, index, p_tag_length)) {
                return -1;
            }
            if (strncmp((const char *const)pWiki_article + index + 1, "p>", p_tag_length) == 0) {
                return index + p_tag_length + 1;
            }
        }
    }
    return -1;
}

bool maintainPunctuationCounts(const char8_t wiki_char, uint_fast8_t *const restrict pParanthesis_count,
                               uint_fast8_t *const restrict pSquare_bracket_count, uint_fast8_t *const restrict pCurly_bracket_count) {
    switch (wiki_char) {
    default:
        return false;
    case '(':
        ++*pParanthesis_count;
        return true;
    case ')':
        --*pParanthesis_count;
        return true;
    case '[':
        ++*pSquare_bracket_count;
        return true;
    case ']':
        --*pSquare_bracket_count;
        return true;
    case '{':
        ++*pCurly_bracket_count;
        return true;
    case '}':
        --*pCurly_bracket_count;
        return true;
    }
}

char8_t *getNextWikiArticleSlugFromWikiParagraph(const char8_t *const restrict pWiki_article, const int_least64_t global_offset) {
    // The URL slug of the next wiki article is found inside a paragraph tag, immediately after the first occurrence of the string <a href=\"/wiki/
    // and continues until the next \ character. This function also ignores slugs that contained inside sets of (), {}, [].
    int_least64_t p_tag_offset = getIndexOfFirstWikiParagraph(pWiki_article, global_offset);

    if (p_tag_offset == -1) {
        return nullptr;
    }

    constexpr int_fast64_t a_href_length = 15;
    int_least64_t link_offset = -1;
    uint_least8_t live_parenthesis_present = 0;
    uint_least8_t live_square_bracket_present = 0;
    uint_least8_t live_curly_bracket_present = 0;
    // find first <a rel=" which is not within a set of parenthesis or brackets
    for (int_fast64_t index = p_tag_offset; pWiki_article[index] != '\0'; ++index) {
        if (maintainPunctuationCounts(pWiki_article[index], &live_parenthesis_present, &live_square_bracket_present, &live_curly_bracket_present)) {
            continue;
        }

        if (pWiki_article[index] == '<' && !live_parenthesis_present && !live_square_bracket_present && !live_curly_bracket_present) {
            if (!stringHasNAdditionalLength(pWiki_article, index, a_href_length)) {
                return nullptr;
            }
            if (strncmp((const char *const)pWiki_article + index + 1, "a href=\\\"/wiki/", a_href_length) == 0) {
                link_offset = index + a_href_length + 1;
                break;
            }
        }
    }

    if (link_offset == -1) {
        return nullptr;
    }

    uint_least32_t link_length = 0;
    for (int_fast64_t index = link_offset; pWiki_article[index] != '\\'; ++index) {
        if (pWiki_article[index] == '\0') {
            return nullptr;
        }
        ++link_length;
    }

    char8_t *const pNext_link = malloc(link_length + 1);
    // flawfinder: ignore; we know that everything passed to this function will be null-terminated
    strncpy((char *const)pNext_link, (const char *const)pWiki_article + link_offset, link_length);
    pNext_link[link_length] = '\0';

    return pNext_link;
}

/*
 * The below  versions of the functions would work if you make a curl request directly to the wiki article instead of using the wiki api
 *
char8_t *getWikiTitle(const char8_t *const restrict pWiki_article, int_least64_t *const restrict pGlobal_index) {
    constexpr int_fast64_t title_tag_length = 6;
    int_fast64_t title_index = -1;

    for (int_fast64_t index = 0; pWiki_article[index] != '\0'; ++index) {
        if (pWiki_article[index] == '<') {
            if (!stringHasNAdditionalLength(pWiki_article, index, title_tag_length)) {
                return nullptr;
            }
            if (strncmp((const char *const)pWiki_article + index + 1, "title>", title_tag_length) == 0) {
                title_index = index + title_tag_length + 1;
                break;
            }
        }
    }

    if (title_index == -1) {
        return nullptr;
    }

    uint_fast8_t article_title_length = 0;
    for (int_fast64_t index = title_index; pWiki_article[index] != '-'; ++index) {
        if (pWiki_article[index] == '\0' || article_title_length > LONGEST_WIKI_ARTICLE_NAME) {
            return nullptr;
        }
        ++article_title_length;
    }

    *pGlobal_index = title_index + article_title_length;
    char8_t *const pArticle_title = malloc(article_title_length + 1);
    strncpy((char *const)pArticle_title, (const char *const)pWiki_article + title_index, article_title_length);
    pArticle_title[article_title_length] = '\0';

    return pArticle_title;
}

int_least64_t getIndexOfFirstWikiParagraph(const char8_t *const restrict pWiki_article, const int_least64_t global_index) {
    constexpr int_fast64_t p_id_length = 4;
    for (int_fast64_t index = global_index; pWiki_article[index] != '\0'; ++index) {
        if (pWiki_article[index] == '<') {
            if (!stringHasNAdditionalLength(pWiki_article, index, p_id_length)) {
                return -1;
            }
            if (strncmp((const char *const)pWiki_article + index + 1, "p id", p_id_length) == 0) {
                return index + p_id_length + 1;
            }
        }
    }
    return -1;
}


char8_t *getNextWikiArticleLinkFromWikiParagraph(const char8_t *const restrict pWiki_article, const int_least64_t global_index) {
    constexpr int_fast64_t a_rel_length = 7;
    int_least64_t link_tag_offset = -1;
    uint_least8_t live_parenthesis_present = 0;
    uint_least8_t live_square_bracket_present = 0;
    uint_least8_t live_curly_bracket_present = 0;
    // find first <a rel=" which is not within a set of parenthesis or brackets
    for (int_fast64_t index = global_index; pWiki_article[index] != '\0'; ++index) {
        if (maintainPunctuationCounts(pWiki_article[index], &live_parenthesis_present, &live_square_bracket_present, &live_curly_bracket_present))
{ continue;
        }

        if (pWiki_article[index] == '<' && !live_parenthesis_present && !live_square_bracket_present && !live_curly_bracket_present) {
            if (!stringHasNAdditionalLength(pWiki_article, index, a_rel_length)) {
                return nullptr;
            }
            if (strncmp((const char *const)pWiki_article + index + 1, "a rel=\"", a_rel_length) == 0) {
                link_tag_offset = index + a_rel_length + 1;
                break;
            }
        }
    }

    if (link_tag_offset == -1) {
        return nullptr;
    }

    constexpr int_fast64_t ref_length = 5;
    int_least64_t link_offset = -1;
    // inside that <a rel=", the next link is contained within the quotes following the href="
    for (int_fast64_t index = link_tag_offset; pWiki_article[index] != '\0'; ++index) {
        if (pWiki_article[index] == 'h') {
            if (!stringHasNAdditionalLength(pWiki_article, index, ref_length)) {
                return nullptr;
            }
            if (strncmp((const char *const)pWiki_article + index + 1, "ref=\"", ref_length) == 0) {
                link_offset = index + ref_length + 1;
                break;
            }
        }
    }

    if (link_offset == -1) {
        return nullptr;
    }

    uint_least32_t link_length = 0;
    for (int_fast64_t index = link_offset; pWiki_article[index] != '"'; ++index) {
        if (pWiki_article[index] == '\0') {
            return nullptr;
        }
        ++link_length;
    }

    char8_t *const pNext_link = malloc(link_length + 1);
    strncpy((char *const)pNext_link, (const char *const)pWiki_article + link_offset, link_length);
    pNext_link[link_length] = '\0';

    return pNext_link;
}
*/
