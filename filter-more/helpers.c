#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avg = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image2[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int redsum = 0;
            int greensum = 0;
            int bluesum = 0;
            int addcount = 0;
            if (i != 0)
            {
                redsum += image[i - 1][j].rgbtRed;
                greensum += image[i - 1][j].rgbtGreen;
                bluesum += image[i - 1][j].rgbtBlue;
                addcount++;
                if (j != 0)
                {
                    redsum += image[i - 1][j - 1].rgbtRed;
                    greensum += image[i - 1][j - 1].rgbtGreen;
                    bluesum += image[i - 1][j - 1].rgbtBlue;
                    addcount++;
                }
                if (j != width - 1)
                {
                    redsum += image[i - 1][j + 1].rgbtRed;
                    greensum += image[i - 1][j + 1].rgbtGreen;
                    bluesum += image[i - 1][j + 1].rgbtBlue;
                    addcount++;
                }
            }
            if (i != height - 1)
            {
                redsum += image[i + 1][j].rgbtRed;
                greensum += image[i + 1][j].rgbtGreen;
                bluesum += image[i + 1][j].rgbtBlue;
                addcount++;
                if (j != 0)
                {
                    redsum += image[i + 1][j - 1].rgbtRed;
                    greensum += image[i + 1][j - 1].rgbtGreen;
                    bluesum += image[i + 1][j - 1].rgbtBlue;
                    addcount++;
                }
                if (j != width - 1)
                {
                    redsum += image[i + 1][j + 1].rgbtRed;
                    greensum += image[i + 1][j + 1].rgbtGreen;
                    bluesum += image[i + 1][j + 1].rgbtBlue;
                    addcount++;
                }
            }
            if (j != 0)
            {
                redsum += image[i][j - 1].rgbtRed;
                greensum += image[i][j - 1].rgbtGreen;
                bluesum += image[i][j - 1].rgbtBlue;
                addcount++;
            }
            if (j != width - 1)
            {
                redsum += image[i][j + 1].rgbtRed;
                greensum += image[i][j + 1].rgbtGreen;
                bluesum += image[i][j + 1].rgbtBlue;
                addcount++;
            }
            redsum += image[i][j].rgbtRed;
            greensum += image[i][j].rgbtGreen;
            bluesum += image[i][j].rgbtBlue;
            addcount++;
            image2[i][j].rgbtBlue = round(bluesum / (float) addcount);
            image2[i][j].rgbtGreen = round(greensum / (float) addcount);
            image2[i][j].rgbtRed = round(redsum / (float) addcount);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image2[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image2[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int gxred = 0;
            int gyred = 0;
            int gxgreen = 0;
            int gygreen = 0;
            int gxblue = 0;
            int gyblue = 0;

            if (i != 0)
            {
                gxred += image[i - 1][j].rgbtRed * 0;
                gyred += image[i - 1][j].rgbtRed * (-2);
                gxgreen += image[i - 1][j].rgbtGreen * 0;
                gygreen += image[i - 1][j].rgbtGreen * (-2);
                gxblue += image[i - 1][j].rgbtBlue * 0;
                gyblue += image[i - 1][j].rgbtBlue * (-2);
                if (j != 0)
                {
                    gxred += image[i - 1][j - 1].rgbtRed * (-1);
                    gyred += image[i - 1][j - 1].rgbtRed * (-1);
                    gxgreen += image[i - 1][j - 1].rgbtGreen * (-1);
                    gygreen += image[i - 1][j - 1].rgbtGreen * (-1);
                    gxblue += image[i - 1][j - 1].rgbtBlue * (-1);
                    gyblue += image[i - 1][j - 1].rgbtBlue * (-1);
                }
                if (j != width - 1)
                {
                    gxred += image[i - 1][j + 1].rgbtRed;
                    gyred += image[i - 1][j + 1].rgbtRed * (-1);
                    gxgreen += image[i - 1][j + 1].rgbtGreen;
                    gygreen += image[i - 1][j + 1].rgbtGreen * (-1);
                    gxblue += image[i - 1][j + 1].rgbtBlue;
                    gyblue += image[i - 1][j + 1].rgbtBlue * (-1);
                }
            }
            if (i != height - 1)
            {
                gxred += image[i + 1][j].rgbtRed * 0;
                gyred += image[i + 1][j].rgbtRed * 2;
                gxgreen += image[i + 1][j].rgbtGreen * 0;
                gygreen += image[i + 1][j].rgbtGreen * 2;
                gxblue += image[i + 1][j].rgbtBlue * 0;
                gyblue += image[i + 1][j].rgbtBlue * 2;
                if (j != 0)
                {
                    gxred += image[i + 1][j - 1].rgbtRed * (-1);
                    gyred += image[i + 1][j - 1].rgbtRed;
                    gxgreen += image[i + 1][j - 1].rgbtGreen * (-1);
                    gygreen += image[i + 1][j - 1].rgbtGreen;
                    gxblue += image[i + 1][j - 1].rgbtBlue * (-1);
                    gyblue += image[i + 1][j - 1].rgbtBlue;
                }
                if (j != width - 1)
                {
                    gxred += image[i + 1][j + 1].rgbtRed;
                    gyred += image[i + 1][j + 1].rgbtRed;
                    gxgreen += image[i + 1][j + 1].rgbtGreen;
                    gygreen += image[i + 1][j + 1].rgbtGreen;
                    gxblue += image[i + 1][j + 1].rgbtBlue;
                    gyblue += image[i + 1][j + 1].rgbtBlue;
                }
            }
            if (j != 0)
            {
                gxred += image[i][j - 1].rgbtRed * (-2);
                gyred += image[i][j - 1].rgbtRed * 0;
                gxgreen += image[i][j - 1].rgbtGreen * (-2);
                gygreen += image[i][j - 1].rgbtGreen * 0;
                gxblue += image[i][j - 1].rgbtBlue * (-2);
                gyblue += image[i][j - 1].rgbtBlue * 0;
            }
            if (j != width - 1)
            {
                gxred += image[i][j + 1].rgbtRed * 2;
                gyred += image[i][j + 1].rgbtRed * 0;
                gxgreen += image[i][j + 1].rgbtGreen * 2;
                gygreen += image[i][j + 1].rgbtGreen * 0;
                gxblue += image[i][j + 1].rgbtBlue * 2;
                gyblue += image[i][j + 1].rgbtBlue * 0;
            }
            if (round(sqrt(gxblue * gxblue + gyblue * gyblue)) <= 255)
            {
                image2[i][j].rgbtBlue = round(sqrt(gxblue * gxblue + gyblue * gyblue));
            }
            else
            {
                image2[i][j].rgbtBlue = 255;
            }
            if (round(sqrt(gxgreen * gxgreen + gygreen * gygreen)) <= 255)
            {
                image2[i][j].rgbtGreen = round(sqrt(gxgreen * gxgreen + gygreen * gygreen));
            }
            else
            {
                image2[i][j].rgbtGreen = 255;
            }
            if (round(sqrt(gxred * gxred + gyred * gyred)) <= 255)
            {
                image2[i][j].rgbtRed = round(sqrt(gxred * gxred + gyred * gyred));
            }
            else
            {
                image2[i][j].rgbtRed = 255;
            }
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image2[i][j];
        }
    }
    return;
}
