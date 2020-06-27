#include    "base_g/BL_bilinearinterp.h"
#include    <math.h>
/*
calculate bilinear interpolation coefficients, refering to Wikipedia.
https://en.wikipedia.org/wiki/Bilinear_interpolation
*/
static void calc_a_element(const BL_4r32_t fQ, const BL_2r32_t real_grid_pitch, BL_1r32_t* a)
{
    const float _dx = 1.0f/real_grid_pitch[0];
    const float _dy = 1.0f/real_grid_pitch[1];
    const float _dxdy = _dx * _dy;
    a[0] = fQ[0];
    a[1] = _dx * (-fQ[0] + fQ[1]);
    a[2] = _dy * (-fQ[0] + fQ[2]);
    a[3] = _dxdy * (fQ[0] - fQ[1] - fQ[2] + fQ[3]);
}

pBL_bilinearinterp_t BL_bilinearinterp_new(
    const BL_2u32_t wh_nodes,
    const BL_2r32_t table_origin,
    const BL_2r32_t grid_pitch)
{
    pBL_bilinearinterp_t p = (pBL_bilinearinterp_t)malloc(sizeof(BL_bilinearinterp_t));
    BL_copy2(p->table_org_offset, table_origin);
    BL_copy2(p->grid_pitch, grid_pitch);
    BL_arrayMD_dims dims = { (uint16_t)wh_nodes[0], (uint16_t)wh_nodes[1], 1,1,1,1,1,1 };
    p->values = BL_arrayMD_new(dims, BL_1r32_t);
    dims[0]--;
    dims[1]--;
    p->coeff = BL_arrayMD_new(dims, BL_4r32_t);
    return p;
}

void BL_bilinearinterp_delete(pBL_bilinearinterp_t* ppobj)
{
    if (ppobj && (*ppobj))
    {
        pBL_bilinearinterp_t p = *ppobj;
        free((void*)(p->values));
        free((void*)(p->coeff));
        free((void*)p);
        *ppobj = NULL;
    }
}

void BL_bilinearinterp_fill_coeff(pBL_bilinearinterp_t obj)
{
    BL_cptr_t i_values = BL_arrayMD_cbegin(obj->values);
    BL_ptr_t i_coeff = BL_arrayMD_begin(obj->coeff);
    uint16_t* dims_coeff = obj->coeff->dims;
    uint16_t* dims_values = obj->values->dims;
    for (uint16_t iy = 0; iy != dims_coeff[1]; iy++)
    {
        for (uint16_t ix = 0; ix != dims_coeff[0]; ix++)
        {
            const BL_4r32_t fQ = {
                *i_values._1r32,
                *(i_values._1r32 + 1),
                *(i_values._1r32 + dims_values[0]),
                *(i_values._1r32 + dims_values[0] + 1)
            };
            calc_a_element(fQ, obj->grid_pitch, i_coeff._1r32);
            i_values._1r32++;
            i_coeff._4r32++;
        }
        // values table width is larger than that of coeff table by 1 element.
        // skip the end node of each row.
        i_values._1r32++;
    }
}

int BL_bilinearinterp_calc_value(pcBL_bilinearinterp_t obj, const BL_2r32_t xy, BL_1r32_t* value)
{
    int err = ESUCCESS;
    do {
        BL_2r32_t in_table_offset = {
            xy[0] - obj->table_org_offset[0], 
            xy[1] - obj->table_org_offset[1]
        };
        BL_2i32_t cell_index = {
            (int32_t)floorf(in_table_offset[0]/(obj->grid_pitch[0])),
            (int32_t)floorf(in_table_offset[1]/(obj->grid_pitch[1]))
        };
        const int32_t cell_index_limit_x = obj->coeff->dims[0];
        const int32_t cell_index_limit_y = obj->coeff->dims[1];
        if ((cell_index[0] < 0) ||
            (cell_index[1] < 0) ||
            (cell_index[0] >= cell_index_limit_x) ||
            (cell_index[1] >= cell_index_limit_y))
        { // out-of-range error
            err = ERANGE;
            break;
        }
        const BL_2r32_t in_cell_offset_xy = {
            in_table_offset[0] - (float)cell_index[0] * obj->grid_pitch[0],
            in_table_offset[1] - (float)cell_index[1] * obj->grid_pitch[1]
        };
        BL_cptr_t i_coeff = BL_arrayMD_cbegin(obj->coeff);
        i_coeff._4r32 += (cell_index[0] + cell_index[1] * obj->coeff->dims[0]);
        *value =
            i_coeff._1r32[0] +
            i_coeff._1r32[1] * in_cell_offset_xy[0] +
            i_coeff._1r32[2] * in_cell_offset_xy[1] +
            i_coeff._1r32[3] * in_cell_offset_xy[0] * in_cell_offset_xy[1];
    } while (0);
    return err;
}

