#include <stdio.h>
#include <stdlib.h>
#include "trie.h"

/* Allocation */
trie_t  *trie_new_root(void)
{
    return trie_new_node('\0');
}

trie_t  *trie_new_node(const char key)
{
    trie_t  *node = malloc(sizeof(*node));

    if (node == NULL)
    {
        fprintf(stderr, "new_node failed: Running out of virtual memory.\n");
        return NULL;
    }
    node->key = key;
    node->child = NULL;
    node->sibling = NULL;
    return node;
}

/* Manipulation */
bool  trie_add_string(const char *string, trie_t *trie)
{
    trie_t  *child = trie->child;
    trie_t  *last = NULL;

    while (child != NULL)
    {
        //If the key is the current letter we are looking for..
        if (child->key == string[0])
        {
            if (string[0] == '\0')//..and its the end of the word..
                return false;//..then the word is already in the trie
            return trie_add_string(string + 1, child);//..else we keep going
        }
        //..or if the key is past the letter (looking for 'b' but already looking at 'f')
        else if (child->key > string[0])
            break;//..then we break, we need to insert the word here
        last = child;
        child = child->sibling;
    }
    trie_t  *node = trie_new_node(string[0]);
    //If we need to insert the word before others.. ('inn' goes before 'tea')
    if (last == NULL)
    {
        trie->child = node;
        node->sibling = child;
    }
    else//..else insert in between last and next node
    {
        node->sibling = last->sibling;
        last->sibling = node;
    }
    //Since we just created a new subtrie for our word,
    //we just need to insert all the letters from here.
    string++;//we did this during this loop
    while (string[0] != '\0')
    {
        node->child = trie_new_node(string[0]);
        node = node->child;
        string++;
    }
    /* Last node to specify end of word */
    node->child = trie_new_node(string[0]);
    return true;
}

void  delete_trie(trie_t *trie)
{
    trie_t  *tmp = NULL;

    while (trie != NULL)
    {
        delete_trie(trie->child);
        tmp = trie;
        trie = trie->sibling;
        free(tmp);
    }
}

/* Lookup */
bool  is_in_trie(const char *string, const trie_t *trie)
{
    const trie_t  *child = trie->child;

    while (child != NULL)
    {
        //If the key is the current letter we are looking for..
        if (child->key == string[0])
        {
            if (string[0] == '\0')//..and its the end of the word..
                return true;//..then we found the string
            return is_in_trie(string + 1, child);//..else we keep looking
        }
        //..or if the key is past the letter (looking for 'b' but already looking at 'f')
        else if (child->key > string[0])
            return false;//..then we cannot find this word anymore
        child = child->sibling;
    }
    return false;
}
