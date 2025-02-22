import csv
import sys


def main():
    # Check for command-line usage
    while True:
        if len(sys.argv) == 3:
            break
        print("USAGE: python dna.py DATABASE SEQUENCE")
    # TODO: Read database file into a variable
    people = []
    with open(sys.argv[1], "r") as file:
        line = file.readline()
        subsequences = []
        subsequence = ""
        for c in line:
            if c == "A" or c == "G" or c == "C" or c == "T":
                subsequence = subsequence + c
            if c == "," and subsequence != "":
                subsequences.append(subsequence)
                subsequence = ""
        subsequences.append(subsequence)
        field_names = ["name"]
        for i in subsequences:
            field_names.append(i)
        reader = csv.DictReader(file, field_names)
        for row in reader:
            people.append(row)
    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as file:
        sequence = file.readline()
    # TODO: Find longest match of each STR in DNA sequence
    patient = {}
    for i in subsequences:
        patient[i] = longest_match(sequence, i)
    # TODO: Check database for matching profiles
    for i in people:
        for j in subsequences:
            match_flag = False
            i[j] = int(i[j])
            if i[j] == patient[j]:
                match_flag = True
            if match_flag == False:
                break
        if match_flag == True:
            print(f'{i["name"]}')
            return
    if match_flag == False:
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
