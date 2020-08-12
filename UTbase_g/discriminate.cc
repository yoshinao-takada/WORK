#include    <gtest/gtest.h>
#include    "base_g/BL_discriminate.h"
#include    "base_g/BL_texture.h"
#include    <base_l/BL_futils.h>
#include    <base_l/BL_errno.h>
#include    <opencv2/core.hpp>
#include    <opencv2/imgproc.hpp>
#include    <opencv2/imgcodecs.hpp>
#include    <base_l/BL_debutil.h>
#include    "UTbase_g/testdefs.h"

#define DISCRIMINATE_WORK_DIR   WORK_DIR   "discriminate/"
#define IMAGE_SAVE_CSV      DISCRIMINATE_WORK_DIR  "image.csv"
#define II0_SAVE_CSV         DISCRIMINATE_WORK_DIR  "ii0.csv"
#define RESTORED_CSV        DISCRIMINATE_WORK_DIR  "restored.csv"
#define IMAGE_BEFORE_FILTER_JPG DISCRIMINATE_WORK_DIR  "image_before_filter.jpg"
#define IMAGE_AFTER_FILTER_JPG  DISCRIMINATE_WORK_DIR  "image_after_filter.jpg"
#define IMAGE_FOR_COLOR_DISCRIMINATION  DISCRIMINATE_WORK_DIR  "image_color_disc.jpg"
#define IMAGE_RED_DOTS      DISCRIMINATE_WORK_DIR  "red_dots.jpg"
#define IMAGE_BLUE_DOTS     DISCRIMINATE_WORK_DIR  "blue_dots.jpg"
#define RED_DOT_CENTERS_CSV DISCRIMINATE_WORK_DIR  "red_dot_centers.csv"
#define BLUE_DOT_CENTERS_CSV    DISCRIMINATE_WORK_DIR  "blue_dot_centers.csv"
#define COLOR_INDEX_MAP_CSV DISCRIMINATE_WORK_DIR  "color_index_map.csv"
#define RED_DOT_SEGIDS_CSV  DISCRIMINATE_WORK_DIR  "red_dot_segIDs.csv"
#define BLUE_DOT_SEGIDS_CSV DISCRIMINATE_WORK_DIR  "blue_dot_segIDs.csv"
#define BLUE_DOT_CENTERS_CSV    DISCRIMINATE_WORK_DIR  "blue_dot_centers.csv"
#define RED_DOT_CENTERS_CSV     DISCRIMINATE_WORK_DIR  "red_dot_centers.csv"

#define SUIT    discriminate
namespace
{
    int PrepareWorkDir()
    {
        int err = ESUCCESS;
        do {
            if (!BL_futils_exists(DISCRIMINATE_WORK_DIR))
            {
                err = BL_futils_create_directories(DISCRIMINATE_WORK_DIR);
            }
        } while (false);
        return err;
    }

    TEST(SUIT, sum1x1)
    {
        ASSERT_EQ(ESUCCESS, PrepareWorkDir());
        BL_arrayMD_dims dims = { 16, 16, 1,1,1,1,1,1 };
        pBL_arrayMD_t src_image = BL_arrayMD_new(dims, uint8_t);
        BL_ptr_t i_src_image = BL_arrayMD_begin(src_image);
        uint32_t padding = 4;
        for (uint16_t iy = 0; iy != dims[1]; iy++)
        {
            for (uint16_t ix = 0; ix != dims[0]; ix++)
            {
                *i_src_image._1u8++ = ix + iy * dims[0];
            }
        }
        pBL_arrayMD_t ii = BL_IIu8u32(src_image, 1, 0, padding);
        {
            BL_4i16_t sumarea = {0,0,1,1};
            for (sumarea[1] = 0; sumarea[1] != dims[1]; sumarea[1]++)
            {
                for (sumarea[0] = 0; sumarea[0] != dims[0]; sumarea[0]++)
                {
                    uint32_t restored = BL_IIu32_partialsum(ii, sumarea, padding);
                    ASSERT_EQ((uint32_t)(sumarea[0] + sumarea[1]*dims[0]), restored);
                }
            }
        }
        {
            BL_4i16_t sumarea = {0,0,3,3};
            BL_cptr_t i_src_image2 = BL_arrayMD_cbegin(src_image);
            const uint8_t* p_src_image = i_src_image2._1u8;
            for (sumarea[1] = 0; sumarea[1] != dims[1]-2; sumarea[1]++)
            {
                for (sumarea[0] = 0; sumarea[0] != dims[0]-2; sumarea[0]++)
                {
                    uint32_t restored = BL_IIu32_partialsum(ii, sumarea, padding);
                    uint32_t boring_sum = 0;
                    for (int16_t iy = 0; iy != sumarea[3]; iy++)
                    {
                        for (int16_t ix = 0; ix != sumarea[2]; ix++)
                        {
                            boring_sum += (uint32_t)(*(p_src_image + sumarea[0]+ix + (sumarea[1]+iy)*src_image->dims[0]));
                        }
                    }
                    ASSERT_EQ(boring_sum, restored);
                }
            }
        }
        free((void*)ii);
        free((void*)src_image);
    }

