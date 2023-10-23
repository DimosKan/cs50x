import math


def structfunc(height_input):
    """ Gets the number inputted by the user and constructs the pyramid with those data in mind"""
    tempCount = 1
    for _ in range(height_input):
        print(" " * (height_input - tempCount), end='')
        print("#" * tempCount, end=''),
        print("  ", end='')
        print("#" * tempCount, end='')
        print('')
        tempCount = tempCount + 1


def main():
    """ Checks for a valid input, it only accept integer numbers and nothing else. """
    while (True):
        height_input = input("Size: ")
        try:
            # Tries to see if the input just inserted is indeed an integer.
            height_input = int(height_input)
            isnanbool = math.isnan(height_input)
            # checks to see if the number is less that 0 and it is a number
            if not isnanbool and (height_input > 0 and height_input < 9):
                break
            else:
                print("Please enter a positive number")
        except ValueError:
            print("There is an error in the value you just inserted, try again.")
            pass
    structfunc(height_input)


main()