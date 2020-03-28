#include "base_cg/BL_oomesh.h"

void BL_oomesh_setdefaulttriangles(pBL_oomesh_t mesh)
{
    uint32_t reduced_width = mesh->wh[0] - 1;
    uint32_t reduced_height = mesh->wh[1] - 1;
    BL_3i32_t* ptri = mesh->tri;
    for (uint32_t row = 0; row < reduced_height; row++)
    {
        int index_topleft = row * mesh->wh[0];
        for (uint32_t col = 0; col < reduced_width; col++, index_topleft++)
        {
            int index_topright = index_topleft + 1;
            int index_bottomleft = index_topleft + mesh->wh[0];
            int index_bottomright = index_bottomleft + 1;
            (*ptri)[0] = index_topleft;
            (*ptri)[1] = index_bottomleft;
            (*ptri)[2] = index_topright;
            ptri++;
            (*ptri)[0] = index_topright;
            (*ptri)[1] = index_bottomleft;
            (*ptri)[2] = index_bottomright;
            ptri++;
        }
    }
}

pBL_oomesh_t BL_oomesh_newcommon(const BL_2u32_t wh, uint32_t n_vert, uint32_t n_tri)
{
    size_t cb_head = BL_ALIGN8(sizeof(BL_oomesh_t));
    size_t cb_vert = BL_ALIGN8(n_vert * sizeof(BL_3r32_t));
    size_t cb_tex = BL_ALIGN8(n_vert * sizeof(BL_2r32_t));
    size_t cb_flags = BL_ALIGN8(n_vert * sizeof(BL_1u8_t));
    size_t cb_tri = BL_ALIGN8(n_tri * sizeof(BL_3i32_t));
    size_t cb_alloc = cb_head + cb_vert + cb_tex + cb_flags + cb_tri;
    pBL_oomesh_t p = (pBL_oomesh_t)calloc(cb_alloc, 1);
    p->wh[0] = wh[0]; p->wh[1] = wh[1];
    p->n_tri = n_tri;
    p->n_vert = n_vert;
    uint8_t* pu8 = (uint8_t*)p;
    pu8 += cb_head;
    p->vert = (BL_3r32_t*)pu8;
    pu8 += cb_vert;
    p->tex = (BL_2r32_t*)pu8;
    pu8 += cb_tex;
    p->flags = (BL_1u8_t*)pu8;
    pu8 += cb_flags;
    p->tri = (BL_3i32_t*)pu8;
    return p;
}

pBL_oomesh_t BL_oomesh_new2(uint32_t n_vert, uint32_t n_tri)
{
    const BL_2u32_t wh = { n_vert, 1 };
    return BL_oomesh_newcommon(wh, n_vert, n_tri);
}

pBL_oomesh_t BL_oomesh_new(const BL_2u32_t wh)
{
    uint32_t n_vert = wh[0] * wh[1];
    uint32_t n_tri = 2 * (wh[0] - 1) * (wh[1] - 1);
    return BL_oomesh_newcommon(wh, n_vert, n_tri);
}

void BL_oomesh_applymask(pBL_oomesh_t mesh, const BL_3u8_t* mask)
{
    uint8_t* pflags = mesh->flags;
    uint32_t count = mesh->wh[0] * mesh->wh[1];
    do {
        if ((*mask)[0] == 0 && (*mask)[1] == 0 && (*mask)[2] == 0)
        {
            *pflags++ &= ~BL_oomesh_attr_enabled; // disable the vertex
        }
        else
        {
            *pflags++ |= BL_oomesh_attr_enabled; // enable the vertex
        }
        mask++;
    } while (--count);
}

void BL_oomesh_settex(pBL_oomesh_t mesh, const BL_2r32_t tex0, const BL_2r32_t tex1)
{
    float dx = (tex1[0] - tex0[0])/(float)(mesh->wh[0]);
    float dy = (tex1[1] - tex0[1])/(float)(mesh->wh[1]);
    BL_2r32_t* ptex = mesh->tex;
    float y = tex0[1];
    for (uint32_t row = 0; row < mesh->wh[1]; row++)
    {
        float x = tex0[0];
        for (uint32_t col = 0; col < mesh->wh[0]; col++)
        {
            (*ptex)[0] = x;
            (*ptex)[1] = y;
            ptex++;
            x += dx;
        }
        y += dy;
    }
}

