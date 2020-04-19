#include <math.h>

#include "primes.h"

bool is_prime(size_t x) {
    if (x < 2) {
        return false;
    } else if (x < 4) {
        return true;
    } else if ((x % 2) == 0) {
        return false;
    }
    for (size_t i = 3; (float)i <= floorf(sqrtf((float)x)); i += 2) {
        if ((x % i) == 0) {
            return false;
        }
    }
    return true;
}

size_t next_prime(size_t x) {
    while (!is_prime(x)) {
        ++x;
    }
    return x;
}