void    BL_bilinearinterp_xy_range(pcBL_bilinearinterp_t obj, BL_2r32_t begin, BL_2r32_t end)
{
    begin[0] = obj->table_org_offset[0];
    begin[1] = obj->table_org_offset[1];
    end[0] = begin[0] + obj->values->dims[0] * obj->grid_pitch[0];
    end[1] = begin[1] + obj->values->dims[1] * obj->grid_pitch[1];
}

static int check_range(const BL_2r32_t xy, const BL_2r32_t begin, const BL_2r32_t end)
{
    return ((xy[0] < begin[0]) || (xy[1] < begin[1]) || (xy[0] >= end[0]) || (xy[1] >= end[1])) ?
        ERANGE : ESUCCESS;
}

static void scan_end(const BL_2r32_t begin, const BL_2r32_t pitch, const BL_2u16_t grid_wh,
    BL_2r32_t end)
{
    end[0] = begin[0] + pitch[0] * grid_wh[0];
    end[1] = begin[1] + pitch[1] * grid_wh[1];
}

int BL_bilinearinterp_calc_values(
    pcBL_bilinearinterp_t obj,
    const BL_2r32_t begin,
    const BL_2r32_t pitch,
    pBL_arrayMD_t values)
{
    int err = ESUCCESS;
    do {
        // check range of calculation target grid is in the range of the interpolator.
        BL_2r32_t range_begin, range_end;
        BL_bilinearinterp_xy_range(obj, range_begin, range_end);
        if (ESUCCESS != (err = check_range(begin, range_begin, range_end)))
        {
            break;
        }
        BL_2r32_t grid_end;
        scan_end(begin, pitch, obj->values->dims, grid_end);
        if (ESUCCESS != (err = check_range(grid_end, range_begin, range_end)))
        {
            break;
        }
        // interpolation
        BL_ptr_t i_values = BL_arrayMD_begin(values);
        for (uint16_t iy = 0; iy != values->dims[1]; iy++)
        {
            for (uint16_t ix = 0; ix != values->dims[0]; ix++)
            {
                BL_2r32_t xy = { begin[0] + pitch[0] * ix, begin[1] + pitch[1] * iy };
                BL_bilinearinterp_calc_value(obj, xy, i_values._1r32);
                i_values._1r32++;
            }
        }
    } while (false);
    return err;
}

pBL_bilinearinterp_t BL_bilinearinterpv_new(
    const BL_2u32_t wh_nodes,
    const BL_2r32_t table_origin,
    const BL_2r32_t grid_pitch,
    uint32_t dim)
{
    pBL_bilinearinterp_t p = (pBL_bilinearinterp_t)malloc(sizeof(BL_bilinearinterp_t));
    BL_copy2(p->table_org_offset, table_origin);
    BL_copy2(p->grid_pitch, grid_pitch);
    BL_arrayMD_dims dims = { (uint16_t)wh_nodes[0], (uint16_t)wh_nodes[1], (uint16_t)dim,1,1,1,1,1 };
    p->values = BL_arrayMD_new(dims, BL_1r32_t);
    dims[0]--;
    dims[1]--;
    p->coeff = BL_arrayMD_new(dims, BL_4r32_t);
    return p;
}

    
void BL_bilinearinterpv_delete(pBL_bilinearinterp_t* ppobj)
{
    BL_bilinearinterp_delete(ppobj);
}


void BL_bilinearinterpv_fill_coeff(pBL_bilinearinterp_t obj)
{
    BL_cptr_t i_values = BL_arrayMD_cbegin(obj->values);
    BL_ptr_t i_coeff = BL_arrayMD_begin(obj->coeff);
    uint16_t* dims_coeff = obj->coeff->dims;
    uint16_t* dims_values = obj->values->dims;
    for (uint16_t iz = 0; iz != dims_coeff[2]; iz++)
    {
        for (uint16_t iy = 0; iy != dims_coeff[1]; iy++)
        {
            for (uint16_t ix = 0; ix != dims_coeff[0]; ix++)
            {
                const BL_4r32_t fQ = {
                    *i_values._1r32,
                    *(i_values._1r32 + 1),
                    *(i_values._1r32 + dims_values[0]),
                    *(i_values._1r32 + dims_values[0] + 1)
                };
                calc_a_element(fQ, obj->grid_pitch, i_coeff._1r32);
                i_values._1r32++;
                i_coeff._4r32++;
            }
            // values table width is larger than that of coeff table by 1 element.
            // skip the end node of each row.
            i_values._1r32++;
        }
        i_values._1r32 += dims_values[0];
    }
}


