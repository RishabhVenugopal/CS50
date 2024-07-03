// Implements a dictionary's functionality

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

const unsigned int N = 115336;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index = hash(word);
    node *p = table[index];
    while (p != NULL)
    {
        char word2[strlen(p->word)];
        strcpy(word2, p->word);
        if (strcasecmp(word, word2) == 0)
        {
            return true;
        }
        p = p->next;
    }
    return false;
}

bool chkprime(int n)
{
    for(int i = 2; i < n; i++)
    {
        if (n % i == 0)
        {
            return false;
        }
    }
    return true;
}
// Hashes word to a number
unsigned int hash(const char *word)
{
    int hash = 0;
    int index = 0;
    int prime = 3;
    while (index < strlen(word))
    {
        if (chkprime(prime))
        {
            if (isalpha(word[index]) != 0)
            {
                hash += ((toupper(word[index]) - 'A') + 1) * prime;
            }
            index++;
        }
        prime++;
    }

    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool loaded;
bool load(const char *dictionary)
{
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        loaded = false;
        return false;
    }
    char word[LENGTH+1];
    node *n;
    int i = 1;
    while (fscanf(file, "%s", word) != EOF)
    {
        int index = hash(word);
        if (table[index] == NULL)
        {
            table[index] = malloc(sizeof(node));
            if (table[index] == NULL)
            {
                loaded = false;
                return false;
            }
            strcpy(table[index]->word, word);
            table[index]->next = NULL;
        }
        else
        {
            n = malloc(sizeof(node));
            if (n == NULL)
            {
                loaded = false;
                return false;
            }
            strcpy(n->word, word);
            n->next = table[index];
            table[index] = n;
        }
        i++;
    }
    fclose(file);
    loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    int size = 0;
    if (loaded)
    {
        for (int i = 0; i < N; i++)
        {
            node *p = table[i];
            while (p != NULL)
            {
                size++;
                p = p->next;
            }
        }
    }
    return size;
}

//Frees all nodes of an individual linked list
bool unloadHelper(node *p)
{
    bool unloaded = false;
    if (p == NULL)
    {
        return true;
    }
    else
    {
        unloaded = unloadHelper(p->next);
        free(p);
    }
    return unloaded;
}
// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    bool unloaded;
    for (int i = 0; i < N; i++)
    {
        unloaded = unloadHelper(table[i]);
        if (!unloaded)
        {
            return false;
        }
        table[i] = NULL;
    }
    return true;
}
