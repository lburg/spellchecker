#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "autocomplete.h"

t_suggestion  *new_suggestion_node(const char *word)
{
    t_suggestion  *node = malloc(sizeof(*node));

    if (node == NULL)
    {
        fprintf(stderr, "new_suggestion_node failed: Running out of virtual memory.\n");
        return NULL;
    }
    node->suggestion = strdup(word);
    node->next = NULL;
    return node;
}

static t_suggestion *build_suggestions(const trie_t *trie, t_suggestion *suggestions)
{
    while (trie != NULL)
    {
        //Recursive call to go to the maximum depth of this branch
        suggestions = build_suggestions(trie->child, suggestions);
        //If we found the end of a word..
        if (trie->key == '\0')
        {
            //..then this is a suggestion
            t_suggestion *new_suggestion = new_suggestion_node(trie->whole_word);
            if (suggestions == NULL)
                suggestions = new_suggestion;
            else
            {
                new_suggestion->next = suggestions;
                suggestions = new_suggestion;
            }
        }
        //Anyway, we keep looking for every words in this subtrie
        trie = trie->sibling;
    }
    return suggestions;
}

static t_suggestion *rec_find_suggestions(const char *word, int idx,
                                           const trie_t *correct_words)
{
    const trie_t  *child = correct_words->child;

    while (child != NULL)
    {
        //If I finished the word..
        if (word[idx] == '\0')
        {
            //..I must build suggestions based on the possible words left
            return build_suggestions(child, NULL);
        }
        if (child->key == word[idx])
        {
            return rec_find_suggestions(word, idx + 1, child);
        }
        if (child->key > word[idx])
            return NULL;//no such word in the dictionary
        child = child->sibling;
    }
    return NULL;
}

t_suggestion  *find_suggestions(const char *word, const trie_t *correct_words)
{
    if (strlen(word) == 0)
        return NULL;//to prevent returning the whole dictionary on an empty string
    return rec_find_suggestions(word, 0, correct_words);
}

void  clear_suggestions(t_suggestion *suggestions)
{
    t_suggestion  *tmp;

    while (suggestions != NULL)
    {
        tmp = suggestions;
        suggestions = suggestions->next;
        free(tmp->suggestion);
        free(tmp);
    }
}
