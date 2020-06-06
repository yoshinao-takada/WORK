#include    "base_g/BL_discriminate.h"

static void fill_padding_top(pBL_arrayMD_t ii, const BL_2u32_t src_img_wh, uint32_t n_padding);
static void fill_padding_bottom(pBL_arrayMD_t ii, const BL_2u32_t src_img_wh, uint32_t n_padding);
static void fill_padding_left(pBL_arrayMD_t ii, const BL_2u32_t src_img_wh, uint32_t n_padding);
static void fill_padding_right(pBL_arrayMD_t ii, const BL_2u32_t src_img_wh, uint32_t n_padding);
static void fill_padding_topleft(pBL_arrayMD_t ii, const BL_2u32_t src_img_wh, uint32_t n_padding);
static void fill_padding_topright(pBL_arrayMD_t ii, const BL_2u32_t src_img_wh, uint32_t n_padding);
static void fill_padding_bottomleft(pBL_arrayMD_t ii, const BL_2u32_t src_img_wh, uint32_t n_padding);
static void fill_padding_bottomright(pBL_arrayMD_t ii, const BL_2u32_t src_img_wh, uint32_t n_padding);
// #define II_PROCESSING_VIEW_CSV      DEBUG_DATA_DIR  "II_PROCESSING.csv"
// #include    "base_l/BL_futils.h"
// #include    "base_l/BL_debutil.h"
// #define     COUNTERS_CSV                DEBUG_DATA_DIR  "counters.csv"

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
    // fill padding area
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


static void fill_padding_top(pBL_arrayMD_t ii, const BL_2u32_t src_img_wh, uint32_t n_padding)
{

}
static void fill_padding_bottom(pBL_arrayMD_t ii, const BL_2u32_t src_img_wh, uint32_t n_padding)
{

}
static void fill_padding_left(pBL_arrayMD_t ii, const BL_2u32_t src_img_wh, uint32_t n_padding)
{

}
static void fill_padding_right(pBL_arrayMD_t ii, const BL_2u32_t src_img_wh, uint32_t n_padding)
{

}

static void fill_padding_topleft(pBL_arrayMD_t ii, const BL_2u32_t src_img_wh, uint32_t n_padding)
{

}
static void fill_padding_topright(pBL_arrayMD_t ii, const BL_2u32_t src_img_wh, uint32_t n_padding)
{

}
static void fill_padding_bottomleft(pBL_arrayMD_t ii, const BL_2u32_t src_img_wh, uint32_t n_padding)
{

}
static void fill_padding_bottomright(pBL_arrayMD_t ii, const BL_2u32_t src_img_wh, uint32_t n_padding)
{

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

void setID_recursively(
    uint16_t segID,
    int16_t ix, int16_t iy, int16_t width, int16_t height,
    const uint8_t* p_binary_map_begin,
    uint16_t* p_segID_map_begin)
{
    static const BL_2i16_t offset[] = {
        {-1,-1}, {0, -1}, {1, -1},
        {-1, 0}, {1, 0},
        {-1, 1}, {0, 1}, {1, 1}
    };
    // printf("%d,%d,%d,%d\n", ix, iy, width, height);
    *(p_segID_map_begin + ix + iy * width) = segID;
    for (uint16_t i = 0; i != ARRAYSIZE(offset); i++)
    {
        int16_t new_ix = ix + offset[i][0];
        int16_t new_iy = iy + offset[i][1];
        if (new_ix < 0 || new_iy < 0 || new_ix >= width || new_iy >= height) continue; // out of range
        if (*(p_binary_map_begin + new_ix + new_iy * width) &&
            (*(p_segID_map_begin + new_ix + new_iy * width) == 0))
        {
            setID_recursively(segID, new_ix, new_iy, width, height, p_binary_map_begin, p_segID_map_begin);
        }
    }
}

pBL_arrayMD_t BL_segmentIDs(pcBL_arrayMD_t binary_map)
{
    uint16_t segID_counter = 1;
    pBL_arrayMD_t segID_map = BL_arrayMD_new(binary_map->dims, BL_1u16_t);
    BL_cptr_t i_binary_map = BL_arrayMD_cbegin(binary_map);
    BL_ptr_t i_segID_map = BL_arrayMD_begin(segID_map);
    for (uint16_t iy = 0; iy != binary_map->dims[1]; iy++)
    {
        for (uint16_t ix = 0; ix != binary_map->dims[0]; ix++)
        {
            if (*(i_binary_map._1u8 + ix + iy * binary_map->dims[0]) &&
                (*(i_segID_map._1u16 + ix + iy * binary_map->dims[0]) == 0))
            {
                setID_recursively(
                    segID_counter++, 
                    (int16_t)ix, (int16_t)iy,
                    (int16_t)binary_map->dims[0], (int16_t)binary_map->dims[1],
                    i_binary_map._1u8, i_segID_map._1u16);
            }
        }
    }
    return segID_map;
}

static uint16_t get_max(pcBL_arrayMD_t segID_map)
{
    uint16_t u = 0;
    BL_cptr_t i_segID_map = BL_arrayMD_cbegin(segID_map);
    uint32_t element_count = segID_map->data.unit_count;
    do {
        u = __max(u, *i_segID_map._1u16);
        i_segID_map._1u16++;
    } while (--element_count);
    return u;
}

pBL_array_t BL_segment_centers(pcBL_arrayMD_t segID_map)
{
    uint16_t segID_max = get_max(segID_map);
    pBL_array_t accumulators = BL_array_new(segID_max, BL_2r64_t);
    pBL_array_t counters = BL_array_new(segID_max, BL_1u32_t);
    {
        BL_cptr_t i_segID_map = BL_arrayMD_cbegin(segID_map);
        BL_ptr_t i_accumulators = BL_array_begin(accumulators);
        BL_ptr_t i_counters = BL_array_begin(counters);
        for (uint16_t iy = 0; iy < segID_map->dims[1]; iy++)
        {
            for (uint16_t ix = 0; ix < segID_map->dims[0]; ix++)
            {
                uint16_t segID = *i_segID_map._1u16++;
                if (segID)
                {
                    segID--;
                    i_counters._1u32[segID]++;
                    i_accumulators._2r64[segID][0] += (double)ix;
                    i_accumulators._2r64[segID][1] += (double)iy;
                }
            }
        }
    }
    pBL_array_t centers = BL_array_new(segID_max, BL_2r32_t);
    {
        BL_ptr_t i_centers = BL_array_begin(centers);
        BL_cptr_t i_accumulators = BL_array_cbegin(accumulators);
        BL_cptr_t i_counters = BL_array_cbegin(counters);
        for (uint32_t i = 0; i != centers->unit_count; i++)
        {
            double r_seg_area = 1.0 / (double)(*i_counters._1u32++); // reciprocal of segment area
            (*i_centers._2r32)[0] = (*i_accumulators._2r64)[0] * r_seg_area;
            (*i_centers._2r32++)[1] = (*i_accumulators._2r64++)[1] * r_seg_area;
        }
    }
    free((void*)accumulators);
    free((void*)counters);
    return centers;
}
