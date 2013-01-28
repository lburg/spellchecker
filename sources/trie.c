#include <stdio.h>
#include <stdlib.h>
#include "trie.h"

trie_t  *new_node(const char key)
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

//TO DO: Organize alphabetically to improve search
//(no need to keep looking after "b" if we are looking for "a")
bool  add_string(const char *string, trie_t *trie)
{
    trie_t  *child = trie->child;
    trie_t  *last = NULL;

    while (child != NULL)
    {
        if (child->key == string[0])
        {
            if (string[0] == '\0')
                return false;//failed to insert, string already exists in trie
            return add_string(string + 1, child);
        }
        else if (child->key > string[0])//the current key is passed the current letter (alphabetically)
            break;
        last = child;
        child = child->sibling;
    }
    trie_t  *node = new_node(string[0]);
    if (last == NULL)//the key must be inserted before others
    {
        trie->child = node;
        node->sibling = child;
    }
    else
    {
        /*
         ** Inserts new node in sibling list, keeping
         ** the alphabetical order. The node will be inserted
         ** either at the end of the list, or at the
         ** point where the previous loop called `break`.
         */
        node->sibling = last->sibling;
        last->sibling = node;
    }
    /*
    ** We just need to add all the remaining letters since
    ** we got on a new subtrie.
    */
    string++;
    while (string[0] != '\0')
    {
        node->child = new_node(string[0]);
        node = node->child;
        string++;
    }
    /* Last node to specify end of word */
    node->child = new_node(trie_delimiter);
    return true;
}

bool  is_in_trie(const char *string, const trie_t *trie)
{
    const trie_t  *child = trie->child;

    while (child != NULL)
    {
        if (child->key == string[0])
        {
            if (string[0] == '\0')//if we just looked at the trie_delimiter
                return true;//we found the string
            return is_in_trie(string + 1, child);
        }
        else if (child->key > string[0])//we passed the current letter, impossible to find the word
            return false;
        child = child->sibling;
    }
    return false;
}
