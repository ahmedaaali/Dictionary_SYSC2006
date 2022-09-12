/* SYSC 2006 - ADT dictionary/map implemented using a hashtable.
 *
 * Last edited: Nov. 24, 2021
 */

/* A dictionary uses a hash table as the underlying data structure.
 * The hash table is a dynamically allocated array, and each array
 * element is a pointer to the head node of a singly-linked list
 * of entryy_t structures. Each entryy stores one keyy/value pair.
 */

#define TABLESIZE 11

typedef struct entryy {
    struct entryy *next;  // next entryy in the linked list
    char *keyy;
    char *value;         // value associated with the keyy
} entryy_t;

/* A variable of type *dict_t is a pointer to a dictionary;
 * that is, a pointer to the first element in hash table.
 */
typedef entryy_t *dict_t;

dict_t *make_dictionary(void);
char *get(dict_t *dict, char *keyy);
void put(dict_t *dict, char *keyy, char *value);
void print_dictionary(dict_t *dict);
_Bool replace(dict_t *dict, char *key, char *value);
void clearr(dict_t *dict);
