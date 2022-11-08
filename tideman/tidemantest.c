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
int cells_filled = 0;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void swap(pair *a, pair *b);
void quicksort(pair arr[], int low, int high);
pair partition(pair arr[], int low, int high);
int margin (pair arr[], int idx);

// for debugging only
void print_pairs(pair arr[], int len);
void print_locked(int len, string label);
void print_pref(int len, string label);

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

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_pref(candidate_count, "preferences"); // for debugging only
    printf("\n");
    printf("preferences cells visited: %d\n", cells_filled);
    // print_pairs(pairs, pair_count); // for debugging only
    printf("\n");
    // print_locked(candidate_count, "locked");  // for debugging only
    printf("\n");
    // printf("bob vs charlie pair: (%d, %d)\n", pairs[2].winner, pairs[2].loser);
    // printf("bob vs charlie locked value: %d\n", locked[1][2]);
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // traverse the candidate array
    for (int i = 0; i < candidate_count; i++)
    {
        // look for a valid name
        if (strcmp(name, candidates[i]) == 0)
        {
            // if found, set ranks[] = 0, 1, or 2
            // i is the ith postiion in the candidates array
            ranks[rank] = i;
            return true;    // Indicates a successful ballot.
        }
    }
    return false;   // name was not found on ballot
}

// Called once for each voter and records their rankings for all of the candidates
// preferences[i][j] should represent the number of voters who prefer candidate i over candidate j.
// Update the preferences array based on the current voter's ranks
// void record_preferences(int ranks[])
// {
//     int ranks_len = candidate_count;    // every voter needs to vote for every candidate

//     int outer_loop_count = 1;
//     int inner_loop_count = 1;

//     for (int i = 0; i < ranks_len - 1; i++)
//     {
//       printf("\nouter loop %d:\n", outer_loop_count);
//         for (int j = (i + 1); j < ranks_len; j++)
//         {
//             printf("  inner loop %d:\n", inner_loop_count);
//             preferences[ranks[i]][ranks[j]] += 1;
//             printf("      preferences[ranks[%d]][ranks[%d]] += 1;\n", i, j);
//             cells_filled++;

//             inner_loop_count++;
//         }
//         outer_loop_count++;
//     }

//     return;
// }

// This function is called for each voter to tally up their preferred candidates
void record_preferences(int ranks[])
{
    int ranks_len = candidate_count;    // every voter needs to vote for every candidate

    int outer_loop_count = 1;
    int inner_loop_count = 1;

    for (int i = 0; i < ranks_len; i++) // ranks length minus 1 because we only need to traverse each row twice
    {
      inner_loop_count = 1; // reset the inner loop count
      printf("\nouter loop %d:  i = %d\n", outer_loop_count, i);
        for (int j = (i + 1); j < ranks_len; j++)
        {
            printf("  inner loop %d:  i = %d  j = %d\n", inner_loop_count, i, j);
            preferences[ranks[i]][ranks[j]] += 1;
            printf("      cell [%d][%d] gets one point\n", ranks[i], ranks[j]);
            cells_filled++;

            inner_loop_count++;
        }
        outer_loop_count++;
    }

    return;
}

void add_pairs(void)
{
    // add each pair of candidates to pairs array if one candidate is preferred over the other
    // update global variable pair_count to be the total number of pairs
    // printf("pairs: ");    // for debugging only

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
                pair_count += 1;
                // printf("(%d, %d) ", pairs[pair_count].winner, pairs[pair_count].loser);    // for debugging only
                // printf("winner: %s\n", candidates[i]);  // for debugging only
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count += 1;
                // printf("(%d, %d) ", pairs[pair_count].winner, pairs[pair_count].loser);    // for debugging only
                // printf("winner: %s\n", candidates[j]);  // for debugging only
            }
        }
    }
    // printf("pair_count: %d\n", pair_count);    // for debugging only
    // printf("\n");   // for debugging only
    return;
}

// Sort pairs in decreasing order by margin of victory
// uses pair values as a lookup into preferences array to capture the total votes & calculate margin
void sort_pairs(void)
{
    // traverse the pairs array to sort them by margin
    for (int step = 1; step < pair_count; step++)
    {
        // the second pair in the index is the key, this will be swapped
        pair key = pairs[step];

        // the margin of the key that will be used for comparison
        // int key_margin = margin(pairs, step);
        // printf("margin: %d\n", key_margin); // for debugging only

        // index for the pair that we will compare the key to
        int j = (step - 1);

        // Compare key with unsorted pairs
        while (margin(pairs, step) > margin(pairs, j) && j >=0)
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

    // represents candidate graph: locked[i][j] == true means we've locked in the edge pointing from candidate i to candidate j
        // also called an "adjacency matrix"
        // bool locked[MAX][MAX];
    // int i_loop = 0;
    // int j_loop = 0;
    // traverse the the locked[][] boolean array and use the values from the sorted_pairs[] array to populate it
    for (int i = 0; i < candidate_count; i++)
    {
        // printf("outer loop %d\n", i+1);
        for (int j = (i + 1); j < candidate_count; j++)
        {
            // printf("    inner loop %d:\n", j+1);

            // candidates can't be compared to oneanother
            if ( i == j)
            {
                locked[i][j] = false;
            }
            // determine the winner from the sorted pairs array and fill their cell with true and the loser's cell with false
            else if (i == pairs[i].winner && j == pairs[i].loser)
            {
                locked[i][j] = true;
            }
            // else if (i == pairs[i].loser)
            // {
            //     locked[i][j] = false;
            // }
            // j_loop++;
            // printf("        does %d == %d && %d == %d ?  locked[%d][%d] = %d\n", i, pairs[i].winner, j, pairs[i].loser, i, j, locked[i][j]);
        }
        // i_loop++;
    }
    // printf("num of inner loops: %d\n", i_loop);
    // printf("num of j loops: %d\n", j_loop);
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}

// Swap 2 structs using pointers
void swap(pair *a, pair *b)
{
	pair t = *a;
	*a = *b;
	*b = t;
}

// returns the margin of victory between a winner and loser in a pair
int margin (pair arr[], int idx)
{
    // margin is absolute value of the difference in the votes between winner and loser
    int c1 = preferences[arr[idx].winner][arr[idx].loser];
    int c2 = preferences[arr[idx].loser][arr[idx].winner];
    int margin = abs(c1 - c2);

    return margin;
}

// for debugging only
// used to print the pairs array
void print_pairs(pair arr[], int len)
{
    printf("sorted pairs:\n[ ");
    for (int i = 0; i < len; i++)
    {
        printf("(%d,%d) ", arr[i].winner, arr[i].loser);
    }
    printf("]\n");
}

// for debugging only
// used to print the locked[] boolean array
void print_locked(int len, string label)
{
    printf("%s\n", label);
    for (int i = 0; i < len; i++)
    {
        printf("[ ");
        for (int j = 0; j < len; j++)
        {
            printf("%i ", locked[i][j]);
        }
        printf("]\n");
    }
}

void print_pref(int len, string label)
{
    printf("\n%s\n", label);
    for (int i = 0; i < len; i++)
    {
        printf("[ ");
        for (int j = 0; j < len; j++)
        {
            printf("%i ", preferences[i][j]);
        }
        printf("]\n");
    }
}
