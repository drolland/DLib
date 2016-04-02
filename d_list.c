
#include "d_memory.h"
#include "d_list.h"

DSList* d_slist_append(DSList* list,void* to_insert){
        
    DSList* new_element = (DSList*)d_malloc(sizeof(DSList));
    new_element->content = to_insert;
    new_element->next = NULL;
    
    if ( list == NULL){
        return new_element;
    }
    
    DSList* current = list;
    while(current->next != NULL)
        current = current->next;
    
    current->next = new_element;
    return list;
       
}

DSList* d_slist_prepend(DSList* list,void* to_insert){
           
    DSList* new_element = (DSList*)d_malloc(sizeof(DSList));
    new_element->content = to_insert;
    
    if ( list == NULL)
        new_element->next = NULL;    
    else 
        new_element->next = list;
    
    return new_element;
    
}

void d_slist_free(DSList* list){
    
    DSList* iter = list;
    while( iter != NULL){
        DSList* next = iter->next;
        free(iter);
        iter = next;
    }    
}


DSList* d_slist_search(DSList* list,void* to_search){
       
    DSList* iter = list;
    
    while ( iter != NULL){
        if ( iter->content == to_search)
            return iter;
        iter = iter->next;
    }
    return NULL;
}