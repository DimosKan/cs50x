#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int main(void)
{
    //Declarations.
    int extra_dig = 0;
    int sum_dig = 0;
    long n;
    char cr_type[50];
    int count = 0;

    //Gets a long number and forbidding anything else but that, as instructed.
    do
    {
        n = get_long("Number: ");

    }
    while (n < 1);

    count = (n == 0) ? 1 : log10(n) + 1; //formula to count the number of digits.
    int two_init_digits = count - 1; //Gets the first two digits of each card.

    //passes
    for (int i = 1; i <= count; i++)
    {
        int dig = n % 10;

        if (i == two_init_digits)
        {
            if (n == 34 || n == 37)
            {
                strcpy(cr_type, "AMEX\n");
            }
            else if (n > 50 && n < 56)
            {
                strcpy(cr_type, "MASTERCARD\n");
            }
            else if (n > 39 && n < 50)
            {
                strcpy(cr_type, "VISA\n");
            }
            else
            {
                strcpy(cr_type, "INVALID\n");
            }
        }

        //if a number's mod is 0 that means it is an even.
        if (i % 2 == 0)
        {
            //4 is the first number that when you multiply by 2, makes a number with 2 digits, so I filter everything that will give two-digits numbers.
            if (dig > 3)
            {
                int temp_dig = dig * 2;
                int tempdig1 = temp_dig / 10;
                int tempdig2 = temp_dig % 10;
                sum_dig = sum_dig + tempdig1 + tempdig2;

            }
            else
            {
                sum_dig = sum_dig + (dig * 2);
            }

        }
        else
        {
            extra_dig = extra_dig + dig;

        }
        //divides the card number by 10, making it a float, since the n is not declared as float, it just ignore whatever is after the point (.).
        n = n / 10;

    }

    //sums the sums of the odd numbers with the sums of the even numbers of the card
    int valid_ch = extra_dig + sum_dig;

    //Checks if the card has more than 12 digits (which is the minimum)
    if (count < 12)
    {
        strcpy(cr_type, "INVALID\n");
    }

    //Checks if the checksum is valid.
    if (valid_ch % 10 == 0)
    {
        printf("%s", cr_type);
    }
    else
    {
        printf("INVALID\n");
    }

}
