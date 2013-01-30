#ifndef   __AUTOCOMPLETE_H__
# define  __AUTOCOMPLETE_H__

# include "trie.h"

typedef struct  s_suggestion  t_suggestion;
struct  s_suggestion
{
    char          *suggestion;
    t_suggestion  *next;
};

t_suggestion  *new_suggestion_node(const char *word);
t_suggestion  *find_suggestions(const char *word, const trie_t *correct_words);
void  clear_suggestions(t_suggestion *suggestions);

#endif /* !__AUTOCOMPLETE_H__ */
