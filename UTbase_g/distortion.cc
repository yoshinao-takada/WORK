#include    <gtest/gtest.h>
#include    <base_g/BL_distortion.h>
#define     SUIT    distortion

namespace 
{
    TEST(SUIT, test_fan_angle)
    {
        const BL_2r32_t point0 = {0.0f, 0.0f};
        const BL_2r32_t point1 = { 1.0f, 1.0f };
        const BL_2r32_t polygon0[] = {
            { 2.0f, 0.5f }, { 0.0f, 1.0f }, { -1.0f, 0.0f }, { 0.0f, -1.0f }
        };
        float angle = fan_angle(point0, ARRAYSIZE(polygon0), polygon0);
        printf("%f\n", angle);
        angle = fan_angle(point1, ARRAYSIZE(polygon0), polygon0);
        printf("%f\n", angle);
    }
}