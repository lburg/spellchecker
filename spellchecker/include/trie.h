#ifndef   __TRIE_H__
# define  __TRIE_H__

# include <stdbool.h>

typedef struct s_trie trie_t;
struct  s_trie
{
    char    key;
    char    *whole_word;
    trie_t  *child;
    trie_t  *sibling;
};

/* Allocation */
trie_t  *trie_new_root(void);
trie_t  *trie_new_node(const char key);
trie_t  *trie_new_delimiter_node(const char *word);

/* Manipulation */
bool  trie_add_string(const char *string, trie_t *trie);
void  delete_trie(trie_t *trie);

/* Lookup */
bool  is_in_trie(const char *string, const trie_t *trie);

#endif /* !__TRIE_H__ */
