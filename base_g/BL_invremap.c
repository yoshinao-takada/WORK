#include    "base_g/BL_invremap.h"
#include    <math.h>
#include    "base_g/BL_matexport.h"
#include    "base_l/BL_futils.h"


static const char* neighbormap_save_csv = NULL;
void BL_invremap_set_neighbormapsavepath(const char* path)
{
    neighbormap_save_csv = path;
}

int BL_neighbormap_new(const BL_2u32_t work_size, pBL_neighbormap_t *ppmap)
{
    int err = ESUCCESS;
    do {
        size_t cb_data = work_size[0] * work_size[1] * sizeof(BL_neighbormap_element_t);
        size_t cb_alloc = cb_data + sizeof(BL_neighbormap_t);
        pBL_neighbormap_t p = (*ppmap = (pBL_neighbormap_t)calloc(cb_alloc, 1));
        if (!p)
        {
            err = ENOMEM;
            break;
        }
        BL_copy2(p->work_size, work_size);
    } while (0);
    return err;
}

void BL_neighbormap_fill(pBL_neighbormap_t nbmap, pcBL_source_shift_map_t ssmap)
{
    static const uint32_t flag_masks[] = { 1, 2, 4, 8 };
    const BL_2u32_t roi_end = { ssmap->roi[0] + ssmap->roi[2], ssmap->roi[1] + ssmap->roi[3] };
    const BL_4u32_t full_work_rect = { 0u, 0u, ssmap->work_size[0], ssmap->work_size[1] };
    BL_2u32_t i_ssmap;
    for (i_ssmap[1] = ssmap->roi[1]; i_ssmap[1] != roi_end[1]; i_ssmap[1]++)
    {
        for (i_ssmap[0] = ssmap->roi[0]; i_ssmap[0] != roi_end[0]; i_ssmap[0]++)
        {
            uint32_t linear_index = i_ssmap[0] + i_ssmap[1] * ssmap->work_size[0];
            // coordinate of the mapping destination from the source map to disparity shifted map
            const BL_2r32_t dest_coord = {
                i_ssmap[0] + ssmap->shift[linear_index][0],
                i_ssmap[1] + ssmap->shift[linear_index][1]
            };
            // make floor integral number, i.e. top-left corner of the square cell surrounding the destination coord
            const BL_2u32_t dest_neighbor0 = 
                { (uint32_t)floorf(dest_coord[0]), (uint32_t)floorf(dest_coord[1]) };
            // four indices of the corners of the square cell
            BL_2u32_t dest_neighbors[] = {
                { dest_neighbor0[0], dest_neighbor0[1] },
                { dest_neighbor0[0] + 1, dest_neighbor0[1] },
                { dest_neighbor0[0], dest_neighbor0[1] + 1 },
                { dest_neighbor0[0] + 1, dest_neighbor0[1] + 1 },
            };
            // node layout of the destination cell
            // i == 0 *----* i == 1
            //        |    |
            //        |    |
            // i == 2 *----* i == 3
            for (uint i = 0; i != ARRAYSIZE(dest_neighbors); i++)
            {
                if (ERANGE == BL_is_in_rect(dest_neighbors[i], full_work_rect)) continue;
                uint32_t dest_linear_index = dest_neighbors[i][0] + dest_neighbors[i][1] * ssmap->work_size[0];
                pBL_neighbormap_element_t pelement = nbmap->elements + dest_linear_index;
                if (((pelement->flags) & flag_masks[i]) == 0)
                { // there is no preceding registration
                    BL_copy2(pelement->neighbor_sources[i], i_ssmap);
                    pelement->flags |= flag_masks[i]; 
                }
                else
                { // there is a preceding registration
                    // There is no idear to improve accuracy or reliability
                }
            }
        }
    }
    BL_neighbormap_refine(nbmap);
}

