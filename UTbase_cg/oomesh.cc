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
        size_t center_index = (wh[0]/2) + wh[0] * (wh[1])/2;
        pBL_oomesh_t mesh = BL_oomesh_new(wh);
        BL_oomesh_setdefaulttriangles(mesh);
        BL_3r32_t vert0 = { -10.0f, -5.0f, 20.0f };
        BL_3r32_t vert1 = { 10.0f, 10.0f, 20.0f };
        BL_oomesh_setvert(mesh, vert0, vert1);
        BL_2r32_t tex0 = {0.0f, 0.0f};
        BL_2r32_t tex1 = {1.0f, 1.0f};
        BL_oomesh_settex(mesh, tex0, tex1);
        ASSERT_EQ(wh[0] * wh[1], mesh->n_vert);
        ASSERT_EQ((uint8_t)BL_oomesh_attr_enabled, mesh->flags[0]);
        ASSERT_EQ((uint8_t)BL_oomesh_attr_enabled, mesh->flags[center_index]);
        BL_oomesh_applymask(mesh, (const BL_3u8_t*)cvimg_mask.ptr());
        ASSERT_EQ((uint8_t)0, mesh->flags[0]);
        ASSERT_EQ((uint8_t)BL_oomesh_attr_enabled, mesh->flags[center_index]);
    }
}