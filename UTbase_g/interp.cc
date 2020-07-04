#include    <gtest/gtest.h>
#include    <opencv2/core.hpp>
#include    <opencv2/imgproc.hpp>
#include    <opencv2/imgcodecs.hpp>
#include    "base_g/BL_bilinearinterp.h"
#define SUIT    interp
#define IMAGE_SAVE_PATH0    UTDATA  "img0.jpg"
#define IMAGE_SAVE_PATH1    UTDATA  "img1.jpg"
#define IMAGE_SAVE_PATH2    UTDATA  "imgc.jpg"
namespace
{
    static const float tolF = 1.0e-5f;
    bool equalF(float f0, float f1, float tol)
    {
        float abs_sum = fabsf(f0 + f1);
        float abs_diff = fabsf(f0 - f1);
        return (abs_diff/abs_sum) < tol;
    }

    uint8_t clamp(float f)
    {
        return (uint8_t)__max(0.0f, __min(255.0f, f));
    }
    
    TEST(SUIT, new_delete)
    {
        const BL_2u32_t wh_nodes = { 16, 12 };
        const BL_2r32_t table_origin = { -15.0f, -10.0f };
        const BL_2r32_t grid_pitch = { 3.0f, 2.0f };
        pBL_bilinearinterp_t interpolator = BL_bilinearinterp_new(wh_nodes, table_origin, grid_pitch);
        ASSERT_NE(interpolator, nullptr);
        const uint16_t* dims_values = interpolator->values->dims;
        const uint16_t* dims_coeff = interpolator->coeff->dims;
        ASSERT_EQ(dims_values[0], (uint16_t)16);
        ASSERT_EQ(dims_values[1], (uint16_t)12);
        ASSERT_EQ(dims_coeff[0], (uint16_t)15);
        ASSERT_EQ(dims_coeff[1], (uint16_t)11);
        BL_bilinearinterp_delete(&interpolator);
        ASSERT_EQ(interpolator, nullptr);
    }

    // only 1-cell interpolation
    TEST(SUIT, simple_interpolation)
    {
        const float node_value_ref[] = {
            0.0f, 128.0f,
            0.0f, 255.0f
        };
        const BL_2u32_t wh_nodes = { 2, 2 };
        const BL_2r32_t table_origin = { 0.0f, 0.0f };
        const BL_2r32_t grid_pitch = { 160.0f, 160.0f };
        pBL_bilinearinterp_t interpolator = BL_bilinearinterp_new(wh_nodes, table_origin, grid_pitch);
        pBL_arrayMD_t values = interpolator->values;
        BL_ptr_t i_values = BL_arrayMD_begin(values);
        BL_copy4(i_values._1r32, node_value_ref);
        BL_bilinearinterp_fill_coeff(interpolator);
        float value = -10.0f;
        int err = BL_bilinearinterp_calc_value(interpolator, table_origin, &value);
        ASSERT_EQ(ESUCCESS, err);
        ASSERT_TRUE(equalF(1.0f, 1.0f+value, tolF));
        BL_2r32_t xy = { grid_pitch[0], table_origin[1] };
        err = BL_bilinearinterp_calc_value(interpolator, xy, &value);
        ASSERT_EQ(ERANGE, err);
        xy[0] *= 0.99f;
        err = BL_bilinearinterp_calc_value(interpolator, xy, &value);
        ASSERT_EQ(ESUCCESS, err);
        printf("%f\n", value);
        ASSERT_TRUE(equalF(node_value_ref[1], value, 2.0f));
        cv::Mat1b img((int)grid_pitch[1], (int)grid_pitch[0]);
        cv::Point2i i_img;
        for (i_img.y = 0; i_img.y != img.rows; i_img.y++)
        {
            for (i_img.x = 0; i_img.x != img.cols; i_img.x++)
            {
                float f;
                BL_2r32_t xy = {(float)i_img.x, (float)i_img.y};
                BL_bilinearinterp_calc_value(interpolator, xy, &f);
                img(i_img) = clamp(f);
            }
        }
        cv::imwrite(IMAGE_SAVE_PATH0, img);
        BL_bilinearinterp_delete(&interpolator);
    }


