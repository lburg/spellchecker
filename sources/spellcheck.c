#include <strings.h>
#include <stdlib.h>
#include "trie.h"

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
                //we found the whole word, so let's allocate some space to store it
                corrected_word = malloc((corrected_word_size + 1) *
                                        sizeof(*corrected_word));
            }
            else
            {
                //we didn't find the word yet, keep looking
                corrected_word = rec_find_correction(word, word_idx + 1,
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
                corrected_word = malloc((corrected_word_size + 1) *
                                        sizeof(*corrected_word));
            }
            else
            {
                corrected_word = rec_find_correction(word, word_idx + 1,
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
    /*
    ** If the previous letter is the same as the current,
    ** then we consider it a double and skip it.
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

