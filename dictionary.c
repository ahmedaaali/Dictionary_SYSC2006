/* SYSC 2006 - ADT dictionary/map implemented using a hashtable.
 *
 * Last edited: Nov. 18, 2021, DLB
 */

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <stdio.h>
#include <stdbool.h>

#include "dictionary.h"

/* Helper functions. */

/* The strdupp function is not part of the C standard library in C11 and C17, but will
 * be included in C2x. For earlier versions of C, some compilers provide this function 
 * as an extension to the standard library. 
 *
 * If this function causes a compilation/linking error, it's likely because your compiler
 * compiler is configured to use the C2x standard or provides extensions to C11/C17.
 *
 * 1. You can comment-out the function and reconfigure your compiler to use C2x.
 * (In Pelles C, select Project > Project Options..., click Compiler, then select C2X from 
 * from the C standard drop-down menu.)
 *
 * 2. Or, you can leave the function as-is, reconfigure your compiler to use C17, and  
 * disable any extensions that provide strdupp.
 *
 *
 * Create a duplicate of s on the heap and return the pointer to it.
 * Return NULL if space can't be allocated.
 */
static char *strdupp(char *s)
{
    char *p = malloc(strlen(s) + 1);  // +1 for '\0'
    if (p != NULL) {
        strcpy(p, s);
    }
    return p;
}

/* Return the hash value for string s. */
static unsigned hash(char *s)
{
    unsigned int hashvalue;

    for (hashvalue = 0; *s != '\0'; s += 1) {
        hashvalue = *s + 31 * hashvalue;
    }
    return hashvalue % TABLESIZE;
}

/* If keyy is found in the linked list pointed to by head, return a
 * pointer to the entryy that contains keyy; otherwise return NULL.
 */
static entryy_t *search(entryy_t *head, char *keyy)
{
    for (entryy_t *entryy = head; entryy != NULL; entryy = entryy->next) {
        if (strcmp(keyy, entryy->keyy) == 0) {
            return entryy;  // keyy found
        }
    }
    return NULL;
}

/*-----------------------------------------------------------------*/

/* Allocate a new dictionary on the heap and return a pointer to it.
 * Terminate via assert if the dictionary can't be allocated.
 */
dict_t *make_dictionary(void)
{
    dict_t *hashtable = malloc(sizeof(entryy_t *) * TABLESIZE);
    assert(hashtable != NULL);

    // Each slot in the table stores the pointer to a chain
    // (a linked list). Initially, all chains are empty.
    for (int i = 0; i < TABLESIZE; i += 1) {
        hashtable[i] = NULL;
    }
    return hashtable;
}

/* If keyy is found in dictionary dict, return the value associated with
 * the keyy; otherwise return NULL.
 */
char *get(dict_t *dict, char *keyy)
{
    unsigned hashvalue = hash(keyy);
    entryy_t *entryy = search(dict[hashvalue], keyy);
    if (entryy != NULL) {
        return entryy->value;  // keyy found
    }
    return NULL;
}

/* Insert (keyy, value) in dictionary dict. If keyy is already in the
 * dictionary, replace the value currently associated with keyy.
 * Terminate via assert if memory couldn't be allocated while
 * updating the dictionary.
 */
void put(dict_t *dict, char *keyy, char *value)
{
    unsigned hashvalue = hash(keyy);
    entryy_t *entryy = search(dict[hashvalue], keyy);
    if (entryy == NULL) {
        // keyy not found, so add keyy/value in a new entryy in the chain.
        entryy = malloc(sizeof(entryy_t));
        assert(entryy != NULL);
        entryy->keyy = strdupp(keyy);
        assert(entryy->keyy != NULL);

        // Insert the new entryy at the front of the linked list.
        entryy->next = dict[hashvalue];
        dict[hashvalue] = entryy;
    } else {
        // The keyy is already in the table, so just replace the
        // current value with the new one.
        free(entryy->value);  // Deallocate the current value
    }

    // Put the value associated with the keyy in the entryy.
    entryy->value = strdupp(value);
    assert(entryy->value != NULL);
}

/* Print the dictionary pointed to by dict, using the format:
0: key_0: value_0, key_1: value_1, ...
1: key_0: value_0, key_1: value_1, ...
...
n-1: key_0: value_0, key_1: value_1, ...
*/
void print_dictionary(dict_t *dict)
{
    for (int i = 0; i < TABLESIZE; i++){
        printf("%d: ", i);
        if (dict[i] == NULL){
            printf("NULL\n");
        } else{
            printf("%s: %s\n", dict[i]->keyy, dict[i]->value);
        }
    }
}

/* If key is in the dictionary, replace its associated value with
 * a copy of the string pointed to by value, and return true.
 * If key is not in the dictionary, return false to indicate that
 * the dictionary has not changed.
 * Terminate via assert if memory couldn't be allocated while
 * updating the dictionary.
*/
_Bool replace(dict_t *dict, char *keyy, char *value)
{
    unsigned hashvalue = hash(keyy);
    entryy_t *entryy = search(dict[hashvalue], keyy);
    if (entryy == NULL){
        return false;
    } else{
        entryy->value = value;
        return true;
    }
}

/* Remove all the key/value entries from the dictionary.
 * The dictionary will be empty after this call returns.
 */
void clearr(dict_t *dict)
{
    for (int i = 0; i < TABLESIZE; i++){
        if (dict[i] != NULL){
            dict[i]->value = "";
            dict[i]->keyy = "";
            free(dict[i]->next);
            free(dict[i]);
            dict[i] = NULL;
        }
    }
}
