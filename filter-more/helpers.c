#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Function prototypes
double cap(double n);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // read & store the channel values for each pixel
            float b = image[i][j].rgbtBlue;
            float g = image[i][j].rgbtGreen;
            float r = image[i][j].rgbtRed;
            // printf("B: %d, G: %d, R: %d", b, g, r);

            // calculate the average
            int avg = round((r + g + b) / 3);
            // printf(" AVG: %d ", avg);
            // set each channel to new value
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
        // printf("\n");
    }
    // printf("\n");
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int row = height - 1;
    int col = width - 1;

    // examine each pixel in the image
    for (int i = 0; i <= row; i++)
    {
        // divide width by 2 because we change left & right pixels with each pass
        for (int j = 0; j <= col / 2 ; j++)
        {
            // store values for the left and right pixels to swap
            RGBTRIPLE l = image[i][j];
            RGBTRIPLE r = image[i][col - j];

            // replace the left-hand pixel with its counterpart on the right
            image[i][j].rgbtRed = r.rgbtRed;
            image[i][j].rgbtGreen = r.rgbtGreen;
            image[i][j].rgbtBlue = r.rgbtBlue;

            // and vice versa
            image[i][col - j].rgbtRed = l.rgbtRed;
            image[i][col - j].rgbtGreen = l.rgbtGreen;
            image[i][col - j].rgbtBlue = l.rgbtBlue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // indices for image and kernel
    int i, j, k, l, count;

    // radius of the kernel size: 1 is 3x3
    int rad = 1;

    // int row = height-1;
    // int col = width-1;

    // these accumulate values for each color channel
    float r, g, b;
    // int b_avg, g_avg, r_avg;

    // allocate memory for image copy
    RGBTRIPLE **p_image = malloc(height * sizeof(RGBTRIPLE *));
    for (i = 0; i < height; i++)
    {
        p_image[i] = malloc(width * sizeof(RGBTRIPLE));
    }

    // make a copy of the image to save the original pixel values
    for (i = 0; i < height; i++)
    {
        // printf("row %d: ", i);
        for (j = 0; j < width; j++)
        {
            // printf("(%d, %d, %d) ", image[i][j].rgbtRed, image[i][j].rgbtGreen, image[i][j].rgbtBlue);
            p_image[i][j].rgbtRed = image[i][j].rgbtRed;
            p_image[i][j].rgbtGreen = image[i][j].rgbtGreen;
            p_image[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
        // printf("\n");
    }

    // examine each pixel in the image
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            // reset color channel value accumulators for each pixel
            r = 0.0, b = 0.0, g = 0.0;
            count = 0;

            // Convolution kernel that averages channel values for neighboring pixels
            for (k = -rad; k <= rad; k++)
            {
                for (l = -rad; l <= rad; l++)
                {
                    // Account for corner cases and edge cases by ensuring we don't read values beyond image border
                    if ((i + k < height) && (i + k >= 0) && (j + l < width) && (j + l >= 0))
                    {
                        // accumulate the totals for surrounding pixels
                        r += p_image[i + k][j + l].rgbtRed;
                        g += p_image[i + k][j + l].rgbtGreen;
                        b += p_image[i + k][j + l].rgbtBlue;
                        count++;
                    }
                }
            }

            // calculate average by dividing total by the number of pixels
            // and set each channel to new, rounded value
            image[i][j].rgbtRed = round(r / count);
            image[i][j].rgbtGreen = round(g / count);
            image[i][j].rgbtBlue = round(b / count);
        }
    }
    // printf("center pixel: (%d, %d, %d)\n", image[1][1].rgbtRed, image[1][1].rgbtGreen, image[1][1].rgbtBlue);

    // free the allocated memory
    for (i = 0; i < height; i ++)
    {
        free(p_image[i]);
    }
    free(p_image);
    return;
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // indices for image and kernel
    int i, j, k, l;

    // radius of the kernel size: 1 is 3x3
    int rad = 1;

    // Convolution kernels
    int gx[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int gy[3][3] =
    {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    //accumulation values for each color channel
    int r_gx, b_gx, g_gx;
    int r_gy, g_gy, b_gy;

    // weighted values for each channel
    double r, g, b;

    // allocate memory for image copy
    RGBTRIPLE **p_image = malloc(height * sizeof(RGBTRIPLE *));
    for (i = 0; i < height; i++)
    {
        p_image[i] = malloc(width * sizeof(RGBTRIPLE));
    }

    // make a copy of the image to save the original pixel values
    for (i = 0; i < height; i++)
    {
        // printf("row %d: ", i);
        for (j = 0; j < width; j++)
        {
            // printf("(%d, %d, %d) ", image[i][j].rgbtRed, image[i][j].rgbtGreen, image[i][j].rgbtBlue);
            p_image[i][j].rgbtRed = image[i][j].rgbtRed;
            p_image[i][j].rgbtGreen = image[i][j].rgbtGreen;
            p_image[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
        // printf("\n");
    }

    // examine each pixel in the image
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            // reset color channel value accumulators for each pixel
            r_gx = 0, g_gx = 0, b_gx = 0;
            r_gy = 0, g_gy = 0, b_gy = 0;

            // account for edge/corner cases by setting pixels outside of image boundaries to 0
            double img_r_val = 0, img_g_val = 0, img_b_val = 0;

            // Convolution kernel that averages channel values for neighboring pixels
            for (k = -rad; k <= rad; k++)
            {
                for (l = -rad; l <= rad; l++)
                {

                    // if the pixel we are reading is within the boundaries of the image
                    // then assign the value of the pixel itself instead
                    if ((i + k >= 0) && (i + k < height) && (j + l >= 0) && (j + l < width))
                    {
                        img_r_val = p_image[i + k][j + l].rgbtRed;
                        img_g_val = p_image[i + k][j + l].rgbtGreen;
                        img_b_val = p_image[i + k][j + l].rgbtBlue;
                    }
                    else
                    {
                        img_r_val = 0;
                        img_g_val = 0;
                        img_b_val = 0;
                    }

                    // the value of the current pixel is multiplied by the corresponding value in the kernel
                    int gx_val = gx[k + 1][l + 1];
                    int gy_val = gy[k + 1][l + 1];

                    // for each color channel, accumulate the totals for surrounding pixels

                    // weighted values along x axis
                    r_gx += img_r_val * gx_val;
                    g_gx += img_g_val * gx_val;
                    b_gx += img_b_val * gx_val;

                    // weighted values along y axis
                    r_gy += img_r_val * gy_val;
                    g_gy += img_g_val * gy_val;
                    b_gy += img_b_val * gy_val;
                }
            }

            // square Gx & Gy to ensure the magnitude we end up with is a positive value
            // take the square root to normalize
            r = sqrt(pow(r_gx, 2) + pow(r_gy, 2));
            g = sqrt(pow(g_gx, 2) + pow(g_gy, 2));
            b = sqrt(pow(b_gx, 2) + pow(b_gy, 2));

            // printf("%f %f %f\n", r, g, b);

            // Pixel values are only 1 byte large, so cap all values at 255
            r = cap(r);
            g = cap(g);
            b = cap(b);

            // printf("%f %f %f\n", r, g, b);

            // set each channel to new, rounded value
            image[i][j].rgbtRed = round(r);
            image[i][j].rgbtGreen = round(g);
            image[i][j].rgbtBlue = round(b);

            // printf("%d %d %d\n", image[i][j].rgbtRed, image[i][j].rgbtGreen, image[i][j].rgbtBlue);
        }
    }
    // printf("center pixel: (%d, %d, %d)\n", image[1][1].rgbtRed, image[1][1].rgbtGreen, image[1][1].rgbtBlue);
    // printf("%d %d %d\n", image[0][0].rgbtRed, image[0][0].rgbtGreen, image[0][0].rgbtBlue);
    // free the allocated memory
    for (i = 0; i < height; i ++)
    {
        free(p_image[i]);
    }
    free(p_image);
    return;
}

double cap(double n)
{
    double val = n;
    if (val > 255.0)
    {
        val = 255.0;
    }
    return val;
}

// save this for how to mirror an image across horizontal access
// void mirror(int height, int width, RGBTRIPLE image[height][width])
// {
//     height = height-1;
//     width = width-1;
//     // traverse through the matrix
//     for (int i = 0; i < height; i++)
//     {
//         for (int j = 0; j < width/2; j++)
//         {
//             image[i][j].rgbtBlue = image[i][width-j].rgbtBlue;
//             image[i][j].rgbtGreen = image[i][width-j].rgbtGreen;
//             image[i][j].rgbtRed = image[i][width-j].rgbtRed;
//         }
//     }
//     return;
// }


