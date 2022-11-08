#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];


// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);
// void selectionSort(candidate arrCandidates[], int numCandidates);


int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // If name matches one of the names of the candidates in the election, increase candidate's vote count
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            candidates[i].votes ++;
            return true;    // Indicates a successful ballot.
        }
    }
    return false;   // name was not found on ballot
}

// Prints name of the candidate who received the most votes using a comparison algorithm
void print_winner(void)
{
    int i = 0;
    // the first candidate in the index is the starting point all other elements will compare to
    int most_votes = candidates[i].votes;

    // traverse the vote tallies for each candidate to check for a tie
    for (i = 1; i < candidate_count; i++)
    {
        if (candidates[i].votes > most_votes)
        {
            most_votes = candidates[i].votes;
        }
    }

    // print all candidates with the most votes
    for (i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == most_votes)
        {
            printf("%s\n", candidates[i].name);
        }
    }
    return;
}


// // Prints name of the candidate who received the most votes using a selection sort algorithm
// void print_winner(void)
// {
//     int i, j, min_index, most_votes;

//     // traverse the unsorted array
//     for (i = 0; i < (candidate_count - 1); i++)   // you need -1 here because last item is null
//     {
//         // find minimum element in unsorted array, on first pass it's the first element
//         min_index = i;

//         // create a 2nd loop to compare another element
//         for (j = (i + 1); j < candidate_count; j++)
//         {
//             // compare the current element with the next one
//             if (candidates[j].votes > candidates[min_index].votes)
//             {
//                 min_index = j;
//             }

//             else if (min_index != i)
//             {
//                 // Swap the found minimum element with the first element to sort the array by highest to lowest
//                 candidate temp = candidates[min_index];
//                 candidates[min_index] = candidates[i];
//                 candidates[i] = temp;
//             }
//         }
//     }
//     // the first candidate in the index has the most votes
//     most_votes = candidates[0].votes;

//     // traverse the vote tallies for each candidate to check for a tie
//     for (i = 0; i < candidate_count; i++)
//     {

//         if (candidates[i].votes == most_votes)
//         {
//             printf("%s\n", candidates[i].name);     // print all candidates with the most votes
//         }
//     }
//     return;
// }

// algorithm for selection sort that accepts a candidate struct
// void selectionSort(candidate arrCandidates[], int numCandidates)
// {

//      int i, j, minIndex;

//     // traverse the unsorted array
//     for (i = 0; i < (numCandidates - 1); i++)   // you need -1 here because last item is null
//     {
//         // find minimum element in unsorted array, on first pass it's the first element
//         minIndex = i;

//         // create a 2nd loop to compare another element
//         for (j = i + 1; j < numCandidates; j++)
//         {
//             // compare the current element with the next one
//             if (arrCandidates[j].votes > arrCandidates[minIndex].votes)
//             {
//                 minIndex = j;
//             }

//             else if (minIndex != i)
//             {
//                 // Swap the found minimum element with the first element to sort the array by highest to lowest
//                 // swap(candidates[min_index], candidates[i]);
//                 candidate temp = arrCandidates[minIndex];
//                 arrCandidates[minIndex] = arrCandidates[i];
//                 arrCandidates[i] = temp;
//             }
//         }
//         // printf("%s: %d votes", candidates[i].name, candidates[i].votes);
//     }
//     if (arrCandidates[0].votes == arrCandidates[1].votes)
//     {
//         printf("%s\n%s\n", arrCandidates[0].name, arrCandidates[1].name);
//     }
//     else
//     {
//         string winner = arrCandidates[0].name;     // the first candidate in the index has the most votes
//         printf("%s\n", winner);
//     }
//     return;
// }