int BL_bilinearinterpv_calc_value(pcBL_bilinearinterp_t obj, const BL_2r32_t xy, BL_1r32_t* value)
{
    int err = ESUCCESS;
    do {
        BL_2r32_t in_table_offset = {
            xy[0] - obj->table_org_offset[0], 
            xy[1] - obj->table_org_offset[1]
        };
        BL_2i32_t cell_index = {
            (int32_t)floorf(in_table_offset[0]/(obj->grid_pitch[0])),
            (int32_t)floorf(in_table_offset[1]/(obj->grid_pitch[1]))
        };
        const int32_t cell_index_limit_x = obj->coeff->dims[0];
        const int32_t cell_index_limit_y = obj->coeff->dims[1];
        if ((cell_index[0] < 0) ||
            (cell_index[1] < 0) ||
            (cell_index[0] >= cell_index_limit_x) ||
            (cell_index[1] >= cell_index_limit_y))
        { // out-of-range error
            err = ERANGE;
            break;
        }
        const BL_2r32_t in_cell_offset_xy = {
            in_table_offset[0] - (float)cell_index[0] * obj->grid_pitch[0],
            in_table_offset[1] - (float)cell_index[1] * obj->grid_pitch[1]
        };
        BL_cptr_t i_coeff = BL_arrayMD_cbegin(obj->coeff);
        const uint16_t* dims = obj->coeff->dims;
        for (uint16_t ich = 0; ich != dims[2]; ich++)
        {
            const BL_1r32_t* p_coeff = 
                (const BL_1r32_t*)(i_coeff._4r32 + (cell_index[0] + cell_index[1] * obj->coeff->dims[0]));
            value[ich] =
                p_coeff[0] +
                p_coeff[1] * in_cell_offset_xy[0] +
                p_coeff[2] * in_cell_offset_xy[1] +
                p_coeff[3] * in_cell_offset_xy[0] * in_cell_offset_xy[1];
            i_coeff._4r32 += (dims[0] * dims[1]);
        }
    } while (0);
    return err;
}


int BL_bilinearinterpv_calc_values(
    pcBL_bilinearinterp_t obj,
    const BL_2r32_t begin,
    const BL_2r32_t pitch,
    pBL_arrayMD_t values)
{
    int err = ESUCCESS;
    do {
        // check range of calculation target grid is in the range of the interpolator.
        BL_2r32_t range_begin, range_end;
        BL_bilinearinterp_xy_range(obj, range_begin, range_end);
        if (ESUCCESS != (err = check_range(begin, range_begin, range_end)))
        {
            break;
        }
        BL_2r32_t grid_end;
        scan_end(begin, pitch, obj->values->dims, grid_end);
        if (ESUCCESS != (err = check_range(grid_end, range_begin, range_end)))
        {
            break;
        }
        // interpolation
        uint32_t element_dim = obj->coeff->dims[2];
        BL_ptr_t i_values = BL_arrayMD_begin(values);
        for (uint16_t iy = 0; iy != values->dims[1]; iy++)
        {
            for (uint16_t ix = 0; ix != values->dims[0]; ix++)
            {
                BL_2r32_t xy = { begin[0] + pitch[0] * ix, begin[1] + pitch[1] * iy };
                BL_bilinearinterpv_calc_value(obj, xy, i_values._1r32);
                i_values._1r32 += element_dim;
            }
        }
    } while (0);
    return err;
}


pBL_bilinearinterp_t BL_bilinearinterpv_new2(
    const BL_2u32_t wh_nodes,
    const BL_3u8_t* bgr_pixels
) {
    const BL_2r32_t table_origin = {0.0f, 0.0f};
    const BL_2r32_t grid_pitch = {1.0f, 1.0f};
    pBL_bilinearinterp_t p = BL_bilinearinterpv_new(wh_nodes, table_origin, grid_pitch, 3);
    const uint16_t* dims = p->values->dims;
    BL_ptr_t i_values = BL_arrayMD_begin(p->values);
    for (uint16_t ich = 0; ich != dims[2]; ich++)
    {
        const BL_1u8_t* src = (const BL_1u8_t*)bgr_pixels + ich;
        for (uint16_t iy = 0; iy != dims[1]; iy++)
        {
            for (uint16_t ix = 0; ix != dims[0]; ix++)
            {
                *i_values._1r32++ = (BL_1r32_t)(*src);
                src += dims[2];
            }
        }
    }
    BL_bilinearinterpv_fill_coeff(p);
    return p;
}


pBL_bilinearinterp_t BL_bilinearinterpv_new3(
    const BL_2u32_t wh_nodes,
    const BL_4u8_t* bgra_pixels
) {
    const BL_2r32_t table_origin = {0.0f, 0.0f};
    const BL_2r32_t grid_pitch = {1.0f, 1.0f};
    pBL_bilinearinterp_t p = BL_bilinearinterpv_new(wh_nodes, table_origin, grid_pitch, 4);
    const uint16_t* dims = p->values->dims;
    BL_ptr_t i_values = BL_arrayMD_begin(p->values);
    for (uint16_t ich = 0; ich != dims[2]; ich++)
    {
        const BL_1u8_t* src = (const BL_1u8_t*)bgra_pixels + ich;
        for (uint16_t iy = 0; iy != dims[1]; iy++)
        {
            for (uint16_t ix = 0; ix != dims[0]; ix++)
            {
                *i_values._1r32++ = (BL_1r32_t)(*src);
                src += dims[2];
            }
        }
    }
    BL_bilinearinterpv_fill_coeff(p);
    return p;
}