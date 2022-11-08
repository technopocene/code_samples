#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// represents candidate graph: locked[i][j] == true means we've locked in the edge pointing from candidate i to candidate j
// also called an "adjacency matrix"
bool locked[MAX][MAX];

int matrix[4][4] =
{
    {10, 11, 12, 13},
    {14, 15, 16, 17},
    {18, 19, 20, 21},
    {22, 23, 24, 25}
};


// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;


// Array of candidates
string candidates[MAX];
// Array of all pairs where one candidate is preferred above another
pair pairs[MAX * (MAX - 1) / 2];    // 36 pairings max


int pair_count;
int candidate_count;


//for debugging only
// int loop_count = 0;
// int cycle_loop_count = 0;


// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
int margin(pair arr[], int idx);
bool has_cycle(int source, int next);
string check_col(int rows, int cols);


// for debugging only
// void print_locked(int len, string label);
// void print_pairs(pair arr[], int len, string label);
// void print_pref(int len, string label);
// void print_1D_array(int arr[], int len, string label);
// int print_pref_col();

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }
        record_preferences(ranks);
    }
    printf("\n");
    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    // print_pref(candidate_count, "preferences"); // for debugging only
    // printf("\n");
    // print_pairs(pairs, pair_count, "pairs"); // for debugging only
    // printf("\n");
    // print_pairs(pairs, pair_count, "sorted pairs"); // for debugging only
    // printf("\n");
    // print_locked(candidate_count, "locked");  // for debugging only
    // printf("\n");
    return 0;
}

// Update ranks given a new vote
// Thisfunction is called every time the user fills in a name
bool vote(int rank, string name, int ranks[])
{
    // here, we will traverse the ranks[] array for each voter
    // we will visit every position in the array and fill it with the the position number of the candidate in the candidates[] array
    for (int i = 0; i < candidate_count; i++)
    {
        // look for a valid name -- strcmp() returns 0 if true
        // the name the user gave should match the candidate's index number
        // But remember this function will run candidate_count times to make sure we only fill in the correct cells for each candidate
        if (strcmp(name, candidates[i]) == 0)
        {
            // rank is 0, 1.. which is mapped to the name the user gave. For example, Alice = 0, Bob = 1, Charlie = 2
            // fill the ranks[] array with the candidate's index numbers, but in the correct ranking position
            ranks[rank] = i;    // i is the ith postiion in the candidates[] array
            return true;    // Indicates a successful ballot
        }
    }
    return false;   // name was not found on ballot
}

