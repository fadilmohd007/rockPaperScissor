#include "random_gen.h"
#include <stdio.h>
#include <stdlib.h>

int genIntRandomInRange(int range_max, int range_min)
{
    int rand_num = rand();
    int r = ((double)rand() / RAND_MAX) * (range_max - range_min + 1) + range_min;
    return r;
}