    TEST(SUIT, boxfilter)
    {
        ASSERT_EQ(ESUCCESS, PrepareWorkDir());
        BL_checkerconf_t texture_conf = {
            false, { 640, 480 }, { 64,64 },
            {{ 255, 255, 0 }, { 0, 255, 255 }}
        };
        pBL_arrayMD_t src_image = BL_texture_checker(&texture_conf);
        BL_arrayMD_dims dims = { (uint16_t)texture_conf.wh[0], (uint16_t)texture_conf.wh[1], 1,1,1,1,1,1 };
        pBL_arrayMD_t filtered_image = BL_arrayMD_new(dims, BL_3u8_t);
        uint32_t padding = 16;
        pcBL_arrayMD_t ii = BL_IIu8u32(src_image, 3, 0, padding);
        int err = BL_IIu32u8_boxfilter(ii, 4, padding, filtered_image, 3, 0);
        ASSERT_EQ(ESUCCESS, err);
        free((void*)ii);
        ii = BL_IIu8u32(src_image, 3, 1, padding);
        err = BL_IIu32u8_boxfilter(ii, 4, padding, filtered_image, 3, 1);
        free((void*)ii);
        ii = BL_IIu8u32(src_image, 3, 2, padding);
        err = BL_IIu32u8_boxfilter(ii, 4, padding, filtered_image, 3, 2);
        free((void*)ii);
        {
            BL_ptr_t ptr = BL_arrayMD_begin(src_image);
            cv::Mat3b cvimg((int)dims[1], (int)dims[0], (cv::Vec3b*)ptr._1u8);
            cv::imwrite(IMAGE_BEFORE_FILTER_JPG, cvimg);
        }
        {
            BL_ptr_t ptr = BL_arrayMD_begin(filtered_image);
            cv::Mat3b cvimg((int)dims[1], (int)dims[0], (cv::Vec3b*)ptr._1u8);
            cv::imwrite(IMAGE_AFTER_FILTER_JPG, cvimg);
        }
        free((void*)filtered_image);
        free((void*)src_image);
    }

    #define COLOR_WHITE { 255, 255, 255 }
    #define COLOR_RED   { 0, 0, 255 }
    #define COLOR_BLUE  { 255, 0, 0 }
    #define COLOR_ORANGE    { 0, 64, 255 }
    #define COLOR_BLUEGREEN { 255, 64, 0 }

    pBL_arrayMD_t   blur(pBL_arrayMD_t src, uint32_t box_k_2)
    {
        uint32_t n_ch = 3;
        pBL_arrayMD_t blured = BL_arrayMD_new(src->dims, BL_3u8_t);

        for (uint i_ch = 0; i_ch != n_ch; i_ch++)
        {
            pBL_arrayMD_t ii = BL_IIu8u32(src, n_ch, i_ch, box_k_2);
            int err = BL_IIu32u8_boxfilter(ii, box_k_2, box_k_2, blured, n_ch, i_ch);
            if (err) 
            {
                free((void*)blured);
                blured = nullptr;
                break;
            }
        }
        return blured;
    }

