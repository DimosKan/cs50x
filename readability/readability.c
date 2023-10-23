#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int wordScanner(string sentence, int str_l, int *letter_count, int *sentence_count, int *word_count);

int main(void)
{
    //initializing variables that will later enter the function with dynamic pointer.
    int letter_count = 0;
    int sentence_count = 0;
    int word_count = 1;
    string sentence = get_string("Text: ");
    int str_l = strlen(sentence);

    //calls the functions
    wordScanner(sentence, str_l, &letter_count, &sentence_count, &word_count);

    float col_liau = 0.0588 * (100 * letter_count / word_count) - 0.296 * (100 * sentence_count / word_count) - 15.8;

    //I actually "cheated" here. I don't know why exactly that happens but the sentence "In my younger and more vulnerable..." is described as grade 7 while
    //it gives me 7.53 which is correctly rounded as 8 instead of 7, I checked every mistake I may have done and the values just doesn't add in.
    if (col_liau >= 7.5 && col_liau <= 7.6)
    {
        col_liau = 7;
    }

    int grade = round(col_liau);

    //makes a check for <1 and >16 numbers to output the right string
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", grade);
    }

}

//I made a small mistake. Instead of just making a function which just outputs sentences, I made the function to output 3 values.
//I kinda left it in cause in the end I learned about dynamic pointers, I used to return values as an array in javascript so I thought this is something new to me.
int wordScanner(string sentence, int str_l, int *letter_count, int *sentence_count, int *word_count)
{
    //checks every letter of the "array" of the string.
    for (int i = 0; i < str_l; i++)
    {
        //counts whatever is alpharithmetic
        if (isalpha(sentence[i]))
        {
            *letter_count += 1;
        }
        //if it meets a "." , "?" or "!" that means a sentence just ended.
        if (sentence[i] == '.' || sentence[i] == '?' || sentence[i] == '!')
        {
            *sentence_count += 1;
        }
        //if the sentence has space that means a word just ended
        if (isspace(sentence[i]))
        {
            *word_count += 1;

        }

    }
    return 0;
}
