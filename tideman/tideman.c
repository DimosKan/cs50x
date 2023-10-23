#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
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
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool contains_loop(int current, int origin);

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

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
//Like the readability exercise, it passes through all candidates and checks if the names exists.
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int r = 0; r < candidate_count; r++)
    {
        for (int rr = r + 1; rr < candidate_count; rr++)
        {
            preferences[ranks[r]][ranks[rr]] ++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int m = 0; m < candidate_count; m++)
    {
        for (int mm = m + 1; mm < candidate_count; mm++)
        {
            if (preferences[m][mm] > preferences[mm][m])
            {
                pairs[pair_count].winner = m;
                pairs[pair_count].loser = mm;
                pair_count++;
            }
            else if (preferences[m][mm] < preferences[mm][m])
            {
                pairs[pair_count].winner = mm;
                pairs[pair_count].loser = m;
                pair_count++;
            }
        }
    }
}

// looping over all the candidates and uses recurse in order to check checking values between the current node and the candidate node
bool contains_loop(int current, int origin)
{
    if (current == origin)
    {
        return true;
    }
    for (int c = 0; c <= candidate_count; c++)
    {
        if (locked[current][c])
        {
            if (contains_loop(c, origin))
            {
                return true;
            }
        }
    }
    return false;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int n = 0; n < pair_count - 1; n++)
    {
        for (int nn = 0; nn < pair_count - n - 1; nn++)
        {
            if (preferences[pairs[nn + 1].winner][pairs[nn + 1].loser] > preferences[pairs[nn].winner][pairs[nn].loser])
            {
                pair temp = pairs[nn];
                pairs[nn] = pairs[nn + 1];
                pairs[nn + 1] = temp;
            }
        }
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int k = 0; k < pair_count; k++)
    {
        if (!contains_loop(pairs[k].loser, pairs[k].winner))
        {
            locked[pairs[k].winner][pairs[k].loser] = true;
        }
    }
}


// Print the winner of the election
void print_winner(void)
{
    for (int counter = 0; counter < candidate_count; counter++)
    {
        int win = 0;
        for (int k = 0; k < candidate_count; k++)
        {
            if (!locked[k][counter])
            {
                win++;
                if (win == candidate_count)
                {
                    printf("%s\n", candidates[counter]);
                }
            }
        }
    }
}

