#include    <gtest/gtest.h>
#include    <base_g/BL_mesh.h>

#define SUIT    mesh
namespace 
{
    TEST(SUIT, new_delete)
    {
        const BL_arrayMD_dims dims = { 320, 240, 1, 1, 1, 1, 1, 1 };
        const BL_vertidxtype_t types[] = {
            BL_vertidx_triangle, BL_vertidx_tristrip, BL_vertidx_trifan, BL_vertidx_edge_end_pair
        };

        pBL_mesh_t pobj = BL_mesh_new(dims, BL_6r32_t, ARRAYSIZE(types), types);
        ASSERT_NE(nullptr, pobj);

        BL_mesh_delete(&pobj);
    }
}