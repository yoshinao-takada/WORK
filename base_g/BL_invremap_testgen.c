#include    "base_g/BL_invremap.h"
#include    "base_l/BL_base.h"
#include    "base_l/BL_errno.h"
#include    "base_l/BL_types.h"

int BL_source_shift_map_new(const BL_2u32_t work_size, pBL_source_shift_map_t* ppmap)
{
    int err = ESUCCESS;
    do {
        size_t cb_data = work_size[0] * work_size[1] * sizeof(BL_2r32_t);
        size_t cb_alloc = sizeof(BL_source_shift_map_t) + cb_data;
        pBL_source_shift_map_t p = (*ppmap = (pBL_source_shift_map_t)malloc(cb_alloc));
        if (!p)
        {
            err = ENOMEM;
            break;
        }
        p->roi[0] = p->roi[1] = 0;
        p->roi[2] = work_size[0];
        p->roi[3] = work_size[1];
        BL_copy2(p->work_size, work_size);
        BL_1r32_t* pdata = (BL_1r32_t*)p->shift;
        uint32_t count = work_size[0] * work_size[1];
        do {
            *pdata++ = 0.0f;
            *pdata++ = 0.0f;
        } while (--count);
    } while (0);
    return err;
}

int BL_source_shift_map_create(pBL_source_shift_map_t map, pcBL_source_shift_map_TPG_conf_t conf)
{
    int err = ESUCCESS;
    do {
        if (conf->roi[2] >= map->work_size[0] || conf->roi[3] >= map->work_size[1])
        {
            err = ERANGE; 
            break;
        }
        BL_copy4(map->roi, conf->roi);
        BL_2u32_t i = {0,0};
        BL_2r32_t* p = map->shift;
        for (i[1] = 0; i[1] != map->work_size[1]; i[1]++)
        {
            for (i[0] = 0; i[0] != map->work_size[0]; i[0]++)
            {
                if (BL_is_in_rect(i, map->roi) == ESUCCESS)
                {
                    BL_2u32_t offset = { i[0] - map->roi[0], i[1] - map->roi[1] };
                    (*p)[0] = conf->shift0[0] + conf->dshiftx[0] * offset[0] + conf->dshifty[0] * offset[1];
                    (*p)[1] = conf->shift0[1] + conf->dshiftx[1] * offset[0] + conf->dshifty[1] * offset[1];
                }
                else
                {
                    (*p)[0] = (*p)[1] = 0.0f;
                }
                p++;
            }
        }
    } while (0);
    return err;
}

int BL_source_shift_map_savecsv(pcBL_source_shift_map_t map, FILE* pf)
{
    int err = ESUCCESS;
    do {
        fprintf(pf, "x,y,shift-x, shift-y\n");
        BL_2u32_t roi_end = { map->roi[0] + map->roi[2], map->roi[1] + map->roi[3] };
        BL_2u32_t xy;
        for (xy[1] = map->roi[1]; xy[1] != roi_end[1]; xy[1]++)
        {
            for (xy[0] = map->roi[0]; xy[0] != roi_end[0]; xy[0]++)
            {
                uint32_t linear_index = xy[0] + xy[1] * map->work_size[0];
                fprintf(pf, "%d,%d,%f,%f\n", xy[0], xy[1], map->shift[linear_index][0], map->shift[linear_index][1]);
            }
        }
    } while (0);
    return err;
}