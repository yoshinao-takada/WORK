#include    "base_l/BL_rational.h"
float BL_rational_rvalue(const BL_2u32_t r)
{
    return (float)r[0] / (float)r[1];
}

void    BL_rational_create(uint32_t row, uint32_t pos, BL_2u32_t r)
{
    uint32_t num_den_sum = row + 1;
    r[0] = pos + 1;
    r[1] = num_den_sum - r[0] + 1;
}