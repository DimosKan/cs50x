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
    for (int i = 0; i < voter_count ; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
            i--;
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
// loops through all the candidates names, and compares them with the user's input, if he finds an equal number, he gives +1 to this person's vote.
// if not he returns false and removes one number from the counter. Only then the vote will be as it never counted.
bool vote(string name)
{
    for (int k = 0; k < candidate_count ; k++)
    {
        if (strcmp(candidates[k].name, name) == 0)
        {
            candidates[k].votes++;
            return true;
        }

    }
    return false;
}

// Print the winner (or winners) of the election
//First "for" loop searches for biggest number of votes that exists, then in the second loop it gets compared with the other candidates, he prints whoever is equal with the max
void print_winner(void)
{
    int max = 0;
    string win_name;
    for (int j = 0; j < candidate_count ; j++)
    {
        if (candidates[j].votes > max)
        {
            max = candidates[j].votes;
            win_name = candidates[j].name;
        }
    }
    for (int m = 0; m < candidate_count; m++)
    {
        if (candidates[m].votes == max)
        {
            printf("%s\n", candidates[m].name);
        }
    }
    return;
}