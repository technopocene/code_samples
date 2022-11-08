#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <complex.h>

// replaces header file
int lett_count(string str);
int word_count(string str);
int sent_count(string str);
void grade_level(int l, int w, int s);

int main(void)
{
    // variables for letters, words & sentences
    int l = 0;
    int w = 0;
    int s = 0;

    // prompt user for string
    string text = get_string("Text: ");

    // count the number of letters, words and sentences
    for (int i = 0; i < strlen(text); i++)
    {
        l = lett_count(text);
        w = word_count(text);
        s = sent_count(text);
    }
    // output should be "Grade X"
    grade_level(l, w, s);
}

int lett_count(string str)
{
    int l = 0;

    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] >= 65 && str[i] <= 122)    //letters = a-z or A-Z)
        {
            l++;
        }
    }
    return l;
}

int word_count(string str)
{
    int w = 0;

    for (int i = 0; i <= strlen(str); i++)
    {
        if (str[i] == 32 || str[i] == '\0')   // words = anything between spaces, & also include word at the end
        {
            w++;
        }
    }
    return w;
}

int sent_count(string str)
{
    int s = 0;

    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == 46 || str[i] == 63 || str[i] == 33)    // sentences end in . ? or !
        {
            s++;
        }
    }
    return s;
}

void grade_level(int l, int w, int s)
{
    int level = 0;
    float L = ((float)l / (float)w) * 100.0;    // L = letters per 100 words
    float S = ((float)s / (float)w) * 100.0;    // S = sentences per 100 words

    // Coleman-Liau Index formula
    float result = 0.0588 * L - 0.296 * S - 15.8;
    level = round(result);
    // if index < 1, "Before Grade 1"
    if (level < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (level >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", level);
    }
}