void BL_neighbormap_refine(pBL_neighbormap_t nbmap)
{
    static const uint32_t flag_masks[] = { 1, 2, 4, 8 };
    static const BL_2i32_t i2D_offset[] = { {-2,0}, {-2,0}, {-2,0}, {-2,0}};
    static const BL_2i32_t i2D_offset2[] = { {1,0}, {1,0}, {1,0}, {1,0}};
    BL_2u32_t i_nbmap;
    for (i_nbmap[1] = 0; i_nbmap[1] != nbmap->work_size[1]; i_nbmap[1]++)
    {
        for (i_nbmap[0] = 0; i_nbmap[0] != nbmap->work_size[0]; i_nbmap[0]++)
        {
            uint32_t linear_index = i_nbmap[0] + i_nbmap[1] * nbmap->work_size[0];
            pBL_neighbormap_element_t pelement = nbmap->elements + linear_index;
            for (uint32_t i_flag_masks = 0; i_flag_masks != ARRAYSIZE(flag_masks); i_flag_masks++)
            {
                if ((pelement->flags & flag_masks[i_flag_masks]) == 0)
                { // copy from the right lower element if it is not empty
                    BL_2u32_t i_nbmap_src = {
                        i_nbmap[0] + i2D_offset[i_flag_masks][0],
                        i_nbmap[1] + i2D_offset[i_flag_masks][1]
                    };
                    uint32_t linear_index = i_nbmap_src[0] + i_nbmap_src[1] * nbmap->work_size[0];
                    if ((i_nbmap_src[0] < nbmap->work_size[0]) && (i_nbmap_src[1] < nbmap->work_size[1]) &&
                        (i_nbmap_src[0] >= 0) && (i_nbmap_src[1] >= 0))
                    {
                        pBL_neighbormap_element_t pelement_src = nbmap->elements + linear_index;
                        if ((pelement_src->flags & flag_masks[i_flag_masks]) != 0)
                        {
                            pelement->flags |= flag_masks[i_flag_masks];
                            BL_copy2(
                                pelement->neighbor_sources[i_flag_masks],
                                pelement_src->neighbor_sources[i_flag_masks]);
                        }
                    }
                }
            }
            for (uint32_t i_flag_masks = 0; i_flag_masks != ARRAYSIZE(flag_masks); i_flag_masks++)
            {
                if ((pelement->flags & flag_masks[i_flag_masks]) == 0)
                { // copy from the right lower element if it is not empty
                    BL_2u32_t i_nbmap_src = {
                        i_nbmap[0] + i2D_offset2[i_flag_masks][0],
                        i_nbmap[1] + i2D_offset2[i_flag_masks][1]
                    };
                    uint32_t linear_index = i_nbmap_src[0] + i_nbmap_src[1] * nbmap->work_size[0];
                    if ((i_nbmap_src[0] < nbmap->work_size[0]) && (i_nbmap_src[1] < nbmap->work_size[1]) &&
                        (i_nbmap_src[0] >= 0) && (i_nbmap_src[1] >= 0))
                    {
                        pBL_neighbormap_element_t pelement_src = nbmap->elements + linear_index;
                        if ((pelement_src->flags & flag_masks[i_flag_masks]) != 0)
                        {
                            pelement->flags |= flag_masks[i_flag_masks];
                            BL_copy2(
                                pelement->neighbor_sources[i_flag_masks],
                                pelement_src->neighbor_sources[i_flag_masks]);
                        }
                    }
                }
            }
        }
    }
}

/*!
\brief coordinate offsets
U(x0,y0) = u0
U(x1,y1) = u1
U(x2,y2) = u2
U(x3,y3) = u3
U(x,y) = Au0 + Au1*x + Au2*y + Au3*x*y
[u0,u1,u2,u3]t =
[
    [1,x0,y0,x0*y0],
    [1,x1,y1,x1*y1],
    [1,x2,y2,x2*y2],
    [1,x3,y3,x3*y3]
] * [Au0,Au1,Au2,Au3]t
[v0,v1,v2,v3]t =
[
    [1,x0,y0,x0*y0],
    [1,x1,y1,x1*y1],
    [1,x2,y2,x2*y2],
    [1,x3,y3,x3*y3]
] * [Av0,Av1,Av2,Av3]t
*/
int BL_calc_source_coord(const BL_1r32_t *xy, const BL_1r32_t *uv, BL_2r32_t u0v0)
{
    const size_t N = 4; // number of linear equations
    const size_t NBC = 2; // number of boundary conditions
    int err = ESUCCESS;
    pcBL_matfnr32RM_t fn = BL_matfnr32RM();
    do {
        BL_1r32_t matxy[] = {
            1.0f, xy[0], xy[1], xy[0] * xy[1],
            1.0f, xy[2], xy[3], xy[2] * xy[3],
            1.0f, xy[4], xy[5], xy[4] * xy[5],
            1.0f, xy[6], xy[7], xy[6] * xy[7]
        }; // [[1,x0,y0,x0*y0],[1,x1,y1,x1*y1],[1,x2,y2,x2*y2],[1,x3,y3,x3*y3], ]
        BL_1r32_t rhs[] = {
            uv[0], uv[1],
            uv[2], uv[3],
            uv[4], uv[5],
            uv[6], uv[7],
        };
        BL_1r32_t a[ARRAYSIZE(rhs)];
        BL_1r32_t matwork[ARRAYSIZE(matxy) + ARRAYSIZE(rhs)];
        fn->setsubmat(matwork, matxy, N+NBC, N, N, N, 0, 0);
        fn->setsubmat(matwork, rhs, N+NBC, N, NBC, N, N, 0);
        if (ESUCCESS != (err = fn->soldense(matwork, N+NBC, N, a)))
        {
            break;
        }
        BL_copy2(u0v0, a);
    } while(0);
    return err;
}

