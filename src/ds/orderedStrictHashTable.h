// Copyright [2026] Ethan Heckart
#ifndef WIKICYCLE_SRC_DS_ORDEREDSTRICTHASHTABLE_H_
#define WIKICYCLE_SRC_DS_ORDEREDSTRICTHASHTABLE_H_

#include "returnCodes.h"
#include <stdint.h>
#include <uchar.h>

typedef struct OrderedStrictHashTableNode {
    char8_t *pNode_name;
    struct OrderedStrictHashTableNode *pNext_node;
} OrderedStrictHashTableNode;

/*
 * A domain-specific, ordered, data structure with O(1) de-duped insertions and O(1) random node access.
 * ORDERED because the insertion order of elements its tracked. STRICT because duplicate inserts do nothing and crashes on collisions.
 * Under the hood, this is essentially a linked list living inside a hash table/set.
 * Available functions are createOSHT(), destoryOSHT(), hashExistsInOSHT(), insertToOSHT().
 */
typedef struct OrderedStrictHashTable {
    struct OrderedStrictHashTableNode *pStart_node;
    struct OrderedStrictHashTableNode *pTail_node;
    uint_least32_t bucket_size;
    OrderedStrictHashTableNode *pHash_table[];
} OrderedStrictHashTable;

// Purpose:    Generate a bounded hash value based on given input
// Parameters: A char8_t* to generate a hash for, an OrderedStrictHashTable* for the bounds to respect.
// Returns:    A uint_least32_t hash for the passed char8_t*, that falls within the bucket size of the passed table*.
// Exits:      No.
// Modifies:   Nothing.
// Tests:      None.
// Status:     Done.
uint_least32_t calcHash(const char8_t *const restrict pNode_name, const OrderedStrictHashTable *const restrict pTable);

// Purpose:    Instantiate a new OrderedStrictHashTable of user defined hash table size.
// Parameters: A uint_least32_t representing the node count of the hash table.
// Returns:    An OrderedStrictHashTable* pointing to the newly defined table.
// Exits:      Exit(8) if bucket_size is <= 0. Exit(9) if malloc of the OSHT pointer fails.
// Modifies:   Nothing existing. On AMD64, allocates (24 + 8*bucket_size) bytes on the heap.
// Tests:      tests/unit
// Status:     Done.
OrderedStrictHashTable *createOSHT(const uint_least32_t bucket_size);

// Purpose:    Destroys an OrderedStrictHashTable.
// Parameters: An OrderedStrictHashTable* pointing to the OSHT to be destroyed.
// Returns:    Nothing.
// Exits:      No.
// Modifies:   Frees all memory pointed to by the passed OSHT. On AMD64 deallocatets (24 + 8*bucket_size + (16*total occupied buckets + total
//             node_name UTF-8 bytes))) bytes from the heap.
// Tests:      tests/unit
// Status:     Done.
void destroyOSHT(OrderedStrictHashTable *const restrict pTable);

// Purpose:    Checker for whether or not a given hash value has its corresponding OSHT node occupied.
// Parameters: An OrderedStrictHashTable* pointing to the OSHT being checked. A uint_least32_t of the hash to be
//             checked.
// Returns:    True if the node hash is occupied, else false.
// Exits:      No.
// Modifies:   Nothing.
// Tests:      tests/unit
// Status:     Done.
static inline bool hashExistsInOSHT(const OrderedStrictHashTable *const restrict pTable, const uint_least32_t hash) {
    return pTable->pHash_table[hash] != nullptr;
}

// Purpose:    Inserts a value into an existing OSHT and points the OSHT's tail_node to the inserted value. If it's the OSHT's first node, also
//             updates the parent value. If there's a detected duplicate value or collision, does nothing.
// Parameters: An OrderedStrictHashTable* pointing to the OSHT being inserted to. A char8_t* contianing the value to be inserted.
// Returns:    0 if insertion is successful, 1 if a duplicate node insertion was attempted.
// Exits:      Exit(2) if there is a collision, Exit(9) if malloc of the OSHTNode fails.
// Modifies:   The OSHT gains a new node. On AMD64 allocates (16 + node_name's UTF-8 bytes) bytes to the heap.
// Tests:      tests/unit
// Status:     Improve collision print message once applicable.
enum insertionRC insertToOSHT(OrderedStrictHashTable *const restrict pTable, const char8_t *const restrict pNode_name);

#endif // WIKICYCLE_SRC_DS_ORDEREDSTRICTHASHTABLE_H_
