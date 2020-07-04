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
    BL_arrayMD_dims dims = { (uint16_t)wh_nodes[0], (uint16_t)wh_nodes[1] };
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
    end[0] = begin[0] + obj->coeff->dims[0] * obj->grid_pitch[0];
    end[1] = begin[1] + obj->coeff->dims[1] * obj->grid_pitch[1];
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
    uint32_t dim_element_vector)
{
    pBL_bilinearinterp_t p = (pBL_bilinearinterp_t)malloc(sizeof(BL_bilinearinterp_t));
    BL_copy2(p->table_org_offset, table_origin);
    BL_copy2(p->grid_pitch, grid_pitch);
    BL_arrayMD_dims dims = 
        { (uint16_t)dim_element_vector, (uint16_t)wh_nodes[0], (uint16_t)wh_nodes[1] };
    p->values = BL_arrayMD_new(dims, BL_1r32_t);
    dims[1]--;
    dims[2]--;
    p->coeff = BL_arrayMD_new(dims, BL_4r32_t);
    return p;
}

void BL_bilinearinterpv_fill_coeff(pBL_bilinearinterp_t obj)
{
    BL_cptr_t i_values = BL_arrayMD_cbegin(obj->values);
    BL_ptr_t i_coeff = BL_arrayMD_begin(obj->coeff);
    uint16_t* dims_coeff = obj->coeff->dims;
    uint16_t* dims_values = obj->values->dims;
    for (uint16_t iy = 0; iy != dims_coeff[2]; iy++)
    {
        for (uint16_t ix = 0; ix != dims_coeff[1]; ix++)
        {
            uint32_t i_offset_index[] = 
            { 0, dims_values[0], dims_values[0] * dims_values[1], dims_values[0] * (dims_values[1] + 1) };
            for (uint16_t iv = 0; iv != dims_coeff[0]; iv++)
            {
                const BL_4r32_t fQ = {
                    *(i_values._1r32 + i_offset_index[0]),
                    *(i_values._1r32 + i_offset_index[1]),
                    *(i_values._1r32 + i_offset_index[2]),
                    *(i_values._1r32 + i_offset_index[3])
                };
                calc_a_element(fQ, obj->grid_pitch, i_coeff._1r32);
                i_values._1r32++;
                i_coeff._4r32++;
            }
        }
        // values table width is larger than that of coeff table by 1 element.
        // skip the end node of each row.
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
        const int32_t cell_index_limit_x = obj->coeff->dims[1];
        const int32_t cell_index_limit_y = obj->coeff->dims[2];
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
        i_coeff._4r32 += obj->coeff->dims[0] * (cell_index[0] + cell_index[1] * obj->coeff->dims[1]);
        for (uint32_t iv = 0; iv != obj->coeff->dims[0]; iv++)
        {
            value[iv] = 
                i_coeff._1r32[0] +
                i_coeff._1r32[1] * in_cell_offset_xy[0] +
                i_coeff._1r32[2] * in_cell_offset_xy[1] +
                i_coeff._1r32[3] * in_cell_offset_xy[0] * in_cell_offset_xy[1];
            i_coeff._4r32++;
        }
    } while (0);
    return err;
}

void    BL_bilinearinterpv_xy_range(pcBL_bilinearinterp_t obj, BL_2r32_t begin, BL_2r32_t end)
{
    begin[0] = obj->table_org_offset[0];
    begin[1] = obj->table_org_offset[1];
    end[0] = begin[0] + obj->coeff->dims[1] * obj->grid_pitch[0];
    end[1] = begin[1] + obj->coeff->dims[2] * obj->grid_pitch[1];
}

