#define ARRAY_IMPL
#include "array.h"

#include <assert.h>

void test_array_init()
{
    float *ar = array_init();
    assert(ar == NULL);
    assert(array_count(ar) == 0);
    assert(array_capacity(ar) == 0);

    array_free(ar);
    assert(ar == NULL);
}

void test_array_push()
{
    float *ar = array_init();
    array_push(ar, 1.0f);
    array_push(ar, 2.0f);
    array_push(ar, 3.0f);

    assert(ar[0] == 1.0f);
    assert(ar[1] == 2.0f);
    assert(ar[2] == 3.0f);

    assert(array_count(ar) == 3);
    assert(array_capacity(ar) >= 3);

    array_free(ar);
    assert(ar == NULL);
}

void test_array_ensure_capacity()
{
    float *ar = array_init();
    array_ensure_capacity(ar, 100);
    assert(array_capacity(ar) == 100);

    array_free(ar);
    assert(ar == NULL);
}

void test_array_grow()
{
    float *ar = array_init();
    array_grow(ar, 100);
    assert(array_capacity(ar) >= 100);

    array_free(ar);
    assert(ar == NULL);
}

void test_array_insert()
{
    float *ar = array_init();
    array_push(ar, 1.0f);
    array_push(ar, 2.0f);
    array_push(ar, 3.0f);
    array_insert(ar, 4.0f, 1);

    assert(ar[0] == 1.0f);
    assert(ar[1] == 4.0f);
    assert(ar[2] == 2.0f);
    assert(ar[3] == 3.0f);
    assert(array_count(ar) == 4);

    array_free(ar);
    assert(ar == NULL);
}

void test_array_appendn()
{
    float *ar = array_init();

    float items[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    array_appendn(ar, items, 4);

    assert(ar[0] == 1.0f);
    assert(ar[1] == 2.0f);
    assert(ar[2] == 3.0f);
    assert(ar[3] == 4.0f);
    assert(array_count(ar) == 4);

    array_free(ar);
    assert(ar == NULL);
}

void test_array_insertn()
{
    float *ar = array_init();
    array_push(ar, 1.0f);
    array_push(ar, 2.0f);

    float items[4] = {3.0f, 4.0f, 5.0f, 6.0f};
    array_insertn(ar, items, 4, 1);

    assert(ar[0] == 1.0f);
    assert(ar[1] == 3.0f);
    assert(ar[2] == 4.0f);
    assert(ar[3] == 5.0f);
    assert(ar[4] == 6.0f);
    assert(ar[5] == 2.0f);
    assert(array_count(ar) == 6);

    array_free(ar);
    assert(ar == NULL);
}

void test_array_delete()
{
    float *ar = array_init();
    array_push(ar, 1.0f);
    array_push(ar, 2.0f);
    array_push(ar, 3.0f);

    array_delete(ar, 1);
    assert(array_count(ar) == 2);
    assert(ar[0] == 1.0f);
    assert(ar[1] == 3.0f);

    array_free(ar);
    assert(ar == NULL);
}

void test_array_pop()
{
    float *ar = array_init();
    array_push(ar, 1.0f);
    array_push(ar, 2.0f);
    array_push(ar, 3.0f);

    float item = array_pop(ar);
    assert(item == 3.0f);
    assert(array_count(ar) == 2);

    array_free(ar);
    assert(ar == NULL);
}

void test_array_front_back_clear()
{
    float *ar = array_init();
    array_push(ar, 1.0f);
    array_push(ar, 2.0f);
    array_push(ar, 3.0f);

    assert(array_front(ar) == 1.0f);
    assert(array_back(ar) == 3.0f);

    size_t cap = array_capacity(ar);
    array_clear(ar);
    assert(array_capacity(ar) == cap);
    assert(array_count(ar) == 0);

    array_free(ar);
    assert(ar == NULL);
}

int main()
{
    test_array_init();
    test_array_push();
    test_array_ensure_capacity();
    test_array_grow();
    test_array_insert();
    test_array_appendn();
    test_array_insertn();
    test_array_delete();
    test_array_pop();
    test_array_front_back_clear();
    return 0;
}