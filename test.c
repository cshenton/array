#define ARRAY_IMPL
#include "array.h"

#include <stdio.h>

int main()
{
    // Initialise the array
    float *ar = array_init();

    // Push some data to it
    for (int i = 0; i < 100; i++)
    {
        array_push(ar, (float)i / 3.0f);
    }

    // Print out the data and array count + capacity
    printf("\n");
    printf("Array length: %zu\n", array_count(ar));
    printf("Array capacity: %zu\n", array_capacity(ar));
    printf("Array elements:");

    for (int i = 0; i < 100; i++)
    {
        if (i % 10 == 0)
        {
            printf("\n\t");
        }
        printf("%.2f, ", ar[i]);
    }
    printf("\n\n");

    // Free the array
    array_free(ar);
    return 0;
}