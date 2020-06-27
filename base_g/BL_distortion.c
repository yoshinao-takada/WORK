#include    "base_g/BL_distortion.h"
#include    "base_g/BL_matexport.h"
#include    "base_g/BL_distortion.h"
#include    <math.h>
#include    <stdbool.h>

pBL_arrayMD_t BL_dist_polymodel_create_distmap(pcBL_dist_polymodel_t model)
{
    BL_arrayMD_dims dims = { (uint16_t)(model->wh[0]), (uint16_t)(model->wh[1]), 1,1,1,1,1,1 };
    pBL_arrayMD_t distmap = BL_arrayMD_new(dims, BL_2r32_t);
    BL_ptr_t i_distmap = BL_arrayMD_begin(distmap);
    for (uint16_t iy = 0; iy != dims[1]; iy++)
    {
        for (uint16_t ix = 0; ix != dims[0]; ix++)
        {
            float x = (float)ix - model->dist_center[0];
            float y = (float)iy - model->dist_center[1];
            float angle = atan2f(y, x);
            float kex = model->k + model->kc*cosf(angle) + model->ks*sinf(angle);
            float xprime = x * (1.0f + kex);
            float yprime = y * (1.0f + kex);
            i_distmap._1r32[0] = xprime + (float)model->src_center[0];
            i_distmap._1r32[1] = yprime + (float)model->src_center[1];
            i_distmap._2r32++;
        }
    }
    return distmap;
}

static void GetSrcROI(pcBL_arrayMD_t distmap, BL_4u16_t roi)
{
    float xmin = 10000.0f;
    float xmax = -xmin;
    float ymin = xmin;
    float ymax = -ymin;
    BL_cptr_t i_distmap = BL_arrayMD_cbegin(distmap);
    for (uint16_t iy = 0; iy != distmap->dims[1]; iy++)
    {
        for (uint16_t ix = 0; ix != distmap->dims[0]; ix++)
        {
            float x = i_distmap._1r32[0];
            float y = i_distmap._1r32[1];
            xmin = __min(xmin, x);
            xmax = __max(xmax, x);
            ymin = __min(ymin, y);
            ymax = __max(ymax, y);
            i_distmap._2r32++;
        }
    }
    xmin = __min(0.0f, floorf(xmin));
    ymin = __min(0.0f, floorf(ymin));
    roi[0] = (uint16_t)xmin;
    roi[1] = (uint16_t)ymin;
    roi[2] = (uint16_t)(xmax - xmin);
    roi[3] = (uint16_t)(ymax - ymin);
}

static float Length2f(const BL_2r32_t v)
{
    return sqrtf(v[0]*v[0] + v[1]*v[1]);
}

static float angle(const BL_2r32_t v0, const BL_2r32_t v1)
{
    float dot = v0[0] * v1[0] + v0[1] * v1[1];
    float cos_angle_v0_v1 = dot /Length2f(v0)/Length2f(v1);
    return  (((v0[0] * v1[1] - v0[1] * v1[0]) > 0.0f) ? 1.0f : -1.0f) * acosf(cos_angle_v0_v1);
}

float  fan_angle(const BL_2r32_t point, uint32_t n_corner, const BL_2r32_t* corners)
{
    float a = 0.0;
    for (uint32_t i = 0; i != n_corner; i++)
    {
        const float *i_corner0 = (const float*)(corners + i);
        const float *i_corner1 = (const float*)(corners + ((i + 1) % n_corner));
        BL_2r32_t corner0, corner1;
        BL_copy2(corner0, i_corner0);
        BL_copy2(corner1, i_corner1);
        corner0[0] -= point[0];
        corner0[1] -= point[1];
        corner1[0] -= point[0];
        corner1[1] -= point[1];
        a += angle(corner0, corner1);
    }
    return a;
}

bool is_in_polygon(const BL_2r32_t point, uint32_t n_corner, const BL_2r32_t* corners)
{
    return fabsf(fan_angle(point, n_corner, corners)) > (float)M_PI;
}

int    circuscribe_rect(const BL_2r32_t* points, BL_4u32_t rect)
{
    int err = ESUCCESS;
    float xmin = 10000.0f;
    float xmax = -xmin;
    float ymin = xmin;
    float ymax = xmax;
    for (uint32_t i = 0; i < 4; i++)
    {
        float x = ((const float*)points)[0];
        float y = ((const float*)points)[1];
        points++;
        xmin = __min(xmin, x);
        xmax = __max(xmax, x);
        ymin = __min(ymin, y);
        ymax = __max(ymax, y);
    }
    if (xmin < 0.0f || ymin < 0.0f)
    {
        err = ERANGE;
    }
    xmin = __max(0.0f, xmin);
    ymin = __max(0.0f, ymin);
    rect[0] = (uint32_t)floorf(xmin);
    rect[1] = (uint32_t)floorf(ymin);
    rect[2] = (uint32_t)ceilf(xmax) - rect[0];
    rect[3] = (uint32_t)ceilf(ymax) - rect[1];
    return err;
}

pBL_arrayMD_t BL_disp_polymodel_create_undistmap(pcBL_dist_polymodel_t model)
{
    pBL_arrayMD_t distmap = BL_dist_polymodel_create_distmap(model);
    BL_4u16_t roi; GetSrcROI(distmap, roi);
    BL_arrayMD_dims undist_dims = { roi[2], roi[3], 1,1,1,1,1,1 };
    pBL_arrayMD_t undistmap_refpoints = BL_arrayMD_new(undist_dims, BL_2u16_t);
    BL_cptr_t i_distmap = BL_arrayMD_cbegin(distmap);
    uint16_t ny = distmap->dims[1];
    uint16_t nx = distmap->dims[0];
    uint16_t ny_1 = ny - 1;
    uint16_t nx_1 = nx - 1;
    for (uint16_t iy = 0; iy != ny_1; iy++)
    {
        for (uint16_t ix = 0; ix != nx_1; ix++)
        {
            // four corners of the cell in the distorted plane
            const BL_2u16_t indices_in_dist_plane[] = { {ix,iy}, {ix, iy+1}, {ix+1, iy+1}, {ix+1, iy} };
            // pointer to a cell of distortion shift vectors of the top left corner of the cell
            const BL_2r32_t* cell_top_left = i_distmap._2r32 + ix + iy * nx;
            // pointers to four corners of the current cell in the distorted plane
            const BL_1r32_t* cells_in_dist_plane[] = {
                (BL_1r32_t*)cell_top_left, (BL_1r32_t*)(cell_top_left + nx),
                (BL_1r32_t*)(cell_top_left + nx + 1), (BL_1r32_t*)(cell_top_left + 1)
            };
            // coordinates of the four corners in the source plane
            const BL_2r32_t src_point_coordinates[] = {
                { cells_in_dist_plane[0][0], cells_in_dist_plane[0][1] },
                { cells_in_dist_plane[1][0], cells_in_dist_plane[1][1] },
                { cells_in_dist_plane[2][0], cells_in_dist_plane[2][1] },
                { cells_in_dist_plane[3][0], cells_in_dist_plane[3][1] }
            };
            BL_4u32_t src_points;
            circuscribe_rect(src_point_coordinates, src_points);
        }
    }
}