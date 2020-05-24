#include    <base_cg/BL_die.h>
#include    <opencv2/core.hpp>

namespace BL_die_tex_
{
    int32_t L2dist2D(const uint32_t* p0, const uint32_t* p1)
    {
        BL_2i32_t diff = { (int32_t)(p0[0] - p1[0]), (int32_t)(p0[1] - p1[1]) };
        return diff[0]*diff[0] + diff[1]*diff[1];
    }

    float Euclid_dist2D(const uint32_t* p0, const uint32_t* p1)
    {
        return sqrtf(L2dist2D(p0,p1));
    }

    static const BL_3u8_t red = {0,0,255}, blue = {255,0,0}, 
        gray = {128,128,128}, black={0,0,0}, right_gray = {200,200,200};
    static const BL_3u8_t background = {gray[0],gray[1],gray[2]};
    static const BL_2u32_t face_positions[] = {{ 1, 1 }, { 0, 1 }, { 0, 0 }, { 0, 2 }, { 2, 1 }, { 3, 1 }};
    static const BL_2r32_t dot_centers[6][6] = {
        {{0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f}},
        {{-0.7f,0.7f},{0.7f,-0.7f},{0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f}},
        {{-0.7f,0.7f},{0.0f,0.0f},{0.7f,-0.7f},{0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f}},
        {{-0.7f,0.7f},{0.7f,-0.7f},{0.7f,0.7f},{-0.7f,-0.7f},{0.0f,0.0f},{0.0f,0.0f}},
        {{-0.7f,0.7f},{0.7f,-0.7f},{0.7f,0.7f},{-0.7f,-0.7f},{0.0f,0.0f},{0.0f,0.0f}},
        {{-0.7f,0.7f},{0.7f,-0.7f},{0.7f,0.7f},{-0.7f,-0.7f},{-0.7f,0.0f},{0.7f,0.0f}},
    };
    static const float dotR[] = { 0.3f, 0.15f, 0.15f, 0.15f, 0.15f, 0.15f};

    float DistL2F(const BL_2r32_t p0, const BL_2r32_t p1)
    {
        BL_2r32_t diff = { p0[0]-p1[0], p0[1]-p1[1] };
        return diff[0]*diff[0] + diff[1]*diff[1];
    }

    bool IsInCircles(const BL_2r32_t xy, const BL_2r32_t* centers, float R, uint32_t dot_count)
    {
        bool b = false;
        float thR2 = R*R;
        for (uint32_t i = 0; i != dot_count && !b; i++)
        {
            b |= (DistL2F(xy, centers[i]) < thR2);
        }
        return b;
    }

    void Fill_face(BL_3u8_t* face_origine, uint32_t stride, uint32_t edge_length, const BL_2r32_t *centers, uint32_t dot_count, float dotR)
    {
        const BL_3u8_t &color_dot = dot_count == 1 ? red : blue;
        const BL_3u8_t &color_face = right_gray;
        BL_2r32_t top_left = { -1.0f, 1.0f }, bottom_right = { 1.0f, -1.0f };
        BL_2r32_t dxdy = { bottom_right[0]-top_left[0], bottom_right[1]-top_left[1] };
        BL_3u8_t *row_begin = face_origine;
        for (uint32_t iy = 0; iy < edge_length; iy++)
        {
            BL_3u8_t *pixel = row_begin;
            BL_2r32_t xy = { top_left[0], top_left[1]+iy*dxdy[1] };
            for (uint32_t ix = 0; ix < edge_length; ix++)
            {
                const BL_3u8_t& color = IsInCircles(xy,centers,dotR,dot_count) ? color_dot : color_face;
                BL_copy3(*pixel,color);
                pixel++;
                xy[0] += dxdy[0];
            }
            row_begin += stride;
        }
    }
}

int BL_die_tex(pcBL_dieconf_t conf, pBL_data2D_t* tex)
{
    using namespace BL_die_tex_;
    int err = ESUCCESS;
    BL_2u32_t face_origines[ARRAYSIZE(face_positions)];
    for (uint32_t i = 0; i < ARRAYSIZE(face_positions); i++)
    {
        face_origines[i][0] = face_positions[i][0] * conf->cube_edge_length;
        face_origines[i][1] = face_positions[i][1] * conf->cube_edge_length;
    }

    do {
        // init tex 2D data array object and bind to a cv::Mat3b container
        BL_2u32_t tex_wh = { conf->cube_edge_length * 4, conf->cube_edge_length * 3 };
        if (*tex)
        {
            free((void*)(*tex));
        }
        *tex = BL_data2D_alloc(tex_wh, sizeof(BL_3u8_t));
        BL_data2D_fill(*tex, (const void*)&gray);
        
        // process six surfaces 1-by-1.
        for (uint32_t i = 0; i < ARRAYSIZE(dot_centers); i++)
        {
            cv::Rect2i roi = {
                cv::Point2i(face_origines[i][0], face_origines[i][1]),
                cv::Size2i(conf->cube_edge_length, conf->cube_edge_length)
            };
            BL_3u8_t* pface_origine = (BL_3u8_t*)((*tex)->ptr) + face_origines[i][0] + tex_wh[0] * face_origines[i][1];
            Fill_face(pface_origine, tex_wh[0], conf->cube_edge_length, dot_centers[i], (i + 1), dotR[i]);
        }
    } while (false);
    if (err)
    {
        if (*tex) free((void*)(*tex));
        *tex = NULL;
    }
    return err;
}

namespace BL_die_vert_
{

}

int BL_die_mesh(pcBL_dieconf_t conf, pBL_oomesh_t* mesh)
{

}