int BL_bilinearinterpv_calc_values(
    pcBL_bilinearinterp_t obj,
    const BL_2r32_t begin,
    const BL_2r32_t pitch,
    const BL_2u32_t wh,
    pBL_arrayMD_t* ppvalues)
{
    int err = ESUCCESS;
    do {
        // check if ppvalues is valid and cleared.
        if (!ppvalues || *ppvalues)
        {
            err = EINVAL;
            break;
        }
        // check range of calculation target grid is in the range of the interpolator.
        BL_2r32_t range_begin, range_end;
        BL_bilinearinterpv_xy_range(obj, range_begin, range_end);
        if (ESUCCESS != (err = check_range(begin, range_begin, range_end)))
        {
            break;
        }
        BL_2r32_t grid_end;
        BL_2u16_t wh_ = { (uint16_t)wh[0], (uint16_t)wh[1] };
        scan_end(begin, pitch, wh_, grid_end);
        if (ESUCCESS != (err = check_range(grid_end, range_begin, range_end)))
        {
            break;
        }
        // allocate memory block to *ppvales
        BL_arrayMD_dims dims_values = { obj->values->dims[0], wh[0], wh[1] };
        pBL_arrayMD_t values = (*ppvalues = BL_arrayMD_new(dims_values, BL_1r32_t));

        // interpolation
        uint32_t element_dim = obj->coeff->dims[2];
        BL_ptr_t i_values = BL_arrayMD_begin(values);
        for (uint16_t iy = 0; iy != values->dims[2]; iy++)
        {
            for (uint16_t ix = 0; ix != values->dims[1]; ix++)
            {
                BL_2r32_t xy = { begin[0] + pitch[0] * ix, begin[1] + pitch[1] * iy };
                BL_bilinearinterpv_calc_value(obj, xy, i_values._1r32);
                i_values._1r32 += obj->values->dims[0];
            }
        }
    } while (0);
    return err;
}


pBL_bilinearinterp_t BL_bilinearinterpv_newBGR(
    const BL_2u32_t wh_nodes,
    const BL_3u8_t* bgr_pixels
) {
    const BL_2r32_t table_origin = {0.0f, 0.0f};
    const BL_2r32_t grid_pitch = {1.0f, 1.0f};
    pBL_bilinearinterp_t p = BL_bilinearinterpv_new(wh_nodes, table_origin, grid_pitch, 3);
    const uint16_t* dims = p->values->dims;
    BL_ptr_t i_values = BL_arrayMD_begin(p->values);
    const BL_1u8_t* src = (const BL_1u8_t*)bgr_pixels;
    for (uint16_t iy = 0; iy != dims[2]; iy++)
    {
        for (uint16_t ix = 0; ix != dims[1]; ix++)
        {
            for (uint16_t iv = 0; iv != dims[0]; iv++)
            {
                *i_values._1r32++ = (BL_1r32_t)(*src++);
            }
        }
    }
    BL_bilinearinterpv_fill_coeff(p);
    return p;
}

int BL_bilinearinterpv_getBGR(
    pcBL_bilinearinterp_t obj,
    const BL_2r32_t begin,
    const BL_2r32_t pitch,
    const BL_2u32_t wh,
    pBL_arrayMD_t* ppimage)
{
    pBL_arrayMD_t values = (pBL_arrayMD_t)NULL;
    int err = ESUCCESS;
    do {
        if (ESUCCESS != (err = BL_bilinearinterpv_calc_values(obj, begin, pitch, wh, &values))) { break; }
        pBL_arrayMD_t image = (*ppimage = BL_arrayMD_new(values->dims, BL_1u8_t));
        if (!image)
        {
            err = ENOMEM;
            break;
        }
        BL_cptr_t i_src = BL_arrayMD_cbegin(values);
        BL_ptr_t i_dst = BL_arrayMD_begin(image);
        uint32_t cb_array = values->dims[0] * values->dims[1] * values->dims[2];
        do {
            *i_dst._1u8++ = CLAMP_R32_TO_U8(*i_src._1r32);
            // i_src._1r32 is referenced multiple times in CLAMP_R32_TO_U8(). Don't increment in () of the macro.
            i_src._1r32++; 
        } while (--cb_array);
    } while (0);
    BL_SAFEFREE(&values);
    return err;
}