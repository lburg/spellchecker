#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "trie.h"

static bool case_char_cmp(char c1, char c2)
{
    return strncasecmp(&c1, &c2, 1) == 0;
}

static bool isvowel(char c)
{
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y')
        return true;
    return false;
}

static bool vowel_char_cmp(char c1, char c2)
{
    /*
    ** Puts c1 and c2 in lower case.
    */
    c1 = tolower(c1);
    c2 = tolower(c2);

    if (isvowel(c1) == true && isvowel(c2) == true)
        return true;
    return false;
}

/*
** Kind of nasty.. I don't like repeating the loop but I
** also don't like the idea of a sub-function recursively
** calling its caller...
*/
static char *rec_find_correction(const char *word, const int word_idx,
                                 const size_t corrected_word_size, const trie_t *trie)
{
    const trie_t  *child = trie->child;

    /*
    ** First loop to see if we got the word as is.
    ** We don't want 'hell' to be corrected as 'Hell'
    */
    while (child != NULL)
    {
        if (child->key == word[word_idx])
        {
            char  *corrected_word = NULL;
            if (word[word_idx] == '\0')
            {
                //we found the whole word, so let's return it from the last node
                return strdup(child->whole_word);
            }
            else
            {
                //we didn't find the word yet, keep looking
                corrected_word = rec_find_correction(word, word_idx + 1,
                                                     corrected_word_size + 1,
                                                     child);
            }
            //if the word was found..
            if (corrected_word != NULL)
            {
                return corrected_word;//..we return it
            }
        }
        else if (child->key > word[word_idx])
            break;//impossible to find the word anymore without spelling correction
        child = child->sibling;
    }

    /* Second loop, applying suggestion here */
    child = trie->child;
    while (child != NULL)
    {
        if (case_char_cmp(child->key, word[word_idx]) == true ||
            vowel_char_cmp(child->key, word[word_idx]) == true)
        {
            char  *corrected_word = NULL;
            if (word[word_idx] == '\0')
            {
                return strdup(child->whole_word);
            }
            else
            {
                corrected_word = rec_find_correction(word, word_idx + 1,
                                                     corrected_word_size + 1,
                                                     child);
            }
            if (corrected_word != NULL)
            {
                return corrected_word;
            }
        }
        child = child->sibling;
    }
    /*
    ** If the previous letter is the same as the current,
    ** then we consider it a double and skip it.
    ** Should we consider case or not?
    */
    if (word_idx > 0 &&
        word[word_idx - 1] == word[word_idx])
        return rec_find_correction(word, word_idx + 1, corrected_word_size, trie);
    return NULL;
}

char  *find_correction(const char *word, const trie_t *trie)
{
    return rec_find_correction(word, 0, 0, trie);
}

