#include <cs50.h>
#include <stdio.h>
#include <string.h>

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

// for debugging only
int num_voters;
int alice = 0;
int bob = 1;
int charlie = 2;

int alice_pos = 2;
int bob_pos = 0;
int charlie_pos = 1;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

// for debugging only
void print_arrays(int ranks[]);
void set_position(int ranks[]);

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

    // for debugging only
    num_voters = voter_count;

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

        set_position(ranks);  // for debugging only
        record_preferences(ranks);
        print_arrays(ranks);    // for debugging only
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
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
// for debugging only
void print_arrays(int ranks[])
{

    printf("candidates[]: ");

    for (int i = 0; i < candidate_count; i++)
    {
        printf("%s ", candidates[i]);
    }
    printf("\n");

    printf("ranks[]: ");
    for (int i = 0; i < candidate_count; i++)
    {
        printf("%i ", ranks[i]);
    }
    printf("\n");

    for (int i = 0; i < candidate_count; i++)
    {
        printf("preferences[]: ");
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i ", preferences[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


// Called once for each voter and records their rankings for all of the candidates
// preferences[i][j] should represent the number of voters who prefer candidate i over candidate j.
// Update the preferences array based on the current voter's ranks
void record_preferences(int ranks[])
{
    int ranks_len = candidate_count;
    int val = 0;

    for (int i = 0; i < ranks_len; i++)
    {
        for (int j = 0; j < ranks_len; j++)
        {
            // fill in Alice's row
            if (ranks[i] == alice && ranks[j] == bob)
            {
                // if alice is preferred over bob
                if (alice_pos < bob_pos)
                {
                    preferences[ranks[i]][ranks[j]] += 1;
                    printf("alice preferred over bob\n");
                }
            }

            if (ranks[i] == alice && ranks[j] == charlie)
            {
                // if alice is preferred over charlie
                if (alice_pos < charlie_pos)
                {
                    preferences[ranks[i]][ranks[j]] += 1;
                    printf("alice preferred over charlie\n");
                }
            }

            // fill in Bob's row
            if (ranks[i] == bob && ranks[j] == alice)
            {
                // if bob is preferred over alice
                if (bob_pos < alice_pos)
                {
                    preferences[ranks[i]][ranks[j]] += 1;
                    printf("bob preferred over alice\n");
                }
            }

            if (ranks[i] == bob && ranks[j] == charlie)
            {
                // if bob is preferred over charlie
                if (bob_pos < charlie_pos)
                {
                    preferences[ranks[i]][ranks[j]] += 1;
                    printf("bob preferred over charlie\n");
                }
            }

            // fill in Charlie's row
            if (ranks[i] == charlie && ranks[j] == alice)
            {
                // if charlie is preferred over alice
                if (charlie_pos < alice_pos)
                {
                    preferences[ranks[i]][ranks[j]] += 1;
                    printf("charlie preferred over alice\n");
                }
            }

            if (ranks[i] == charlie && ranks[j] == bob)
            {
                // if charlie is preferred over bob
                if (charlie_pos < bob_pos)
                {
                    preferences[ranks[i]][ranks[j]] += 1;
                    printf("charlie preferred over bob\n");
                }
            }
        }
    }

    printf("alice position: %d\n", alice_pos);
    printf("bob position: %d\n", bob_pos);
    printf("charlie position: %d\n", charlie_pos);
    return;
}

void set_position(int ranks[])
{
    int ranks_len = candidate_count;

    for (int i= 0; i < ranks_len; i++)
    {
        if (ranks[i] == alice)
        {
            alice_pos = i;
        }

        if (ranks[i] == bob)
        {
            bob_pos = i;
        }

        if (ranks[i] == charlie)
        {
            charlie_pos = i;
        }
    }

    printf("alice position: %d\n", alice_pos);
    printf("bob position: %d\n", bob_pos);
    printf("charlie position: %d\n", charlie_pos);
    return;
}

// video timecode: 13:30
// Record pairs of candidates where one is preferred over the other
// look at each pair of candidates
void add_pairs(void)
{
    // add each pair of candidates to pairs array if one candidate is preferred over the other
    // update global variable pair_count to be the total number of pairs
    return;
}

// Sort pairs in decreasing order by strength of victory
// use any algorithm you'd like
void sort_pairs(void)
{
    // video timecode: 14:45
    return;
}

// Go through pairs one at a time and Lock pairs into the candidate graph in order, without creating cycles
// locked is a
void lock_pairs(void)
{
    // Update locked (2D boolean array) to create the locked graph by adding all edges in
    // decreasing order of victory strength, as long as there is no cycle, skip those
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}