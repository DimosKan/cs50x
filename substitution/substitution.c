#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    //Makes initial checks if there exactly one argument before I assign values. (Otherwise segmentation fault (core dumbed) appears).
    if (argv[1] == NULL)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (argv[2] != NULL)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    //Assigning values, I initially thought making one table with 26 columns and 2 rows was a good idea... C pointers proved me otherwise. Too complicated for now.
    string key = argv[1]; //key value
    int key_l = strlen(key); //key length
    char key_asentence_letter[26]; //table with key values
    char abc_asentence_letter[26]; //table with the alphabet
    int asciiletter = 97;

    //Checks if the key indded has 26 letters
    if (key_l != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    //Making all chars of the key string lowercase for homogeneity.
    for (int caps_loop = 0; key[caps_loop]; caps_loop++)
    {
        if (isalpha(key[caps_loop]))
        {
            key[caps_loop] = tolower(key[caps_loop]);
        }
        else
        {
            return 1;
        }
    }
    int init_counter = 0;
    for (int key_checker_outer = 0; key_checker_outer < key_l; key_checker_outer++)
    {
        init_counter++;
        for (int key_checker_inner = 0; key[init_counter + key_checker_inner] ; key_checker_inner++)
        {
            if (key[key_checker_outer] == key[init_counter + key_checker_inner])
            {
                return 1;
            }
        }
    }

    //Assigning all the characters of the key to cells of keys_asentence_letter
    for (int key_counter = 0; key_counter < key_l; key_counter++)
    {
        key_asentence_letter[key_counter] = key[key_counter];
    }

    //Assigning all the characters of the alphabet to cells of abc_asentence_letter.
    //it actually loops from 97 to 122 which is the integers for the ASCII of the alphabet and then assigns the integers as chars.
    for (int abc_counter = 0; abc_counter < 26; abc_counter++)
    {

        abc_asentence_letter[abc_counter] = (char)asciiletter;
        asciiletter += 1;
    }

    //lets the user insert the sentence
    string sentence = get_string("plaintext: ");

    //counts the letters of the sentence
    int sentence_let_counter = strlen(sentence);

    //compares every letter of text to the alphabet. if it collerates with an alphabet letter,
    //it assignes the same column of the key array to the string array of the input sentence
    //it even converts the key letter into uppercase when it sees that the inputted text letter is uppercase
    //outer loop is responsible to go through every letter of the input text (line 63), while inner loop is responsible to go through all the alphabet letters(line 65)
    for (int sentence_letter = 0; sentence_letter < sentence_let_counter; sentence_letter++)
    {
        for (int abc_scanner = 0; abc_scanner < 26; abc_scanner++)
        {
            if (isalpha(sentence[sentence_letter]))
            {
                if (sentence[sentence_letter] == abc_asentence_letter[abc_scanner])
                {
                    sentence[sentence_letter] = key_asentence_letter[abc_scanner];
                    break;
                }
                else if (sentence[sentence_letter] == toupper(abc_asentence_letter[abc_scanner]))
                {
                    sentence[sentence_letter] = toupper(key_asentence_letter[abc_scanner]);
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }
    printf("ciphertext: %s\n", sentence);

}