#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "trie.h"

static trie_t *g_correct_words = NULL;

static void signal_handler(int signal_number)
{
    if (signal_number == SIGINT ||
        signal_number == SIGTERM ||
        signal_number == SIGQUIT)
    {
        delete_trie(g_correct_words); g_correct_words = NULL;
        exit(EXIT_SUCCESS);
    }
}

static void print_prompt(void)
{
    printf("> "); fflush(stdout);
}

static void run_autocorrect(trie_t *correct_words)
{
    char    *user_word = NULL;
    size_t  user_word_size = 0;

    print_prompt();
    while (getline(&user_word, &user_word_size, stdin) != -1)
    {
        char  *correction = NULL;

        user_word[strlen(user_word) - 1] = 0;
        correction = find_correction(user_word, correct_words);
        if (correction != NULL)
        {
            printf("%s\n", correction);
            free(correction); correction = NULL;
        }
        else
            printf("NO SUGGESTION\n");
        print_prompt();
    }
    free(user_word); user_word = NULL;
}

int   main(void)
{
    if (signal(SIGINT, &signal_handler) == SIG_ERR ||
        signal(SIGTERM, &signal_handler) == SIG_ERR ||
        signal(SIGQUIT, &signal_handler) == SIG_ERR)
    {
        fprintf(stderr, "Failed to register some signals. Possible leaks at program termination.\n");
    }
    g_correct_words = trie_new_root();
    if (g_correct_words == NULL)
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
    size_t  line_size = 0;
    char    *line = NULL;

    while (getline(&line, &line_size, dictionary_stream) != -1)
    {
        line[strlen(line) - 1] = 0;//removes newline
        trie_add_string(line, g_correct_words);
    }
    free(line); line = NULL;
    fclose(dictionary_stream);

    run_autocorrect(g_correct_words);
    return EXIT_SUCCESS;
}
