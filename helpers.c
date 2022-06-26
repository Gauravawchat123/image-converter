#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    uint8_t gray;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // finding avg of rgb component and update original image rbg value with avg rgb value.
            gray = round((image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / 3.0);
            image[i][j].rgbtBlue = gray;
            image[i][j].rgbtRed = gray;
            image[i][j].rgbtGreen = gray;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // swaping first pixel with last one and vice versa and so on.
    for (int i = 0; i < height; i++)
    {
        int m = 0, n = width - 1;
        RGBTRIPLE temp;
        while (m < n)
        {
            temp = image[i][m];
            image[i][m] = image[i][n];
            image[i][n] = temp;
            m++;
            n--;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int Red = 0, Green = 0, Blue = 0;
    int count = 0;

    // creating copy of original image to perform opeartion on it.
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // finding avg rgb value of all connected cell for given element.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // start counting red, green, blue from selected element.
            Red = Red + copy[i][j].rgbtRed;
            Green = Green + copy[i][j].rgbtGreen;
            Blue = Blue + copy[i][j].rgbtBlue;

            count += 1;

            // to check left side of selected element
            if (j - 1 >= 0)
            {
                Red = Red + copy[i][j - 1].rgbtRed;
                Green = Green + copy[i][j - 1].rgbtGreen;
                Blue = Blue + copy[i][j - 1].rgbtBlue;

                count += 1;
            }

            // to check right side of selected element;
            if (j + 1 <= width - 1)
            {
                Red = Red + copy[i][j + 1].rgbtRed;
                Green = Green + copy[i][j + 1].rgbtGreen;
                Blue = Blue + copy[i][j + 1].rgbtBlue;

                count += 1;
            }

            // to check upper side of selected element
            if (i - 1 >= 0)
            {
                Red = Red + copy[i - 1][j].rgbtRed;
                Green = Green + copy[i - 1][j].rgbtGreen;
                Blue = Blue + copy[i - 1][j].rgbtBlue;

                count += 1;
            }

            // to check lower side of selected element
            if (i + 1 <= height - 1)
            {
                Red = Red + copy[i + 1][j].rgbtRed;
                Green = Green + copy[i + 1][j].rgbtGreen;
                Blue = Blue + copy[i + 1][j].rgbtBlue;

                count += 1;
            }

            // to check upper right digonal element
            if (i - 1 >= 0 && j + 1 <= width - 1)
            {
                Red = Red + copy[i - 1][j + 1].rgbtRed;
                Green = Green + copy[i - 1][j + 1].rgbtGreen;
                Blue = Blue + copy[i - 1][j + 1].rgbtBlue;

                count += 1;
            }

            //to check upper left digonal element
            if (i - 1 >= 0 &&  j - 1 >= 0)
            {
                Red = Red + copy[i - 1][j - 1].rgbtRed;
                Green = Green + copy[i - 1][j - 1].rgbtGreen;
                Blue = Blue + copy[i - 1][j - 1].rgbtBlue;

                count += 1;
            }

            // to check lower right digonal element
            if (i + 1 <= height - 1 && j + 1 <= width - 1)
            {
                Red = Red + copy[i + 1][j + 1].rgbtRed;
                Green = Green + copy[i + 1][j + 1].rgbtGreen;
                Blue = Blue + copy[i + 1][j + 1].rgbtBlue;

                count += 1;
            }

            // to check lower left digonal element
            if (i + 1 <= height - 1 && j - 1 >= 0)
            {
                Red = Red + copy[i + 1][j - 1].rgbtRed;
                Green = Green + copy[i + 1][j - 1].rgbtGreen;
                Blue = Blue + copy[i + 1][j - 1].rgbtBlue;

                count += 1;
            }

            // update selected element value with new avg value of all connected cell to selected element in image array.
            image[i][j].rgbtRed = round(Red / (float)count);
            image[i][j].rgbtGreen = round(Green / (float)count);
            image[i][j].rgbtBlue = round(Blue / (float)count);

            // Reset red, green, blue for next element.
            Red = 0, Green = 0, Blue = 0;

            // reset count.
            count = 0;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int GxRed = 0, GxBlue = 0, GxGreen = 0, GyRed = 0, GyBlue = 0, GyGreen = 0;
    int newRed, newBlue, newGreen;

    // creating copy of original image to perform opeartion on it.
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // finding sum of multiplication of given cell and corresponding Gx and Gy value.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // start counting red, green, blue from selected element.
            GxRed += copy[i][j].rgbtRed * Gx[1][1];
            GxBlue += copy[i][j].rgbtBlue * Gx[1][1];
            GxGreen += copy[i][j].rgbtGreen * Gx[1][1];

            GyRed += copy[i][j].rgbtRed * Gy[1][1];
            GyBlue += copy[i][j].rgbtBlue * Gy[1][1];
            GyGreen += copy[i][j].rgbtGreen * Gy[1][1];

            // to check left side of selected element
            if (j - 1 >= 0)
            {
                GxRed += copy[i][j - 1].rgbtRed * Gx[1][0];
                GxBlue += copy[i][j - 1].rgbtBlue * Gx[1][0];
                GxGreen += copy[i][j - 1].rgbtGreen * Gx[1][0];

                GyRed += copy[i][j - 1].rgbtRed * Gy[1][0];
                GyBlue += copy[i][j - 1].rgbtBlue * Gy[1][0];
                GyGreen += copy[i][j - 1].rgbtGreen * Gy[1][0];
            }

            // to check right side of selected element;
            if (j + 1 <= width - 1)
            {
                GxRed += copy[i][j + 1].rgbtRed * Gx[1][2];
                GxBlue += copy[i][j + 1].rgbtBlue * Gx[1][2];
                GxGreen += copy[i][j + 1].rgbtGreen * Gx[1][2];

                GyRed += copy[i][j + 1].rgbtRed * Gy[1][2];
                GyBlue += copy[i][j + 1].rgbtBlue * Gy[1][2];
                GyGreen += copy[i][j + 1].rgbtGreen * Gy[1][2];
            }

            // to check upper side of selected element
            if (i - 1 >= 0)
            {
                GxRed += copy[i - 1][j].rgbtRed * Gx[0][1];
                GxBlue += copy[i - 1][j].rgbtBlue * Gx[0][1];
                GxGreen += copy[i - 1][j].rgbtGreen * Gx[0][1];

                GyRed += copy[i - 1][j].rgbtRed * Gy[0][1];
                GyBlue += copy[i - 1][j].rgbtBlue * Gy[0][1];
                GyGreen += copy[i - 1][j].rgbtGreen * Gy[0][1];
            }

            // to check lower side of selected element
            if (i + 1 <= height - 1)
            {
                GxRed += copy[i + 1][j].rgbtRed * Gx[2][1];
                GxBlue += copy[i + 1][j].rgbtBlue * Gx[2][1];
                GxGreen += copy[i + 1][j].rgbtGreen * Gx[2][1];

                GyRed += copy[i + 1][j].rgbtRed * Gy[2][1];
                GyBlue += copy[i + 1][j].rgbtBlue * Gy[2][1];
                GyGreen += copy[i + 1][j].rgbtGreen * Gy[2][1];
            }

            // to check upper right digonal element
            if (i - 1 >= 0 && j + 1 <= width - 1)
            {
                GxRed += copy[i - 1][j + 1].rgbtRed * Gx[0][2];
                GxBlue += copy[i - 1][j + 1].rgbtBlue * Gx[0][2];
                GxGreen += copy[i - 1][j + 1].rgbtGreen * Gx[0][2];

                GyRed += copy[i - 1][j + 1].rgbtRed * Gy[0][2];
                GyBlue += copy[i - 1][j + 1].rgbtBlue * Gy[0][2];
                GyGreen += copy[i - 1][j + 1].rgbtGreen * Gy[0][2];
            }

            //to check upper left digonal element
            if (i - 1 >= 0 &&  j - 1 >= 0)
            {
                GxRed += copy[i - 1][j - 1].rgbtRed * Gx[0][0];
                GxBlue += copy[i - 1][j - 1].rgbtBlue * Gx[0][0];
                GxGreen += copy[i - 1][j - 1].rgbtGreen * Gx[0][0];

                GyRed += copy[i - 1][j - 1].rgbtRed * Gy[0][0];
                GyBlue += copy[i - 1][j - 1].rgbtBlue * Gy[0][0];
                GyGreen += copy[i - 1][j - 1].rgbtGreen * Gy[0][0];
            }

            // to check lower right digonal element
            if (i + 1 <= height - 1 && j + 1 <= width - 1)
            {
                GxRed += copy[i + 1][j + 1].rgbtRed * Gx[2][2];
                GxBlue += copy[i + 1][j + 1].rgbtBlue * Gx[2][2];
                GxGreen += copy[i + 1][j + 1].rgbtGreen * Gx[2][2];

                GyRed += copy[i + 1][j + 1].rgbtRed * Gy[2][2];
                GyBlue += copy[i + 1][j + 1].rgbtBlue * Gy[2][2];
                GyGreen += copy[i + 1][j + 1].rgbtGreen * Gy[2][2];
            }

            // to check lower left digonal element
            if (i + 1 <= height - 1 && j - 1 >= 0)
            {
                GxRed += copy[i + 1][j - 1].rgbtRed * Gx[2][0];
                GxBlue += copy[i + 1][j - 1].rgbtBlue * Gx[2][0];
                GxGreen += copy[i + 1][j - 1].rgbtGreen * Gx[2][0];

                GyRed += copy[i + 1][j - 1].rgbtRed * Gy[2][0];
                GyBlue += copy[i + 1][j - 1].rgbtBlue * Gy[2][0];
                GyGreen += copy[i + 1][j - 1].rgbtGreen * Gy[2][0];
            }

            // update selected element value with new R,G,B value which is obtain by sum of R,G,B(multiplying each connected cell to its respected Gx and Gy).
            newRed = round(sqrt((GxRed * GxRed) + (GyRed * GyRed)));
            newBlue = round(sqrt((GxBlue * GxBlue) + (GyBlue * GyBlue)));
            newGreen = round(sqrt((GxGreen * GxGreen) + (GyGreen * GyGreen)));

            if (newRed > 255)
            {
                newRed = 255;
            }
            if (newBlue > 255)
            {
                newBlue = 255;
            }
            if (newGreen > 255)
            {
                newGreen = 255;
            }
            image[i][j].rgbtRed = newRed;
            image[i][j].rgbtGreen = newGreen;
            image[i][j].rgbtBlue = newBlue;

            // Reset red, green, blue for next element.
            GxRed = 0, GxGreen = 0, GxBlue = 0, GyRed = 0, GyGreen = 0, GyBlue = 0;

        }
    }
    return;
}
