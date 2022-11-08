#include <cs50.h>
#include <stdio.h>

// Putting the first line of a function below main tricks the compiler so it can run
int get_size(void);
void print_grid(int height);

int main(void)
{
    int height = get_size();
    // printf("Height: %d\n", height);
    print_grid(height);
}

// Ask the user for size
int get_size(void)
{
    int n;
    do
    {
        n = get_int("Enter number of rows: ");
    }
    while ((n < 1) || (n > 8));
    return n;
}

/*
    This prints a grid like
        #  #
       ##  ##
      ###  ###
     ####  ####
    #####  #####
*/
void print_grid(int size)
{
    // outer loop will create size number of rows
    for (int i = 1; i <= size; i++)
    {
        // print spaces
        for (int j = 1; j <= size - i; j++)
        {
            printf(" ");
        }
        // print left hashes
        for (int k = 1; k <= size + i - size; k++)
        {
            printf("#");
        }
        // print gap
        for (int l = 1; l <= 2; l++)
        {
            printf(" ");
        }
        // print right hashes
        for (int m = 1; m <= size + i - size; m++)
        {
            printf("#");
        }
        printf("\n");
    }
}