    uint8_t get_max(pcBL_arrayMD_t pix_map)
    {
        uint8_t u = 0;
        BL_cptr_t ptr = BL_arrayMD_cbegin(pix_map);
        for (uint16_t iy = 0; iy != pix_map->dims[1]; iy++)
        {
            for (uint16_t ix = 0; ix != pix_map->dims[0]; ix++)
            {
                u = __max(u, *ptr._1u8);
                ptr._1u8++;
            }
        }
        return u;
    }

    TEST(SUIT, nearest_color)
    {
        ASSERT_EQ(ESUCCESS, PrepareWorkDir());
        const BL_3u8_t ref_colors[] = { COLOR_WHITE, COLOR_RED, COLOR_BLUE };
        const BL_3u8_t colors[] = { COLOR_WHITE, COLOR_RED, COLOR_BLUE, COLOR_ORANGE, COLOR_BLUEGREEN };
        const uint8_t ref_values[] = { 0, 1, 2, 1, 2 };
        for (uint32_t i_colors = 0; i_colors != ARRAYSIZE(colors); i_colors++)
        {
            uint8_t disc_result = BL_nearest_color(colors[i_colors], ARRAYSIZE(ref_colors), ref_colors);
            ASSERT_EQ(ref_values[i_colors], disc_result);
        }
    }

    bool is_in(const cv::Point2i& end, const cv::Point2i& point_under_test)
    {
        return
            (point_under_test.x >= 0) &&
            (point_under_test.y >= 0) &&
            (point_under_test.x < end.x) &&
            (point_under_test.y < end.y);
    }

    int DrawCenterMarks(const char* cvimg_path, pcBL_array_t blue_dot_centers, pcBL_array_t red_dot_centers)
    {
        int err = ESUCCESS;
        do {
            cv::Vec3b white_dot(255,255,255);
            cv::Vec3b black_dot(0,0,0);
            cv::Mat3b img = cv::imread(cvimg_path);
            if (img.cols < 100 || img.rows < 100)
            { // invalid image file
                err = EINVAL;
                break;
            }
            cv::Point2i pix_end = { img.cols, img.rows };
            {
                BL_cptr_t i_centers = BL_array_cbegin(blue_dot_centers);
                for (uint32_t i = 0; i != blue_dot_centers->unit_count; i++)
                {
                    for (int j = -10; j <= 10; j++)
                    {
                        BL_2r32_t offset1 = { (float)j, (float)j }, offset2 = { -(float)j, (float)j };
                        cv::Point2i target1 = {
                            (int32_t)lroundf(i_centers._2r32[i][0] + offset1[0]),
                            (int32_t)lroundf(i_centers._2r32[i][1] + offset1[1])
                        };
                        cv::Point2i target2 = {
                            (int32_t)lroundf(i_centers._2r32[i][0] + offset2[0]),
                            (int32_t)lroundf(i_centers._2r32[i][1] + offset2[1])
                        };
                        if (is_in(pix_end, target1))
                        {
                            img(target1) = white_dot;
                        }
                        if (is_in(pix_end, target2))
                        {
                            img(target2) = white_dot;
                        }
                    }
                }
            }
            {
                BL_cptr_t i_centers = BL_array_cbegin(red_dot_centers);
                for (uint32_t i = 0; i != red_dot_centers->unit_count; i++)
                {
                    for (int j = -10; j <= 10; j++)
                    {
                        BL_2r32_t offset1 = { (float)j, 0.0f }, offset2 = { 0.0f, (float)j };
                        cv::Point2i target1 = {
                            (int32_t)lroundf(i_centers._2r32[i][0] + offset1[0]),
                            (int32_t)lroundf(i_centers._2r32[i][1] + offset1[1])
                        };
                        cv::Point2i target2 = {
                            (int32_t)lroundf(i_centers._2r32[i][0] + offset2[0]),
                            (int32_t)lroundf(i_centers._2r32[i][1] + offset2[1])
                        };
                        if (is_in(pix_end, target1))
                        {
                            img(target1) = black_dot;
                        }
                        if (is_in(pix_end, target2))
                        {
                            img(target2) = black_dot;
                        }
                    }
                }
            }
            cv::imwrite(cvimg_path, img);
        } while (false);
        return err;
    }