    // 4-cell interpolation
    // N0-----N1------N2
    //  |      |       |
    //  |      |       |
    // N3-----N4------N5
    //  |      |       |
    //  |      |       |
    // N6-----N7------N8
    TEST(SUIT, _4cell_interpolation)
    {
        const float node_value_ref[] = {
            0.0f, 0.0f, 0.0f,
            127.0f, 254.0f, 127.0f,
            0.0f, 0.0f, 0.0f
        };
        const BL_2u32_t wh_nodes = { 3, 3 };
        const BL_2r32_t table_origin = { 0.0f, 0.0f };
        const BL_2r32_t grid_pitch = { 100, 100 };
        pBL_bilinearinterp_t interpolator = BL_bilinearinterp_new(wh_nodes, table_origin, grid_pitch);
        pBL_arrayMD_t values = interpolator->values;
        BL_ptr_t i_values = BL_arrayMD_begin(values);
        for (uint16_t i = 0; i != ARRAYSIZE(node_value_ref); i++)
        {
            i_values._1r32[i] = node_value_ref[i];
        }
        BL_bilinearinterp_fill_coeff(interpolator);
        cv::Mat1b img(200,200);
        cv::Point2i i_img;
        for (i_img.y = 0; i_img.y != img.rows; i_img.y++)
        {
            for (i_img.x = 0; i_img.x != img.cols; i_img.x++)
            {
                float value;
                BL_2r32_t xy = { (BL_1r32_t)i_img.x, (BL_1r32_t)i_img.y };
                int err = BL_bilinearinterp_calc_value(interpolator, xy, &value);
                img(i_img) = clamp(value);
            }
        }
        cv::imwrite(IMAGE_SAVE_PATH1, img);
        BL_bilinearinterp_delete(&interpolator);
    }

    // 4-cell interpolation 3-ch
    TEST(SUIT, _4cell3ch_interpolation)
    {
        const float node_value_ref[] = {
            0, 0, 0,   0, 127, 0,   0, 255, 0,
            127, 0, 0,   0, 0, 127,   0, 0, 0,
            255, 0, 0,   0, 0, 0,   0, 0, 255
        };
        const BL_2u32_t wh_nodes = { 3, 3 };
        const BL_2r32_t table_origin = { -100.0f, -100.0f };
        const BL_2r32_t grid_pitch = { 150, 150 };
        pBL_bilinearinterp_t interpolator = BL_bilinearinterpv_new(wh_nodes, table_origin, grid_pitch, 3);
        BL_ptr_t i_values = BL_arrayMD_begin(interpolator->values);
        for (uint16_t i = 0; i != ARRAYSIZE(node_value_ref); i++)
        {
            i_values._1r32[i] = node_value_ref[i];
        }
        BL_bilinearinterpv_fill_coeff(interpolator);
        cv::Mat3b img(200,200);
        cv::Point2i i_img;
        for (i_img.y = 0; i_img.y != img.rows; i_img.y++)
        {
            for (i_img.x = 0; i_img.x != img.cols; i_img.x++)
            {
                BL_3r32_t value;
                BL_2r32_t xy = { (BL_1r32_t)i_img.x, (BL_1r32_t)i_img.y };
                int err = BL_bilinearinterpv_calc_value(interpolator, xy, value);
                img(i_img)[0] = clamp(value[0]);
                img(i_img)[1] = clamp(value[1]);
                img(i_img)[2] = clamp(value[2]);
                
            }
        }
        cv::imwrite(IMAGE_SAVE_PATH2, img);
        BL_bilinearinterpv_delete(&interpolator);
    }

    // serious floating number vector interpolation with 4-cells
    TEST(SUIT, _4cell3ch_real_number_vector_interpolation)
    {
        
    }
}