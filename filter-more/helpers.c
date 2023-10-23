#include "helpers.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row_c = 0; row_c < height; row_c++)
    {
        for (int col_c = 0; col_c < width; col_c++)
        {
            double ave_color = (image[row_c][col_c].rgbtRed + image[row_c][col_c].rgbtGreen + image[row_c][col_c].rgbtBlue) / 3.0;
            image[row_c][col_c].rgbtRed = round(ave_color);
            image[row_c][col_c].rgbtGreen = round(ave_color);
            image[row_c][col_c].rgbtBlue = round(ave_color);
        }
    }
    return;
}


// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = tmp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    memcpy(temp, image, sizeof(RGBTRIPLE) * height * width);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int count = 0;
            int sum_r = 0, sum_g = 0, sum_b = 0;

            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    int x = i + k;
                    int y = j + l;

                    if (x >= 0 && x < height && y >= 0 && y < width)
                    {
                        sum_r += temp[x][y].rgbtRed;
                        sum_g += temp[x][y].rgbtGreen;
                        sum_b += temp[x][y].rgbtBlue;
                        count++;
                    }
                }
            }

            image[i][j].rgbtRed = round((float) sum_r / count);
            image[i][j].rgbtGreen = round((float) sum_g / count);
            image[i][j].rgbtBlue = round((float) sum_b / count);
        }
    }
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            int redX = 0;
            int rgbtGreenX = 0;
            int rgbtBlueX = 0;
            int redY = 0;
            int rgbtBlueY = 0;
            int rgbtGreenY = 0;

            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    if (i - 1 + x < 0 || i - 1 + x > height - 1 || j - 1 + y < 0 || j - 1 + y > width - 1)
                    {
                        continue;
                    }
                    redX = redX + (image[i - 1 + x][j - 1 + y].rgbtRed * Gx[x][y]);
                    rgbtGreenX = rgbtGreenX + (image[ i - 1 + x][j - 1 + y].rgbtGreen * Gx[x][y]);
                    rgbtBlueX = rgbtBlueX + (image[i - 1 + x][j - 1 + y].rgbtBlue * Gx[x][y]);
                    redY = redY + (image[i - 1 + x][j - 1 + y].rgbtRed * Gy[x][y]);
                    rgbtGreenY = rgbtGreenY + (image[i - 1 + x][j - 1 + y].rgbtGreen * Gy[x][y]);
                    rgbtBlueY = rgbtBlueY + (image[i - 1 + x][j - 1 + y].rgbtBlue * Gy[x][y]);
                }
            }

            int red = round(sqrt((redX * redX) + (redY * redY)));
            int rgbtGreen = round(sqrt((rgbtGreenX * rgbtGreenX) + (rgbtGreenY * rgbtGreenY)));
            int rgbtBlue = round(sqrt((rgbtBlueX * rgbtBlueX) + (rgbtBlueY * rgbtBlueY)));

            if (red > 255)
            {
                red = 255;
            }
            if (rgbtGreen > 255)
            {
                rgbtGreen = 255;
            }
            if (rgbtBlue > 255)
            {
                rgbtBlue = 255;
            }

            temp[i][j].rgbtRed = red;
            temp[i][j].rgbtGreen = rgbtGreen;
            temp[i][j].rgbtBlue = rgbtBlue;

        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width ; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }
    }

    return;
}

