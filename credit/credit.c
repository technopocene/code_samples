// Uses Luhn's checksum algorithm

// 1. Multiply every other digit by 2, starting with the number’s second-to-last digit,

// 2. then add those products’ digits together

// 3. Add the above number to the sum of the digits that weren’t multiplied by 2

// 4. If the total’s last digit is 0 (or, put more formally, if the total modulo 10
// is congruent to 0), the number is valid!

#include <cs50.h>
#include <stdio.h>

long get_number(void);
string checksum(long number);

int main(void)
{
    long number = get_number();
    checksum(number);
    // string company = checksum(number);
    // printf("%s \n", company);
    // printf("%s: %ld \n", company, number);
}

long get_number(void)
{
    long number = get_long("Enter a CC number: ");
    return number;
}

string checksum(long number)
{
    //TODO : do we have to initialize variables with a value here?
    long num = number; // don't want to write over the original variable
    // printf("num: %ld\n", num);
    int num_digits = 0; // count the total number of digits
    int digit; // the rightmost digit
    int product = 0; // ever other digit will be multiplied by 2
    int productLeftDigit = 0; // for adding the products digits together if they are > 10
    int productRightDigit = 0; // for adding the products digits together if they are > 10
    int sumProduct = 0; // sum of the product's digits
    int sumRemaining = 0; // sum of the remaining digits that weren't multiplied by 2
    int sumTotal = 0; // the last digit in this must be 0 for the number to be valid

    do
    {
        // printf("Iteration %i\n", num_digits + 1);
        //printf("number: %li\n", number); // print the remaining number with digits removed
        digit = num % 10; // % 10 gives you the rightmost digit of a long number
        //printf("digit: %i\n", digit);
        num /= 10; // dividing by 10 cuts off last digit
        num_digits ++;
        //printf("num_digits: %i\n", num_digits);
        if (num_digits % 2 == 0) // starting with the number’s second-to-last digit
        {
            product = digit * 2; // i think this is okay?
            // printf("%i * 2 = %i\n", digit, product);
            // we need to add the product's digits, not the products themselves
            // so we are repeating the same algorithm as above to separate the product's digits
            if (product >= 10)
            {
                productLeftDigit = product / 10; // capture leftmost digit of the product
                // printf("product left digit: %i ", productLeftDigit);
                productRightDigit = product % 10; // capture rightmost digit of the product
                // printf("product right digit: %i\n", productRightDigit);
                sumProduct += productLeftDigit + productRightDigit; // finally! adding the 2 digits together
            }
            else
            {
                sumProduct += product;
            }
            //sumProduct += product % 10; // need to add the product's digits, not the products themselves
            // printf("Product sum: %i\n", sumProduct);
        }
        else
        {
            sumRemaining += digit;
            // printf("Remaining sum: %i\n", sumRemaining);
        }
    }
    while (num > 0);
    // printf("input has %i digits\n", num_digits);
    sumTotal = sumProduct + sumRemaining; // the rightmost digit of the total must be 0 to be valid
    // printf("%i + %i = %i\n", sumProduct, sumRemaining, sumTotal);

    // we need these to determine which card company
    int digit_1;
    long num_1 = number; // don't want to write over the original variable
    int digit_2;
    long num_2 = number; // don't want to write over the original variable
    // gotta loop through the digits again, need to create a function that does this
    for (int i = 1; i <= num_digits; i++)
    {
        digit_1 = num_1 % 10;
        num_1 /= 10;
    }
    for (int j = 1; j <= num_digits - 1; j++)
    {
        digit_2 = num_2 % 10;
        num_2 /= 10;
    }
    // printf("digit_1: %i\n", digit_1);
    // printf("digit_2: %i\n", digit_2);

    // only accept 13, 15 and 16 digit numbers
    if ((num_digits == 13) && (sumTotal % 10 == 0)) // 13 digit number & starts w/ 4: visa
    {
        if (digit_1 == 4)
        {
            printf("VISA\n");
            return 0;
        }
        else
        {
            printf("INVALID\n");
            return 0;
        }
    }
    else if ((num_digits == 15) && (sumTotal % 10 == 0))   // 15 digit number & starts w/ 34 or 37: american express
    {
        if ((digit_1 == 3) && ((digit_2 == 4) || (digit_2 == 7)))
        {
            printf("AMEX\n");
            return 0;
        }
        else
        {
            printf("INVALID\n");
            return 0;
        }
    }
    else if ((num_digits == 16)
             && (sumTotal % 10 == 0))   // 16 digit number & starts w/ 51, 52, 53, 54, 55: mastercard | 16 digit number & starts w/ 4: visa
    {
        if (digit_1 == 4)
        {
            printf("VISA\n");
            return 0;
        }
        else if ((digit_1 == 5) && ((digit_2 == 1) || (digit_2 == 2) || (digit_2 == 3) || (digit_2 == 4) || (digit_2 == 5)))
        {
            printf("MASTERCARD\n");
            return 0;
        }
        else
        {
            printf("INVALID\n");
            return 0;
        }
    }
    else
    {
        printf("INVALID\n");
        return 0;
    }
}