#include <stdio.h>
#include "myheader.h"

#define PI 3.14
#define ADD(a,b) ((a)+(b))

// Main function
int main()
{
    float radius = 5;
    float area = PI * radius * radius;
    int sum = ADD(10, 20);

    /* Display results */
    printf("Area = %.2f\n", area);
    printf("Sum = %d\n", sum);

    printf("Value = %d\n", VALUE);

    return 0;
}
