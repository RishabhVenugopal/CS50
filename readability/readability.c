#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

//main function to calculate index
int main(void)
{
    string text = get_string("Enter Text: ");
    int lnum = count_letters(text);
    int wnum = count_words(text);
    int snum = count_sentences(text);
    double l = ((double) lnum / wnum) * 100;
    double s = ((double) snum / wnum) * 100;
    int index = round(0.0588 * l - 0.296 * s - 15.8);
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }

    if (index > 16)
    {
        printf("Grade 16+\n");
    }

    if (index >= 1 && index <= 16)
    {
        printf("Grade %i\n", index);
    }
}

//to count words
int count_words(string text)
{
    int num = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i - 1] == ' ')
        {
            num ++;
        }
    }

    return num + 1;
}

//to count letters
int count_letters(string text)
{
    int num = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]) != 0)
        {
            num++;
        }
    }

    return num;
}

//to count sentences
int count_sentences(string text)
{
    int num = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            num++;
        }
    }

    return num;
}