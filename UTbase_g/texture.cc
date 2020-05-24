#include    <gtest/gtest.h>
#include    <base_g/BL_texture.h>
#include    <opencv2/core.hpp>
#include    <opencv2/imgproc.hpp>
#include    <opencv2/imgcodecs.hpp>
#define SUIT    texture
#define DATADIR "../UTData/"
#define CHECKER_SAVE_PATH   DATADIR "Checker.png"
#define CHECKER_SAVE_PATH2  DATADIR "Checker2.png"
#define CHECKER_SAVE_PATH3  DATADIR "Checker3.png"
#define CHECKER_SAVE_PATH4  DATADIR "Checker4.png"
#define DOTS_SAVE_PATH      DATADIR "Dots.png"
#define DOTS_SAVE_PATH2     DATADIR "Dots2.png"

namespace
{
    TEST(SUIT, new_delete)
    {
        const BL_2u32_t wh = { 640, 480 };
        pBL_texture_t pobj = BL_texture_alloc(wh, true);
        ASSERT_NE(nullptr, pobj);
        free((void*)pobj);
    }

#define COLOR0  {0,0,255,64}
#define COLOR1  {255,255,255,64}
#define COLOR2  {255,0,0,128}
#define COLOR3  {255,255,0,48}

    TEST(SUIT, tex_checker)
    {
        BL_checkerconf_t conf = {
            false, {640,480}, {64,64},
            { COLOR0, COLOR1 }
        };
        pBL_texture_t tex = BL_texture_checker(&conf);
        BL_ptr_t ptr = BL_arrayMD_begin(tex);
        cv::Mat3b cvtex((int)conf.wh[1], (int)conf.wh[0], (cv::Vec3b*)ptr._1u8);
        cv::imwrite(CHECKER_SAVE_PATH, cvtex);
        free((void*)tex);
        conf.with_alpha = true;
        tex = BL_texture_checker(&conf);
        BL_ptr_t ptr2 = BL_arrayMD_begin(tex);
        cv::Mat4b cvtex2((int)conf.wh[1], (int)conf.wh[0], (cv::Vec4b*)ptr2._1u8);
        cv::imwrite(CHECKER_SAVE_PATH2, cvtex2);
        free((void*)tex);
    }

    TEST(SUIT, tex_checker2)
    {
        BL_checkerconf2_t conf = {
            {
                false, {640,480}, {64,64},
                { COLOR0, COLOR1 }
            },
            { 128, 256}, { COLOR2, COLOR3 }
        };

        pBL_texture_t tex = BL_texture_checker2(&conf);
        BL_ptr_t ptr = BL_arrayMD_begin(tex);
        cv::Mat3b cvtex((int)conf.base.wh[1], (int)conf.base.wh[0], (cv::Vec3b*)ptr._1u8);
        cv::imwrite(CHECKER_SAVE_PATH3, cvtex);
        free((void*)tex);
        conf.base.with_alpha = true;
        tex = BL_texture_checker2(&conf);
        BL_ptr_t ptr2 = BL_arrayMD_begin(tex);
        cv::Mat4b cvtex2((int)conf.base.wh[1], (int)conf.base.wh[0], (cv::Vec4b*)ptr2._1u8);
        cv::imwrite(CHECKER_SAVE_PATH4, cvtex2);
        free((void*)tex);
    }

    TEST(SUIT, tex_dots)
    {
        BL_checkerconf_t conf = {
            false, { 640, 480 }, { 64, 64 },
            { COLOR0, COLOR1 }
        };
        pBL_texture_t tex = BL_texture_circulardots(&conf);
        BL_ptr_t ptr = BL_arrayMD_begin(tex);
        cv::Mat3b cvtex((int)conf.wh[1], (int)conf.wh[0], (cv::Vec3b*)ptr._1u8);
        cv::imwrite(DOTS_SAVE_PATH, cvtex);
        free((void*)tex);
    }

    TEST(SUIT, tex_dots2)
    {
        BL_checkerconf2_t conf = {
            {
                false, {640,480}, {32,64},
                { COLOR0, COLOR1 }
            },
            { 128, 256}, { COLOR2, COLOR3 }
        };
        pBL_texture_t tex = BL_texture_circulardots2(&conf);
        BL_ptr_t ptr = BL_arrayMD_begin(tex);
        cv::Mat3b cvtex((int)conf.base.wh[1], (int)conf.base.wh[0], (cv::Vec3b*)ptr._1u8);
        cv::imwrite(DOTS_SAVE_PATH2, cvtex);
        free((void*)tex);
    }
}