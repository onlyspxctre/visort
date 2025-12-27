#pragma once

#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

static int visort_seeded = 0;

int visort_redbar = -1;

size_t find_max(const atomic_int* data, const size_t n) {
    size_t idx = 0;

    for (size_t i = 1; i < n; ++i) {
        if (data[i] > data[idx]) {
            idx = i;
        }
    }

    return idx;
}

#define swap(T, a, b) \
    do {              \
        T temp = a;   \
        a = b;        \
        b = temp;     \
    } while (0)

[[deprecated]]
void swap_xor(int* a, int* b) {
    if (a == b)
        return;

    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

void shuffle(atomic_int* data, const size_t n) {
    if (!visort_seeded) {
        srand((unsigned int) time(NULL));
        visort_seeded = 1;
    }

    for (size_t i = 0; i < n; ++i) {
        size_t j = (size_t) rand() % n;

        swap(int, data[i], data[j]);
    }
}

void visort_quicksort(atomic_int* data, const size_t l, const size_t r) {
    if ((r - l) < 1)
        return;

    struct timespec delay = {
        .tv_sec = 0,
        .tv_nsec = (long) (2 * 1000000), // ms
    };
    size_t pivot = l + (r - l) / 2;

    size_t i = l;
    while (i < pivot) {
        nanosleep(&delay, NULL); 

        if (data[i] > data[pivot]) {
            swap(int, data[i], data[pivot]);
            --pivot;
            swap(int, data[i], data[pivot]);
        }
        else {
            ++i;
        }
    }

    i = r;
    while (i > pivot) {
        nanosleep(&delay, NULL);

        if (data[i] < data[pivot]) {
            swap(int, data[i], data[pivot]);
            ++pivot;
            swap(int, data[i], data[pivot]);
        }
        else {
            --i;
        }
    }

    visort_quicksort(data, l, pivot);
    visort_quicksort(data, pivot + 1, r);
}
