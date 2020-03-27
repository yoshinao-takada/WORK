#include "base_cg/BL_oomesh.h"

pBL_oomesh_t BL_oomesh_new(const BL_2u32_t wh_)
{
    size_t vertex_count = wh_[0]*wh_[1];
    size_t cb_head = BL_ALIGN8(sizeof(BL_oomesh_t));
    size_t cb_vert = BL_ALIGN8(vertex_count * sizeof(BL_3r32_t));
    size_t cb_tex = BL_ALIGN8(vertex_count * sizeof(BL_2r32_t));
    size_t cb_flags = BL_ALIGN8(vertex_count * sizeof(BL_1u8_t));
    size_t cb_alloc = cb_head + cb_vert + cb_tex + cb_flags;
    pBL_oomesh_t p = (pBL_oomesh_t)calloc(cb_alloc, 1);
    p->wh[0] = wh_[0];
    p->wh[1] = wh_[1];
    uint8_t* pu8 = (uint8_t*)p;
    pu8 += cb_head;
    p->vert = (BL_3r32_t*)pu8;
    pu8 += cb_vert;
    p->tex = (BL_2r32_t*)pu8;
    pu8 += cb_tex;
    p->flags = (BL_1u8_t*)pu8;
    return p;
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
        }
        y += dy;
    }
}