// Called once for each voter and records their rankings for all of the candidates
// preferences[i][j] should represent the number of voters who prefer candidate i over candidate j.
// Update the preferences array based on the current voter's ranks
void record_preferences(int ranks[])
{
    // i & j indices will not be used to update the preferences[][] matrix
    // we will loop over each row multiple times if necessary until the voter's preferences are recorded
    // we only need one less outer loop because we don't "visit" every cell in a row
    for (int i = 0; i < candidate_count - 1; i++)
    {
        // j + 1 because when j = i, that's comparing one candidate against themselves
        for (int j = (i + 1); j < candidate_count; j++)
        {
            // each cell keeps a tally of all the voters' preferences
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }
    return;
}

void add_pairs(void)
{
    // add each pair of candidates to pairs array if one candidate is preferred over the other
    // update global variable pair_count to be the total number of pairs

    // traverse the 2d preferences array to find pairs and add them to pair_count
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = (i + 1); j < candidate_count; j++)
        {
            // compare 2 candidate totals in preferences[][] & place the winner and loser in pairs[]
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by margin of victory
// uses pair values as a lookup into preferences array to capture the total votes & calculate margin
// Academic honesty: I wrote the sorting algorithm based on pseudocode from geeksforgeeks.com
void sort_pairs(void)
{
    // traverse the pairs array to sort them by margin
    for (int step = 1; step < pair_count; step++)
    {
        // the second pair in the index is the key, this will be swapped
        pair key = pairs[step];

        // the margin of the key that will be used for comparison
        // index for the pair that we will compare the key to
        int j = (step - 1);

        // Compare key with unsorted pairs
        while (margin(pairs, step) > margin(pairs, j) && j >= 0)
        {
            // If the unsorted pair is greater than key, then key is placed in front of the first element.
            pairs[j + 1] = pairs[j];
            j -= 1;
        }
        // that pair is now the highest
        pairs[j + 1] = key;
    }
    return;
}

// Go through pairs one at a time and Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Update locked (2D boolean array) to create the locked graph by adding all edges in
    // decreasing order of victory strength, as long as there is no cycle, skip those

    // You need a mechanism to detect if adding that edge will lead to a cycle where you could
    // follow some path to get back to the original candidate (like a closed circuit)

    // loop should run for the maximum amount of pairs possible given the number of candidates we have
    int pairs_max = candidate_count * (candidate_count - 1) / 2;

    // keep track of number of edges we've added for debugging
    int n_edges = 0;

    // traverse through the sorted pairs array
    for (int i = 0; i < pairs_max; i++)
    {
        // reset the inner loop count
        // cycle_loop_count = 0;

        // marker for the beginning of the cycle
        int origin = pairs[i].winner;

        // printf("\n---------------- edge %d ----------------\n", n_edges);
        // printf("has_cycle(%d, %d)\n", pairs[i].winner, pairs[i].loser);

        // not having 1st condition fills in true value for locked[0][0], dunno why
        if (pairs[i].winner != pairs[i].loser
            && !has_cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
            n_edges++;
        }
    }

    return;
}

// start with the next node and recursively visit each of its neighbors
bool has_cycle(int source, int next)
{
    // base case 1: If we end up back at the source, there's a cycle
    if (next == source)
    {
        return true;
    }
    /*
    check to see if the loser (next) has won against any other candidate so far
    by checking their row in the locked matrix for any true values
    */
    for (int v = 0; v < candidate_count; v++)
    {
        // if we find a true value, then we check the row(s) for each candidate they beat
        if (locked[next][v] && has_cycle(source, v))
        {
            return true;
        }
    }
    // base case 2: We exhausted our search so we can place the edge
    return false;
}

// Print the winner of the election
// Winner is the source of the graph that has no other nodes pointing to it
void print_winner(void)
{
    int i, j, idx;
    string winner;
    int cols = candidate_count;
    int rows = candidate_count;

    // traverse each column in the matrix to check for true values
    for (j = 0; j < cols; j++)
    {
        for (i = 0; i < rows; i++)
        {
            bool val = locked[i][j];
            // if we reached the end of the column and no true values were found, that's our winner
            if (!locked[i][j] && i == rows - 1)
            {
                idx = j;
                break;
            }
            else if (locked[i][j])
            {
                break;
            }
        }
    }

    winner = candidates[idx];
    printf("%s\n", winner);

    return;
}



// returns the margin of victory between a winner and loser in a pair
int margin(pair arr[], int idx)
{
    // margin is absolute value of the difference in the votes between winner and loser
    int c1 = preferences[arr[idx].winner][arr[idx].loser];
    int c2 = preferences[arr[idx].loser][arr[idx].winner];
    int margin = abs(c1 - c2);

    return margin;
}

// for debugging only
// used to print the locked[] boolean array
// void print_locked(int len, string label)
// {
//     printf("%s\n", label);
//     for (int i = 0; i < len; i++)
//     {
//         printf("[ ");
//         for (int j = 0; j < len; j++)
//         {
//             printf("%d ", locked[i][j]);
//         }
//         printf("]\n");
//     }
// }

// // for debugging only
// // used to print the pairs array
// void print_pairs(pair arr[], int len, string label)
// {
//     printf("%s:\n[ ", label);
//     for (int i = 0; i < len; i++)
//     {
//         printf("(%d,%d) ", arr[i].winner, arr[i].loser);
//     }
//     printf("]\n");
// }

// void print_1D_array(int arr[], int len, string label)
// {
//     printf("%s\n[ ", label);
//     for (int i = 0; i < len; i++)
//     {
//         printf("%d ", arr[i]);
//     }
//     printf("]\n");
// }

// void print_pref(int len, string label)
// {
//     printf("\n%s\n", label);
//     for (int i = 0; i < len; i++)
//     {
//         printf("[ ");
//         for (int j = 0; j < len; j++)
//         {
//             printf("%d ", preferences[i][j]);
//         }
//         printf("]\n");
//     }
// }

// int print_pref_col()
// {
//     int i;
//     int n = 1;
//     printf("column %d\n", n);
//     for (i = 0; i < candidate_count; i++)
//     {
//         printf("[ %d ]\n", preferences[i][n]);
//     }
//     printf("\n\n");
//     return i;
// }

// int print_col(int rows, int cols, int arr[rows][cols])
// {
//     int i;
//     printf("column 1\n");
//     for (i = 0; i < rows; i++)
//     {
//         printf("[ %d ]\n", arr[i][1]);
//     }
//     printf("\n\n");
//     return i;

// }