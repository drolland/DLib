#pragma once

typedef struct _d_slist{
    void* content;
    struct _d_slist* next;
} DSList;

/* Append data at the end of the list.
 * Return the new start of the list ( Be sure to use it ! )
 */
DSList* d_slist_append(DSList* list,void* to_insert);

/* Prepend data at the start of the list.
 * Return the new start of the list ( Be sure to use it ! )
 */
DSList* d_slist_prepend(DSList* list,void* to_insert);

/* Free a list but not its content
 */
void d_slist_free(DSList* list);

/* Free a list but not its content
 */
void d_slist_free_full(DSList* list,void (*DFreeFunc)(void*));

/* Search for content match in a list and return the element list containing it or NULL if no match*/
DSList* d_slist_search(DSList* list,void* to_search);








