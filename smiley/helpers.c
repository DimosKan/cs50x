#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{

    RGBTRIPLE makered;
    makered.rgbtRed = 255;
    makered.rgbtBlue = 0;
    makered.rgbtGreen = 0;

    //scans every pixel of the image as 2d array
    for (int i = 0; i < height;  i++)
    {
        for (int j = 0; j < width;  j++)
        {
            //checks if every pixel to see if the pixel is black, if it is black, change its RGB values to with the one that corresponds to red (255,0,0)
            if (image[i][j].rgbtRed == 0 && image[i][j].rgbtGreen == 0 && image[i][j].rgbtBlue == 0)
            {
                image[i][j] = makered;
            }
        }
    }
}
