#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// message is encrypted by replacing every letter with another letter
// message can be decrypted with the key
// ignore any nonalphabetical characters
// preserve letter case

// Writing the first line of each function for compiler's sake
string encipher(string key, string msg);

int main(int argc, string argv[])
{
    string key = argv[1];

    // validate user input: exactly 2 arguments should be provided
    // example: ./substitution JTREKYAVOGDXPSNCUIZLFBMWHQ
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1; // this signifies an error
    }

    // Key must have exactly 26 characters
    if (strlen(key) != 26)
    {
        printf("Error: You must provide a key with 26 characters\n");
        return 1;
    }

    // iterate through all of the characters in the key and validate
    for (int i = 0; i < strlen(key); i++)
    {
        // Key must not have any non-alphabetic characters
        if (!isalpha(key[i]))
        {
            printf("Error: Alphabetic characters only\n");
            return 1;
        }

        // All letters in key must be unique (case insensitive)
        for (int j = i + 1; j < strlen(key); j++)
        {
            if (key[i] == key[j])
            {
                printf("Error: Key must not contain duplicates\n");
                return 1;
            }
        }
    }

    // Valid key, so get message from user to be encoded
    string txt = get_string("plaintext:  ");

    // encode the message
    string cipher = encipher(key, txt);

    // print result
    printf("ciphertext: %s\n", cipher);
}

// encodes the secret message
string encipher(string key, string txt)
{

    // create an array to hold differences in values
    // int *diff = malloc(strlen(key));

    // create an array to hold encoded message
    char *msg = malloc(strlen(txt));

    // pointer for the key to get the correct index
    int n;

    // Iterate through the message to decode it
    for (int i = 0; i < strlen(txt); i++)
    {
        // Handling non-alphabetic characters
        if (!isalpha(txt[i]))
        {
            msg[i] = txt[i];
        }

        if (isupper(txt[i]))
        {
            // subtract 65 from the letter to find the correct index
            // for example, H(72) - A(65) = 7
            // H is the 8th letter of the alphabet and will be at index 7
            n = txt[i] - 65;

            if (isupper(key[n]))
            {
                // encoded message is replaced with the letter at the nth index in the key
                msg[i] = key[n];
            }

            if (islower(key[n]))
            {
                // encoded message is replaced with the letter at the nth index in the key
                msg[i] = key[n] - 32;
            }
        }

        if (islower(txt[i]))
        {
            // subtract 65 from the letter to find the correct index
            // for example, H(72) - A(65) = 7
            // H is the 8th letter of the alphabet and will be at index 7
            n = txt[i] - 97;

            if (isupper(key[n]))
            {
                // encoded message is replaced with the letter at the nth index in the key
                msg[i] = key[n] + 32;
            }

            if (islower(key[n]))
            {
                // encoded message is replaced with the letter at the nth index in the key
                msg[i] = key[n];
            }
        }
    }
    printf("\n");
    return msg;
}