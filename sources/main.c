#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "trie.h"
#include "autocomplete.h"
#include "spellcheck.h"

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

static void print_correction(const char *user_word, const trie_t *correct_words)
{
    char  *correction = find_correction(user_word, correct_words);

    if (correction != NULL)
    {
        printf("%s\n", correction);
        free(correction); correction = NULL;
    }
    else
        printf("NO SUGGESTION\n");
}

static void print_suggestions(const char *user_word, const trie_t *correct_words)
{
    t_suggestion  *suggestions = find_suggestions(user_word, correct_words);

    if (suggestions != NULL)
    {
        t_suggestion  *tmp = suggestions;
        while (tmp != NULL)
        {
            printf("%s\n", tmp->suggestion);
            tmp = tmp->next;
        }
        clear_suggestions(suggestions);
    }
    else
        printf("NO COMPLETION\n");
}

static void run(const trie_t *correct_words, bool autocomplete)
{
    char    *user_word = NULL;
    size_t  user_word_size = 0;

    print_prompt();
    while (getline(&user_word, &user_word_size, stdin) != -1)
    {
        user_word[strlen(user_word) - 1] = 0;
        if (autocomplete == true)
            print_suggestions(user_word, correct_words);
        else
            print_correction(user_word, correct_words);
        print_prompt();
    }
    free(user_word); user_word = NULL;
}

int   main(int argc, char *argv[])
{
    bool  autocomplete = false;

    if (argc > 1 && strcmp(argv[1], "--autocomplete") == 0)
        autocomplete = true;
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
        if (trie_add_string(line, g_correct_words) == false)
            printf("Failed to insert: [%s]\n", line);
    }
    free(line); line = NULL;
    fclose(dictionary_stream);

    run(g_correct_words, autocomplete);
    return EXIT_SUCCESS;
}
