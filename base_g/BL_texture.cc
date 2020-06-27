#include    "base_g/BL_texture.h"
#include    <opencv2/core.hpp>
#include    <opencv2/imgproc.hpp>

pBL_texture_t BL_texture_alloc(const BL_2u32_t wh, bool with_alpha)
{
    BL_arrayMD_dims dims = { (uint16_t)wh[0], (uint16_t)wh[1], 1,1,1,1,1,1 };
    return with_alpha ? BL_arrayMD_new(dims, BL_4u8_t) : BL_arrayMD_new(dims, BL_3u8_t);
}

pBL_texture_t BL_texture_checker(pcBL_checkerconf_t conf)
{
    pBL_texture_t p = BL_texture_alloc(conf->wh, conf->with_alpha);
    BL_ptr_t tex_element0 = BL_arrayMD_begin(p);
    for (uint32_t iy = 0, block_count_y = 0;
        iy != conf->wh[1];
        iy++, block_count_y += ((iy % conf->block_size[1]) == 0) ? 1 : 0)
    {
        for (uint32_t ix = 0, block_count_x = 0;
            ix != conf->wh[0];
            ix++, block_count_x += ((ix % conf->block_size[0]) == 0) ? 1 : 0)
        {
            uint32_t color_index = (block_count_x + block_count_y) & 1;
            if (conf->with_alpha)
            { // with alpha channel
                BL_copy4(tex_element0._1u8, conf->colors[color_index]);
                tex_element0._4u8++;
            }
            else
            { // without alpha channel
                BL_copy3(tex_element0._1u8, conf->colors[color_index]);
                tex_element0._3u8++;
            }            
        }
    }
    return p;
}


pBL_texture_t BL_texture_checker2(pcBL_checkerconf2_t conf)
{
    pBL_texture_t p = BL_texture_alloc(conf->base.wh, conf->base.with_alpha);
    BL_ptr_t tex_element0 = BL_arrayMD_begin(p);
    for (uint32_t iy = 0, block_count_y = 0;
        iy != conf->base.wh[1];
        iy++, block_count_y += (iy % conf->base.block_size[1] == 0) ? 1 : 0)
    {
        for (uint32_t ix = 0, block_count_x = 0;
            ix != conf->base.wh[0];
            ix++, block_count_x += (ix % conf->base.block_size[0] == 0) ? 1 : 0)
        {
            uint32_t color_index = (block_count_x + block_count_y) & 1;
            if ((ix < conf->border[0]) && (iy < conf->border[1]))
            {
                if (conf->base.with_alpha)
                { // with alpha channel
                    BL_copy4(tex_element0._1u8, conf->base.colors[color_index]);
                    tex_element0._4u8++;
                }
                else
                { // without alpha channel
                    BL_copy3(tex_element0._1u8, conf->base.colors[color_index]);
                    tex_element0._3u8++;
                }            
            }
            else
            {
                if (conf->base.with_alpha)
                { // with alpha channel
                    BL_copy4(tex_element0._1u8, conf->colors[color_index]);
                    tex_element0._4u8++;
                }
                else
                { // without alpha channel
                    BL_copy3(tex_element0._1u8, conf->colors[color_index]);
                    tex_element0._3u8++;
                }            
            }            
        }
    }
    return p;
}

int BL_texture_remap(pBL_texture_t in, pBL_texture_t out, pBL_arrayMD_t map)
{
    int err = ESUCCESS;
    do {
        BL_ptr_t ptr_in = BL_arrayMD_begin(in);
        const cv::Mat3b in_((int)(in->dims[1]), (int)(in->dims[0]), (cv::Vec3b*)ptr_in._1u8);
        BL_ptr_t ptr_out = BL_arrayMD_begin(out);
        cv::Mat3b out_((int)(out->dims[1]), (int)(out->dims[0]), (cv::Vec3b*)ptr_out._1u8);
        BL_ptr_t ptr_map = BL_arrayMD_begin(map);
        const cv::Mat2f map_((int)(map->dims[1]), (int)(map->dims[0]), (cv::Vec2f*)ptr_map._1u8);
        cv::Mat map2;
        cv::remap(in_, out_, map_, map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT);
    } while (false);
    return err;
}

static uint32_t sq_distance(const BL_2u32_t p0, const BL_2u32_t p1)
{
    BL_2i32_t diff = { (int32_t)p0[0] - (int32_t)p1[0], (int32_t)p0[1] - (int32_t)p1[1] };
    return diff[0]*diff[0] + diff[1]*diff[1];
}

static bool near_to_grid_node(const BL_2u32_t pgrid[4], const BL_2u32_t p, uint32_t sq_r_max)
{
    bool is_near = false;
    for (uint32_t i = 0; i != 4; i++)
    {
        if (sq_distance(pgrid[i], p) < sq_r_max)
        {
            is_near = true;
            break;
        }
    }
    return is_near;
}

