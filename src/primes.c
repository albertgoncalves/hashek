#include <math.h>
#include <stdbool.h>

#include "primes.h"

static bool is_prime(uint32_t value) {
    if (value < 2) {
        return false;
    } else if (value < 4) {
        return true;
    } else if ((value % 2) == 0) {
        return false;
    }
    float    limit_f = floorf(sqrtf((float)value));
    uint32_t limit_i = (uint32_t)limit_f;
    for (uint32_t i = 3; i <= limit_i; i += 2) {
        if ((value % i) == 0) {
            return false;
        }
    }
    return true;
}

uint32_t next_prime(uint32_t value) {
    while (!is_prime(value)) {
        ++value;
    }
    return value;
}
