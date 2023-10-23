#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Already-made function in cs50.h library which asks for a string.
    string name = get_string("What's your name? ");
    printf("hello, %s\n", name);
}
