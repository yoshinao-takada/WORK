#include    <gtest/gtest.h>
#include    <base_cg/BL_oomesh.h>
#include    <base_cg/BL_glctx.h>
#include    <base_cg/BL_glutils.h>
#include    <base_l/BL_data2D.h>

#define     SUIT     viewer0
#define     DEF_SIZE2D  { 320, 240 }
#define     _3U8_WHITE  { 255,255,255 }
#define     _3U8_BLACK  { 0, 0, 0 }
#define     _3U8_RED    { 0, 0, 255 }
#define     _3U8_BLUE   { 255, 0, 0 }

namespace td 
{
    static const BL_2u32_t size2D = DEF_SIZE2D;
    static pBL_data2D_t tex = nullptr;
    static pBL_oomesh_t mesh = nullptr;
    static const BL_3u8_t colors[] = { _3U8_RED, _3U8_WHITE, _3U8_BLUE };
}

namespace
{
    void create_texture()
    {

        td::tex = BL_data2D_alloc(td::size2D, 3);
        size_t grid_pitch = __max(4, __min(td::size2D[0], td::size2D[1])/16);
        BL_3u8_t* ptex = (BL_3u8_t*)td::tex->ptr;
        for (uint32_t iy = 0; iy < td::size2D[1]; iy++)
        {
            uint32_t iy_grid = iy / grid_pitch;
            for (uint32_t ix = 0; ix < td::size2D[0]; ix++)
            {
                uint32_t ix_grid = ix/grid_pitch;
                if (ix_grid == 0 && iy_grid == 0)
                {
                    BL_copy3(*ptex++, td::colors[2]);
                }
                else
                {
                    uint32_t color_index = (ix_grid + iy_grid) & 1;
                    BL_copy3(*ptex++, td::colors[color_index]);
                }
                
            }
        }
    }

    void create_mesh()
    {
        td::mesh = BL_oomesh_new()
    }

    TEST(SUIT, viewer_texturedmesh)
    {

    }
} 