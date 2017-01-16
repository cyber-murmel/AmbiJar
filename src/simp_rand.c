#include <stdint.h>
#include "simp_rand.h"
#ifndef SEED
#define SEED (0x13371337u)
#endif

static uint32_t lfsr = SEED;

uint32_t random() {
    uint32_t bit;

    bit  = ((lfsr >> 1) ^ (lfsr >> 10) ^ (lfsr >> 19) ^ (lfsr >> 28) ) & 1;
    lfsr =  (lfsr >> 1) | (bit << 15);

    return lfsr;
}