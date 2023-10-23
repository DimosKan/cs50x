import csv
import sys


def main():
    # Checks if there are 3 and only 3 arguments on the commandline
    if len(sys.argv) != 3:
        sys.exit("Usage: python tournament.py Database suspect")

    # Assigns values to the arguments
    dnadb = sys.argv[1]
    suspect = sys.argv[2]

    # Reads the txt of the sequences
    f = open(suspect, "r")
    sus_seq = f. readline()

    # Decides which table it should use for small and large database
    if "small" in dnadb:
        subsequence = ['AGATC', 'AATG', 'TATC']
    elif "large" in dnadb:
        subsequence = ['AGATC', 'TTTTTTCT', 'AATG', 'TCTAG', 'GATA', 'TATC', 'GAAA', 'TCTG']

    counter = 0
    datacollector = []
    matchfoundflag = False

    # Uses longest_match for each of value of the table of subsequences, depending if the user wants to use the small or large database
    for i in subsequence:
        maxseq = longest_match(sus_seq, i)
        # Saves all the values to a list
        datacollector.append(str(maxseq))
        counter += 1

    # Opens the CSV
    with open(dnadb, "r") as file:
        reader = csv.DictReader(file)
        for row in reader:
            counter = 0
            correctmatches = 0
            # Removes the name from the row in order to make good use of the loop
            name = row.pop("name")
            for i in subsequence:
                # Compares every column of the row with every entry of my datacollector list, the values should be aligned
                if row[i] == datacollector[counter]:
                    correctmatches += 1
                    # Checks if all the matches are correct, it has two modes, for large database and small database
                    if (correctmatches == 3 and len(subsequence) == 3) or (correctmatches == 8 and len(subsequence) == 8):
                        matchfoundflag = True
                        print(name)
                else:
                    break
                counter += 1

    if matchfoundflag == 0:
        print("No Match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
