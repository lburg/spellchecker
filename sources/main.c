#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "trie.h"

int   main(void)
{
    trie_t  *words = trie_new_root();

    if (words == NULL)
    {
        fprintf(stderr, "Failed to create trie root.\n");
        return EXIT_FAILURE;
    }
    const char  *dictionary_path = "/usr/share/dict/words";
    FILE  *dictionary_stream = fopen(dictionary_path, "r");

    if (dictionary_stream == NULL)
    {
        fprintf(stderr, "Failed to open dictionary at %s\n", dictionary_path);
        return EXIT_FAILURE;
    }
    size_t  useless = 0;
    char    *line = NULL;

    while (getline(&line, &useless, dictionary_stream) != -1)
    {
        line[strlen(line) - 1] = 0;//removes newline
        trie_add_string(line, words);
    }
    delete_trie(words);
    free(line);
    fclose(dictionary_stream);
    return EXIT_SUCCESS;
}
