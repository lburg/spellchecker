#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

static void repeat_or_change_case(char c)
{
    bool  repeat_letter = rand() % 2;

    if (repeat_letter == true)
    {
        //randomly repeat the letter up to ten times
        int nb_repeats = (rand() % 10) + 1;

        for (int i = 0; i < nb_repeats; i++)
            printf("%c", c);
    }
    else
    {
        if (isupper(c) != 0)
            c = tolower(c);
        else
            c = toupper(c);
        printf("%c", c);
    }
}

static void generate_word_mistakes(FILE *dict_stream)
{
    char          *word = NULL;
    size_t        word_size = 0;
    const char    *vowels = "aeiouy";
    const size_t  nb_vowels = strlen(vowels);

    srand(time(NULL));
    while (getline(&word, &word_size, dict_stream) != -1)
    {
        size_t  word_len = strlen(word);
        for (unsigned int i = 0; i < word_len; ++i)
        {
            //if the current letter is a vowel..
            if (strchr(vowels, word[i]) != NULL)
                printf("%c", vowels[rand() % nb_vowels]);//..print a random vowel
            else if (isalpha(word[i]))
                repeat_or_change_case(word[i]);
            else
                printf("%c", word[i]);
        }
    }
    free(word); word = NULL;
}

int main(void)
{
    const char  *dict_path = "/usr/share/dict/words";
    FILE        *dict_stream = fopen(dict_path, "r");

    if (dict_stream == NULL)
    {
        fprintf(stderr, "Failed to open %s.\n", dict_path);
        return EXIT_FAILURE;
    }
    generate_word_mistakes(dict_stream);
    fclose(dict_stream);
    return EXIT_SUCCESS;
}