void BL_oomesh_setvert(pBL_oomesh_t mesh, const BL_3r32_t vert0, const BL_3r32_t vert1)
{
    float z = 0.5f * (vert0[2] + vert1[2]);
    float dx = (vert1[0] - vert0[0])/(float)(mesh->wh[0]);
    float dy = (vert1[1] - vert0[1])/(float)(mesh->wh[1]);
    BL_3r32_t *pvert = mesh->vert;
    uint8_t* pflags = mesh->flags;
    float y = vert0[1];
    for (uint32_t row = 0; row < mesh->wh[1]; row++)
    {
        float x = vert0[0];
        for (uint32_t col = 0; col < mesh->wh[0]; col++)
        {
            (*pvert)[0] = x;
            (*pvert)[1] = y;
            (*pvert)[2] = z;
            pvert++;
            x += dx;
            *pflags++ = BL_oomesh_attr_enabled;
        }
        y += dy;
    }
}

uint32_t count_enabled(const uint8_t *pcflags, const uint8_t* pcflags_end)
{
    uint32_t n = 0;
    for (; pcflags != pcflags_end; pcflags++)
    {
        if (0 != (*pcflags & BL_oomesh_attr_enabled))
        {
            n++;
        }
    }
    return n;
}

uint32_t count_enabled_triangles(const BL_3i32_t* pctri, const BL_3i32_t* pctri_end, const uint8_t* pcflags)
{
    uint32_t n = 0;
    for (; pctri != pctri_end; pctri++)
    {
        uint8_t flags =
            pcflags[(*pctri)[0]] & pcflags[(*pctri)[1]] & pcflags[(*pctri)[2]];
        if (0 != (flags & BL_oomesh_attr_enabled))
        {
            n++;
        }
    }
    return n;
}

void BL_oomesh_extractenabled(pcBL_oomesh_t mesh_in, pBL_oomesh_t* mesh_out)
{
    uint32_t enabled_point_count = 
        count_enabled(mesh_in->flags, mesh_in->flags + mesh_in->n_vert);
    uint32_t enabled_triangle_count = 
        count_enabled_triangles(mesh_in->tri, mesh_in->tri + mesh_in->n_tri, mesh_in->flags);
    if (*mesh_out)
    {
        free((void*)(*mesh_out));
        *mesh_out = NULL;
    }
    *mesh_out = BL_oomesh_new2(enabled_point_count, enabled_triangle_count);

    BL_oomeshconstpointinfo_t info_in = BL_oomesh_cbegin(mesh_in);
    BL_oomeshpointinfo_t info_out = BL_oomesh_begin(*mesh_out);
    const uint8_t *flags_end = (const uint8_t*)((*mesh_out)->flags + (*mesh_out)->n_vert);
    for (; info_out.flags == flags_end; BL_oomeshconstpointinfo_gotonext(&info_in))
    {
        if (0 != (*info_in.flags & BL_oomesh_attr_enabled))
        {
            BL_oomeshpointinfo_copy(&info_out, &info_in);
            BL_oomeshpointinfo_gotonext(&info_out);
        }
    }
}

#pragma region accessor_related_methods
void BL_oomeshpointinfo_gotonext(pBL_oomeshpointinfo_t info)
{
    info->flags++;
    info->tex++;
    info->vert++;
}

void BL_oomeshconstpointinfo_gotonext(pBL_oomeshconstpointinfo_t info)
{
    info->flags++;
    info->tex++;
    info->vert++;
}

BL_oomeshpointinfo_t BL_oomesh_begin(pBL_oomesh_t mesh)
{
    BL_oomeshpointinfo_t info =
    {
        mesh->vert, mesh->tex, mesh->flags
    };
    return info;
}

BL_oomeshconstpointinfo_t BL_oomesh_cbegin(pcBL_oomesh_t mesh)
{
    BL_oomeshconstpointinfo_t info =
    {
        mesh->vert, mesh->tex, mesh->flags
    };
    return info;
}

void BL_oomeshpointinfo_copy(pcBL_oomeshpointinfo_t dst, pcBL_oomeshconstpointinfo_t src)
{
    *(dst->flags) = *(src->flags);
    (*(dst->tex))[0] = (*(src->tex))[0];
    (*(dst->tex))[1] = (*(src->tex))[1];
    (*(dst->vert))[0] = (*(src->vert))[0];
    (*(dst->vert))[1] = (*(src->vert))[1];
    (*(dst->vert))[2] = (*(src->vert))[2];
}

#pragma endregion