    TEST(SUIT, color_discriminate)
    {
        ASSERT_EQ(ESUCCESS, PrepareWorkDir());
        BL_checkerconf2_t texture_conf = {
            { false, { 640, 480 }, { 64, 64 }, { COLOR_ORANGE, COLOR_WHITE }},
            { 256, 128 }, { COLOR_BLUEGREEN, COLOR_WHITE }
        };
        pBL_arrayMD_t src_image = BL_texture_circulardots2(&texture_conf);
        pBL_arrayMD_t blured_image = blur(src_image, 8);
        free((void*)src_image);
        src_image = blured_image;
        {
            BL_ptr_t i_src_image = BL_arrayMD_begin(src_image);
            cv::Mat3b cv_image = cv::Mat3b((int)src_image->dims[1], (int)src_image->dims[0], (cv::Vec3b*)i_src_image._1u8);
            cv::imwrite(IMAGE_FOR_COLOR_DISCRIMINATION, cv_image);
        }
        const BL_3u8_t ref_colors[] = { COLOR_WHITE, COLOR_RED, COLOR_BLUE };
        pBL_arrayMD_t color_index_map = BL_discriminate(src_image, ARRAYSIZE(ref_colors), ref_colors);
        free((void*)src_image);
        pBL_arrayMD_t retrieved_red_dots = BL_discriminate2(color_index_map, 1);
        pBL_arrayMD_t retrieved_blue_dots = BL_discriminate2(color_index_map, 2);
        ASSERT_EQ((uint8_t)2, get_max(color_index_map));
        ASSERT_EQ((uint8_t)1, get_max(retrieved_red_dots));
        ASSERT_EQ((uint8_t)1, get_max(retrieved_blue_dots));
        pBL_arrayMD_t red_dot_segIDs = BL_segmentIDs(retrieved_red_dots);
        pBL_arrayMD_t blue_dot_segIDs = BL_segmentIDs(retrieved_blue_dots);
        {
            FILE* pf = nullptr;
            int err = BL_futils_open(&pf, RED_DOT_SEGIDS_CSV, "w");
            ASSERT_EQ(ESUCCESS, err);
            BL_debutil_fwrite_array2D(pf, red_dot_segIDs, "y,x,segID", BL_debutil_write_1u16);
            if (pf) fclose(pf);
        }
        {
            FILE* pf = nullptr;
            int err = BL_futils_open(&pf, BLUE_DOT_SEGIDS_CSV, "w");
            ASSERT_EQ(ESUCCESS, err);
            BL_debutil_fwrite_array2D(pf, blue_dot_segIDs, "y,x,segID", BL_debutil_write_1u16);
            if (pf) fclose(pf);
        }
        free((void*)color_index_map);
        free((void*)retrieved_blue_dots);
        free((void*)retrieved_red_dots);
        pBL_array_t blue_dot_centers = BL_segment_centers(blue_dot_segIDs);
        pBL_array_t red_dot_centers = BL_segment_centers(red_dot_segIDs);      
        free((void*)blue_dot_segIDs);
        free((void*)red_dot_segIDs);
        {
            FILE *pf = nullptr;
            int err = BL_futils_open(&pf, BLUE_DOT_CENTERS_CSV, "w");
            ASSERT_EQ(ESUCCESS, err);
            BL_debutil_fwrite_array(pf, blue_dot_centers, "i,x,y", BL_debutil_write_2r32);
            fclose(pf);
        }
        {
            FILE *pf = nullptr;
            int err = BL_futils_open(&pf, RED_DOT_CENTERS_CSV, "w");
            ASSERT_EQ(ESUCCESS, err);
            BL_debutil_fwrite_array(pf, red_dot_centers, "i,x,y", BL_debutil_write_2r32);
            fclose(pf);
        }
        {
            int err = DrawCenterMarks(IMAGE_FOR_COLOR_DISCRIMINATION, blue_dot_centers, red_dot_centers);
        }
        free((void*)blue_dot_centers);
        free((void*)red_dot_centers);
    }
}