#include    "base_g/BL_discriminate.h"
// #define II_PROCESSING_VIEW_CSV      DEBUG_DATA_DIR  "II_PROCESSING.csv"
// #include    "base_l/BL_futils.h"

pBL_arrayMD_t BL_IIu8u32(pcBL_arrayMD_t image, uint32_t n_ch, uint32_t i_ch, uint32_t n_padding)
{
    if (image == NULL || n_ch == 0)
    {
        return NULL;
    }
    BL_2u32_t wh = { image->dims[0], image->dims[1] };
    BL_2u32_t wh_with_padding = { wh[0] + 2*n_padding + 2, wh[1] + 2*n_padding + 2 };
        // +2 is for integral image
    BL_arrayMD_dims ii_dims = { (uint16_t)wh_with_padding[0], (uint16_t)wh_with_padding[1], 1,1,1,1,1,1};
    pBL_arrayMD_t ii = BL_arrayMD_new(ii_dims, uint32_t);
    BL_ptr_t i_ii = BL_arrayMD_begin(ii);
    uint32_t* p_ii = i_ii._1u32 + (ii_dims[0] + 1) * (n_padding + 1);

    // copy image
    ptrdiff_t ii_skip = ii_dims[0] - image->dims[0];
    BL_cptr_t i_image = BL_arrayMD_cbegin(image);
    for (uint16_t iy = 0; iy != image->dims[1]; iy++)
    {
        for (uint16_t ix = 0; ix != image->dims[0]; ix++)
        {
            *p_ii++ = (uint32_t)(*(i_image._1u8 + i_ch));
            i_image._1u8 += n_ch;
        }
        p_ii += ii_skip;
    }
    // integration
    p_ii = i_ii._1u32 + ii->dims[0] + 1;
    for (uint16_t iy = 1; iy != ii->dims[1]; iy++)
    {
        for (uint16_t ix = 1; ix != ii->dims[0]; ix++)
        {
            uint32_t additional = *(p_ii - 1) + *(p_ii - ii->dims[0]) - *(p_ii - ii->dims[0] - 1);
            *p_ii += additional;
            p_ii++;
        }
        p_ii++;
    }
    return ii;
}

uint32_t BL_IIu32_partialsum(pcBL_arrayMD_t ii, const BL_4i16_t sum_area, uint32_t n_padding)
{
    BL_ptr_t i_ii = BL_arrayMD_begin(ii);
    uint32_t* p_ii = i_ii._1u32 + (ii->dims[0] + 1) * n_padding;
    uint32_t* p0 = p_ii + sum_area[0] + sum_area[1] * ii->dims[0];
    uint32_t* p1 = p0 + sum_area[2];
    uint32_t* p2 = p0 + sum_area[3] * ii->dims[0];
    uint32_t* p3 = p2 + sum_area[2];
    uint32_t sum = (*p3 - *p2) + (*p0 - *p1);
    return sum;
}

static int dims_ne(const BL_arrayMD_dims ref, const BL_arrayMD_dims val)
{
    int err = ESUCCESS;
    for (uint32_t i = 0; i < 8; i++)
    {
        if (ref[i] != val[i])
        {
            err = EINVAL;
            break;
        }
    }
    return err;
}

#define CLAMP_U8(f)  (uint8_t)__max(0, __min(255, (int16_t)f))
int BL_IIu32u8_boxfilter(
    pcBL_arrayMD_t ii,
    uint32_t k_2,
    uint32_t n_padding,
    pBL_arrayMD_t filtered,
    uint32_t n_ch,
    uint32_t i_ch)
{
    int err = ESUCCESS;
    do {
        BL_arrayMD_dims refdims = { ii->dims[0] - 2*(n_padding+1), ii->dims[1] - 2*(n_padding+1), 1,1,1,1,1,1 };
        if (ESUCCESS != (err = dims_ne(refdims, filtered->dims)))
        {
            break;
        }
        BL_ptr_t i_filtered = BL_arrayMD_begin(filtered);
        i_filtered._1u8 += i_ch;
        int16_t box_edge_len = 2*k_2 + 1;
        BL_4i16_t sumarea0 = {-k_2,-k_2, box_edge_len, box_edge_len};
        float box_edge_lenf = (float)box_edge_len;
        float _box_area = 1.0f/(box_edge_lenf*box_edge_lenf);
        for (uint16_t iy = 0; iy != filtered->dims[1]; iy++)
        {
            for (uint16_t ix = 0; ix != filtered->dims[0]; ix++)
            {
                BL_4i16_t sumarea = {sumarea0[0]+ix, sumarea0[1]+iy, sumarea0[2], sumarea0[3]};
                *i_filtered._1u8 = (uint8_t)(_box_area * (float)BL_IIu32_partialsum(ii, sumarea, n_padding));
                i_filtered._1u8 += n_ch;
            }
        }
    } while (false);
    return err;
}

