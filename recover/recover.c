#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    //Failsafe for wrong parameter input
    if (argc != 2)
    {
        printf("you have not inputted the parameters correctly, try again with ./recover FILENAME");
        return 1;
    }

    // Declare variables
    int count = 0;
    unsigned char bytes[512];
    FILE *png = NULL;
    FILE *f = fopen(argv[1], "r");

    // Read input file in blocks of 512 bytes
    while (fread(bytes, 512, 1, f) == 1)
    {
        //Check for the sequence required for jpg header.
        if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff &&
            (bytes[3] & 0xf0) == 0xe0)
        {
            // Close previous file, if any
            if (png != NULL)
            {
                fclose(png);
            }
            // Create new file with name based on count
            char filename[8];
            snprintf(filename, sizeof(filename), "%03i.jpg", count);
            png = fopen(filename, "wb");
            if (png == NULL)
            {
                printf("error");
                return 1;
            }
            count++;
        }
        // If currently writing to a file, write block of 512 bytes to file
        if (png != NULL)
        {
            fwrite(bytes, 512, 1, png);
        }
    }
    //close any opened files.
    if (png != NULL)
    {
        fclose(png);
    }
    fclose(f);
    return 0;
}