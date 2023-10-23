#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n;

    do
    {
        n = get_int("Size: ");

    }
    while (n < 1);

    //sp_count is just another counter which counts opposite of the n for every loop, it is responsible for counting how many spaces needed at the start of every row
    int sp_count = n-1;
    n = n + 1;
    int passedonce = 0;
    for (int j = 0; j < n ; j++)
    {
        for (int k = sp_count;k >= 0;k--)
        {
            if (passedonce == 1)
            {
                printf(" ");
            }
        }

        //first for responsible for the one half of the pyramid
        for (int i = 0; i < j; i++)
        {
            printf("#");
        }

        if (passedonce == 1)
        {
            printf("  "); //responsible for the space in the middle of the pyramid
        }
        //second for responsible for the second half of the pyramid
        for (int i = 0;i<j;i++)
        {
            printf("#");
        }
        //changes the line in order for a new pyramid row to start

        if (passedonce == 1){
            printf("\n");
        }
        sp_count = sp_count - 1;
        passedonce = 1;
    }
}