pBL_texture_t BL_texture_circulardots(pcBL_checkerconf_t conf)
{
    uint32_t ref_pitch = __min(conf->block_size[0], conf->block_size[1]);
    uint32_t dot_radius = ref_pitch / 8;
    uint32_t sq_dot_radius = dot_radius * dot_radius;
    pBL_texture_t p = BL_texture_alloc(conf->wh, conf->with_alpha);
    BL_ptr_t tex_element = BL_arrayMD_begin(p);
    for (uint32_t iy = 0; iy != p->dims[1]; iy++)
    {
        uint32_t block_tick = iy / conf->block_size[1];
        uint32_t floor_y = block_tick * conf->block_size[1];
        uint32_t ceil_y = floor_y + conf->block_size[1];
        for (uint32_t ix = 0; ix != p->dims[0]; ix++)
        {
            uint32_t block_tick = ix / conf->block_size[0];
            uint32_t floor_x = block_tick * conf->block_size[0];
            uint32_t ceil_x = floor_x + conf->block_size[0];
            BL_2u32_t pgrid[4] = {
                { floor_x, floor_y }, { ceil_x, floor_y }, { floor_x, ceil_y }, { ceil_x, ceil_y }
            };
            BL_2u32_t point = { ix, iy };
            uint32_t color_index = near_to_grid_node(pgrid, point, sq_dot_radius) ? 0 : 1;
            if (conf->with_alpha)
            {
                BL_copy4(tex_element._1u8, conf->colors[color_index]);
                tex_element._4u8++;
            }
            else
            {
                BL_copy3(tex_element._1u8, conf->colors[color_index]);
                tex_element._3u8++;
            }
        }
    }
    return p;
}

static bool near_to_node(const BL_2u32_t point_ref, const BL_2u32_t point, uint32_t sq_r_max)
{
    uint32_t sq_dist = sq_distance(point_ref, point);
    return sq_dist < sq_r_max;
}

pBL_texture_t BL_texture_circulardots2(pcBL_checkerconf2_t conf)
{
    uint32_t ref_pitch = __min(conf->base.block_size[0], conf->base.block_size[1]);
    uint32_t dot_radius = ref_pitch / 4;
    uint32_t sq_dot_radius = dot_radius * dot_radius;
    pBL_texture_t p = BL_texture_alloc(conf->base.wh, conf->base.with_alpha);
    BL_ptr_t tex_element = BL_arrayMD_begin(p);
    BL_2u32_t ref_point = { (uint32_t)conf->border[0], (uint32_t)conf->border[1] };
    for (uint32_t iy = 0; iy != p->dims[1]; iy++)
    {
        uint32_t block_tick = iy / conf->base.block_size[1];
        uint32_t floor_y = block_tick * conf->base.block_size[1];
        uint32_t ceil_y = (block_tick+1) * conf->base.block_size[1];
        for (uint32_t ix = 0; ix != p->dims[0]; ix++)
        {
            uint32_t block_tick = ix / conf->base.block_size[0];
            uint32_t floor_x = block_tick * conf->base.block_size[0];
            uint32_t ceil_x = (block_tick+1) * conf->base.block_size[0];
            BL_2u32_t pgrid[4] = {
                { floor_x, floor_y }, { ceil_x, floor_y }, { floor_x, ceil_y }, { ceil_x, ceil_y }
            };
            BL_2u32_t point = { ix, iy };
            if (near_to_node(ref_point, point, sq_dot_radius))
            {
                if (conf->base.with_alpha)
                {
                    BL_copy4(tex_element._1u8, conf->colors[0]);
                    tex_element._4u8++;
                }
                else
                {
                    BL_copy3(tex_element._1u8, conf->colors[0]);
                    tex_element._3u8++;
                }
                
            }
            else if (near_to_grid_node(pgrid, point, sq_dot_radius))
            {
                uint32_t color_index = 0;
                if (conf->base.with_alpha)
                {
                    BL_copy4(tex_element._1u8, conf->base.colors[color_index]);
                    tex_element._4u8++;
                }
                else
                {
                    BL_copy3(tex_element._1u8, conf->base.colors[color_index]);
                    tex_element._3u8++;
                }
            }
            else
            {
                uint32_t color_index = 1;
                if (conf->base.with_alpha)
                {
                    BL_copy4(tex_element._1u8, conf->base.colors[color_index]);
                    tex_element._4u8++;
                }
                else
                {
                    BL_copy3(tex_element._1u8, conf->base.colors[color_index]);
                    tex_element._3u8++;
                }
            }
        }
    }
    return p;
}
