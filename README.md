# array

Single Header stretchy array / arraylist / vector in C99.

This is my own port of a C stretchy array, based heavily on the implementations in `stb` and `zpl`.
I put this together mainly as an exercise in understanding those implementations, but also so that
I had an implementation with the precise names / semantics that I wanted, namely:

- Verbose but not too verbose `array_` namespacing
- Compile time (macro based) custom allocators, rather than runtime (vtable based) ones
- Compile time (macro based) configurable, assert based, bounds and allocation failure checks

See `test.c` for full examples, but here is a rough idea of how to use it:

```c
#include <stdio.h>

#define ARRAY_IMPL
#include "array.h"

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
```
