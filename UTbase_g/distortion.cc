#include    <gtest/gtest.h>
#include    <base_g/BL_matexport.h>
#include    <base_g/BL_distortion.h>
#include    <base_g/BL_texture.h>
#include    <base_l/BL_futils.h>
#include    <base_l/BL_debutil.h>
#include    <opencv2/core.hpp>
#include    <opencv2/imgproc.hpp>
#include    <opencv2/imgcodecs.hpp>
#define     SUIT    Dist
#define     DATADIR UTDATA
#define     SRCIMG  DATADIR "remap-src.jpg"
#define     DSTIMG  DATADIR "remap-dst.jpg"
#define     MAPCSV  DATADIR "map.csv"
// source texture configuration
#define     W_SRC   1024
#define     H_SRC   768
#define     WH_SRC  { W_SRC, H_SRC }
#define     BLOCK_SIZE  { W_SRC/32, H_SRC/32 }
#define     COLOR0  { 255,255,255,255 }
#define     COLOR1  { 0, 255, 255, 255 }
#define     COLOR2  COLOR0
#define     COLOR3  { 255,255,0,255 }

// source texture related remap configuration
#define     ROT_CENTER_SRC  { (W_SRC * 0.52f), (H_SRC * 0.49f) }
#define     ROT_ANGLE_SRC   0.0f
#define     SHIFT_SRC   { (W_SRC * 0.05f), (H_SRC * 0.03f)}
#define     CENTER_SRC { (W_SRC*0.5f), (H_SRC*0.5f)}
#define     BORDER_SRC { W_SRC/2, H_SRC/2 }

// destination texture related remap configuration
#define     W_DST   640
#define     H_DST   480
#define     WH_DST  { W_DST, H_DST }
#define     CENTER_DST  { (W_DST*0.5f), (H_DST*0.5f)}

#define     K       (-0.1f)

namespace
{
    pBL_texture_t CreateSourceImage()
    {
        BL_checkerconf2_t conf = {
            { false, WH_SRC, BLOCK_SIZE, { COLOR1, COLOR0 } },
            BORDER_SRC, { COLOR3, COLOR2 }
        };
        return BL_texture_checker2(&conf);
    }

    pBL_map2D_t CreateMap2D()
    {
        BL_rotshift_t conf = {
            WH_DST, WH_SRC, ROT_ANGLE_SRC, SHIFT_SRC
        };
        return BL_rotshift_createmap(&conf);
    }

    pBL_map2D_t CreateMap2D_dist()
    {
        BL_rotshiftdist_t conf = {
            WH_DST, WH_SRC, CENTER_DST, { 0.0015f, 0.0015f }
        };
        return BL_rotshiftdist_createmap(&conf);
    }

    TEST(SUIT, remap)
    {
        pBL_texture_t tex_src = CreateSourceImage();
        {
            BL_ptr_t ptr_src = BL_arrayMD_begin(tex_src);
            cv::Mat3b tex_src_((int)tex_src->dims[1], (int)tex_src->dims[0], (cv::Vec3b*)ptr_src._1u8);
            cv::imwrite(SRCIMG, tex_src_);
        }
        const BL_2u32_t wh_dst = WH_DST;
        pBL_texture_t tex_dst = BL_texture_alloc(wh_dst, false);
        // pBL_map2D_t map2D = CreateMap2D();
        pBL_map2D_t map2D = CreateMap2D_dist();
        FILE* pf = NULL;
        ASSERT_EQ(ESUCCESS, BL_futils_open(&pf, MAPCSV, "w"));
        BL_debutil_fwrite_array2D(pf, map2D, "y,x,fx,fy", BL_debutil_write_2r32);
        if (pf) { fclose(pf); pf = NULL; }
        BL_texture_remap(tex_src, tex_dst, map2D);
        {
            BL_ptr_t ptr_dst = BL_arrayMD_begin(tex_dst);
            cv::Mat3b tex_dst_((int)tex_dst->dims[1], (int)tex_dst->dims[0], (cv::Vec3b*)ptr_dst._1u8);
            cv::imwrite(DSTIMG, tex_dst_);
        }
        free((void*)map2D);
        free((void*)tex_dst);
        free((void*)tex_src);
    }
}