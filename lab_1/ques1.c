// Write a C program to swap the content of 2 variables using pointer.

#include <stdio.h>
void swap(int *x, int *y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void main()
{
    int n1, n2;
    printf("Enter the value of number 1: ");
    scanf("%d", &n1);
    printf("Enter the value of number 2: ");
    scanf("%d", &n2);

    printf("\n---Before Swapping---\n");
    printf("Number 1: %d\n", n1);
    printf("Number 2: %d\n", n2);

    swap(&n1, &n2);

    printf("\n---After Swapping---\n");
    printf("Number 1: %d\n", n1);
    printf("Number 2: %d\n", n2);
}

