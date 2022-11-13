// Write a C program to extract each byte from a given number and store them in separate character
// variables and print the content of those variables.
#include <stdio.h>
int main()
{
    int x = 258;
    for (int i = 0; i < sizeof(x); ++i)
    {
        unsigned char byte = *((unsigned char *)&x + i);
        printf("Byte %d = %u\n", i, (unsigned)byte);
    }
    return 0;
}