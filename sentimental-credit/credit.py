import math

def main():
    extra_dig = 0
    sum_dig = 0
    cr_type = ""
    count = 0

    n = int(input("Number: "))
    while n < 1:
        n = int(input("Number: "))

    count = int(math.log10(n)) + 1
    two_init_digits = count - 1

    for i in range(1, count+1):
        dig = n % 10

        if i == two_init_digits:
            if n == 34 or n == 37:
                cr_type = "AMEX"
            elif 50 < n < 56:
                cr_type = "MASTERCARD"
            elif 39 < n < 50:
                cr_type = "VISA"
            else:
                cr_type = "INVALID"

        if i % 2 == 0:

            if dig > 3:
                temp_dig = dig * 2
                tempdig1 = temp_dig // 10
                tempdig2 = temp_dig % 10
                sum_dig += tempdig1 + tempdig2
            else:
                sum_dig += dig * 2
        else:
            extra_dig += dig

        n = n // 10

    valid_ch = extra_dig + sum_dig

    if count < 12:
        cr_type = "INVALID"

    if valid_ch % 10 == 0:
        print(cr_type)
    else:
        print("INVALID")


main()
