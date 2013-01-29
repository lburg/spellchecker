#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
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

static bool case_char_cmp(char c1, char c2)
{
    return strncasecmp(&c1, &c2, 1) == 0;
}

static bool vowel_char_cmp(char c1, char c2)
{
    /*
    ** Puts c1 and c2 in lower case.
    */
    if (c1 >= 'A' && c1 <= 'Z')
        c1 += ('a' - 'A');
    if (c2 >= 'A' && c2 <= 'Z')
        c2 += ('a' - 'A');

    if ((c1 == 'a' || c1 == 'e' || c1 == 'i' || c1 == 'o' || c1 == 'u' || c1 == 'y') &&
        (c2 == 'a' || c2 == 'e' || c2 == 'i' || c2 == 'o' || c2 == 'u' || c2 == 'y'))
        return true;
    return false;
}

/*
** Kind of nasty.. I don't like repeating the loop but I
** also don't like the idea of a sub-function recursively
** calling its caller...
*/
static char *rec_find_correction(const char *word, const size_t corrected_word_size,
                                 const trie_t *trie)
{
    const trie_t  *child = trie->child;

    /*
    ** First loop to see if we got the word as is.
    ** We don't want 'hell' to be corrected as 'Hell'
    */
    while (child != NULL)
    {
        if (child->key == word[0])
        {
            char  *corrected_word = NULL;
            if (word[0] == '\0')
            {
                //we found the whole word, so let's allocate some space to store it
                corrected_word = malloc((corrected_word_size + 1) *
                                        sizeof(*corrected_word));
            }
            else
            {
                //we didn't find the word yet, keep looking
                corrected_word = rec_find_correction(word + 1,
                                                     corrected_word_size + 1,
                                                     child);
            }
            //if the word was found/allocated..
            if (corrected_word != NULL)
            {
                corrected_word[corrected_word_size] = child->key;//..we build it
                return corrected_word;//the word is built as we go back the call stack
            }
        }
        else if (child->key > word[0])
            break;//impossible to find the word anymore without spelling correction
        child = child->sibling;
    }

    /* Second loop, applying suggestion here */
    child = trie->child;
    while (child != NULL)
    {
        if (case_char_cmp(child->key, word[0]) == true ||
            vowel_char_cmp(child->key, word[0]) == true)
        {
            char  *corrected_word = NULL;
            if (word[0] == '\0')
            {
                corrected_word = malloc((corrected_word_size + 1) *
                                        sizeof(*corrected_word));
            }
            else
            {
                corrected_word = rec_find_correction(word + 1,
                                                     corrected_word_size + 1,
                                                     child);
            }
            if (corrected_word != NULL)
            {
                corrected_word[corrected_word_size] = child->key;//..we build it
                return corrected_word;//the word is built as we go back the call stack
            }
        }
        child = child->sibling;
    }
    return NULL;
}

char  *find_correction(const char *word, const trie_t *trie)
{
    return rec_find_correction(word, 0, trie);
}
