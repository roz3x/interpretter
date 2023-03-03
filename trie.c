#include "trie.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* we assign 0xFF values for
each level of the trie */
struct trie { 
    struct trie *next;
    void *data; 
}  T; 

void init_trie() {
    T.next = NULL; 
}


void _insert(char* key , void* value , struct trie *t , int len) { 
    if (len == 0 ) { 
        t->data = value; 
        return ; 
    }
    assert(t != NULL);
    if (t->next == NULL) { 
        t->next = (struct trie*) malloc(0xff * sizeof(struct trie));
        for (int i = 0 ;i < 0xFF; i++) { 
            t->next[i].next = t->next[i].data = NULL;
        }
    }
    return _insert(key + 1 , value , &(t->next[key[0]]) , len - 1 );   
}

void insert(char* key  , void* value) { 
    return _insert(key , value ,&T , strlen(key));
}

void* _get(char* key , struct trie* t , int len ) { 
    if (len == 0) { 
        return t->data; 
    }
    if (t->next == NULL) return NULL; 
    return _get(key + 1 , &(t->next[key[0]]), len  -1 ); 
}
void* get(char *key ) { 
    return _get(key , &T , strlen(key));
}
