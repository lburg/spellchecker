#ifndef   __TRIE_H__
# define  __TRIE_H__

# include <stdbool.h>

/*
** End of string character to distinguish
** between "cat" and "catalog".
*/
static const char trie_delimiter = 0;

typedef struct s_trie trie_t;
struct  s_trie
{
    char    key;
    trie_t  *child;
    trie_t  *sibling;
};

trie_t  *new_node(const char key);
bool  add_string(const char *string, trie_t *trie);
bool  is_in_trie(const char *string, const trie_t *trie);

#endif /* !__TRIE_H__ */
