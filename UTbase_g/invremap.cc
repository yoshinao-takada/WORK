#include    "UTbase_g/testdefs.h"
#include    <gtest/gtest.h>
#include    "base_g/BL_invremap.h"
#include    "base_l/BL_futils.h"
#include    "base_l/BL_errno.h"
#include    <opencv2/core.hpp>
#include    <opencv2/imgproc.hpp>
#include    <opencv2/imgcodecs.hpp>
#include    <vector>

#define SUIT    InvRemap
#define RESULT_DIR_  RESULT_DIR "InvRemap/"

#define WK_SIZE     { 640, 480 }
#define ROI         { 40, 40, 560, 400 }
#define SHIFT0      { -22.0f, -18.0f}
#define DSHIFTX     { (22.0f/1120.0f), (18.0f/1120.0f) }
#define DSHIFTY     { (22.0f/800.0f), (18.0f/800.0f) }
#define SSMAP_CSV   RESULT_DIR_  "ssmap.csv"
#define NBMAP_CSV   RESULT_DIR_  "nbmap.csv"
#define REMAP_CSV   RESULT_DIR_  "remap.csv"
#define SRCIMG_JPG  RESULT_DIR_  "srcimg.jpg"
#define SRCIMGCENTER_JPG    RESULT_DIR_  "srcimg_center.jpg"
#define DSTIMG_JPG  RESULT_DIR_  "dstimg.jpg"
#define RESTOREDIMG_JPG  RESULT_DIR_  "restoredimg.jpg"
#define RESTOREDIMG_CENTER_JPG  RESULT_DIR_ "restoredimg_center.jpg"
namespace
{
    int PrepareDir()
    {
        int err = ESUCCESS;
        do {
            if (!BL_futils_exists(RESULT_DIR_))
            {
                err = BL_futils_create_directories(RESULT_DIR_);
            }
        } while (false);
        return err;
    }

    int create_shiftmap(pBL_source_shift_map_t *ppssmap, pBL_neighbormap_t *ppnbmap)
    {
        const BL_2u32_t work_size = WK_SIZE;
        const BL_source_shift_map_TPG_conf_t conf = {
            ROI, SHIFT0, DSHIFTX, DSHIFTY
        };
        int err = ESUCCESS;
        do {
            PrepareDir();
            if (ESUCCESS != (err = BL_source_shift_map_new(work_size, ppssmap)))
            {
                break;
            }
            if (ESUCCESS != (err = BL_source_shift_map_create(*ppssmap, &conf)))
            {
                break;
            }
            if (ESUCCESS != (err = BL_neighbormap_new(work_size, ppnbmap)))
            {
                break;
            }
            BL_neighbormap_fill(*ppnbmap, *ppssmap);
        } while (false);
        return err;
    }
    // examine neighbormap contents
    TEST(SUIT, nbmap)
    {
        PrepareDir();
        FILE* pf = nullptr;
        pBL_source_shift_map_t ssmap = nullptr;
        pBL_neighbormap_t nbmap = nullptr;
        ASSERT_EQ(ESUCCESS, create_shiftmap(&ssmap, &nbmap));
        ASSERT_EQ(ESUCCESS, BL_futils_open(&pf, SSMAP_CSV, "w"));
        BL_source_shift_map_savecsv(ssmap, pf);
        if (pf)
        {
            fclose(pf);
            pf = nullptr;
        }
        ASSERT_EQ(ESUCCESS, BL_futils_open(&pf, NBMAP_CSV, "w"));
        BL_neighbormap_savecsv(pf, nbmap);
        if (pf)
        {
            fclose(pf);
            pf = nullptr;
        }
    }

    TEST(SUIT, create_remap)
    {
        PrepareDir();
        FILE* pf = nullptr;
        pBL_source_shift_map_t ssmap = nullptr;
        pBL_neighbormap_t nbmap = nullptr;
        pBL_remap_t remap = nullptr;
        ASSERT_EQ(ESUCCESS, create_shiftmap(&ssmap, &nbmap));
        ASSERT_EQ(ESUCCESS, BL_invremap_create(ssmap, &remap));
        ASSERT_EQ(ESUCCESS, BL_futils_open(&pf, REMAP_CSV, "w"));
        BL_remap_savecsv(pf, remap);
        if (pf)
        {
            fclose(pf);
            pf = nullptr;
        }
    }