/*!
\fill remap in conjunction with nbmap
*/
static void BL_neighbormap_to_remap(pBL_remap_t remap, pcBL_neighbormap_t nbmap, pcBL_source_shift_map_t ssmap)
{
    BL_2u32_t dest_uv; // destination map node index
    for (dest_uv[1] = 0; dest_uv[1] != remap->work_size[1]; dest_uv[1]++)
    {
        for (dest_uv[0] = 0; dest_uv[0] != remap->work_size[0]; dest_uv[0]++)
        {
            uint32_t linear_index = dest_uv[0] + dest_uv[1] * remap->work_size[0];
            pcBL_neighbormap_element_t pelement = nbmap->elements + linear_index;
            if (pelement->flags & 0x0f)
            {
                BL_2r32_t dest_coord_deltas[4];
                BL_2r32_t idx_src2Ds[4];
                for (uint32_t i = 0; i != ARRAYSIZE(dest_coord_deltas); i++)
                {
                    const BL_2u32_t idx_src2D = 
                    { pelement->neighbor_sources[i][0], pelement->neighbor_sources[i][1] };
                    uint32_t index_src_linear = idx_src2D[0] + idx_src2D[1] * ssmap->work_size[0];
                    const BL_2r32_t* pshift = ssmap->shift + index_src_linear;
                    BL_2r32_t dest_coord_delta = {
                        (*pshift)[0] + idx_src2D[0] - dest_uv[0],
                        (*pshift)[1] + idx_src2D[1] - dest_uv[1]
                    };
                    BL_copy2(dest_coord_deltas[i], dest_coord_delta);
                    BL_copy2(idx_src2Ds[i], idx_src2D);
                }
                {
                    int err = BL_calc_source_coord(
                        (const BL_1r32_t*)dest_coord_deltas, (const BL_1r32_t*)idx_src2Ds,
                        remap->source_coord[linear_index]);
                    if (err)
                    {
                        // fprintf(stderr, "%s,%d,singular,(u,v)=%d,%d\n", __FUNCTION__, __LINE__, dest_uv[0], dest_uv[1]);
                    }
                }
            }
            else
            {
                remap->rel[linear_index] = 1; // unreliable
            }
        }
    }
}

int BL_remap_new(const BL_2u32_t work_size, pBL_remap_t* ppremap)
{
    int err = ESUCCESS;
    do {
        uint32_t work_area = work_size[0] * work_size[1];
        size_t cb_data = work_area * sizeof(BL_2r32_t) + BL_ALIGN8(work_area);
        size_t cb_alloc = cb_data + sizeof(BL_remap_t);
        pBL_remap_t p = (*ppremap = calloc(cb_alloc, 1));
        if (!p)
        {
            err = ENOMEM;
            break;
        }
        BL_copy2(p->work_size, work_size);
        p->source_coord = (BL_2r32_t *)p->data;
        p->rel = (BL_1u8_t*)(p->source_coord + work_area);
        uint32_t count = work_area;
        do {
            --count;
            p->source_coord[count][0] = p->source_coord[count][1] = 0.0f;
            p->rel[count] = 0;
        } while (count);
    } while (0);
    return err;
}

void BL_neighbormap_savecsv(FILE* pf, pcBL_neighbormap_t nbmap)
{
    fprintf(pf, "x,y,src[0].u,src[0].v,src[1].u,src[1].v,src[2].u,src[2].v,src[3].u,src[3].v,flags\n");
    for (uint32_t iy = 0; iy != nbmap->work_size[1]; iy++)
    {
        for (uint32_t ix = 0; ix != nbmap->work_size[0]; ix++)
        {
            uint32_t linear_index = ix + iy * nbmap->work_size[0];
            pcBL_neighbormap_element_t element = nbmap->elements + linear_index;
            fprintf(pf, "%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n",
                ix,iy,
                element->neighbor_sources[0][0],element->neighbor_sources[0][1],
                element->neighbor_sources[1][0],element->neighbor_sources[1][1],
                element->neighbor_sources[2][0],element->neighbor_sources[2][1],
                element->neighbor_sources[3][0],element->neighbor_sources[3][1],
                element->flags
            );
        }
    }
}
int BL_invremap_create(pcBL_source_shift_map_t ssmap, pBL_remap_t *ppremap)
{
    int err = ESUCCESS;
    pBL_neighbormap_t nbmap = NULL;
    //----------- file stream for debugging -------------//
    do {
        // init neighbor map
        if (ESUCCESS != (err = BL_neighbormap_new(ssmap->work_size, &nbmap)))
        {
            break;
        }
        // init ppremap
        if (ESUCCESS != (err = BL_remap_new(ssmap->work_size, ppremap)))
        {
            break;
        }

        BL_neighbormap_fill(nbmap, ssmap);
        BL_neighbormap_to_remap(*ppremap, nbmap, ssmap);
    } while (0);
    BL_SAFEFREE(&nbmap);
    return err;
}

void BL_remap_savecsv(FILE* pf, pcBL_remap_t remap)
{
    fprintf(pf, "x,y,src.x,src.y,rel\n");
    for (uint32_t iy = 0; iy != remap->work_size[1]; iy++)
    {
        for (uint32_t ix = 0; ix != remap->work_size[0]; ix++)
        {
            uint32_t linear_index = ix + iy * remap->work_size[0];
            fprintf(pf, "%u,%u,%f,%f,%d\n",
                ix,iy,remap->source_coord[linear_index][0], remap->source_coord[linear_index][1],
                remap->rel[linear_index]);
        }
    }
}