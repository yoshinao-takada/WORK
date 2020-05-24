#include    "base_g/BL_distortion.h"
#include    "base_g/BL_matexport.h"
#define STD_LAST_ROW_3X3    0.0f,0.0f,1.0f

pBL_map2D_t alloc_map2D(const BL_2u32_t wh)
{
    const uint16_t dims[] = { (uint16_t)wh[0], (uint16_t)wh[1], 1,1,1,1,1,1 };
    return (pBL_map2D_t)BL_arrayMD_new(dims, BL_2r32_t);
}

static void fillmap(pBL_map2D_t map, pcBL_matfnr32RM_t fntab, const BL_1r32_t* transmat)
{
    BL_ptr_t ptr = BL_arrayMD_begin(map);
    BL_3r32_t in = { 0.0f, 0.0f, 1.0f }, work;
    for (uint16_t iy = 0; iy != map->dims[1]; iy++)
    {
        for (uint16_t ix = 0; ix != map->dims[0]; ix++)
        {
            in[0] = (BL_1r32_t)ix;
            in[1] = (BL_1r32_t)iy;
            const BL_1r32_t* out = fntab->mul(transmat, in, work, 1, 3, 3);
            BL_copy2(ptr._1r32, out);
            ptr._2r32++;
        }
    }
}

pBL_map2D_t BL_distortion_createmap(pcBL_distconf_t conf)
{
    pcBL_matfnr32RM_t matfnr32RM = BL_matfnr32RM();
    BL_1r32_t sub_center_offset[9] = {
        1.0f, 0.0f, -conf->rot_center[0],
        0.0f, 1.0f, -conf->rot_center[1],
        STD_LAST_ROW_3X3
    };
    BL_1r32_t add_center_offset[9] = {
        1.0f, 0.0f, conf->rot_center[0],
        0.0f, 1.0f, conf->rot_center[1],
        STD_LAST_ROW_3X3
    };
    float rot_angle_rad = conf->rot_angle_deg * M_PI / 180.0f;
    BL_1r32_t rotation[9] = {
        cosf(rot_angle_rad), -sinf(rot_angle_rad), 0.0f,
        sinf(rot_angle_rad), cosf(rot_angle_rad), 0.0f,
        STD_LAST_ROW_3X3
    };
    BL_1r32_t shift[9] = {
        1.0f, 0.0f, conf->shift[0],
        0.0f, 1.0f, conf->shift[1],
        STD_LAST_ROW_3X3
    };
    BL_1r32_t works[3][9];
    const BL_1r32_t* transform_1st_stage = 
        matfnr32RM->mul(
            shift, matfnr32RM->mul(
                add_center_offset, matfnr32RM->mul(
                    rotation, sub_center_offset, &works[0][0], 3, 3, 3),
                &works[1][0], 3,3,3),
            &works[2][0], 3,3,3);
    pBL_map2D_t map = alloc_map2D(conf->wh);
    fillmap(map, matfnr32RM, transform_1st_stage);
    return map;
}

pBL_map2D_t BL_rotshift_createmap(pcBL_rotshift_t conf)
{
    pcBL_matfnr32RM_t matfnr32RM = BL_matfnr32RM();

    // cancel center offset of the destination image
    BL_1r32_t sub_dst_center_offset[9] = {
        1.0f, 0.0f, -0.5f * conf->wh_dst[0],
        0.0f, 1.0f, -0.5f * conf->wh_dst[1],
        STD_LAST_ROW_3X3
    };

    // rotate counter direction of the source rotation
    float rot_angle_rad = conf->rot_angle_deg * ((float)M_PI) / 180.0f;
    BL_1r32_t rot[9] = {
        cosf(rot_angle_rad), -sinf(rot_angle_rad), 0.0f,
        sinf(rot_angle_rad), cosf(rot_angle_rad), 0.0f,
        STD_LAST_ROW_3X3
    };

    // add ((center offset of the source image) - (source image shifting))
    BL_1r32_t add_src_center_minus_shift[9] = {
        1.0f, 0.0f, (0.5f * conf->wh_src[0] - conf->shift[0]),
        0.0f, 1.0f, (0.5f * conf->wh_src[1] - conf->shift[1]),
        STD_LAST_ROW_3X3
    };

    // combine the three matrices
    BL_1r32_t works[2][9];
    const BL_1r32_t* transmat = matfnr32RM->mul(
        matfnr32RM->mul(
            add_src_center_minus_shift,
            rot,
            &works[1][0],
            3,3,3
        ),
        sub_dst_center_offset,
        &works[0][0],
        3,3,3
    );

    // filling the mapping table
    pBL_map2D_t map = alloc_map2D(conf->wh_dst);
    fillmap(map, matfnr32RM, transmat);
    return map;
}

#define SQ(x)   ((x)*(x))

pBL_map2D_t BL_rotshiftdist_createmap(pcBL_rotshiftdist_t conf)
{
    pcBL_matfnr32RM_t matfnr32RM = BL_matfnr32RM();

    // distortion
    pBL_map2D_t map = alloc_map2D(conf->wh_dst);
    BL_ptr_t ptr = BL_arrayMD_begin(map);
    for (uint16_t iy = 0; iy != map->dims[1]; iy++)
    {
        for (uint16_t ix = 0; ix != map->dims[0]; ix++)
        {
            BL_2r32_t relative_xy = { ix - conf->c_dist[0], iy - conf->c_dist[1] };
            // BL_2r32_t relative_xy_dist = {
            //     (1.0f + fabsf(conf->k[0] * relative_xy[0]) + fabsf(conf->k[0] * relative_xy[1])) * relative_xy[0],
            //     (1.0f + fabsf(conf->k[1] * relative_xy[1]) + fabsf(conf->k[1] * relative_xy[0])) * relative_xy[1]
            // };
            BL_2r32_t relative_xy_dist = {
                (1.0f + SQ(conf->k[0] * relative_xy[0]) + SQ(conf->k[0] * relative_xy[1])) * relative_xy[0],
                (1.0f + SQ(conf->k[1] * relative_xy[0]) + SQ(conf->k[1] * relative_xy[1])) * relative_xy[1]
            };
            BL_2r32_t src_coord_dist = {
                relative_xy_dist[0] + 0.5f * conf->wh_src[0],
                relative_xy_dist[1] + 0.5f * conf->wh_src[1]
            };
            BL_copy2(ptr._1r32, src_coord_dist);
            ptr._2r32++;
        }
    }
    return map;
}