    cv::Rect2i CenterROI()
    {
        cv::Size2i work_size = WK_SIZE;
        cv::Size2i ROI_size = { 16, 16 };
        cv::Point2i center(work_size.width/2, work_size.height/2);
        cv::Point2i origin(center.x - ROI_size.width/2, center.y - ROI_size.height/2);
        cv::Rect_<int> center_roi(origin, ROI_size);
        return center_roi;
    }

    cv::Mat1b CreateSourceImage()
    {
        const int grid_pitch = 5;
        const BL_2u32_t work_size = WK_SIZE;
        cv::Mat1b m((int)work_size[1], (int)work_size[0]);
        srand(1);
        cv::Point2i index;
        cv::Mat1b org_image(work_size[1]/grid_pitch, work_size[0]/grid_pitch);
        for (index.y = 0; index.y != org_image.rows; index.y++)
        {
            for (index.x = 0; index.x != org_image.cols; index.x++)
            {
                org_image(index) = (uint8_t)rand();
            }
        }

        for (index.y = 0; index.y != m.rows; index.y++)
        {
            for (index.x = 0; index.x != m.cols; index.x++)
            {
                cv::Point2i index_org = index/grid_pitch;
                m(index) = org_image(index_org);
            }
        }

        cv::Size2i ksize(5,5);
        cv::GaussianBlur(m, m, ksize, 2);
        cv::imwrite(SRCIMG_JPG, m);
        cv::Rect2i roi(CenterROI());
        cv::Mat1b m_center = m(roi).clone();
        cv::imwrite(SRCIMGCENTER_JPG, m_center);
        return m;
    }

    cv::Mat2f CreateCvRemapTable(pcBL_remap_t remap)
    {
        int ny = (int)remap->work_size[1], nx = (int)remap->work_size[0];
        cv::Mat2f m(ny, nx);
        cv::Point2i index;
        for (index.y = 0; index.y != ny; index.y++)
        {
            for (index.x = 0; index.x != nx; index.x++)
            {
                uint32_t linear_index = index.x + index.y * nx;
                m(index) = cv::Vec2f(
                    remap->source_coord[linear_index][0],
                    remap->source_coord[linear_index][1]
                );
            }
        }
        return m;
    }

    cv::Mat2f CreateCvRemapTable(pcBL_source_shift_map_t ssmap)
    {
        int ny = (int)ssmap->work_size[1], nx = (int)ssmap->work_size[0];
        cv::Mat2f m(ny, nx);
        cv::Point2i index;
        for (index.y = 0; index.y != ny; index.y++)
        {
            for (index.x = 0; index.x != nx; index.x++)
            {
                uint32_t linear_index = index.x + index.y * nx;
                m(index) = cv::Vec2f(
                    index.x + ssmap->shift[linear_index][0],
                    index.y + ssmap->shift[linear_index][1]
                );
            }
        }
        return m;
    }
    TEST(SUIT, invremap)
    {
        cv::Mat1b m = CreateSourceImage();
        pBL_source_shift_map_t ssmap = nullptr;
        pBL_neighbormap_t nbmap = nullptr;
        pBL_remap_t remap = nullptr;
        ASSERT_EQ(ESUCCESS, create_shiftmap(&ssmap, &nbmap));
        ASSERT_EQ(ESUCCESS, BL_invremap_create(ssmap, &remap));
        cv::Mat2f cvremap_table = CreateCvRemapTable(remap);
        cv::Mat1b remapped;
        cv::remap(CreateSourceImage(), remapped, cvremap_table, cv::Mat(), cv::INTER_LINEAR);
        printf("remapped.size = (%d, %d)\n", remapped.cols, remapped.rows);
        cv::imwrite(DSTIMG_JPG, remapped);
        cv::Mat2f cvremap_table2 = CreateCvRemapTable(ssmap);
        cv::Mat1b reremapped;
        cv::remap(remapped, reremapped, cvremap_table2, cv::Mat(), cv::INTER_LINEAR);
        cv::imwrite(RESTOREDIMG_JPG, reremapped);
        cv::Mat1b reremapped_center = reremapped(CenterROI()).clone();
        cv::imwrite(RESTOREDIMG_CENTER_JPG, reremapped_center);
    }
}