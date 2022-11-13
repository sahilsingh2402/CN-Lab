// Write a C program to assign values to each members of the following structure. 
// Pass the populated structure to a function Using call-by address and print the value of each member of the 
// structure with in that function.

#include <stdio.h>
#include <string.h>

struct info
{
    int roll_no;
    char name[50];
    float CGPA;
};

void show(struct info display)
{
    printf("Roll: %d\n", display.roll_no);
    printf("Name: %s\n", display.name);
    printf("CGPA: %.2f\n", display.CGPA);
};

int main()
{
    struct info display;
    display.roll_no = 2005535;
    strcpy(display.name, "Sahil");
    display.CGPA = 9.15;
    show(display);
    return 0;
};