static int32_t square_color_distance(const BL_3u8_t c0, const BL_3u8_t c1)
{
    int32_t dist = 0;
    for (int32_t i = 0; i != 3; i++)
    {
        int32_t diff = (int32_t)c0[i] - (int32_t)c1[i];
        dist += diff*diff;
    }
    return dist;
}

uint8_t BL_nearest_color(const BL_3u8_t color_under_test, uint32_t color_count, const BL_3u8_t* colors)
{
    uint32_t i_found = 0;
    int32_t dist0 = square_color_distance(color_under_test, colors[0]);
    for (uint32_t i = 1; i != color_count; i++)
    {
        int32_t dist = square_color_distance(color_under_test, colors[i]);
        if (dist < dist0)
        {
            dist0 = dist;
            i_found = i;
        }
    }
    return (uint8_t)i_found;
}
#include    <stdio.h>
pBL_arrayMD_t BL_discriminate(pcBL_arrayMD_t image, uint32_t color_count, const BL_3u8_t *colors)
{
    pBL_arrayMD_t color_map_table = BL_arrayMD_new(image->dims, BL_1u8_t);
    BL_cptr_t i_image = BL_arrayMD_cbegin(image);
    BL_ptr_t i_color_map_table = BL_arrayMD_begin(color_map_table);
    for (uint16_t iy = 0; iy != image->dims[1]; iy++)
    {
        for (uint16_t ix = 0; ix != image->dims[0]; ix++)
        {
            uint8_t color_index = BL_nearest_color(i_image._1u8, color_count, colors);
            *i_color_map_table._1u8++ = color_index;
            i_image._3u8++;
        }
    }
    return color_map_table;
}

pBL_arrayMD_t BL_discriminate2(pcBL_arrayMD_t index_map, uint8_t ref_index)
{
    pBL_arrayMD_t binary_map = BL_arrayMD_new(index_map->dims, BL_1u8_t);
    BL_cptr_t ptr_index = BL_arrayMD_cbegin(index_map);
    BL_ptr_t ptr_binary = BL_arrayMD_begin(binary_map);
    for (uint16_t iy = 0; iy != index_map->dims[1]; iy++)
    {
        for (uint16_t ix = 0; ix != index_map->dims[0]; ix++)
        {
            *ptr_binary._1u8++ = (*ptr_index._1u8++ == ref_index) ? 1 : 0;
        }
    }
    return binary_map;
}

static uint16_t get_adjacent_segID(const uint16_t* pbegin, uint16_t iy, uint16_t ix, uint16_t stride)
{
    uint16_t foundID = 0;
    const uint16_t* pthis_pixel = pbegin + ix + iy * stride;
    uint16_t stride_m1 = stride - 1;
    do {
        if (iy)
        { // check previous row
            const uint16_t* pprev_row = pthis_pixel - stride;
            if (ix)
            { // check previous column
                if (*(pprev_row - 1))
                {
                    foundID = *(pprev_row - 1);
                    break;
                }
            }
            // check this column
            if (*pprev_row)
            {
                foundID = *pprev_row;
                break;
            }
            if (ix < stride_m1)
            { // check the next column
                if (*(pprev_row + 1))
                {
                    foundID = *pprev_row;
                    break;
                }
            }
        }
        if (ix)
        { // check previous column in the same row
            if (*(pthis_pixel - 1))
            {
                foundID = *(pthis_pixel - 1);
                break;
            }
        }
    } while (false);
    return foundID;
}

pBL_arrayMD_t BL_segmentIDs(pcBL_arrayMD_t binary_map)
{
    uint16_t segID_counter = 1;
    pBL_arrayMD_t segID_map = BL_arrayMD_new(binary_map->dims, BL_1u16_t);
    BL_cptr_t i_binary_map = BL_arrayMD_cbegin(binary_map);
    BL_ptr_t i_segID_map = BL_arrayMD_begin(segID_map);
    const uint16_t* p_segID_map_begin = i_segID_map._1u16;
    for (uint16_t iy = 0; iy != binary_map->dims[1]; iy++)
    {
        for (uint16_t ix = 0; ix != binary_map->dims[0]; ix++)
        {
            if (*i_binary_map._1u8)
            {
                uint16_t foundID = get_adjacent_segID(p_segID_map_begin,iy,ix,segID_map->dims[0]);
                if (foundID != 0)
                {
                    *i_segID_map._1u16 = foundID;
                }
                else
                {
                    *i_segID_map._1u16 = segID_counter++;
                }                
            }
            i_binary_map._1u8++;
            i_segID_map._1u16++;
        }
    }
    return segID_map;
}