// Write a C Program to enter a number and store the number across the following structure and print the 
// content of each member of the structure. 
// Then aggregate each member of the structure to form the original number and print the same.

#include <stdio.h>
#include <string.h>

struct pkt
{
    char ch1;
    char ch2[2];
    char ch3;
};

void aggregate(struct pkt k)
{
    int number, x, y = 1;
    printf("Enter the number: ");
    scanf("%d", &number);
    while (number != 0 && y <= 4)
    {
        x = number % 10;
        if (y == 1)
        {
            k.ch1 = x;
        }
        if (y == 2)
        {
            k.ch2[0] = x;
        }
        if (y == 3)
        {
            k.ch2[1] = x;
        }
        if (y == 4)
        {
            k.ch3 = x;
        }
        y++;
        number = number / 10;
    }
    printf("Aggregated Characters: %d %d %d %d\n", k.ch3, k.ch2[1], k.ch2[0], k.ch1);
}

int main()
{
    struct pkt run;
    aggregate(run);
}