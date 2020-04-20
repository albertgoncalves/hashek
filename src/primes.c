#include <math.h>

#include "primes.h"

bool is_prime(size_t value) {
    if (value < 2) {
        return false;
    } else if (value < 4) {
        return true;
    } else if ((value % 2) == 0) {
        return false;
    }
    float v = floorf(sqrtf((float)value));
    for (size_t i = 3; i <= (size_t)v; i += 2) {
        if ((value % i) == 0) {
            return false;
        }
    }
    return true;
}

size_t next_prime(size_t value) {
    while (!is_prime(value)) {
        ++value;
    }
    return value;
}
