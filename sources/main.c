#include <stdlib.h>
#include <stdio.h>
#include "trie.h"

int   main(void)
{
    trie_t  *t = new_trie_root();

    if (t == NULL)
    {
        fprintf(stderr, "Failed to build trie from dictionary words.\n");
        return EXIT_FAILURE;
    }
    add_string("tea", t);
    add_string("ted", t);
    add_string("ten", t);
    add_string("in", t);
    add_string("inn", t);

    if (is_in_trie("inn", t) == true)
        printf("ten is in trie!\n");
    else
        printf("ten NOT in trie :(\n");
    return EXIT_SUCCESS;
}
