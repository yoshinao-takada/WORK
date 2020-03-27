#include <gtest/gtest.h>
#include <base_cg/BL_oomesh.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#define SUIT    oomesh
#define DEFAULT_MASK_PATH   UTDATA "In/two-islands-640x480.png"

namespace
{
    TEST(SUIT, create_instance)
    {
        cv::Mat3b cvimg_mask = cv::imread(DEFAULT_MASK_PATH);
        BL_2u32_t wh = { (uint32_t)cvimg_mask.cols, (uint32_t)cvimg_mask.rows };
        pBL_oomesh_t mesh = BL_oomesh_new(wh);
    }
}