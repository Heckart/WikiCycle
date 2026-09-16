#define _POSIX_C_SOURCE 200809L
// longest wikipedia article (as of 2026/09/08) is 255 1-byte unicode characters.
// read: https://en.wikipedia.org/wiki/Wikipedia:Wikipedia_records#Articles_with_the_longest_titles
#define LONGEST_WIKI_ARTICLE_NAME 255
#include "../include/orderedStrictHashTable.h"
#include "../include/returnCodes.h"
#include <assert.h>
#include <stddef.h>
#include <stddefer.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uchar.h>

uint_least32_t calcHash(const char8_t *const restrict pNode_name, const OrderedStrictHashTable *const restrict pTable) {
    // string hashing algorithm taken from Algorithms, 4th Edition by Sedgewick and Wayne
    const uint_least32_t bucket_size = pTable->bucket_size;
    uint_least32_t hash = 0;
    constexpr uint_least32_t prime = 31;
    for (uint_fast32_t character = 0; character < strnlen((const char *const)pNode_name, LONGEST_WIKI_ARTICLE_NAME + 1); character++) {
        hash = (((prime * hash) + (uint_least32_t)pNode_name[character]) % bucket_size);
    }

    return hash;
}

OrderedStrictHashTable *createOSHT(const uint_least32_t bucket_size) {
    if (bucket_size == 0) {
        puts("We can't do anything without buckets. Ending now.");
        exit(EMPTY_BUCKET_RC);
    }

    const uint_fast32_t total_size = offsetof(OrderedStrictHashTable, pHash_table) + (bucket_size * sizeof(OrderedStrictHashTableNode *));
    // puts("Total allocated: %zu bytes\n", total_size);

    OrderedStrictHashTable *const pOSHT_struct = (OrderedStrictHashTable *)malloc(total_size);

    if (pOSHT_struct == nullptr) {
        perror("Allocation of table_struct in createOSHT failed. Program ending");
        exit(MALLOC_FAILURE_RC);
    }

    pOSHT_struct->pStart_node = nullptr;
    pOSHT_struct->pTail_node = nullptr;
    pOSHT_struct->bucket_size = bucket_size;
    for (uint_fast32_t bucket = 0; bucket < bucket_size; bucket++) {
        pOSHT_struct->pHash_table[bucket] = nullptr;
    }

    return pOSHT_struct;
}

void destroyOSHT(OrderedStrictHashTable *const restrict pTable) {
    defer { free(pTable); }

    [[clang::suppress]]
    // cppcheck-suppress deallocuse
    OrderedStrictHashTableNode *pCur_node = pTable->pStart_node;

    if (pCur_node != nullptr) {
        while (pCur_node->pNext_node != nullptr) {
            OrderedStrictHashTableNode *pPrev_node = pCur_node;
            pCur_node = pCur_node->pNext_node;
            free(pPrev_node->pNode_name);
            free(pPrev_node);
            pPrev_node = nullptr;
        }

        free(pCur_node->pNode_name);
        free(pCur_node);
        pCur_node = nullptr;

        pTable->pStart_node = nullptr;
        pTable->pTail_node = nullptr;
    }
}

enum insertionRC insertToOSHT(OrderedStrictHashTable *const restrict pTable, const char8_t *const restrict pNode_name) {
    const uint_least32_t node_hash = calcHash(pNode_name, pTable);
    assert(node_hash <= pTable->bucket_size);

    char8_t *const pOsht_node_name = (char8_t *const)strndup((const char *const)pNode_name, LONGEST_WIKI_ARTICLE_NAME + 1);
    if (pOsht_node_name == nullptr) {
        perror("Allocation of new node name in insertToOSHT failed. Program ending");
        exit(MALLOC_FAILURE_RC);
    }

    if (pTable->pStart_node == nullptr) {
        OrderedStrictHashTableNode *const pHead = (OrderedStrictHashTableNode *)malloc(sizeof(OrderedStrictHashTableNode));
        if (pHead == nullptr) {
            perror("Allocation of head node in insertToOSHT failed. Program ending");
            exit(MALLOC_FAILURE_RC);
        }

        pHead->pNode_name = pOsht_node_name;
        pHead->pNext_node = nullptr;

        pTable->pHash_table[node_hash] = pHead;
        pTable->pStart_node = pTable->pHash_table[node_hash];
        pTable->pTail_node = pTable->pHash_table[node_hash];

    } else {
        if (hashExistsInOSHT(pTable, node_hash)) {
            defer { free(pOsht_node_name); }
            [[clang::suppress]]
            if (strncmp((const char *const)pTable->pHash_table[node_hash]->pNode_name, (const char *const)pOsht_node_name,
                        LONGEST_WIKI_ARTICLE_NAME + 1) == 0) {
                // cppcheck-suppress deallocuse
                //  flawfinder: ignore
                printf("Attempted insert of duplicate value '%s', so doing nothing.\n", pOsht_node_name);
            } else {
                // TODO(ethan): improve message. Once possible suggest how to increase bucket size
                // cppcheck-suppress deallocuse
                //  flawfinder: ignore
                printf("Attempted insertion of value '%s' has a hash collision. WIP!\n", pOsht_node_name);
                exit(INSERTION_COLLISION);
            }
            return DUPLICATE_NODE_INSERTION;
        }
        OrderedStrictHashTableNode *const pNew_node = (OrderedStrictHashTableNode *)malloc(sizeof(OrderedStrictHashTableNode));
        if (pNew_node == nullptr) {
            perror("Allocation of new_node in insertToOSHT failed. Program ending");
            exit(MALLOC_FAILURE_RC);
        }

        pNew_node->pNode_name = pOsht_node_name;
        pNew_node->pNext_node = nullptr;

        pTable->pHash_table[node_hash] = pNew_node;
        pTable->pTail_node->pNext_node = pTable->pHash_table[node_hash];
        pTable->pTail_node = pTable->pHash_table[node_hash];

        return SUCCESSFUL_NODE_INSERTION;
    }
    return SUCCESSFUL_NODE_INSERTION;
}
