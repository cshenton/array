#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <string.h>

#ifndef ARRAY_ASSERT
#include <assert.h>
#define ARRAY_ASSERT(x) assert(x)
#endif

// Header data of the array, stored before the array data
typedef struct array_header_t
{
    size_t count;
    size_t capacity;
} array_header_t;

// Implementations of the array free/reallocs
void array_free_impl(void *array);
void *array_ensure_capacity_impl(void *array, size_t capacity, size_t element_size);

// Create an empty array, to be assigned to any pointer type.
#define array_init() NULL

// Return the header struct allocated before the array's main memory
#define array_header(x) ((array_header_t *)(x)-1)

// Return the number of elements in the array
#define array_count(x) ((x) ? array_header(x)->count : 0)

// Return the array's element capacity (number count can reach before a realloc)
#define array_capacity(x) ((x) ? array_header(x)->capacity : 0)

// Free the array pointed to by x
#define array_free(x)                         \
    do                                        \
    {                                         \
        if (x)                                \
        {                                     \
            array_free_impl(array_header(x)); \
            x = NULL;                         \
        }                                     \
    } while (0)

// Ensure's this array has room for capacity elements, reallocating if necessary
#define array_ensure_capacity(x, capacity)                                         \
    do                                                                             \
    {                                                                              \
        void **array__array = (void **)&(x);                                       \
        *array__array = array_ensure_capacity_impl((x), (capacity), sizeof(*(x))); \
    } while (0)

// Given a minimum capacity, chooses a new capacity using a heuristic and ensures the array has that capacity
#define array_grow(x, min_capacity)                      \
    do                                                   \
    {                                                    \
        size_t new_capacity = 2 * array_capacity(x) + 8; \
        if (new_capacity < (min_capacity))               \
        {                                                \
            new_capacity = (min_capacity);               \
        }                                                \
        array_ensure_capacity(x, new_capacity);          \
    } while (0)

// Append the items to the end of this array, growing it if necessary
#define array_push(x, item)                         \
    do                                              \
    {                                               \
        if (array_capacity(x) < array_count(x) + 1) \
        {                                           \
            array_grow(x, 0);                       \
        }                                           \
        (x)[array_header(x)->count++] = (item);     \
    } while (0)

// Inserts item into the array at idx, moving other elements
#define array_insert(x, item, idx)                                                \
    do                                                                            \
    {                                                                             \
        if (array_count(x) <= idx)                                                \
        {                                                                         \
            array_append(x, item);                                                \
            break;                                                                \
        }                                                                         \
        if (array_capacity(x) < array_count(x) + 1)                               \
        {                                                                         \
            array_grow(x, 0);                                                     \
        }                                                                         \
        memmove(&(x)[idx + 1], (x + idx), sizeof(x[0]) * (array_count(x) - idx)); \
        x[idx] = item;                                                            \
        array_header(x)->count++;                                                 \
    } while (0)

// Appends item_count elements pointed to by items to the end of the array
#define array_appendn(x, items, item_count)                                  \
    do                                                                       \
    {                                                                        \
        ARRAY_ASSERT(sizeof(*(items)) == sizeof(*(x)));                      \
        if (array_capacity(x) < array_count(x) + (item_count))               \
        {                                                                    \
            array_grow(x, array_count(x) + (item_count));                    \
        }                                                                    \
        memcopy(&(x)[array_count(x)], (items), sizeof(*(x)) * (item_count)); \
        array_header(x)->count += (item_count);                              \
    } while (0)

// Inserts item_count elements pointed to by items into the array at idx, moving other elements
#define array_insertn(x, items, item_count, idx)                                 \
    do                                                                           \
    {                                                                            \
        if (array_count(x) <= idx)                                               \
        {                                                                        \
            array_appendv(x, items, item_count);                                 \
            break;                                                               \
        }                                                                        \
        ARRAY_ASSERT(sizeof(*(items)) == sizeof(*(x)));                          \
        if (array_capacity(x) < array_count(x) + (item_count))                   \
        {                                                                        \
            array_grow(x, array_count(x) + (item_count));                        \
        }                                                                        \
        memmove(x + idx + (item_count), x + idx, sizeof(*(x)) * array_count(x)); \
        memcopy(&(x)[idx], (items), sizeof(*(x)) * (item_count));                \
        array_header(x)->count += (item_count);                                  \
    } while (0)

// Deletes the element at idx, moving other elements
#define array_delete(x, idx)                                                    \
    do                                                                          \
    {                                                                           \
        ARRAY_ASSERT(idx < array_count(x));                                     \
        memmove(x + idx, x + idx + 1, sizeof(*x) * (array_count(x) - idx - 1)); \
        array_header(x)->count--;                                               \
    } while (0)

// Removes the last element from the array and returns it
#define array_pop(x) (array_header(x)->length--, (x)[array_header(x)->length])

// Returns the first element of the arraay
#define array_front(x) x[0]

// Returns the last element of the array
#define array_back(x) x[array_count(x) - 1]

// Clears the array, maintaining its capacity
#define array_clear(x)                \
    do                                \
    {                                 \
        array_header_t(x)->count = 0; \
    } while (0)

// Resize the array to contain new_count elements
#define array_resize(x, new_count)            \
    do                                        \
    {                                         \
        if (array_capacity(x) < (new_count))  \
        {                                     \
            array_grow(x, (new_count));       \
        }                                     \
        array_header(x)->count = (new_count); \
    } while (0)

// Reserve capacity for new_capacity elements in the array
#define array_reserve(x, new_capacity)                  \
    do                                                  \
    {                                                   \
        if (array_header(x)->capacity < (new_capacity)) \
        {                                               \
            array_ensure_capacity(x, new_capacity);     \
        }                                               \
    } while (0)

#endif // ARRAY_H

#ifdef ARRAY_IMPL

#include <stdlib.h>

#ifndef ARRAY_REALLOC
#define ARRAY_REALLOC(ptr, size) realloc(ptr, size);
#endif

#ifndef ARRAY_FREE
#define ARRAY_FREE(ptr) free(ptr);
#endif

void array_free_impl(void *ar)
{
    ARRAY_FREE(ar);
}

void *array_ensure_capacity_impl(void *ar, size_t capacity, size_t element_size)
{
    // If we already have this capacity, early out
    if (capacity <= array_capacity(ar))
    {
        return ar;
    }

    // Realloc enough memory for header + capcity elements
    void *new_ar = ARRAY_REALLOC(ar ? array_header(ar) : NULL, element_size * capacity + sizeof(array_header_t));
    ARRAY_ASSERT(new_ar);

    // Shift the pointer to the array start
    new_ar = (char *)new_ar + sizeof(array_header_t);

    // Update the header
    array_header(new_ar)->count = ar ? array_count(new_ar) : 0;
    array_header(new_ar)->capacity = capacity;

    return new_ar;
}

#endif // ARRAY_IMPl