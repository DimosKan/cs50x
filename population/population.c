#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int startSize;
    do
    {
        //Prompts the user to enter starting size
        startSize = get_int("starting size: ");

    }
    while (startSize < 9);

    int endSize;

    do
    {
        //Prompts the user to enter ending size
        endSize = get_int("ending size: ");
    }
    while (endSize < startSize);
    int years = 0;
    int population = startSize;
    while (population < endSize)
    {
        //calculations that has to be done in order to predict the year.
        population += population / 3 - population / 4;
        years++;
    }
    printf("Years: %d\n", years);
}
