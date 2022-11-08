#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;

// declare function prototypes
int jpg_sig(BYTE buffer[]);

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover filename.raw\n");
        return 1;
    }

    // create buffer needed for output file
    int BLOCK_SIZE = 512;
    BYTE buffer[BLOCK_SIZE];
    FILE *img = NULL;

    // jpeg file names should be dynamically named in consecutive order
    char filename[8] = "###.jpg";
    int num_jpegs = 0;

    // Open memory card file
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // dynamically create file name
    sprintf(filename, "%03i.jpg", num_jpegs);   // ex. 001.jpg

    //open a new file
    img = fopen(filename, "w");
    if (img == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Read 512 bytes at a time and repeat until the end of the card
    while (fread(&buffer, 1, BLOCK_SIZE, input) == BLOCK_SIZE)
    {
        // if start of new jpeg (the first 4 bytes match a specified pattern)
        if (jpg_sig(buffer))
        {
            // if it's the first jpeg we've found
            if (num_jpegs == 0)
            {
                // write data to file
                fwrite(&buffer, 1, BLOCK_SIZE, img);
            }

            // if this isn't the first jpeg we've found
            else
            {
                // close the file because we've found all the data
                fclose(img);

                // dynamically create file name
                sprintf(filename, "%03i.jpg", num_jpegs);   // ex. 001.jpg

                //open a new file
                img = fopen(filename, "w");
                if (img == NULL)
                {
                    printf("Could not open file.\n");
                    return 1;
                }

                // write data to file
                fwrite(&buffer, 1, BLOCK_SIZE, img);
            }
            // increment file count
            num_jpegs++;
        }

        // if it's not the beginning of a new jpeg, keep writing to the current file
        else if (num_jpegs > 0)
        {
            // write data to file
            fwrite(&buffer, 1, BLOCK_SIZE, img);
        }
    }

    // close open files
    fclose(input);
    fclose(img);
    // printf("found %d JPEGS\n", num_jpegs);
}

// Checks to see if the first 4 bytes are indiciative of a JPEG signature
// JPEGS do not embed the length of the file
int jpg_sig(BYTE buffer[])
{
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
    {
        return 1;
    }
    return 0;
}
