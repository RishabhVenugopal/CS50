#include <cs50.h>
#include <stdio.h>

long get_Credit(void);
void chkValid(long num);

int main(void)
{
    long credit = get_Credit();
    chkValid(credit);
}


































//To get the credit card number from user
long get_Credit(void)
{
    long credit = get_long("Credit Card Number: ");
    return credit;
}

//To check if the number is valid and determine the company
void chkValid(long num)
{
    int sumodd = 0;
    int sumeve = 0;
    int counter = 0;
    long num2 = num;

    while (num > 0)
    {
        if (counter % 2 == 0)
        {
            sumeve += num % 10;
        }

        else
        {
            if (2 * (num % 10) >= 10)
            {
                sumodd += 1 + (2 * (num % 10)) % 10;
            }

            else
            {
                sumodd += 2 * (num % 10);
            }
        }
        counter++;
        num /= 10;
    }

    if ((sumodd + sumeve) % 10 == 0)
    {
        while (num2 > 99)
        {
            num2 /= 10;
        }

        if (counter == 15)
        {
            if (num2 == 34 || num2 == 37)
            {
                printf("AMEX\n");
                return;
            }
        }

        if (counter == 16)
        {
            if (num2 >= 51 && num2 <= 55)
            {
                printf("MASTERCARD\n");
                return;
            }

            if (num2 / 10 == 4)
            {
                printf("VISA\n");
                return;
            }
        }

        if (counter == 13 && num2 / 10 == 4)
        {
            printf("VISA\n");
            return;
        }

        else
        {
            printf("INVALID\n");
        }

    }

    else
    {
        printf("INVALID\n");
    }
}


