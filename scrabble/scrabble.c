#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

// ASCII values for uppercase letters
int LETTERS [] = {65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90};

// Writing the first line of each function for compiler's sake
string to_upper(string word);
int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);
    // printf("Player 1 score: %i\n", score1); // comment this out
    // printf("Player 2 score: %i\n", score2); // comment this out

    // Determine which player is the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    int score = 0;

    // get the length of the word
    int word_len = strlen(word);

    // convert lowercase letters to uppercase
    word = to_upper(word);

    int i = 0;
    int j = 0;

    // iterate through the word
    for (i = 0; i < word_len; i++)
    {
        for (j = 0; j < 26; j++)
        {
            if (word[i] == LETTERS[j])
            {
                for (int k = 0; k < 26; k++)
                {
                    score += POINTS[j];
                    j++;
                    break;
                }
            }
        }
    }
    return score;
}

// converts a string to uppercase characters
string to_upper(string word)
{
    for (int i = 0; word[i] != '\0'; i++)
    {
        if (islower(word[i]))
        {
            word[i] = word[i] - 32;
        }
    }
    return word;
}