// STANDARD CODE FOR SELECTION SORT
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 100

int myArray[MAX];
int len = 5;
int newArray[MAX];


// declare function prototypes
int findSmallest(int arr[], int length);
int *selectionSort(int arr[], int length);
void printArray(int arr[], int length);

int main(void)
{

    for (int i = 0; i < len; i++)
    {
        myArray[i] = get_int("number: ");
    }

    printf("%d\n", findSmallest(myArray, len));
    selectionSort(myArray, len);
    printArray(myArray, len);
    printArray(newArray, len);

    // selectionSort(&myArray, 5)

    return 0;
}

int findSmallest(int arr[], int length)
{
    int smallestValue = arr[0];     // smallest value so far
    int smallestIndex = 0;

    for (int i = 1; i < length; i++)
    {
        if (arr[i] < smallestValue)
        {
            smallestValue = arr[i];
            smallestIndex = i;
        }
    }
    return smallestIndex;
}

int *selectionSort(int arr[], int length)
{
    int smallest = findSmallest(arr, length);

    for (int i = 1; i < length; i++)
    {
        smallest = findSmallest(arr, length);
        newArray[i] = smallest;
    }
    return newArray;
}

void printArray(int arr[], int length)
{
    printf("[ ");
    for (int i = 0; i < length; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("]\n");
}



// // STANDARD CODE FOR BINARY SEACH
// #include <stdio.h>
// #include <stdlib.h>

// // declare function prototypes
// int binarySearch(int arr[], int len, int item);

// int main()
// {
//     int myArray[] = {1, 3, 5, 7, 9};
//     int len = sizeof(myArray) / sizeof(myArray[0]);  // array length = byte size of array / byte size of a single element
//     int result = binarySearch(myArray, len, 3);
//     printf("%d\n", result);

//     return 0;
// }

// int binarySearch(int arr[], int len, int item)
// {
//     int low = 0;    // low and high keep track of which part of the list you'll search in
//     int high = len - 1;
//     int mid = low + high;
//     int guess = arr[mid];


//     while (low <= high)     // while you haven't narrowed it down to one element
//     {
//         mid = low + high;       // check the middle element
//         guess = arr[mid];

//         if (guess == item)
//         {
//             return mid;     // found the item
//         }
//         if (guess > item)
//         {
//             high = mid - 1;     // guess was too high
//         }
//         else
//         {
//             low = mid + 1;      // guess was too low
//         }
//     }
//     return 5;       // item doesn't exist
// }

// // STANDARD CODE FOR INSERTION SORT


// // C program for insertion sort
// #include <math.h>
// #include <stdio.h>

// /* Function to sort an array using insertion sort*/
// void insertionSort(int arr[], int n)
// {
//     int i, key, j;
//     for (i = 1; i < n; i++) {
//         key = arr[i];
//         j = i - 1;

//         /* Move elements of arr[0..i-1], that are
//           greater than key, to one position ahead
//           of their current position */
//         while (j >= 0 && arr[j] > key) {
//             arr[j + 1] = arr[j];
//             j = j - 1;
//         }
//         arr[j + 1] = key;
//     }
// }

// // A utility function to print an array of size n
// void printArray(int arr[], int n)
// {
//     int i;
//     for (i = 0; i < n; i++)
//         printf("%d ", arr[i]);
//     printf("\n");
// }

/* Driver program to test insertion sort */
// int main()
// {
//     int arr[] = { 12, 11, 13, 5, 6 };
//     int n = sizeof(arr) / sizeof(arr[0]);

//     insertionSort(arr, n);
//     printArray(arr, n);

//     return 0;
// }





// STANDARD CODE FOR QUICKSORT

// #include <stdio.h>

// // Function prototypes
// void swap(int *a, int *b);
// void quicksort(int arr[], int low, int high);
// int partition(int arr[], int low, int high);
// void print_array(int arr[], int len);

// int main()
// {
// 	int arr[] = {10, 80, 30, 90, 40, 50, 70};
//   int n = sizeof(arr) / sizeof(arr[0]);
//   printf("%d\n", n);
//   print_array(arr, n);
//   quicksort(arr, 0, (n - 1));
//   print_array(arr, n);
// 	return 0;
// }

// void swap(int *a, int *b)
// {
// 	int temp = *a;
// 	*a = *b;
// 	*b = temp;
// }

// /*
//   The main function that implements QuickSort
//       arr[] --> Array to be sorted,
//       low --> Starting index,
//       high --> Ending index
// */
// void quicksort(int arr[], int low, int high)
// {
//     if (low < high)
//     {
//         // pi is partitioning index, arr[pi] is now at the right place
//         int p_idx = partition(arr, low, high);

//         // recursively sort both the left and right side of the array
//         quicksort(arr, low, p_idx - 1); // before partition index
//         quicksort(arr, p_idx + 1, high); // after partition index
//     }
// }

// // partition() picks last element as a pivot
// // places pivot at its correct position in sorted array
// // places all smaller elements to the left of p & greater elements to the right of p
// int partition(int arr[], int low, int high)
// {
//     int p = arr[high];  // pivot
//     int i = (low - 1);  // index of smaller element. Indicates the right position of pivot found so far

//     //start with leftmost element
//     // keep track of smaller (or equal to) elements as i
//     for (int j = low; j <= (high - 1); j++)
//     {
//       // if current element is smaller than pivot
//         if (arr[j] < p)
//         {
//             // increment index of smaller element
//             i++;
//             //  swap the current element with arr[i]
//             swap(&arr[i], &arr[j]);
//         }
//     }
//     swap(&arr[i + 1], &arr[high]);
//     return (i + 1);
// }

// void print_array(int arr[], int len)
// {
//     printf("sorted array: [ ");
//     for (int i = 0; i < len; i++)
//     {
//         printf("%d, ", arr[i]);
//     }
//     printf("]\n");
// }