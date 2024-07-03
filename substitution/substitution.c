#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void substitute(string key, string ptext);
bool chk_key(string key);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    if (chk_key(argv[1]))
    {
        string ptext = get_string("Plaintext: ");
        substitute(argv[1], ptext);
        return 0;
    }
    return 1;
}

//To do the actual substitution and print out the ciphertext
void substitute(string key, string ptext)
{
    for (int i = 0; i < strlen(ptext); i++)
    {
        if (islower(ptext[i]) != 0)
        {
            ptext[i] = tolower(key[(int)ptext[i] - 97]);
        }

        if (isupper(ptext[i]) != 0)
        {
            ptext[i] = toupper(key[(int)ptext[i] - 65]);
        }

    }

    printf("ciphertext: %s\n", ptext);
}

//To check if the user input is a valid key
bool chk_key(string key)
{
    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return false;
    }
    for (int i = 0; i < strlen(key); i++)
    {
        if (isalpha(key[i]) == 0)
        {
            printf("Key must contain only alphabets.\n");
            return false;
        }

        for (int j = i + 1; j < strlen(key); j++)
        {
            if (key[i] == key[j])
            {
                printf("You cannot use one character multiple times.\n");
                return false;
            }
        }
    }
    return true;
}