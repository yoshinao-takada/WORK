#include    "base_g/BL_mesh.h"

static const uint32_t g_vertidxsizes[] = BL_VERTIDXSIZES;

pBL_mesh_t  BL_mesh_alloc(const BL_arrayMD_dims vertWH, uint32_t cb_vert_element, uint32_t n_indices, pcBL_vertidxtype_t vertidxtypes)
{
    size_t cb_alloc = sizeof(BL_mesh_t) + sizeof (BL_linkable_array_t) * n_indices;
    pBL_mesh_t p = (pBL_mesh_t)malloc(cb_alloc);
    p->vert = BL_alloc_arrayMD(vertWH, cb_vert_element);
    for (uint32_t i = 0; i != n_indices; i++)
    {
        p->indices[i].link.next = p->indices[i].link.prev = &p->indices[i].link;
        p->indices[i].data.unit_count = (uintptr_t)vertidxtypes[i];
    }
    return p;
}

void    BL_mesh_delete(pBL_mesh_t* ppmesh)
{
    if (ppmesh && *ppmesh)
    {
        if ((*ppmesh)->vert) free((void*)(*ppmesh)->vert);
        free((void*)(*ppmesh));
    }
    *ppmesh = NULL;
}

uint32_t BL_mesh_vertidxsize(BL_vertidxtype_t type) { return g_vertidxsizes[type]; }
