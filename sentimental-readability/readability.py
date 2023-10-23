def wordScanner(sentence, str_l):
    """ Counts letters, sentences and words in an inputted text """
    letter_count = 0
    sentence_count = 0
    word_count = 1
    for i in range(str_l):
        # Checks the letters, it does it by checking if something is alphanumeric
        if sentence[i].isalnum():
            letter_count += 1
        # If there is a . ? or ! character that means that a sentence has ended.
        if sentence[i] == '.' or sentence[i] == '?' or sentence[i] == '!':
            sentence_count += 1
        # Checks for spaces, that means a word has ended.
        if sentence[i] == " ":
            word_count += 1
    # returns all the results as a table.
    return [sentence, str_l, letter_count, sentence_count, word_count]


def main():
    """Asks for input, does the col_liau calculation and then displays a result"""
    sentence = input("Text: ")
    str_l = len(sentence)
    wsfunc = wordScanner(sentence, str_l)
    col_liau = 0.0588 * (100 * wsfunc[2] / wsfunc[4]) - 0.296 * (100 * wsfunc[3] / wsfunc[4]) - 15.8
    grade = int(round(col_liau))

    if grade < 1:
        print("Before Grade 1")
    elif grade > 16:
        print("Grade 16+")
    else:
        print(f"Grade {grade}")


main()